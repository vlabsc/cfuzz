// 0.40

#define WINVER 0x0500

#include <stdio.h>
#include <unistd.h>
#include <windows.h>
#include <winbase.h>
#include <time.h>
#include <fileapi.h>

#include "cfuzz.h"
#include "globals.h"


#define MAXVAL 0XFF
#define MINVAL 0x00
#define PREVIOUS_CRASHED_FILE_NUMBER 0

#define MAXFILES_TO_MUTATE 0
#define MAXTRHEADS 1
//#define MAXMUTATIONLENGTH 512
#define MAXINPUTFILESIZE 1600000
#define LOGSCREENROWS 50







// keeping this global, to avoid stack allocation - fast
// since logscreen function will be always called

/*
// handle for process, threads and files
HANDLE hps[5000], hts[5000];
unsigned char *h_mutated_input_filename[5000];
*/






int process_seed_file(struct fuzzinput *afi, struct fuzzoutput *afo);

/*
int mutate_bytes_storm(struct fuzzinput *afi, struct fuzzoutput *afo);
int mutate_bytes_lightning(struct fuzzinput *afi, struct fuzzoutput *afo);
int mutate_bytes_minimalistic(struct fuzzinput *afi, struct fuzzoutput *afo);
int mutate_bytes_bareminimalistic(struct fuzzinput *afi, struct fuzzoutput *afo);
int mutate_bytes_tiny(struct fuzzinput *afi, struct fuzzoutput *afo);
int mutate_bytes_damn_tiny(struct fuzzinput *afi, struct fuzzoutput *afo);
int mutate_bytes_microscopic(struct fuzzinput *afi, struct fuzzoutput *afo);
*/



int begin_files(unsigned char *title,
                unsigned char *target_application_name,
                unsigned char *target_application_version,
                unsigned char *seed_file_type,
                unsigned char *seed_file_max_size_in_bytes,
                unsigned char *seed_file_extension,
                unsigned char *mutation_type,
                unsigned char *mutation_start_offset,
                unsigned char *mutation_end_offset,
                unsigned char *waitforsingleobject_seconds,
                unsigned char *process_cooling_seconds,
                unsigned char *initial_sleep_time,
                unsigned char *max_threads,
                unsigned char *chamber_folder_name);


int begin_files(unsigned char *title,
                unsigned char *target_application_name,
                unsigned char *target_application_version,
                unsigned char *seed_file_type,
                unsigned char *seed_file_max_size_in_bytes,
                unsigned char *seed_file_extension,
                unsigned char *mutation_type,
                unsigned char *mutation_start_offset,
                unsigned char *mutation_end_offset,
                unsigned char *waitforsingleobject_seconds,
                unsigned char *process_cooling_seconds,
                unsigned char *initial_sleep_time,
                unsigned char *max_threads,
                unsigned char *chamber_folder_name)

