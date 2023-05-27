#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <stdbool.h>
#include "specEntry.h"

typedef int entry_t(int argc, char *argv[], char *envp[]);
#define f(name, N) int (name##_entry##N)(int argc, char *argv[], char *envp[])
// Integer Benchmarks
f(perl, );
f(gcc, __0); f(gcc, __1); f(gcc, __2); f(gcc, __3); f(gcc, __4); /* 502.gcc workaround */
f(mcf, );
f(omnetpp, );
f(xalancbmk, );
f(x264, );
f(deepsjeng, );
f(leela, );
f(exchange2,);
f(xz, );
// Floating point Benchmarks
f(bwaves, );
f(cactusBSSN, );
f(namd, );
f(parest, );
f(povray, );
f(lbm, );
f(wrf, );
f(blender, );
f(cam4, );
f(imagick, );
f(nab, );
f(roms, );
f(fotonik3d, );

#undef f
int proc_pid_rusage(int pid, int flavor, rusage_info_t *buffer);

// TODO: manually reset memory state?
int _gcc_entry(int argc, char *argv[], char *envp[]) {
    static int count = 0;
    static entry_t (*gcc_mapping[5]) = { gcc_entry__0, gcc_entry__1, gcc_entry__2, gcc_entry__3, gcc_entry__4 };
    ((entry_t*)gcc_mapping[count++])(argc, argv, envp);
    return 0;
}

static char commandLine[][6][300] = {
    [500] = {
        "./perlbench -I./lib checkspam.pl 2500 5 25 11 150 1 1 1 1",
        "./perlbench -I./lib diffmail.pl 4 800 10 17 19 300",
        "./perlbench -I./lib splitmail.pl 6400 12 26 16 100 0",
    },

    [502] = {
        "./gcc gcc-pp.c -O3 -finline-limit=0 -fif-conversion -fif-conversion2 -o gcc-pp.opts-O3_-finline-limit_0_-fif-conversion_-fif-conversion2.s",
        "./gcc gcc-pp.c -O2 -finline-limit=36000 -fpic -o gcc-pp.opts-O2_-finline-limit_36000_-fpic.s",
        "./gcc gcc-smaller.c -O3 -fipa-pta -o gcc-smaller.opts-O3_-fipa-pta.s",
        "./gcc ref32.c -O5 -o ref32.opts-O5.s",
        "./gcc ref32.c -O3 -fselective-scheduling -fselective-scheduling2 -o ref32.opts-O3_-fselective-scheduling_-fselective-scheduling2.s",
    },

    [505] = {   "./mcf_r inp.in",   },
    [520] = {   "./omnetpp -c General -r 0",   },
    [523] = {   "./xalancbmk -v t5.xml xalanc.xsl",   },
    [525] = { /* "./ldecod -i BuckBunny.264 -o BuckBunny.yuv", */
        "./x264 place holder1",
        "./x264 place holder2",
        "./x264 --seek 500 --dumpyuv 200 --frames 1250 -o BuckBunny_New.264 BuckBunny.yuv 1280x720",
    },
    
    [531] = {   "./deepsjeng ref.txt",   },
    [541] = {   "./leela ref.sgf",   },
    [548] = {    "./exchange2 6"   },
    [557] = {
        "./xz cld.tar.xz 160 19cf30ae51eddcbefda78dd06014b4b96281456e078ca7c13e1c0c9e6aaea8dff3efb4ad6b0456697718cede6bd5454852652806a657bb56e07d61128434b474 59796407 61004416 6",
        "./xz cpu2006docs.tar.xz 250 055ce243071129412e9dd0b3b69a21654033a9b723d874b2015c774fac1553d9713be561ca86f74e4f16f22e664fc17a79f30caa5ad2c04fbc447549c2810fae 23047774 23513385 6e",
        "./xz input.combined.xz 250 a841f68f38572a49d86226b7ff5baeb31bd19dc637a922a972b2e6d1257a890f6a544ecab967c313e370478c74f760eb229d4eef8a8d2836d233d3e9dd1430bf 40401484 41217675 7",
    },
    [503] = {
        "./bwaves bwaves_1",
        "./bwaves bwaves_2",
        "./bwaves bwaves_3",
        "./bwaves bwaves_4"
    },
    [507] = {   "./cactusBSSN spec_ref.par"   },
    [508] = {   "./namd_r --input apoa1.input --output apoa1.ref.output --iterations 65"   },
    [510] = {   "./parest_r ref.prm"   },
    [511] = {   "./povray_r SPEC-benchmark-ref.ini"   },
    [519] = {   "./lbm_r 3000 reference.dat 0 0 100_100_130_ldc.of"   },
    [521] = {   "./wrf"   },
    [526] = {   "./blender_r sh3_no_char.blend --render-output sh3_no_char_ --threads 1 -b -F RAWTGA -s 849 -e 849 -a"   },
    [527] = {   "./cam4"   },
    [538] = {   "./imagick -limit disk 0 refrate_input.tga -edge 41 -resample 181% -emboss 31 -colorspace YUV -mean-shift 19x19+15% -resize 30% refrate_output.tga"   },
    [544] = {   "./nab 1am0 1122214447 122"   },
    [549] = {   "./fotonik3d"   },
    [554] = {   "./roms"   }
};

