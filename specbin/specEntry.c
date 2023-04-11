#include <stdio.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>

#define run_env(i, func) \
	convert(i); \
	time(&st); \
	func(argc, argv, env); \
	time(&et); \
	total_time += et - st

#define run(i, func) \
	convert(i); \
	time(&st); \
	func(argc, argv); \
	time(&et); \
	total_time += et - st

int perl_entry(int argc, char **argv, char **env);
int gcc_entry (int argc, char **argv);
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
time_t st, et;
long total_time = 0;

static char commandLine[][300] = {
"./perlbench -I./lib checkspam.pl 2500 5 25 11 150 1 1 1 1",
"./perlbench -I./lib diffmail.pl 4 800 10 17 19 300",
"./perlbench -I./lib splitmail.pl 6400 12 26 16 100 0",
    
"./gcc gcc-pp.c -O3 -finline-limit=0 -fif-conversion -fif-conversion2 -o gcc-pp.opts-O3_-finline-limit_0_-fif-conversion_-fif-conversion2.s",
    
"./mcf_r inp.in",
"./omnetpp -c General -r 0",
"./xalancbmk -v t5.xml xalanc.xsl",
"./deepsjeng ref.txt",
"./leela ref.sgf",

"./xz cld.tar.xz 160 19cf30ae51eddcbefda78dd06014b4b96281456e078ca7c13e1c0c9e6aaea8dff3efb4ad6b0456697718cede6bd5454852652806a657bb56e07d61128434b474 59796407 61004416 6",
"./xz cpu2006docs.tar.xz 250 055ce243071129412e9dd0b3b69a21654033a9b723d874b2015c774fac1553d9713be561ca86f74e4f16f22e664fc17a79f30caa5ad2c04fbc447549c2810fae 23047774 23513385 6e",
"./xz input.combined.xz 250 a841f68f38572a49d86226b7ff5baeb31bd19dc637a922a972b2e6d1257a890f6a544ecab967c313e370478c74f760eb229d4eef8a8d2836d233d3e9dd1430bf 40401484 41217675 7",

"525.x264 place holder1",
"525.x264 place holder2",
"./x264 --seek 500 --dumpyuv 200 --frames 1250 -o BuckBunny_New.264 BuckBunny.yuv 1280x720",
"./mcf_r inp.in",
};

void convert(int i) {
	argc = 0;
	char *p2 = strtok(commandLine[i], " ");
	while (p2 && argc < kMaxArgs-1) {
	    argv[argc++] = p2;
	    p2 = strtok(0, " ");
	}
	argv[argc] = 0;
}

void run_perl() {
	run_env(0, perl_entry);
	run_env(1, perl_entry);
	run_env(2, perl_entry);
}

void run_gcc() {
	total_time = 0;
    // TODO: add all gcc commands
	 run(3, gcc_entry);
}

void run_mcf() {
	total_time = 0;
	run(15, mcf_entry);
}

void run_omnetpp() {
	total_time = 0;
	run(5, omnetpp_entry);
}

void run_xalancbmk() {
	total_time = 0;
	run(6, xalancbmk_entry);
}

void run_deepsjeng() {
	total_time = 0;
	run(7, deepsjeng_entry);
}

void run_leela() {
	total_time = 0;
	run(8, leela_entry);
}

void run_xz() {
	total_time = 0;
	run(9, xz_entry);
	run(10, xz_entry);
	run(11, xz_entry);
}

void run_x264() {
	total_time = 0;
    char path[256];
    strcpy(path,getenv("HOME"));
    strcat(path,"/Documents/x264_stats.log");
    
    sprintf(commandLine[12], "./x264 --pass 1 --stats %s --bitrate 1000 --frames 1000 -o BuckBunny_New.264 BuckBunny.yuv 1280x720", path);
    sprintf(commandLine[13], "./x264 --pass 2 --stats %s --bitrate 1000 --dumpyuv 200 --frames 1000 -o BuckBunny_New.264 BuckBunny.yuv 1280x720", path);
	run(12, x264_entry);
	run(13, x264_entry);
	run(14, x264_entry);
}

void run_exchange2() {
	total_time = 0;
	int arg = 6;	
	time(&st);
	exchange2_entry(&arg);
	time(&et);
	total_time += et - st;
}

long specEntry(const char* benchname) {
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
    printf("total_time: %ld\n", total_time);
	return total_time;
}
