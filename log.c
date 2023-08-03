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

int build_log_status_first (const struct fuzzinput *afi, struct fuzzoutput *afo);
int build_log_status (struct fuzzoutput *afo);
int build_log_status_crash (unsigned char *astatusline);


int build_log_screen(struct fuzzinput *afi, struct fuzzrunning *afr, struct fuzzoutput *afo);
int build_log_screen_crash(const struct fuzzinput *afi, struct fuzzoutput *afo);
int build_log_screen_lomem(const struct fuzzinput *afi, struct fuzzoutput *afo);


// optimized for speed - since direct indexing is done like : logscreencontent[0], logscreencontent[1]
int build_log_screen(struct fuzzinput *afi, struct fuzzrunning *afr, struct fuzzoutput *afo)
{
    //https://en.wikipedia.org/wiki/Box_Drawing_(Unicode_block)

    GlobalMemoryStatusEx (&afo->statex);

    gprocesspersecond = 0.0;

    QueryPerformanceCounter(&afo->Endtime);
    afo->performance_time_difference = (afo->Endtime.QuadPart - afo->Starttime.QuadPart) / afo->Frequency.QuadPart;

    if(afo->performance_time_difference != 0)
    {
        gprocesspersecond = (double) (afi->overall_saplings_created) / afo->performance_time_difference;
    }


    afo->hlogscreenfile = CreateFileA(afo->logscreenfilename, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS,
                             FILE_ATTRIBUTE_NORMAL, NULL);

    for(logscreen_i = 0; logscreen_i < 34; logscreen_i++)
    {
        ZeroMemory(afo->logscreencontent[logscreen_i], 160);
    }


     sprintf(afo->logscreencontent[0],"%c%c%c \u250c\u2500\u2500\u2500\u2500\u2500 CFuzz v0.40 \u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2510", 0xEF, 0xBB, 0xBF);
     sprintf(afo->logscreencontent[1]," \u2502  %*s       \u2502", 42, afi->title);
     sprintf(afo->logscreencontent[2]," \u251c\u2500\u2500 exits \u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2524");
     sprintf(afo->logscreencontent[3]," \u2502  crashes: %-*u                                      \u2502", 2, gprocess_crashed);
     sprintf(afo->logscreencontent[4]," \u251c\u2500\u2500 progress \u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2524");
     sprintf(afo->logscreencontent[5]," \u2502   overall completed: %6.2f %%                     \u2502", afo->progress_all_bar);
     sprintf(afo->logscreencontent[6]," \u2502       overall to go: %6.2f %%                     \u2502", (100.00 - afo->progress_all_bar));
     sprintf(afo->logscreencontent[7]," \u2502       cur completed: %6.2f %%                     \u2502", afo->progress_cur_bar);
     sprintf(afo->logscreencontent[8]," \u2502           cur to go: %6.2f %%                     \u2502", (100.00 - afo->progress_cur_bar));
     sprintf(afo->logscreencontent[9]," \u251c\u2500\u2500 saplings (current) \u2500\u2500\u2500\u252c\u2500\u2500\u2500\u2500 saplings (overall) \u2500\u2524");

     //aaa
    //afi->saplings_total = 222;
    //afi->overall_saplings_total = afi->saplings_pending = afi->overall_saplings_pending = 222;

    sprintf(afo->logscreencontent[10]," \u2502      total: %-*llu    \u2502      total: %-*llu  \u2502", 8, afi->saplings_total, 10, afi->overall_saplings_total);
    sprintf(afo->logscreencontent[11]," \u2502    created: %-*llu    \u2502    created: %-*llu  \u2502", 8, afi->saplings_created, 10, afi->overall_saplings_created);
    sprintf(afo->logscreencontent[12]," \u2502    pending: %-*llu    \u2502    pending: %-*llu  \u2502", 8, afi->saplings_pending, 10, afi->overall_saplings_pending);
    sprintf(afo->logscreencontent[13]," \u251c\u2500\u2500 process \u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2534\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2524");

    //afi->overall_saplings_created = 222222222;

    sprintf(afo->logscreencontent[14]," \u2502   overall triggers: %-*llu                    \u2502", 10, afi->overall_saplings_created);
    sprintf(afo->logscreencontent[15]," \u2502   current triggers: %-*llu                    \u2502", 10, afi->saplings_created);
    sprintf(afo->logscreencontent[16]," \u2502        max threads: %-*d                         \u2502", 5, afi->max_threads);
    sprintf(afo->logscreencontent[17]," \u2502       wait seconds: %-*d                          \u2502", 4, afi->waitforsingleobject_seconds);

    sprintf(afo->logscreencontent[18]," \u251c\u2500\u2500 seed info \u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u252c\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500 mutation \u2500\u2500\u2524");
    sprintf(afo->logscreencontent[19]," \u2502        seeds: %-*llu         \u2502       start: %-*d    \u2502", 3, afi->total_seeds, 5, afi->mutation_start_offset);
    sprintf(afo->logscreencontent[20]," \u2502    completed: %-*llu         \u2502         end: %-*d    \u2502", 3, afi->total_seeds_completed, 5, afi->mutation_end_offset);
    sprintf(afo->logscreencontent[21]," \u2502      pending: %-*llu         \u2502    location: %-*u    \u2502", 3, afi->total_seeds_pending, 5, afi->mutation_location);
    sprintf(afo->logscreencontent[22]," \u2502       length: %-*lu   \u2502 len: %.5u, type: %-*d  \u2502", 9, afi->seed_size, afi->mutation_length, 2, afi->mutation_model);
    sprintf(afo->logscreencontent[23]," \u251c\u2500\u2500 performance \u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2534\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2524");




    //sprintf(afo->logscreencontent[25]," \u2502    time since running: %5.2f                      \u2502", (((double)clock() - afo->t)/(CLOCKS_PER_SEC))/3600);
    //sprintf(afo->logscreencontent[25]," \u2502    time since running: %5.2fhr,                       \u2502", afo->performance_time_difference / 3600.0);
    /*
    sprintf(afo->logscreencontent[24]," \u2502    time since running: %2.0f d, %2d h, %2.0f m           \u2502",
            afo->performance_time_difference / 86400.0,
            (int)fmod(afo->performance_time_difference / 3600.0, 24.0),
            fmod(afo->performance_time_difference / 60.0, 60.0));
    */

    int days1 = (int)afo->performance_time_difference / 86400.0;
    sprintf(afo->logscreencontent[24]," \u2502    time since running: %2u d, %2d h, %2.0f m           \u2502",
            days1,
            (int)fmod(afo->performance_time_difference / 3600.0, 24.0),
            fmod(afo->performance_time_difference / 60.0, 60.0));

    if(afi->overall_saplings_total != 0 && afi->overall_saplings_created != 0 && afo->performance_time_difference != 0)
    {
    //sprintf(afo->logscreencontent[26]," \u2502            time to go: %5.2f                      \u2502", (((((double)clock() - afo->t)/(CLOCKS_PER_SEC))/3600) / afi->overall_saplings_created) * (afi->overall_saplings_pending));
    //sprintf(afo->logscreencontent[26]," \u2502            time to go: %5.2f                      \u2502", ((afo->performance_time_difference/3600.0) / afi->overall_saplings_created) * (afi->overall_saplings_pending));
    //sprintf(afo->logscreencontent[26]," \u2502            time to go: %3.0f hrs, %2.0f mins           \u2502", ((afo->performance_time_difference/3600.0) / afi->overall_saplings_created) * (afi->overall_saplings_pending), fmod((((afo->performance_time_difference/60.0) / afi->overall_saplings_created) * (afi->overall_saplings_pending)), 60.0));
    /*
    sprintf(afo->logscreencontent[25]," \u2502            time to go: %2.0f d, %2d h, %2.0f m           \u2502",
            ((afo->performance_time_difference/86400.0) / afi->overall_saplings_created) * (afi->overall_saplings_pending),
            (int)fmod((((afo->performance_time_difference/3600.0) / afi->overall_saplings_created) * (afi->overall_saplings_pending)), 24.0),
            fmod((((afo->performance_time_difference/60.0) / afi->overall_saplings_created) * (afi->overall_saplings_pending)), 60.0));
    */

    int days2 = (int)((afo->performance_time_difference/86400.0) / afi->overall_saplings_created) * (afi->overall_saplings_pending);

    sprintf(afo->logscreencontent[25]," \u2502            time to go: %2d d, %2d h, %2.0f m           \u2502",
            days2,
            (int)fmod((((afo->performance_time_difference/3600.0) / afi->overall_saplings_created) * (afi->overall_saplings_pending)), 24.0),
            fmod((((afo->performance_time_difference/60.0) / afi->overall_saplings_created) * (afi->overall_saplings_pending)), 60.0));
    }
    else
    {
    //sprintf(afo->logscreencontent[25]," \u2502            time to go:  %1.0f d, %1.0f h, %1.0f m             \u2502", 0.0, 0.0, 0.0);
    sprintf(afo->logscreencontent[25]," \u2502            time to go:  %1d d, %1.0f h, %1.0f m             \u2502", 0, 0.0, 0.0);
    }

    sprintf(afo->logscreencontent[26]," \u2502             total RAM: %-*I64d                     \u2502", 6, afo->statex.ullTotalPhys/1048576);
    sprintf(afo->logscreencontent[27]," \u2502        total RAM free: %-*I64d                     \u2502", 6, afo->statex.ullAvailPhys/1048576);
    sprintf(afo->logscreencontent[28]," \u2502     no. of processors: %-*d                        \u2502", 3, afo->SysInfo.dwNumberOfProcessors);
    sprintf(afo->logscreencontent[29]," \u2502         exec / second: %-*d                       \u2502", 4, (unsigned int)gprocesspersecond);

    sprintf(afo->logscreencontent[30]," \u251c\u2500\u2500 file & app \u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2524");
    sprintf(afo->logscreencontent[31]," \u2502      app name: %-*s            \u2502", 23, afi->target_application_filename);
    sprintf(afo->logscreencontent[32]," \u2502   app version: %-*s                    \u2502", 15, afi->target_application_version);
    sprintf(afo->logscreencontent[33]," \u2514\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2518");


    for(logscreen_i = 0; logscreen_i < 34; logscreen_i++)
    {
        WriteFile(afo->hlogscreenfile, afo->logscreencontent[logscreen_i], strlen(afo->logscreencontent[logscreen_i]), &logscreen_bytes, NULL);
        WriteFile(afo->hlogscreenfile, newline, 1, &logscreen_bytes, NULL);
    }

    CloseHandle(afo->hlogscreenfile);

}