entry_t (*function_mapping[]) = {
    [500] = 0,
    [502] = 0,
    [505] = 0,
    [520] = 0,
    [523] = 0,
    [525] = 0,
    [531] = 0,
    [541] = 0,
    [548] = 0,
    [557] = 0,
    [503] = 0,
    [507] = 0,
    [508] = 0,
    [510] = 0,
    [511] = 0,
    [519] = 0,
    [521] = 0,
    [526] = 0,
    [527] = 0,
    [538] = 0,
    [544] = 0,
    [549] = 0,
    [554] = 0
};
mach_timebase_info_data_t _clock_timebase;
pid_t pid;
static inline double mach_time_to_seconds(uint64_t mach_time) {
    double nanos = (mach_time * _clock_timebase.numer) / _clock_timebase.denom;
    return nanos / 1e9;
}

__attribute__((constructor))
void init() {
    function_mapping[500] = perl_entry,
    function_mapping[502] = _gcc_entry,
    function_mapping[505] = mcf_entry,
    function_mapping[520] = omnetpp_entry,
    function_mapping[523] = xalancbmk_entry,
    function_mapping[525] = x264_entry;
    function_mapping[531] = deepsjeng_entry;
    function_mapping[541] = leela_entry;
    function_mapping[548] = exchange2_entry;
    function_mapping[557] = xz_entry;
    function_mapping[503] = bwaves_entry;
    function_mapping[507] = cactusBSSN_entry;
    function_mapping[508] = namd_entry;
    function_mapping[510] = parest_entry;
    function_mapping[511] = povray_entry;
    function_mapping[521] = wrf_entry;
    function_mapping[519] = lbm_entry;
    function_mapping[526] = blender_entry;
    function_mapping[527] = cam4_entry;
    function_mapping[538] = imagick_entry;
    function_mapping[544] = nab_entry;
    function_mapping[549] = fotonik3d_entry;
    function_mapping[554] = roms_entry;
    mach_timebase_info(&_clock_timebase);
    pid = getpid();
    
    // 525.x264 workaround
    char path[256];
    strcpy(path,getenv("HOME"));
    strcat(path,"/Documents/525.x264_r/x264_stats.log");
    sprintf(commandLine[525][0], "./x264 --pass 1 --stats %s --bitrate 1000 --frames 1000 -o BuckBunny_New.264 BuckBunny.yuv 1280x720", path);
    sprintf(commandLine[525][1], "./x264 --pass 2 --stats %s --bitrate 1000 --dumpyuv 200 --frames 1000 -o BuckBunny_New.264 BuckBunny.yuv 1280x720", path);
}

bool __warp = false;
void specEntry(const char* benchname, double results[2]) {
    // initialization
    extern uint64_t __overhead; __overhead = 0;
    static char *envp[] = { 0 };
    rusage_info_current usage1, usage2;
    int count = 0;
    double total_time = 0, total_nj = 0, st = 0, et = 0;
    
    int bench = atoi(benchname);
    if(bench == 525) {
        extern int ldecoed_entry(int argc, char **argv);
        if(access("./BuckBunny.yuv", F_OK) != 0) {
            char decode_cmd[50] = "./ldecod -i BuckBunny.264 -o BuckBunny.yuv";
            __convert(decode_cmd);
            ldecoed_entry(argc, argv);
        }
    }
    while(commandLine[bench][count][0]) { count ++; }
    
    if(bench == 500 || bench == 502 || bench == 531 || bench == 557 || bench == 521 || bench == 527) __warp = true;
    else __warp = false;
    if(bench == 500) __init(); // For 500.perlbench we only need to clean up once.
    for(int j = 0; j < count; j++) {
        __convert(commandLine[bench][j]);
        if(__warp && bench != 500) __init(); /* memory leak patch */
        
        // retrive energy consumption
        proc_pid_rusage(pid, RUSAGE_INFO_CURRENT, (rusage_info_t*)&usage1);
         
        // high resoulution per thread usertime.
        thread_usertime(&st);
        ((entry_t*)function_mapping[bench])(argc, argv, envp);
        thread_usertime(&et);
        
        // retrive energy consumption
        proc_pid_rusage(pid, RUSAGE_INFO_CURRENT, (rusage_info_t*)&usage2);
        
        if(__warp && bench != 500) __freelist(); /* memory leak patch */
        total_time += et - st;
        total_nj += usage2.ri_energy_nj - usage1.ri_energy_nj;
    }
    
    if(bench == 500) __freelist(); // For 500.perlbench we only need to clean up once.
    results[1] = total_nj / 1e9 / total_time;
    results[0] = total_time - mach_time_to_seconds(__overhead); /* gcc workaround */
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
