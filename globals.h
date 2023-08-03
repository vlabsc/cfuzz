//0.39

//#ifndef GLOBALS_H_INCLUDED
//#define GLOBALS_H_INCLUDED

#define STATUS_HEAP_CORRUPTION 0xC0000374
#define STATUS_FATAL_APP_EXIT ((DWORD)0x40000015L)

unsigned int gprocscreated;
unsigned int gthreadscreated;
//HANDLE hps[10000], hts[10000];
//unsigned char *h_mutated_input_filename[10000];

HANDLE hps[40000], hts[40000];
unsigned char *h_mutated_input_filename[40000];

unsigned int gthreadsrunning;
unsigned int gprocess_crashed;
CRITICAL_SECTION critical_section;
unsigned long int gwaitforsingleobject_seconds;

// keeping this global, to avoid stack allocation - fast
// since trigger function will be always called
HINSTANCE safe_hKernel;
FARPROC safe_pfnExitProc;

unsigned int nooftimesinvalidparametererrorcode;
unsigned int nooftimesinvalidhandleerrorcode;
unsigned int nooftimessafeexitfailed;
//unsigned int gprocess_current_seed_crash;


double gprocesspersecond;
unsigned int logscreen_bytes;
unsigned int logscreen_i;

unsigned char *newline;

SYSTEMTIME statuslogsystem;

unsigned int gprocessoraffinitymask, gprocessoraffinitymaskshift;
/*
struct afrafothreaddata
{
    struct fuzzrunning *afr;
    struct fuzzoutput *afo;
};
*/