int build_log_screen_exited_normally(const struct fuzzinput *afi, struct fuzzoutput *afo)
{
    //https://en.wikipedia.org/wiki/Box_Drawing_(Unicode_block)
    QueryPerformanceCounter(&afo->Endtime);
    afo->performance_time_difference = (afo->Endtime.QuadPart - afo->Starttime.QuadPart) / afo->Frequency.QuadPart;


    afo->hlogscreenfile = CreateFileA(afo->logscreenfilename, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS,
                             FILE_ATTRIBUTE_NORMAL, NULL);

    for(logscreen_i = 0; logscreen_i < 37; logscreen_i++)
    {
        ZeroMemory(afo->logscreencontent[logscreen_i], 160);
    }

    GlobalMemoryStatusEx (&afo->statex);
     sprintf(afo->logscreencontent[0],"%c%c%c \u250c\u2500\u2500\u2500\u2500\u2500 CFuzz v0.40 \u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2510", 0xEF, 0xBB, 0xBF);
     sprintf(afo->logscreencontent[1]," \u2502  OVER OVER OVER OVER OVER OVER OVER OVER OVER     \u2502", afi->title);
     sprintf(afo->logscreencontent[2]," \u2502  OVER OVER OVER OVER OVER OVER OVER OVER OVER     \u2502", afi->title);
     sprintf(afo->logscreencontent[3]," \u2502  OVER OVER OVER OVER OVER OVER OVER OVER OVER     \u2502", afi->title);
     sprintf(afo->logscreencontent[4]," \u2502  OVER OVER OVER OVER OVER OVER OVER OVER OVER     \u2502", afi->title);
     sprintf(afo->logscreencontent[5]," \u2502  OVER OVER OVER OVER OVER OVER OVER OVER OVER     \u2502", afi->title);

     sprintf(afo->logscreencontent[6]," \u251c\u2500\u2500 exits \u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2524");
     sprintf(afo->logscreencontent[7]," \u2502  crashes: %-*u                                      \u2502", 2, gprocess_crashed);
     sprintf(afo->logscreencontent[8]," \u251c\u2500\u2500 progress \u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2524");
     sprintf(afo->logscreencontent[9]," \u2502   overall completed: %6.2f %%                     \u2502", afo->progress_all_bar);
     sprintf(afo->logscreencontent[10]," \u2502       overall to go: %6.2f %%                     \u2502", (100.00 - afo->progress_all_bar));
     sprintf(afo->logscreencontent[11]," \u2502       cur completed: %6.2f %%                     \u2502", afo->progress_cur_bar);
     sprintf(afo->logscreencontent[12]," \u2502           cur to go: %6.2f %%                     \u2502", (100.00 - afo->progress_cur_bar));
     sprintf(afo->logscreencontent[13]," \u251c\u2500\u2500 saplings (current) \u2500\u2500\u2500\u252c\u2500\u2500\u2500\u2500 saplings (overall) \u2500\u2524");

    sprintf(afo->logscreencontent[14]," \u2502      total: %-*llu      \u2502      total: %-*llu  \u2502", 6, afi->saplings_total, 10, afi->overall_saplings_total);
    sprintf(afo->logscreencontent[15]," \u2502    created: %-*llu      \u2502    created: %-*llu  \u2502", 6, afi->saplings_created, 10, afi->overall_saplings_created);
    sprintf(afo->logscreencontent[16]," \u2502    pending: %-*llu      \u2502    pending: %-*llu  \u2502", 6, afi->saplings_pending, 10, afi->overall_saplings_pending);
    sprintf(afo->logscreencontent[17]," \u251c\u2500\u2500 process \u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2534\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2524");

    sprintf(afo->logscreencontent[18]," \u2502   overall triggers: %-*llu                    \u2502", 10, afi->overall_saplings_created);
    sprintf(afo->logscreencontent[19]," \u2502   current triggers: %-*llu                    \u2502", 10, afi->saplings_created);
    sprintf(afo->logscreencontent[20]," \u2502        max threads: %-*d                         \u2502", 5, afi->max_threads);
    sprintf(afo->logscreencontent[21]," \u2502       wait seconds: %-*d                          \u2502", 4, afi->waitforsingleobject_seconds);
    sprintf(afo->logscreencontent[22]," \u251c\u2500\u2500 seed info \u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u252c\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500 mutation \u2500\u2500\u2524");
    sprintf(afo->logscreencontent[23]," \u2502        seeds: %-*llu         \u2502       start: %-*d    \u2502", 3, afi->total_seeds, 5, afi->mutation_start_offset);
    sprintf(afo->logscreencontent[24]," \u2502    completed: %-*llu         \u2502         end: %-*d    \u2502", 3, afi->total_seeds_completed, 5, afi->mutation_end_offset);
    sprintf(afo->logscreencontent[25]," \u2502      pending: %-*llu         \u2502    location: %-*u    \u2502", 3, afi->total_seeds_pending, 5, afi->mutation_location);
    sprintf(afo->logscreencontent[26]," \u2502       length: %-*lu   \u2502 len: %.5u, type: %-*d  \u2502", 9, afi->seed_size, afi->mutation_length, 2, afi->mutation_model);
    sprintf(afo->logscreencontent[27]," \u251c\u2500\u2500 performance \u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2534\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2524");

    /*
    sprintf(afo->logscreencontent[28]," \u2502    time since running: %2.0f d, %2.0f h, %2.0f m           \u2502",
            afo->performance_time_difference / 86400.0,
            afo->performance_time_difference / 3600.0,
            fmod(afo->performance_time_difference / 60.0, 60.0));
    */
    int days = (int)afo->performance_time_difference / 86400.0;

    sprintf(afo->logscreencontent[28]," \u2502    time since running: %2u d, %2d h, %2.0f m           \u2502",
            days,
            (int)fmod(afo->performance_time_difference / 3600.0, 24.0),
            fmod(afo->performance_time_difference / 60.0, 60.0));

    sprintf(afo->logscreencontent[29]," \u2502             total RAM: %-*I64d                     \u2502", 6, afo->statex.ullTotalPhys/1048576);
    sprintf(afo->logscreencontent[30]," \u2502        total RAM free: %-*I64d                     \u2502", 6, afo->statex.ullAvailPhys/1048576);
    sprintf(afo->logscreencontent[31]," \u2502     no. of processors: %-*d                        \u2502", 3, afo->SysInfo.dwNumberOfProcessors);
    sprintf(afo->logscreencontent[32]," \u2502         exec / second: %-*d                       \u2502", 4, (unsigned int)gprocesspersecond);

    sprintf(afo->logscreencontent[33]," \u251c\u2500\u2500 file & app \u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2524");
    sprintf(afo->logscreencontent[34]," \u2502      app name: %-*s            \u2502", 23, afi->target_application_filename);
    sprintf(afo->logscreencontent[35]," \u2502   app version: %-*s                    \u2502", 15, afi->target_application_version);
    sprintf(afo->logscreencontent[36]," \u2514\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2518");


    for(logscreen_i = 0; logscreen_i < 37; logscreen_i++)
    {
        WriteFile(afo->hlogscreenfile, afo->logscreencontent[logscreen_i], strlen(afo->logscreencontent[logscreen_i]), &logscreen_bytes, NULL);
        WriteFile(afo->hlogscreenfile, newline, 1, &logscreen_bytes, NULL);
    }

    CloseHandle(afo->hlogscreenfile);
}

