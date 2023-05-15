#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <mach/mach_time.h>
#include <mach/thread_info.h>
#include <mach/thread_act.h>
#include <mach/mach_port.h>
#include <mach/mach_init.h>

#define run_env(i, func) \
    __convert(i); \
	thread_usertime(&st); \
	func(argc, argv, env); \
	thread_usertime(&et); \
	total_time += et - st

#define run(i, func) \
    __convert(i); \
	thread_usertime(&st); \
	func(argc, argv); \
	thread_usertime(&et); \
	total_time += et - st

int perl_entry(int argc, char **argv, char **env);
// gcc workaround
int gcc_entry__0 (int argc, char **argv);
int gcc_entry__1 (int argc, char **argv);
int gcc_entry__2 (int argc, char **argv);
int gcc_entry__3 (int argc, char **argv);
int gcc_entry__4 (int argc, char **argv);

int omnetpp_entry(int argc, char *argv[]);
int xalancbmk_entry(int argc, char *argv[]);
int deepsjeng_entry(int argc, char *argv[]);
int leela_entry(int argc, char *argv[]);
int xz_entry (int argc, char *argv[]);
int x264_entry( int argc, char **argv );
int exchange2_entry(int *arg);
int mcf_entry(int argc, char *argv[]);

enum { kMaxArgs = 64 };
char *env[] = { 0 };
int argc = 0;
char *argv[kMaxArgs];
static double st, et;
double total_time = 0;
mach_timebase_info_data_t _clock_timebase;

static char commandLine[][300] = {
/* 0 */"./perlbench -I./lib checkspam.pl 2500 5 25 11 150 1 1 1 1",
/* 1 */"./perlbench -I./lib diffmail.pl 4 800 10 17 19 300",
/* 2 */"./perlbench -I./lib splitmail.pl 6400 12 26 16 100 0",

/* 3 */"./gcc gcc-pp.c -O3 -finline-limit=0 -fif-conversion -fif-conversion2 -o gcc-pp.opts-O3_-finline-limit_0_-fif-conversion_-fif-conversion2.s",
/* 4 */ "./gcc gcc-pp.c -O2 -finline-limit=36000 -fpic -o gcc-pp.opts-O2_-finline-limit_36000_-fpic.s",
/* 5 */ "./gcc gcc-smaller.c -O3 -fipa-pta -o gcc-smaller.opts-O3_-fipa-pta.s",
/* 6 */ "./gcc ref32.c -O5 -o ref32.opts-O5.s",
/* 7 */ "./gcc ref32.c -O3 -fselective-scheduling -fselective-scheduling2 -o ref32.opts-O3_-fselective-scheduling_-fselective-scheduling2.s",

/* 8 */"./mcf_r inp.in",
/* 9 */"./omnetpp -c General -r 0",
/* 10 */"./xalancbmk -v t5.xml xalanc.xsl",
/* 11 */"./deepsjeng ref.txt",
/* 12 */"./leela ref.sgf",

/* 13 */"./xz cld.tar.xz 160 19cf30ae51eddcbefda78dd06014b4b96281456e078ca7c13e1c0c9e6aaea8dff3efb4ad6b0456697718cede6bd5454852652806a657bb56e07d61128434b474 59796407 61004416 6",
/* 14 */"./xz cpu2006docs.tar.xz 250 055ce243071129412e9dd0b3b69a21654033a9b723d874b2015c774fac1553d9713be561ca86f74e4f16f22e664fc17a79f30caa5ad2c04fbc447549c2810fae 23047774 23513385 6e",
/* 15 */"./xz input.combined.xz 250 a841f68f38572a49d86226b7ff5baeb31bd19dc637a922a972b2e6d1257a890f6a544ecab967c313e370478c74f760eb229d4eef8a8d2836d233d3e9dd1430bf 40401484 41217675 7",

/* 16 */"525.x264 place holder1",
/* 17 */"525.x264 place holder2",
/* 18 */"./x264 --seek 500 --dumpyuv 200 --frames 1250 -o BuckBunny_New.264 BuckBunny.yuv 1280x720",
};

 static inline double mach_time_to_seconds(uint64_t mach_time) {
    double nanos = (mach_time * _clock_timebase.numer) / _clock_timebase.denom;
    return nanos / 1e9;
 }

