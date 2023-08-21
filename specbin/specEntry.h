#include <pthread.h>
#include <mach/mach_time.h>
#include <mach/thread_info.h>
#include <mach/thread_act.h>
#include <mach/mach_port.h>
#include <mach/mach_init.h>
#include <signal.h>

// see https://github.com/apple-oss-distributions/xnu/blob/5c2921b07a2480ab43ec66f5b9e41cb872bc554f/bsd/sys/proc_info.h#L898

struct proc_threadcounts_data {
    uint64_t ptcd_instructions;
    uint64_t ptcd_cycles;
    uint64_t ptcd_user_time_mach;
    uint64_t ptcd_system_time_mach;
    uint64_t ptcd_energy_nj;
};

struct proc_threadcounts {
    uint16_t ptc_len;
    uint16_t ptc_reserved0;
    uint32_t ptc_reserved1;
    struct proc_threadcounts_data ptc_counts[];
};

#define PROC_PIDTHREADCOUNTS 34
#define PROC_PIDTHREADCOUNTS_SIZE (sizeof(struct proc_threadcounts))

int proc_pidinfo(int pid, int flavor, uint64_t arg, void *buffer, int buffersize);


extern void __init();
extern void __freelist();

enum { kMaxArgs = 64 };
int argc = 0;
char *argv[kMaxArgs];

static inline void thread_usertime(double* time) {
    mach_msg_type_number_t count;
    thread_basic_info_t thi;
    thread_basic_info_data_t thi_data;
    count = THREAD_BASIC_INFO_COUNT;
    thi = &thi_data;
    
    thread_info(mach_thread_self(), THREAD_BASIC_INFO, (thread_info_t)thi, &count);
    *time = thi->user_time.seconds + thi->user_time.microseconds * 1e-6;
}

void __convert(char *cmd) {
    argc = 0;
    char *p2 = strtok(cmd, " ");
    while (p2 && argc < kMaxArgs-1) {
        argv[argc++] = p2;
        p2 = strtok(0, " ");
    }
    argv[argc] = 0;
}

// from https://stackoverflow.com/a/11394336/19565461
char* concat(int count, ...)
{
    va_list ap;
    int i;

    // Find required length to store merged string
    int len = 1; // room for NULL
    va_start(ap, count);
    for(i=0 ; i<count ; i++)
        len += strlen(va_arg(ap, char*));
    va_end(ap);

    // Allocate memory to concat strings
    char *merged = calloc(sizeof(char),len);
    int null_pos = 0;

    // Actually concatenate strings
    va_start(ap, count);
    for(i=0 ; i<count ; i++)
    {
        char *s = va_arg(ap, char*);
        strcpy(merged+null_pos, s);
        null_pos += strlen(s);
    }
    va_end(ap);

    return merged;
}
