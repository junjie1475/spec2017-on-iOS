#include <pthread.h>
#include <mach/mach_time.h>
#include <mach/thread_info.h>
#include <mach/thread_act.h>
#include <mach/mach_port.h>
#include <mach/mach_init.h>
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