{
//    int ret = 0;
    int i = 0;
    int process_seed_file_success = 0;
    WIN32_FIND_DATA ffd;
    HANDLE hFind;
    unsigned char *newfilename = (unsigned char *) calloc(64, sizeof(unsigned char));
    ZeroMemory(newfilename, (64 * sizeof(unsigned char)));
    LARGE_INTEGER filesize;

    newline = (unsigned char *) calloc(1, 1);
    ZeroMemory(newline, 1);
    sprintf(newline, "\n");



    printf("fuzz template.\n");

    /*
    printf("title: %s\n", title);
    printf("target app name: %s\n", target_application_name);
    printf("target app version: %s\n", target_application_version);
    printf("seed file type: %s\n", seed_file_type);
    printf("seed file max size: %s\n", seed_file_max_size_in_bytes);
    printf("seed file extension: %s\n", seed_file_extension);
    printf("mutation type (1,2,4) - byte, word, dword: %s\n", mutation_type);
    printf("mutation start offset: %s\n", mutation_start_offset);
    printf("mutation end offset: %s\n", mutation_end_offset);
    printf("previous crashed file number: %s\n", previous_crashed_file_number);
    printf("log name: %s\n", logname);
    printf("waitforsingleobject seconds: %s\n", waitforsingleobject_seconds);
    printf("initial sleep minutes: %s\n", initial_sleep_time);
    printf("max threads: %s\n", max_threads);
    printf("chamber_folder_name: %s\n",chamber_folder_name);
    */

    nooftimesinvalidparametererrorcode = 0;
    nooftimesinvalidhandleerrorcode = 0;
    nooftimessafeexitfailed = 0;

    safe_hKernel = GetModuleHandle(("kernel32"));
    safe_pfnExitProc = GetProcAddress(safe_hKernel, "ExitProcess");

    //for(i = 0; i < 4096; i++)
    //for(i = 0; i < 10000; i++)
    for(i = 0; i < 40000; i++)
    {
        //h_mutated_input_filename[i] = (unsigned char *) calloc(48, 1);
        h_mutated_input_filename[i] = (unsigned char *) calloc(256, 1);
    }

    //InitializeCriticalSectionAndSpinCount(&thread_critical_section, 0x500);
    InitializeCriticalSection(&critical_section);

    struct fuzzinput *fi = (struct fuzzinput *) calloc(1, sizeof(struct fuzzinput));
    struct fuzzrunning *fr = (struct fuzzrunning *) calloc(1, sizeof(struct fuzzrunning));
    struct fuzzoutput *fo = (struct fuzzoutput *) calloc(1, sizeof(struct fuzzoutput));

    fr->process_commandline_name = (unsigned char *) calloc(64, sizeof(unsigned char));

    fi->seed_filename = (unsigned char *) calloc(64, sizeof(unsigned char));
    fo->logcontent = (unsigned char *) calloc(40, sizeof(unsigned char));
    fo->logstatuscontent = (unsigned char *) calloc(512, sizeof(unsigned char));

    for(i = 0; i < LOGSCREENROWS; i++)
    {
        fo->logscreencontent[i] = (unsigned char *) calloc(160, 1);
    }

    GetSystemInfo(&fo->SysInfo);

    fi->title = (unsigned char *) calloc(32, sizeof(unsigned char));
    ZeroMemory(fi->title, (32 * sizeof(unsigned char)));
    sprintf(fi->title, "%s", title);

    fi->target_application_version  = (unsigned char *) calloc(16, sizeof(unsigned char));
    ZeroMemory(fi->target_application_version , 16 * sizeof(unsigned char));
    sprintf(fi->target_application_version, "%s", target_application_version);

    fi->target_application_filename = (unsigned char *) calloc(64, sizeof(unsigned char));
    ZeroMemory(fi->target_application_filename, 64 * sizeof(unsigned char));
    sprintf(fi->target_application_filename, "%s", target_application_name);

    fi->chamber_folder_name = (unsigned char *) calloc(32, sizeof(unsigned char));
    ZeroMemory(fi->chamber_folder_name, (32 * sizeof(unsigned char)));
    sprintf(fi->chamber_folder_name, "%s", chamber_folder_name);

    fi->seed_filetype = (unsigned char *) calloc(32, sizeof(unsigned char));
    ZeroMemory(fi->seed_filetype, (32 * sizeof(unsigned char)));
    sprintf(fi->seed_filetype, "%s", seed_file_type);

    fi->seed_fileextension = (unsigned char *) calloc(8, sizeof(unsigned char));
    ZeroMemory(fi->seed_fileextension, (8 * sizeof(unsigned char)));
    sprintf(fi->seed_fileextension, "%s", seed_file_extension);

    fi->seed_fileextension_wildcard = (unsigned char *) calloc(64, sizeof(unsigned char));
    ZeroMemory(fi->seed_fileextension_wildcard, (64 * sizeof(unsigned char)));
    sprintf(fi->seed_fileextension_wildcard, "%s\\*.%s", chamber_folder_name, seed_file_extension);

    fi->seed_max_size_allowed_in_bytes = unsigned_char_to_unsigned_long_int(seed_file_max_size_in_bytes);

    fi->mutation_type = unsigned_char_to_unsigned_int(mutation_type);
    fi->mutation_start_offset = unsigned_char_to_unsigned_int(mutation_start_offset);
    fi->mutation_end_offset = unsigned_char_to_unsigned_int(mutation_end_offset);
    fi->mutation_length = (fi->mutation_end_offset - fi->mutation_start_offset) + 1;

    fi->waitforsingleobject_seconds = unsigned_char_to_unsigned_long_int(waitforsingleobject_seconds);
    gwaitforsingleobject_seconds = fi->waitforsingleobject_seconds;
    gprocess_crashed = 0;

    fi->process_cooling_seconds = unsigned_char_to_unsigned_long_int(process_cooling_seconds);
    fi->initial_sleep_time = unsigned_char_to_unsigned_int(initial_sleep_time);

    fi->max_threads = unsigned_char_to_unsigned_int(max_threads);

    // by default choose byte lightning

    fi->mutation_model = b1;
    fi->mutation_model = fi->mutation_type;

    if(fi->mutation_type == 1)
    {
        fi->mutation_hexjump = 1;
    }
    else if(fi->mutation_type == 2)
    {
        fi->mutation_hexjump = 1;
    }
    else if(fi->mutation_type == 3)
    {
        fi->mutation_hexjump = 1;
    }
    else if(fi->mutation_type == 4)
    {
        fi->mutation_hexjump = 4;
    }
    else if(fi->mutation_type == 5)
    {
        fi->mutation_hexjump = 4;
    }
    else if(fi->mutation_type == 6)
    {
        fi->mutation_hexjump = 4;
    }
    else if(fi->mutation_type == 7)
    {
        fi->mutation_hexjump = 8;
    }
    else if(fi->mutation_type == 8)
    {
        fi->mutation_hexjump = 8;
    }
    else if(fi->mutation_type == 9)
    {
        fi->mutation_hexjump = 8;
    }
    else if(fi->mutation_type == 10)
    {
        fi->mutation_hexjump = 16;
    }
    else if(fi->mutation_type == 11)
    {
        fi->mutation_hexjump = 16;
    }
    else if(fi->mutation_type == 12)
    {
        fi->mutation_hexjump = 16;
    }
    else if(fi->mutation_type == 13)
    {
        fi->mutation_hexjump = 32;
    }
    else if(fi->mutation_type == 14)
    {
        fi->mutation_hexjump = 32;
    }
    else if(fi->mutation_type == 15)
    {
        fi->mutation_hexjump = 32;
    }
    else if(fi->mutation_type == 16)
    {
        fi->mutation_hexjump = 64;
    }
    else if(fi->mutation_type == 17)
    {
        fi->mutation_hexjump = 64;
    }
    else if(fi->mutation_type == 18)
    {
        fi->mutation_hexjump = 64;
    }
    else if(fi->mutation_type == 19)
    {
        fi->mutation_hexjump = 128;
    }
    else if(fi->mutation_type == 20)
    {
        fi->mutation_hexjump = 128;
    }
    else if(fi->mutation_type == 21)
    {
        fi->mutation_hexjump = 128;
    }

    // just utilising fo->logscreenfilename in this block
    CreateDirectoryA("logs", NULL);
    fo->logscreenfilename = (unsigned char *) calloc(48, sizeof(unsigned char));
    ZeroMemory(fo->logscreenfilename, (48 * sizeof(unsigned char)));
    sprintf(fo->logscreenfilename, "logs\\log%d", fi->mutation_type);
    CreateDirectoryA(fo->logscreenfilename, NULL);
    // just utilising fo->logscreenfilename in this block


    fo->logscreenfilename = (unsigned char *) calloc(48, sizeof(unsigned char));
    ZeroMemory(fo->logscreenfilename, (48 * sizeof(unsigned char)));
    sprintf(fo->logscreenfilename, "logs\\screen.log", fi->mutation_type);

    fo->logfinalscreenfilename = (unsigned char *) calloc(48, sizeof(unsigned char));
    ZeroMemory(fo->logfinalscreenfilename, (48 * sizeof(unsigned char)));
    sprintf(fo->logfinalscreenfilename, "logs\\log%d\\screen_%d_%d.log", fi->mutation_type,fi->mutation_start_offset, fi->mutation_end_offset);

    fo->logstatusfilename = (unsigned char *) calloc(48, sizeof(unsigned char));
    ZeroMemory(fo->logstatusfilename, (48 * sizeof(unsigned char)));
    sprintf(fo->logstatusfilename, "logs\\status.log", fi->mutation_type);

    fo->logfinalstatusfilename = (unsigned char *) calloc(48, sizeof(unsigned char));
    ZeroMemory(fo->logfinalstatusfilename, (48 * sizeof(unsigned char)));
    sprintf(fo->logfinalstatusfilename, "logs\\log%d\\status_%d_%d.log", fi->mutation_type,fi->mutation_start_offset, fi->mutation_end_offset);

    fr->process_crashed =0;
    fi->total_seeds = 0;
    fi->total_seeds_completed = 0;
    fi->seeds_already_completed = 0;

    fi->previous_crashed_file_size = 0;
    fo->progress_all_bar = 0.0;
    fo->progress_all_increment = 0.0;
    fo->progress_all_limit = 0.0;

    printf("------------------------------\n");
    printf("title: %s \n", fi->title);
    printf("target app name: %s \n", fi->target_application_filename);
    printf("target app version: %s \n", fi->target_application_version);
    printf("seed file type: %s \n", fi->seed_filetype);
    printf("seed file max size: %lu \n",fi->seed_max_size_allowed_in_bytes);
    printf("seed file extension: %s \n", fi->seed_fileextension);
    printf("mutation type: %d - mutations per bytes: %d \n", fi->mutation_type, 256 / fi->mutation_hexjump);

    /*
    printf("               - 1  - byte (storm), 2 - word (storm), 3 - dword (storm)\n");
    printf("               - 4  - byte (lightning), 5 - word (lightning), 6 - dword (lightning)\n");
    printf("               - 7  - byte (minimalistic), 8 - word (minimalistic), 9 - dword (minimalistic)\n");
    printf("               - 10 - byte (bare minimal), 11 - word (bare minimal), 12 - dword (bare minimal)\n");
    printf("               - 13 - byte (tiny), 14 - word (tiny), 15 - dword (tiny)\n");
    printf("               - 16 - byte (damn tiny), 17 - word (damn tiny), 18 - dword (damn tiny)\n");
    printf("               - 19 - byte (microscopic), 20 - word (microscopic), 21 - dword (microscopic)\n");
    */

    printf("mutation start offset: %d\n", fi->mutation_start_offset);
    printf("mutation end offset: %d\n", fi->mutation_end_offset);
    printf("mutation length: %d\n", fi->mutation_length);
    printf("mutations per bytes: %d \n", 256 / fi->mutation_hexjump);
    printf("waitforsingleobject seconds: %lu \n", fi->waitforsingleobject_seconds);
    printf("process cooling seconds: %u \n", fi->process_cooling_seconds);
    printf("initial sleep minutes: %d \n", fi->initial_sleep_time);
    printf("max threads: %u\n", fi->max_threads);
    printf("chamber folder name: %s \n", fi->chamber_folder_name);

    printf("guru meditation for %d minute(s). fire the fuzzers.\n", fi->initial_sleep_time);

    Sleep(60000 * fi->initial_sleep_time);

    // logs directory is created above
    //CreateDirectoryA("logs", NULL);
    CreateDirectoryA("forest", NULL);

    // do the calculations - for progress calculation
    ZeroMemory(&ffd, sizeof(ffd));
    hFind = FindFirstFile(fi->seed_fileextension_wildcard, &ffd);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        printf ("FindFirstFile failed in %s: Errorcode %d\n", fi->seed_fileextension_wildcard, GetLastError());
        return;
    }
    else
    {
        do
        {
            if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                // do nothing
            }
            else
            {
                filesize.LowPart = ffd.nFileSizeLow;
                filesize.HighPart = ffd.nFileSizeHigh;
                if(filesize.QuadPart < fi->seed_max_size_allowed_in_bytes)
                {
                    fi->total_seeds++;
                }
            }
        }
        while (FindNextFile(hFind, &ffd) != 0);
        FindClose(hFind);
    }

	fo->progress_all_increment = (float) (100.0 / fi->total_seeds);
    fo->progress_all_bar = fi->seeds_already_completed * fo->progress_all_increment;
    fi->total_seeds_pending = fi->total_seeds - fi->total_seeds_completed;

    fi->overall_saplings_created = 0;
    fi->overall_saplings_total = ((256/ fi->mutation_hexjump) * fi->mutation_length) * fi->total_seeds;
    fi->overall_saplings_pending = fi->overall_saplings_total;


    build_log_status_first(fi, fo);

    sprintf(fo->logstatuscontent, "guru meditation over. waited for %d minute(s)", fi->initial_sleep_time);
    build_log_status(fo);

    sprintf(fo->logstatuscontent, "overall seed count %d.\n", fi->total_seeds);
    build_log_status(fo);


    fo->t = clock();
    QueryPerformanceFrequency(&fo->Frequency);
    QueryPerformanceCounter(&fo->Starttime);

    // now do the actual fuzzing
    ZeroMemory(&ffd, sizeof(ffd));
    hFind = FindFirstFile(fi->seed_fileextension_wildcard, &ffd);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        printf ("FindFirstFile2 failed in %s: Errorcode %d\n", fi->seed_fileextension_wildcard, GetLastError());
        return;
    }
    else
    {
        do
        {
            if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                // do nothing
            }
            else
            {
                filesize.LowPart = ffd.nFileSizeLow;
                filesize.HighPart = ffd.nFileSizeHigh;
                if(filesize.QuadPart < fi->seed_max_size_allowed_in_bytes)
                {
                    fo->statex.dwLength = sizeof (fo->statex);
                    GlobalMemoryStatusEx (&fo->statex);

                    ZeroMemory(fi->seed_filename, (64 * sizeof(unsigned char)));
                    sprintf(fi->seed_filename, "%s\\%s", fi->chamber_folder_name, ffd.cFileName);

                    sprintf(fo->logstatuscontent, "seed %llu started. %s.", fi->total_seeds_completed, fi->seed_filename);
                    build_log_status(fo);

                    sprintf(fo->logstatuscontent, "entering into process_input_file() function for %s.", fi->seed_filename);
                    build_log_status(fo);

                    process_seed_file_success = process_seed_file(fi, fo);

                    sprintf(fo->logstatuscontent, "function process_input_file() completed for %s. Filesize: %lu.", fi->seed_filename, fi->seed_size);
                    build_log_status(fo);

                    if(process_seed_file_success)
                    {
                        if(fi->mutation_end_offset > fi->seed_size)
                        {
                            //printf("ignoring %s. filesize is smaller than mutation_end_offset: %d.", fi->seed_filename, fi->mutation_end_offset);
                            sprintf(fo->logstatuscontent, "filesize for %s is %lu. which is lower than mutation_end_offset (%d). skipping.", fi->seed_filename, fi->seed_size, fi->mutation_end_offset);
                            build_log_status(fo);

                        }
                        // mutate only if the fi->mutation_end_offset is smaller than seed_size
                        if(fi->mutation_end_offset < fi->seed_size)
                        {
                            fr->process_lowmem = 0;

                            fi->saplings_total = (256 / fi->mutation_hexjump) * fi->mutation_length;
                            fi->saplings_pending = fi->saplings_total;
                            fi->saplings_created = 0;
                            fi->process_clean_exits = 0;
                            fi->process_safe_exits = 0;
                            fi->process_non_safe_exits = 0;

                            fo->progress_cur_bar = 0.0;
                            fo->progress_cur_increment = 0.0;
                            fo->progress_cur_limit = 0.0;
                            fo->progress_cur_limit = (float) (fi->mutation_length) * (256 / fi->mutation_hexjump);
                            fo->progress_cur_increment = 100.0 / fo->progress_cur_limit;

                            if(fi->mutation_model == b1)
                            {
                                sprintf(fo->logstatuscontent, "entering into mutate_bytes_storm() function for %s. Filesize: %lu.", fi->seed_filename, fi->seed_size);
                                build_log_status(fo);
                                mutate_bytes_storm(fi, fo);
                                sprintf(fo->logstatuscontent, "function mutate_bytes_storm() completed for %s. Filesize: %lu.", fi->seed_filename, fi->seed_size);
                                build_log_status(fo);
                            }
                            else if(fi->mutation_model == b2)
                            {
                                sprintf(fo->logstatuscontent, "entering into mutate_bytes_lightning() function for %s. Filesize: %lu.", fi->seed_filename, fi->seed_size);
                                build_log_status(fo);
                                mutate_bytes_lightning(fi, fo);
                                sprintf(fo->logstatuscontent, "function mutate_bytes_lightning() completed for %s. Filesize: %lu.", fi->seed_filename, fi->seed_size);
                                build_log_status(fo);
                            }
                            else if(fi->mutation_model == b4)
                            {
                                sprintf(fo->logstatuscontent, "entering into mutate_bytes_minimalistic() function for %s. Filesize: %lu.", fi->seed_filename, fi->seed_size);
                                build_log_status(fo);
                                mutate_bytes_minimalistic(fi, fo);
                                sprintf(fo->logstatuscontent, "function mutate_bytes_minimalistic() completed for %s. Filesize: %lu.", fi->seed_filename, fi->seed_size);
                                build_log_status(fo);
                            }
                            else if(fi->mutation_model == b16)
                            {
                                sprintf(fo->logstatuscontent, "entering into mutate_bytes_bareminimalistic() function for %s. Filesize: %lu.", fi->seed_filename, fi->seed_size);
                                build_log_status(fo);
                                mutate_bytes_bareminimalistic(fi, fo);
                                sprintf(fo->logstatuscontent, "function mutate_bytes_bareminimalistic() completed for %s. Filesize: %lu.", fi->seed_filename, fi->seed_size);
                                build_log_status(fo);
                            }
                            else if(fi->mutation_model == b32)
                            {
                                sprintf(fo->logstatuscontent, "entering into mutate_bytes_tiny() function for %s. Filesize: %lu.", fi->seed_filename, fi->seed_size);
                                build_log_status(fo);
                                mutate_bytes_tiny(fi, fo);
                                sprintf(fo->logstatuscontent, "function mutate_bytes_tiny() completed for %s. Filesize: %lu.", fi->seed_filename, fi->seed_size);
                                build_log_status(fo);
                            }
                            else if(fi->mutation_model == b64)
                            {
                                sprintf(fo->logstatuscontent, "entering into mutate_bytes_damn_tiny() function for %s. Filesize: %lu.", fi->seed_filename, fi->seed_size);
                                build_log_status(fo);
                                mutate_bytes_damn_tiny(fi, fo);
                                sprintf(fo->logstatuscontent, "function mutate_bytes_damn_tiny() completed for %s. Filesize: %lu.", fi->seed_filename, fi->seed_size);
                                build_log_status(fo);
                            }
                            else if(fi->mutation_model == b128)
                            {
                                sprintf(fo->logstatuscontent, "entering into mutate_bytes_microscopic() function for %s. Filesize: %lu.", fi->seed_filename, fi->seed_size);
                                build_log_status(fo);
                                mutate_bytes_microscopic(fi, fo);
                                sprintf(fo->logstatuscontent, "function mutate_bytes_microscopic() completed for %s. Filesize: %lu.", fi->seed_filename, fi->seed_size);
                                build_log_status(fo);
                            }
                        }

                        else if(!process_seed_file_success)
                        {
                            sprintf(fo->logstatuscontent, "function process_input_file() failed for %s.", fi->seed_filename);
                            build_log_status(fo);
                        }

                        sprintf(fo->logstatuscontent, "seed %llu completed. %s.", fi->total_seeds_completed, fi->seed_filename);
                        build_log_status(fo);

                        fi->total_seeds_completed++;
                        fi->total_seeds_pending--;
                        fo->progress_all_bar += fo->progress_all_increment;

                        free(fi->seed_content);
                        fi->seed_content = NULL;

                        /*
                        sprintf(fo->logstatuscontent, "cooling for 5 seconds.\n");
                        build_log_status(fo);
                        Sleep(1000 * 5);
                        */

                        // check for gprocess_crashed and fi->process_lowmem AFTER cooling
                        // because the lastly created threads might alter these two
                        if(gprocess_crashed)
                        {
                            sprintf(fo->logstatuscontent, "stopped because of process CRASH. for seed file %s. seed size: %lu.", fi->seed_filename, fi->seed_size);
                            build_log_status(fo);
                            printf("stopped because of process CRASH. for seed file %s. seed size: %lu. inside mutate_bytes_bareminimalistic() function.\n", fi->seed_filename, fi->seed_size);
                            goto filesend;
                        }

                        if(fi->process_lowmem)
                        {
                            build_log_screen_lomem(fi, fo);
                            sprintf(fo->logstatuscontent, "stopped because of lowmem. for seed file %s. seed size: %lu.", fi->seed_filename, fi->seed_size);
                            build_log_status(fo);
                            printf("stopped because of lomem. for seed file %s. seed size: %lu. inside mutate_bytes_bareminimalistic() function.\n", fi->seed_filename, fi->seed_size);
                            goto filesend;
                        }

                        DeleteFileA(ffd.cFileName);
                        //ZeroMemory(newfilename, (64 * sizeof(unsigned char)));
                        //sprintf(newfilename, "%s.completed", ffd.cFileName);
                        //MoveFileA(ffd.cFileName, newfilename);
                    }
                }
            }
        }
        while (FindNextFile(hFind, &ffd) != 0);
        FindClose(hFind);
    }