int build_log_screen_crash(const struct fuzzinput *afi, struct fuzzoutput *afo)
{
    //https://en.wikipedia.org/wiki/Box_Drawing_(Unicode_block)

    afo->hlogscreenfile = CreateFileA(afo->logscreenfilename, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS,
                             FILE_ATTRIBUTE_NORMAL, NULL);

    for(logscreen_i = 0; logscreen_i < 35; logscreen_i++)
    {
        ZeroMemory(afo->logscreencontent[logscreen_i], 160);
    }


    GlobalMemoryStatusEx (&afo->statex);
     sprintf(afo->logscreencontent[0],"%c%c%c \u250c\u2500\u2500\u2500\u2500\u2500 CFuzz v0.40 \u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2510", 0xEF, 0xBB, 0xBF);
     sprintf(afo->logscreencontent[1]," \u2502  CRASH CRASH CRASH CRASH CRASH CRASH CRASH CRASH  \u2502", afi->title);
     sprintf(afo->logscreencontent[2]," \u2502  CRASH CRASH CRASH CRASH CRASH CRASH CRASH CRASH  \u2502", afi->title);
     sprintf(afo->logscreencontent[3]," \u2502  CRASH CRASH CRASH CRASH CRASH CRASH CRASH CRASH  \u2502", afi->title);
     sprintf(afo->logscreencontent[4]," \u2502  CRASH CRASH CRASH CRASH CRASH CRASH CRASH CRASH  \u2502", afi->title);
     sprintf(afo->logscreencontent[5]," \u2502  CRASH CRASH CRASH CRASH CRASH CRASH CRASH CRASH  \u2502", afi->title);

     sprintf(afo->logscreencontent[6]," \u251c\u2500\u2500 exits \u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2524");
     sprintf(afo->logscreencontent[7]," \u2502  crashes: %-*u                                      \u2502", 2, gprocess_crashed);
     sprintf(afo->logscreencontent[8]," \u251c\u2500\u2500 progress \u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2524");
     sprintf(afo->logscreencontent[9]," \u2502   overall completed: %6.2f %%                     \u2502", afo->progress_all_bar);
     sprintf(afo->logscreencontent[10]," \u2502       overall to go: %6.2f %%                     \u2502", (100.00 - afo->progress_all_bar));
     sprintf(afo->logscreencontent[11]," \u2502       cur completed: %6.2f %%                     \u2502", afo->progress_cur_bar);
     sprintf(afo->logscreencontent[12]," \u2502           cur to go: %6.2f %%                     \u2502", (100.00 - afo->progress_cur_bar));
     sprintf(afo->logscreencontent[13]," \u251c\u2500\u2500 saplings (current) \u2500\u2500\u2500\u252c\u2500\u2500\u2500\u2500 saplings (overall) \u2500\u2524");


    sprintf(afo->logscreencontent[14]," \u2502      total: %-*llu      \u2502      total: %-*llu  \u2502", 6, afi->saplings_total, 10, afi->overall_saplings_total);
    sprintf(afo->logscreencontent[15]," \u2502    created: %-*llu      \u2502    created: %-*llu  \u2502", 6, afi->saplings_created, 10, afi->overall_saplings_created);
    sprintf(afo->logscreencontent[16]," \u2502    pending: %-*llu      \u2502    pending: %-*llu  \u2502", 6, afi->saplings_pending, 10, afi->overall_saplings_pending);
    sprintf(afo->logscreencontent[17]," \u251c\u2500\u2500 process \u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2534\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2524");

    sprintf(afo->logscreencontent[18]," \u2502   overall triggers: %-*llu                    \u2502", 10, afi->overall_saplings_created);
    sprintf(afo->logscreencontent[19]," \u2502   current triggers: %-*llu                    \u2502", 10, afi->saplings_created);
    sprintf(afo->logscreencontent[20]," \u2502        max threads: %-*d                         \u2502", 5, afi->max_threads);
    sprintf(afo->logscreencontent[21]," \u2502       wait seconds: %-*d                          \u2502", 4, afi->waitforsingleobject_seconds);
    sprintf(afo->logscreencontent[22]," \u251c\u2500\u2500 seed info \u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u252c\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500 mutation \u2500\u2500\u2524");
    sprintf(afo->logscreencontent[23]," \u2502        seeds: %-*llu         \u2502       start: %-*d    \u2502", 3, afi->total_seeds, 5, afi->mutation_start_offset);
    sprintf(afo->logscreencontent[24]," \u2502    completed: %-*llu         \u2502         end: %-*d    \u2502", 3, afi->total_seeds_completed, 5, afi->mutation_end_offset);
    sprintf(afo->logscreencontent[25]," \u2502      pending: %-*llu         \u2502    location: %-*u    \u2502", 3, afi->total_seeds_pending, 5, afi->mutation_location);
    sprintf(afo->logscreencontent[26]," \u2502       length: %-*lu   \u2502 len: %.5u, type: %-*d  \u2502", 9, afi->seed_size, afi->mutation_length, 2, afi->mutation_model);
    sprintf(afo->logscreencontent[27]," \u251c\u2500\u2500 performance \u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2534\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2524");

    sprintf(afo->logscreencontent[28]," \u2502    time since running: %2.0f d, %2.0f h, %2.0f m           \u2502",
            afo->performance_time_difference / 86400.0,
            afo->performance_time_difference / 3600.0,
            fmod(afo->performance_time_difference / 60.0, 60.0));

    sprintf(afo->logscreencontent[29]," \u2502             total RAM: %-*I64d                     \u2502", 6, afo->statex.ullTotalPhys/1048576);
    sprintf(afo->logscreencontent[30]," \u2502        total RAM free: %-*I64d                     \u2502", 6, afo->statex.ullAvailPhys/1048576);
    sprintf(afo->logscreencontent[31]," \u2502     no. of processors: %-*d                        \u2502", 3, afo->SysInfo.dwNumberOfProcessors);
    sprintf(afo->logscreencontent[32]," \u2502         exec / second: %-*d                       \u2502", 4, (unsigned int)gprocesspersecond);

    sprintf(afo->logscreencontent[33]," \u251c\u2500\u2500 file & app \u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2524");
    sprintf(afo->logscreencontent[34]," \u2502      app name: %-*s            \u2502", 23, afi->target_application_filename);
    sprintf(afo->logscreencontent[35]," \u2502   app version: %-*s                    \u2502", 15, afi->target_application_version);
    sprintf(afo->logscreencontent[36]," \u2514\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2518");


    for(logscreen_i = 0; logscreen_i < 37; logscreen_i++)
    {
        WriteFile(afo->hlogscreenfile, afo->logscreencontent[logscreen_i], strlen(afo->logscreencontent[logscreen_i]), &logscreen_bytes, NULL);
        WriteFile(afo->hlogscreenfile, newline, 1, &logscreen_bytes, NULL);
    }

    CloseHandle(afo->hlogscreenfile);
}


