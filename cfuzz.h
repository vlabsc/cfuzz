// 0.39

#ifndef CFUZZ_H_INCLUDED
#define CFUZZ_H_INCLUDED


#define LOGSCREENROWS 50

#endif // CFUZZ_H_INCLUDED

struct fuzzinput
{
    unsigned char *title;
    //unsigned long long previous_crashed_file_number;
    int previous_crash_filename_start;
    unsigned long long previous_crashed_file_size;

    unsigned long long total_seeds;
    unsigned long long total_seeds_completed;
    unsigned long long seeds_already_completed;
    unsigned long long total_seeds_pending;
    //unsigned char *seed_file_extension;
    unsigned long int seed_max_size_allowed_in_bytes;
    DWORD seed_size;

    unsigned char *target_application_filename;
    unsigned char *target_application_version;

    unsigned char *seed_content;
    unsigned char *seed_filename;
    unsigned char *seed_fileextension;
    unsigned char *seed_fileextension_wildcard;
    unsigned char *seed_filetype;

    unsigned int mutation_type;
    unsigned int mutation_start_offset;
    unsigned int mutation_end_offset;
    unsigned int mutation_length;
    unsigned int mutation_location;
    unsigned int mutation_hexjump;
    //unsigned char *mutated_input_filename;

    unsigned long long overall_saplings_created;
    unsigned long long overall_saplings_pending;
    unsigned long long overall_saplings_total;

    unsigned long long saplings_total;
    unsigned long long saplings_created;
    unsigned long long saplings_pending;

    unsigned long long process_clean_exits;
    unsigned long long process_safe_exits;
    unsigned long long process_non_safe_exits;
    //unsigned long long process_interesting_crashes;
    unsigned int process_lowmem;


    unsigned long int waitforsingleobject_seconds;
    unsigned long int process_cooling_seconds;

    unsigned int initial_sleep_time;
    unsigned int max_threads;

    unsigned char *chamber_folder_name;

    //enum mutation_models {b8s=1, b16s=2, b32s=3, b8de=4, b16de=5, b32de=6, b8min=7, b16min=8, b32min=9, b8bmin=10, b16bmin=11, b32bmin=12};
    // byte, word, dword
    enum mutation_models {b1=1, w1=2, d1=3, b2=4, w2=5, d2=6, b4=7, w4=8, d4=9, b16=10, w16=11, d16=12, b32=13, w32=14, d32=15, b64=16, w64=17, d64=18, b128=19, w128=20, d128=21};

    enum mutation_models mutation_model;

};


struct fuzzrunning
{
    //unsigned int process_pid;
    //unsigned long int thread_id;
    unsigned char *process_commandline_name;
    unsigned char *process_current_directory;
    //unsigned long int waitforsingleobject_seconds;
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    //DEBUG_EVENT *debug_event;
    unsigned int continue_debugging;
    unsigned int exception_occured;
    //unsigned int process_created;
    unsigned int process_crashed;
    unsigned int process_lowmem;
    unsigned int process_clean_exits;
    unsigned int process_safe_exits;
    unsigned int process_non_safe_exits;
    //unsigned int process_current_seed_crash;
    unsigned long int process_cooling_seconds;

    //unsigned long long process_interesting_crashes;
    unsigned char *mutated_input_filename;
};




struct fuzzoutput
{
    float progress_cur_limit, progress_cur_increment, progress_cur_bar;
    float progress_all_limit, progress_all_increment, progress_all_bar;
    float progress_all_bar_temp;

    MEMORYSTATUSEX statex;
    SYSTEM_INFO SysInfo;

    clock_t t;
    LARGE_INTEGER Frequency;
    LARGE_INTEGER Starttime, Endtime;
    double performance_time_difference;

    HANDLE hlogfile;
    //unsigned char *logfilename;
    unsigned char *logcontent;

    HANDLE hlogscreenfile;
    unsigned char *logscreenfilename;
    unsigned char *logfinalscreenfilename;
    unsigned char *logscreencontent[LOGSCREENROWS];

    HANDLE hlogstautusfile;
    unsigned char *logstatusfilename;
    unsigned char *logfinalstatusfilename;
    unsigned char *logstatuscontent;
};