static inline void thread_usertime(double* time) {
	mach_msg_type_number_t count;
	thread_basic_info_t thi;
	thread_basic_info_data_t thi_data;
	count = THREAD_BASIC_INFO_COUNT;
	thi = &thi_data;
	thread_info(mach_thread_self(), THREAD_BASIC_INFO, (thread_info_t)thi, &count);
	*time = thi->user_time.seconds + thi->user_time.microseconds * 1e-6;
}

void __convert(int i) {
	argc = 0;
	char *p2 = strtok(commandLine[i], " ");
	while (p2 && argc < kMaxArgs-1) {
	    argv[argc++] = p2;
	    p2 = strtok(0, " ");
	}
	argv[argc] = 0;
}

void run_perl() {
	total_time = 0;
	run_env(0, perl_entry);
	run_env(1, perl_entry);
	run_env(2, perl_entry);
}

void run_gcc() {
#define _gcc_run(N, run_entry) __init(); \
                               run(N, gcc_entry__##run_entry); \
                               __freelist(); \
                               used_time = mach_time_to_seconds(__overhead); \
                               total_time -= used_time;

	total_time = 0;
    extern void __init();
    extern void __freelist();
    extern uint64_t __overhead;
    
    double used_time = 0;
    mach_timebase_info(&_clock_timebase);
    
    _gcc_run(3, 0);
    _gcc_run(4, 1);
    _gcc_run(5, 2);
    _gcc_run(6, 3);
    _gcc_run(7, 4);
#undef _gcc_run
}

void run_mcf() {
	total_time = 0;
	run(8, mcf_entry);
}

void run_omnetpp() {
	total_time = 0;
	run(9, omnetpp_entry);
}

void run_xalancbmk() {
	total_time = 0;
	run(10, xalancbmk_entry);
}

void run_deepsjeng() {
	total_time = 0;
	run(11, deepsjeng_entry);
}

void run_leela() {
	total_time = 0;
	run(12, leela_entry);
}

void run_xz() {
	total_time = 0;
	run(13, xz_entry);
	run(14, xz_entry);
	run(15, xz_entry);
}

void run_x264() {
	total_time = 0;
   char path[256];
   strcpy(path,getenv("HOME"));
   strcat(path,"/Documents/x264_stats.log");
    
   sprintf(commandLine[16], "./x264 --pass 1 --stats %s --bitrate 1000 --frames 1000 -o BuckBunny_New.264 BuckBunny.yuv 1280x720", path);
   sprintf(commandLine[17], "./x264 --pass 2 --stats %s --bitrate 1000 --dumpyuv 200 --frames 1000 -o BuckBunny_New.264 BuckBunny.yuv 1280x720", path);
	run(16, x264_entry);
	run(17, x264_entry);
	run(18, x264_entry);
}

void run_exchange2() {
	total_time = 0;
	int arg = 6;	
	thread_usertime(&st);
	exchange2_entry(&arg);
	thread_usertime(&et);
	total_time += et - st;
}

long specEntry(const char* benchname) {
	 mach_timebase_info(&_clock_timebase);
    if (strcmp(benchname, "500.perlbench_r") == 0)
        run_perl();
    if (strcmp(benchname, "502.gcc_r") == 0)
        run_gcc();
    if (strcmp(benchname, "505.mcf_r") == 0)
        run_mcf();
    if (strcmp(benchname, "520.omnetpp_r") == 0)
        run_omnetpp();
    if (strcmp(benchname, "523.xalancbmk_r") == 0)
        run_xalancbmk();
    if (strcmp(benchname, "525.x264_r") == 0)
        run_x264();
    if (strcmp(benchname, "531.deepsjeng_r") == 0)
        run_deepsjeng();
    if (strcmp(benchname, "541.leela_r") == 0)
        run_leela();
    if (strcmp(benchname, "548.exchange2_r") == 0)
        run_exchange2();
    if (strcmp(benchname, "557.xz_r") == 0)
        run_xz();
    printf("total_time: %f\n", total_time);
	return total_time;
}
/* 500: 707s*/
/* 502:
    1. 58s
    2. 70s
    3. 69s
    4. 85s
    5. 104s
        386s */
/* 505: 574s*/
/* 520: 884s*/
/* 523: 304*/
/* 525: 415*/
/* 531: 599*/
/* 541: 633s*/
/* 557: 638s*/