int build_log_screen_lomem(const struct fuzzinput *afi, struct fuzzoutput *afo)
{
    //https://en.wikipedia.org/wiki/Box_Drawing_(Unicode_block)

    afo->hlogscreenfile = CreateFileA(afo->logscreenfilename, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS,
                             FILE_ATTRIBUTE_NORMAL, NULL);

    for(logscreen_i = 0; logscreen_i < 35; logscreen_i++)
    {
        ZeroMemory(afo->logscreencontent[logscreen_i], 160);
    }


    GlobalMemoryStatusEx (&afo->statex);
     sprintf(afo->logscreencontent[0],"%c%c%c \u250c\u2500\u2500\u2500\u2500\u2500 CFuzz v0.40 \u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2510", 0xEF, 0xBB, 0xBF);
     sprintf(afo->logscreencontent[1]," \u2502  LOMEM LOMEM LOMEM LOMEM LOMEM LOMEM LOMEM LOMEM  \u2502", afi->title);
     sprintf(afo->logscreencontent[2]," \u2502  LOMEM LOMEM LOMEM LOMEM LOMEM LOMEM LOMEM LOMEM  \u2502", afi->title);
     sprintf(afo->logscreencontent[3]," \u2502  LOMEM LOMEM LOMEM LOMEM LOMEM LOMEM LOMEM LOMEM  \u2502", afi->title);
     sprintf(afo->logscreencontent[4]," \u2502  LOMEM LOMEM LOMEM LOMEM LOMEM LOMEM LOMEM LOMEM  \u2502", afi->title);
     sprintf(afo->logscreencontent[5]," \u2502  LOMEM LOMEM LOMEM LOMEM LOMEM LOMEM LOMEM LOMEM  \u2502", afi->title);

     sprintf(afo->logscreencontent[6]," \u251c\u2500\u2500 exits \u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2524");
     sprintf(afo->logscreencontent[7]," \u2502  crashes: %-*u                                      \u2502", 2, gprocess_crashed);
     sprintf(afo->logscreencontent[8]," \u251c\u2500\u2500 progress \u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2524");
     sprintf(afo->logscreencontent[9]," \u2502   overall completed: %6.2f %%                     \u2502", afo->progress_all_bar);
     sprintf(afo->logscreencontent[10]," \u2502       overall to go: %6.2f %%                     \u2502", (100.00 - afo->progress_all_bar));
     sprintf(afo->logscreencontent[11]," \u2502       cur completed: %6.2f %%                     \u2502", afo->progress_cur_bar);
     sprintf(afo->logscreencontent[12]," \u2502           cur to go: %6.2f %%                     \u2502", (100.00 - afo->progress_cur_bar));
     sprintf(afo->logscreencontent[13]," \u251c\u2500\u2500 saplings (current) \u2500\u2500\u2500\u252c\u2500\u2500\u2500\u2500 saplings (overall) \u2500\u2524");

     //aaa
    //afi->saplings_total = 222;
    //afi->overall_saplings_total = afi->saplings_pending = afi->overall_saplings_pending = 222;

    sprintf(afo->logscreencontent[14]," \u2502      total: %-*llu      \u2502      total: %-*llu  \u2502", 6, afi->saplings_total, 10, afi->overall_saplings_total);
    sprintf(afo->logscreencontent[15]," \u2502    created: %-*llu      \u2502    created: %-*llu  \u2502", 6, afi->saplings_created, 10, afi->overall_saplings_created);
    sprintf(afo->logscreencontent[16]," \u2502    pending: %-*llu      \u2502    pending: %-*llu  \u2502", 6, afi->saplings_pending, 10, afi->overall_saplings_pending);
    sprintf(afo->logscreencontent[17]," \u251c\u2500\u2500 process \u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2534\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2524");

    //afi->overall_saplings_created = 222222222;

    sprintf(afo->logscreencontent[18]," \u2502   overall triggers: %-*llu                    \u2502", 10, afi->overall_saplings_created);
    sprintf(afo->logscreencontent[19]," \u2502   current triggers: %-*llu                    \u2502", 10, afi->saplings_created);
    sprintf(afo->logscreencontent[20]," \u2502        max threads: %-*d                         \u2502", 5, afi->max_threads);
    sprintf(afo->logscreencontent[21]," \u2502       wait seconds: %-*d                          \u2502", 4, afi->waitforsingleobject_seconds);
    sprintf(afo->logscreencontent[22]," \u251c\u2500\u2500 seed info \u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u252c\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500 mutation \u2500\u2500\u2524");
    sprintf(afo->logscreencontent[23]," \u2502        seeds: %-*llu         \u2502       start: %-*d    \u2502", 3, afi->total_seeds, 5, afi->mutation_start_offset);
    sprintf(afo->logscreencontent[24]," \u2502    completed: %-*llu         \u2502         end: %-*d    \u2502", 3, afi->total_seeds_completed, 5, afi->mutation_end_offset);
    sprintf(afo->logscreencontent[25]," \u2502      pending: %-*llu         \u2502    location: %-*u    \u2502", 3, afi->total_seeds_pending, 5, afi->mutation_location);
    sprintf(afo->logscreencontent[26]," \u2502       length: %-*lu   \u2502 len: %.5u, type: %-*d  \u2502", 9, afi->seed_size, afi->mutation_length, 2, afi->mutation_model);
    sprintf(afo->logscreencontent[27]," \u251c\u2500\u2500 performance \u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2534\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2524");
    sprintf(afo->logscreencontent[28]," \u2502    time since running: %5.2f                      \u2502", (((double)clock() - afo->t)/(CLOCKS_PER_SEC))/3600);
    sprintf(afo->logscreencontent[29]," \u2502             total RAM: %-*I64d                      \u2502", 5, afo->statex.ullTotalPhys/1048576);
    sprintf(afo->logscreencontent[30]," \u2502        total RAM free: %-*I64d                      \u2502", 5, afo->statex.ullAvailPhys/1048576);

    sprintf(afo->logscreencontent[31]," \u251c\u2500\u2500 file & app \u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2524");
    sprintf(afo->logscreencontent[32]," \u2502      app name: %-*s                    \u2502", 15, afi->target_application_filename);
    sprintf(afo->logscreencontent[33]," \u2502   app version: %-*s                    \u2502", 15, afi->target_application_version);
    sprintf(afo->logscreencontent[34]," \u2514\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2518");


    for(logscreen_i = 0; logscreen_i < 35; logscreen_i++)
    {
        WriteFile(afo->hlogscreenfile, afo->logscreencontent[logscreen_i], strlen(afo->logscreencontent[logscreen_i]), &logscreen_bytes, NULL);
        WriteFile(afo->hlogscreenfile, newline, 1, &logscreen_bytes, NULL);
    }

    CloseHandle(afo->hlogscreenfile);
}