filesend:

    if(gprocess_crashed)
    {
        build_log_screen_crash(fi, fo);
    }
    else if(fi->process_lowmem)
    {
        build_log_screen_lomem(fi, fo);
    }
    else
    {
        build_log_screen_exited_normally(fi, fo);
    }

    printf("waiting for 2 minutes, for all threads to complete.\n");
    sprintf(fo->logstatuscontent, "waiting for 2 minutes, for all threads to complete.");
    build_log_status(fo);
    //Sleep(60000 * 2);
    //Sleep(1000 * 15);


    // check for gprocess_crashed and fi->process_lowmem
    // because the lastly created threads might alter these two
    if(gprocess_crashed)
    {
        build_log_screen_crash(fi, fo);
    }
    else if(fi->process_lowmem)
    {
        build_log_screen_lomem(fi, fo);
    }
    else
    {
        build_log_screen_exited_normally(fi, fo);
    }

    sprintf(fo->logstatuscontent, "fuzz completed.");
    build_log_status(fo);

    sprintf(fo->logstatuscontent, " ran for: %2.0f d, %2.0f h, %2.0f m.",
            fo->performance_time_difference / 86400.0,
            fo->performance_time_difference / 3600.0,
            fmod(fo->performance_time_difference / 60.0, 60.0));


    MoveFileA(fo->logscreenfilename, fo->logfinalscreenfilename);
    MoveFileA(fo->logstatusfilename, fo->logfinalstatusfilename);

    return 0;
}


int process_seed_file(struct fuzzinput *afi, struct fuzzoutput *afo)
{
    sprintf(afo->logstatuscontent, "entered into process_input_file() function.");
    build_log_status(afo);

    int bytes = 0;
    int errorcode = 0;
    int ret = 0;

    HANDLE in;

    in = CreateFileA(afi->seed_filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if(in == -1)
    {
        printf("error in CreateFileA in process_input_file() function in reading seed file %s \n", afi->seed_filename);
        errorcode = GetLastError();
        printf("LastError Code: %d\n", errorcode);
        sprintf(afo->logstatuscontent, "error in CreateFileA in process_input_file() function for seed file %s. LastError Code: %d.",
                afi->seed_filename, errorcode);
        build_log_status(afo);
        return 0;
    }
    sprintf(afo->logstatuscontent, "CreateFileA success in process_input_file() function for seed file %s.", afi->seed_filename);
    build_log_status(afo);

    afi->seed_size = GetFileSize(in, NULL);

    if(afi->seed_size > afi->seed_max_size_allowed_in_bytes)
    {
        //printf("%s is bigger than %lu bytes\n", afi->seed_filename, afi->seed_max_size_allowed_in_bytes);
        return 0;
    }

    afi->seed_content = (unsigned char *) calloc(afi->seed_size, sizeof(unsigned char));

    ret = ReadFile(in, afi->seed_content, afi->seed_size, &bytes, NULL);
    if(ret == 0)
    {
        printf("error in ReadFile for %s\n", afi->seed_filename);
        errorcode = GetLastError();
        printf("LastError Code: %d\n", errorcode);
        sprintf(afo->logstatuscontent, "error in ReadFile in process_input_file() function for seed file %s. LastError Code: %d.",
                afi->seed_filename, errorcode);
        build_log_status(afo);
        return 0;
    }

    sprintf(afo->logstatuscontent, "ReadFile success process_input_file() function for seed file %s.", afi->seed_filename);
    build_log_status(afo);

    CloseHandle(in);

    sprintf(afo->logstatuscontent, "returning from process_input_file() function.");
    build_log_status(afo);

    return 1;
}










int exited_normally()
{
    HANDLE hFile;
    hFile = CreateFileA("exited_normally", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    CloseHandle(hFile);
}