int build_log_status_first(const struct fuzzinput *afi, struct fuzzoutput *afo)
{
    afo->hlogstautusfile = CreateFileA(afo->logstatusfilename, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS,
                             FILE_ATTRIBUTE_NORMAL, NULL);

	ZeroMemory(afo->logstatuscontent, (512 * sizeof(unsigned char)));
    sprintf(afo->logstatuscontent, "__ CFuzz v0.40 ____________________________ \n");
    WriteFile(afo->hlogstautusfile, afo->logstatuscontent, strlen(afo->logstatuscontent), &logscreen_bytes, NULL);

	ZeroMemory(afo->logstatuscontent, (512 * sizeof(unsigned char)));
    sprintf(afo->logstatuscontent, "template details:\n");
    WriteFile(afo->hlogstautusfile, afo->logstatuscontent, strlen(afo->logstatuscontent), &logscreen_bytes, NULL);

    ZeroMemory(afo->logstatuscontent, (512 * sizeof(unsigned char)));
    sprintf(afo->logstatuscontent, "title: %s \n", afi->title);
    WriteFile(afo->hlogstautusfile, afo->logstatuscontent, strlen(afo->logstatuscontent), &logscreen_bytes, NULL);

	ZeroMemory(afo->logstatuscontent, (512 * sizeof(unsigned char)));
    sprintf(afo->logstatuscontent, "total seeds: %llu \n", afi->total_seeds);
	WriteFile(afo->hlogstautusfile, afo->logstatuscontent, strlen(afo->logstatuscontent), &logscreen_bytes, NULL);
	ZeroMemory(afo->logstatuscontent, (512 * sizeof(unsigned char)));
    sprintf(afo->logstatuscontent, "seed type: %s \n", afi->seed_filetype);
	WriteFile(afo->hlogstautusfile, afo->logstatuscontent, strlen(afo->logstatuscontent), &logscreen_bytes, NULL);
	ZeroMemory(afo->logstatuscontent, (512 * sizeof(unsigned char)));
    sprintf(afo->logstatuscontent, "chamber folder: %s \n", afi->chamber_folder_name);
	WriteFile(afo->hlogstautusfile, afo->logstatuscontent, strlen(afo->logstatuscontent), &logscreen_bytes, NULL);
	ZeroMemory(afo->logstatuscontent, (512 * sizeof(unsigned char)));
    sprintf(afo->logstatuscontent, "mutation start offset: %d\n", afi->mutation_start_offset);
	WriteFile(afo->hlogstautusfile, afo->logstatuscontent, strlen(afo->logstatuscontent), &logscreen_bytes, NULL);
	ZeroMemory(afo->logstatuscontent, (512 * sizeof(unsigned char)));
    sprintf(afo->logstatuscontent, "mutation end offset: %d \n", afi->mutation_end_offset);
	WriteFile(afo->hlogstautusfile, afo->logstatuscontent, strlen(afo->logstatuscontent), &logscreen_bytes, NULL);
	ZeroMemory(afo->logstatuscontent, (512 * sizeof(unsigned char)));
    sprintf(afo->logstatuscontent, "mutation length: %u \n", afi->mutation_length);
	WriteFile(afo->hlogstautusfile, afo->logstatuscontent, strlen(afo->logstatuscontent), &logscreen_bytes, NULL);
	ZeroMemory(afo->logstatuscontent, (512 * sizeof(unsigned char)));
    sprintf(afo->logstatuscontent, "mutations per bytes: %d \n", 256 / afi->mutation_hexjump);
	WriteFile(afo->hlogstautusfile, afo->logstatuscontent, strlen(afo->logstatuscontent), &logscreen_bytes, NULL);

	ZeroMemory(afo->logstatuscontent, (512 * sizeof(unsigned char)));

    if(afi->mutation_model == b1)
        sprintf(afo->logstatuscontent,"mutation type: %d - single byte (storm). jump bytes: %d.\n", afi->mutation_type, afi->mutation_hexjump);
    else if(afi->mutation_model == w1)
        sprintf(afo->logstatuscontent,"mutation type: %d - two bytes (storm). jump bytes: %d.\n", afi->mutation_type, afi->mutation_hexjump);
    else if(afi->mutation_model == d1)
        sprintf(afo->logstatuscontent,"mutation type: %d - four bytes (storm). jump bytes: %d.\n", afi->mutation_type, afi->mutation_hexjump);
    else if(afi->mutation_model == b2)
        sprintf(afo->logstatuscontent,"mutation type: %d - single byte (lightning). jump bytes: %d.\n", afi->mutation_type, afi->mutation_hexjump);
    else if(afi->mutation_model == w2)
        sprintf(afo->logstatuscontent,"mutation type: %d - two bytes (lightning). jump bytes: %d.\n", afi->mutation_type, afi->mutation_hexjump);
    else if(afi->mutation_model == d2)
        sprintf(afo->logstatuscontent,"mutation type: %d - four bytes (lightning). jump bytes: %d.\n", afi->mutation_type, afi->mutation_hexjump);
    else if(afi->mutation_model == b4)
        sprintf(afo->logstatuscontent,"mutation type: %d - single byte (minimalistic). jump bytes: %d.\n", afi->mutation_type, afi->mutation_hexjump);
    else if(afi->mutation_model == w4)
        sprintf(afo->logstatuscontent,"mutation type: %d - two bytes (minimalistic). jump bytes: %d.\n", afi->mutation_type, afi->mutation_hexjump);
    else if(afi->mutation_model == d4)
        sprintf(afo->logstatuscontent,"mutation type: %d - four bytes (minimalistic). jump bytes: %d.\n", afi->mutation_type, afi->mutation_hexjump);
    else if(afi->mutation_model == b16)
        sprintf(afo->logstatuscontent,"mutation type: %d - single byte (bare minimalistic). jump bytes: %d.\n", afi->mutation_type, afi->mutation_hexjump);
    else if(afi->mutation_model == w16)
        sprintf(afo->logstatuscontent,"mutation type: %d - two bytes (bare minimalistic). jump bytes: %d.\n", afi->mutation_type, afi->mutation_hexjump);
    else if(afi->mutation_model == d16)
        sprintf(afo->logstatuscontent,"mutation type: %d - four bytes (bare minimalistic). jump bytes: %d.\n", afi->mutation_type, afi->mutation_hexjump);
    else if(afi->mutation_model == b32)
        sprintf(afo->logstatuscontent,"mutation type: %d - single byte (tiny). jump bytes: %d.\n", afi->mutation_type, afi->mutation_hexjump);
    else if(afi->mutation_model == w32)
        sprintf(afo->logstatuscontent,"mutation type: %d - two bytes (tiny). jump bytes: %d.\n", afi->mutation_type, afi->mutation_hexjump);
    else if(afi->mutation_model == d32)
        sprintf(afo->logstatuscontent,"mutation type: %d - four bytes (tiny). jump bytes: %d.\n", afi->mutation_type, afi->mutation_hexjump);
    else if(afi->mutation_model == b64)
        sprintf(afo->logstatuscontent,"mutation type: %d - single byte (damn tiny). jump bytes: %d.\n", afi->mutation_type, afi->mutation_hexjump);
    else if(afi->mutation_model == w64)
        sprintf(afo->logstatuscontent,"mutation type: %d - two bytes (damn tiny). jump bytes: %d.\n", afi->mutation_type, afi->mutation_hexjump);
    else if(afi->mutation_model == d64)
        sprintf(afo->logstatuscontent,"mutation type: %d - four bytes (damn tiny). jump bytes: %d.\n", afi->mutation_type, afi->mutation_hexjump);
    else if(afi->mutation_model == b128)
        sprintf(afo->logstatuscontent,"mutation type: %d - single byte (microscopic). jump bytes: %d.\n", afi->mutation_type, afi->mutation_hexjump);
    else if(afi->mutation_model == w128)
        sprintf(afo->logstatuscontent,"mutation type: %d - two bytes (microscopic). jump bytes: %d.\n", afi->mutation_type, afi->mutation_hexjump);
    else if(afi->mutation_model == d128)
        sprintf(afo->logstatuscontent,"mutation type: %d - four bytes (microscopic). jump bytes: %d.\n", afi->mutation_type, afi->mutation_hexjump);


    WriteFile(afo->hlogstautusfile, afo->logstatuscontent, strlen(afo->logstatuscontent), &logscreen_bytes, NULL);

	ZeroMemory(afo->logstatuscontent, (512 * sizeof(unsigned char)));
    sprintf(afo->logstatuscontent, "overall saplings total: %llu \n", afi->overall_saplings_total);
	WriteFile(afo->hlogstautusfile, afo->logstatuscontent, strlen(afo->logstatuscontent), &logscreen_bytes, NULL);


	ZeroMemory(afo->logstatuscontent, (512 * sizeof(unsigned char)));
    sprintf(afo->logstatuscontent, "max threads: %d \n", afi->max_threads);
	WriteFile(afo->hlogstautusfile, afo->logstatuscontent, strlen(afo->logstatuscontent), &logscreen_bytes, NULL);
	ZeroMemory(afo->logstatuscontent, (512 * sizeof(unsigned char)));

    sprintf(afo->logstatuscontent, "wait seconds: %d \n", afi->waitforsingleobject_seconds);
	WriteFile(afo->hlogstautusfile, afo->logstatuscontent, strlen(afo->logstatuscontent), &logscreen_bytes, NULL);
	ZeroMemory(afo->logstatuscontent, (512 * sizeof(unsigned char)));

    sprintf(afo->logstatuscontent, "process cooling seconds: %u \n", afi->process_cooling_seconds);
	WriteFile(afo->hlogstautusfile, afo->logstatuscontent, strlen(afo->logstatuscontent), &logscreen_bytes, NULL);
	ZeroMemory(afo->logstatuscontent, (512 * sizeof(unsigned char)));

    sprintf(afo->logstatuscontent, "app name: %s \n", afi->target_application_filename);
	WriteFile(afo->hlogstautusfile, afo->logstatuscontent, strlen(afo->logstatuscontent), &logscreen_bytes, NULL);
	ZeroMemory(afo->logstatuscontent, (512 * sizeof(unsigned char)));

	sprintf(afo->logstatuscontent, "app version: %s \n", afi->target_application_version);
	WriteFile(afo->hlogstautusfile, afo->logstatuscontent, strlen(afo->logstatuscontent), &logscreen_bytes, NULL);
	ZeroMemory(afo->logstatuscontent, (512 * sizeof(unsigned char)));

	afo->statex.dwLength = sizeof (afo->statex);
    GlobalMemoryStatusEx (&afo->statex);

    sprintf(afo->logstatuscontent, "total RAM (Mbytes): %*I64d \n", 3, afo->statex.ullTotalPhys/1048576);
	WriteFile(afo->hlogstautusfile, afo->logstatuscontent, strlen(afo->logstatuscontent), &logscreen_bytes, NULL);
	ZeroMemory(afo->logstatuscontent, (512 * sizeof(unsigned char)));
    sprintf(afo->logstatuscontent, "total RAM free (Mbytes): %*I64d \n", 3, afo->statex.ullAvailPhys/1048576);
	WriteFile(afo->hlogstautusfile, afo->logstatuscontent, strlen(afo->logstatuscontent), &logscreen_bytes, NULL);
	ZeroMemory(afo->logstatuscontent, (512 * sizeof(unsigned char)));

    sprintf(afo->logstatuscontent, "no. of processors: %d\n\n", afo->SysInfo.dwNumberOfProcessors);
	WriteFile(afo->hlogstautusfile, afo->logstatuscontent, strlen(afo->logstatuscontent), &logscreen_bytes, NULL);
	ZeroMemory(afo->logstatuscontent, (512 * sizeof(unsigned char)));

    GetLocalTime(&statuslogsystem);

    sprintf(afo->logstatuscontent, "%2d:%2d:%2d - %2d:%2d:%2d -- fuzzing started.\n",
            statuslogsystem.wDay, statuslogsystem.wMonth, statuslogsystem.wYear,
            statuslogsystem.wHour, statuslogsystem.wMinute, statuslogsystem.wSecond);
	WriteFile(afo->hlogstautusfile, afo->logstatuscontent, strlen(afo->logstatuscontent), &logscreen_bytes, NULL);
	ZeroMemory(afo->logstatuscontent, (512 * sizeof(unsigned char)));

    CloseHandle(afo->hlogstautusfile);
}

int build_log_status(struct fuzzoutput *afo)
{
    unsigned char *statusline = (unsigned char *) calloc(512, sizeof(unsigned char));

    int ret = 0;

    afo->hlogstautusfile = CreateFileA(afo->logstatusfilename, FILE_APPEND_DATA, FILE_SHARE_WRITE, NULL, OPEN_EXISTING,
                             FILE_ATTRIBUTE_NORMAL, NULL);

    GetLocalTime(&statuslogsystem);
    sprintf(statusline, "%2d:%2d:%2d - %2d:%2d:%2d -- %s\n",
                    statuslogsystem.wDay, statuslogsystem.wMonth, statuslogsystem.wYear,
                    statuslogsystem.wHour, statuslogsystem.wMinute, statuslogsystem.wSecond, afo->logstatuscontent);
    ret = WriteFile(afo->hlogstautusfile, statusline, strlen(statusline), &logscreen_bytes, NULL);

    ZeroMemory(afo->logstatuscontent, (512 * sizeof(unsigned char)));
    ZeroMemory(statusline, (512 * sizeof(unsigned char)));
    free(statusline);
    statusline = NULL;
    CloseHandle(afo->hlogstautusfile);
}

int build_log_status_crash(unsigned char *astatusline)
{
    unsigned char *statusline = (unsigned char *) calloc(512, sizeof(unsigned char));
    ZeroMemory(statusline, 512);

    int ret = 0;

    HANDLE hlogcrashfile = 0;
    hlogcrashfile = CreateFileA("CRASHED1.INFO", GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);

    if(hlogcrashfile == INVALID_HANDLE_VALUE)
    {
        if(GetLastError() == ERROR_FILE_EXISTS)
        {
            hlogcrashfile = CreateFileA("CRASHED2.INFO", GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
            if(hlogcrashfile == INVALID_HANDLE_VALUE)
            {
                if(GetLastError() == ERROR_FILE_EXISTS)
                {
                    hlogcrashfile = CreateFileA("CRASHED3.INFO", GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
                }
            }
        }
    }

    if(hlogcrashfile == INVALID_HANDLE_VALUE)
    {
        return 0;
    }
    GetLocalTime(&statuslogsystem);
    sprintf(statusline, "%d:%d:%d - %d:%d:%d -- %s\n",
                    statuslogsystem.wDay, statuslogsystem.wMonth, statuslogsystem.wYear,
                    statuslogsystem.wHour, statuslogsystem.wMinute, statuslogsystem.wSecond, astatusline);
        //printf("%s", statusline);
    ret = WriteFile(hlogcrashfile, statusline, strlen(statusline), &logscreen_bytes, NULL);

    ZeroMemory(statusline, (512 * sizeof(unsigned char)));
    free(statusline);
    statusline = NULL;
    CloseHandle(hlogcrashfile);
}


