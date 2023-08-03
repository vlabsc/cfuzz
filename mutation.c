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


int mutate_bytes_storm(struct fuzzinput *afi, struct fuzzoutput *afo);
int mutate_bytes_lightning(struct fuzzinput *afi, struct fuzzoutput *afo);
int mutate_bytes_minimalistic(struct fuzzinput *afi, struct fuzzoutput *afo);
int mutate_bytes_bareminimalistic(struct fuzzinput *afi, struct fuzzoutput *afo);
int mutate_bytes_tiny(struct fuzzinput *afi, struct fuzzoutput *afo);
int mutate_bytes_damn_tiny(struct fuzzinput *afi, struct fuzzoutput *afo);
int mutate_bytes_microscopic(struct fuzzinput *afi, struct fuzzoutput *afo);

int triggerthread(LPVOID lpParam);
int trigger(struct fuzzrunning *afr, struct fuzzoutput *afo);
int resumethisprocess(LPVOID lpParam);
int monitorthisprocess(LPVOID lpParam);

int mutate_bytes_storm(struct fuzzinput *afi, struct fuzzoutput *afo)
{
    sprintf(afo->logstatuscontent, "entered into mutate_bytes_storm() function.");
    build_log_status(afo);

    int i = 0;
    int mutation_values = 0;
    int bytes = 0;
    HANDLE out;
    int errorcode = 0;
    gthreadscreated = 0;
    HANDLE threadcreated = 0;

    struct fuzzrunning *afr = (struct fuzzrunning *) calloc(1, sizeof(struct fuzzrunning));
    ZeroMemory(afr, (1 * sizeof(struct fuzzrunning)));

    unsigned char *mcontent = (unsigned char *) calloc(afi->seed_size, sizeof(unsigned char));
    afr->process_commandline_name = (unsigned char *) calloc(384, sizeof(unsigned char));

    afr->mutated_input_filename = (unsigned char *) calloc(256, 1);

    afr->process_current_directory = (unsigned char *) calloc(128, 1);
    GetCurrentDirectory(127, afr->process_current_directory);

    afr->process_cooling_seconds = afi->process_cooling_seconds;

    afr->process_crashed = 0;
    afr->process_clean_exits = 0;
    afr->process_safe_exits = 0;
    afr->process_non_safe_exits = 0;

    afr->continue_debugging = 0;
    afr->exception_occured = 0;
    afr->process_lowmem = 0;

    ZeroMemory(&afr->si, sizeof(afr->si));
    afr->si.dwFlags = afr->si.dwFlags | STARTF_USESHOWWINDOW;
    afr->si.wShowWindow = SW_HIDE;
    afr->si.cb = sizeof(afr->si);
    ZeroMemory(&afr->pi, sizeof(afr->pi));

    gprocscreated = 0;
    gprocessoraffinitymask = 1;
    gprocessoraffinitymaskshift = 1;


    build_log_screen(afi, afr, afo);

    for(i = 0; i< afi->seed_size; i++)
    {
        mcontent[i] = afi->seed_content[i];
    }

    for(afi->mutation_location = afi->mutation_start_offset; afi->mutation_location <= afi->mutation_end_offset; afi->mutation_location++)
    {
        for(mutation_values = 0; mutation_values < 256; mutation_values+=afi->mutation_hexjump)
        {
            ZeroMemory(afr->mutated_input_filename, 256);
            sprintf(afr->mutated_input_filename,"%s\\forest\\%.4X_%.2X.%s", afr->process_current_directory, afi->mutation_location, mutation_values, afi->seed_fileextension);

            ZeroMemory(h_mutated_input_filename[gthreadscreated], 256);
            sprintf(h_mutated_input_filename[gthreadscreated],"%s\\forest\\%.4X_%.2X.%s", afr->process_current_directory, afi->mutation_location, mutation_values, afi->seed_fileextension);

            out = CreateFileA(afr->mutated_input_filename, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
            if(out == -1)
            {
                printf("error in CreateFileA in mutate_bytes_storm() function for mutation file %s.\n", afr->mutated_input_filename);
                errorcode = GetLastError();
                printf("LastError Code: %d\n", errorcode);
                sprintf(afo->logstatuscontent, "error in CreateFileA in mutate_bytes_storm() function for mutation file %s. LastError Code: %d.",
                        afr->mutated_input_filename, errorcode);
                build_log_status(afo);
                return -1;
            }
            mcontent[afi->mutation_location] = mutation_values;
            WriteFile(out, mcontent, afi->seed_size, &bytes, NULL);
            CloseHandle(out);

            ZeroMemory(afr->process_commandline_name, sizeof(unsigned char) * 384);
            sprintf(afr->process_commandline_name, "%s %s", afi->target_application_filename, afr->mutated_input_filename);

            trigger(afr, afo);

            gthreadscreated = gprocscreated;
        }

comeout:
        //gthreadscreated = gprocscreated;

        // update only after one mutation location is completed
        afi->saplings_created += (256 / afi->mutation_hexjump);
        afi->saplings_pending -= (256 / afi->mutation_hexjump);
        afi->overall_saplings_created += (256 / afi->mutation_hexjump);
        afi->overall_saplings_pending -= (256 / afi->mutation_hexjump);

        afi->process_clean_exits = afr->process_clean_exits;
        afi->process_safe_exits = afr->process_safe_exits;
        afi->process_non_safe_exits = afr->process_non_safe_exits;

        afo->progress_cur_bar += (afo->progress_cur_increment * (256.0 / afi->mutation_hexjump));

        GlobalMemoryStatusEx (&afo->statex);
        if((afo->statex.ullAvailPhys/1048576) < 1000)
        {
            sprintf(afo->logstatuscontent, "LOMEM in mutate_bytes_storm() function. cooling for 30 seconds.");
            build_log_status(afo);
            Sleep(30 * 1000);
        }

        if(gthreadscreated >= afi->max_threads)
        {
            gthreadsrunning = gthreadscreated;
            gthreadscreated = 0;
            gprocscreated = 0;

            while(gprocscreated < afi->max_threads)
            {
                threadcreated = CreateThread(NULL, 0, monitorthisprocess, gprocscreated, 0, NULL);
                CloseHandle(threadcreated);
                gprocscreated++;
            }
            gprocscreated = 0;

            while(gthreadsrunning)
            {
                Sleep(100);
            }

            gprocscreated = 0;
            while(gprocscreated < afi->max_threads)
            {
                //printf("deleted: %s. \n", h_mutated_input_filename[gprocscreated]);
                if(!gprocess_crashed)
                {
                    DeleteFileA(h_mutated_input_filename[gprocscreated]);
                }
                gprocscreated++;
            }
            gprocscreated = 0;

            build_log_screen(afi, afr, afo);
        }


        if(gprocess_crashed)
        {
            build_log_screen_crash(afi, afo);
            sprintf(afo->logstatuscontent, "stopped because of process CRASH. inside mutate_bytes_storm() function.");
            build_log_status(afo);
            printf("stopped because of process CRASH. inside mutate_bytes_storm() function.\n");
            goto loopend;
        }
        if(afr->process_lowmem > 3)
        {
            sprintf(afo->logstatuscontent, "stopped because of low mem. inside mutate_bytes_storm() function.");
            build_log_status(afo);
            printf("stopped because of low mem. inside mutate_bytes_storm() function.");
            afi->process_lowmem = afr->process_lowmem;
            goto loopend;
        }
        mcontent[afi->mutation_location] = afi->seed_content[afi->mutation_location];
    }

    // doing this, so that it looks correct in log screen exited normally function
    afi->mutation_location--;

    gthreadsrunning = gthreadscreated;
    gprocscreated = 0;
    while(gprocscreated < gthreadscreated)
    {
        threadcreated = CreateThread(NULL, 0, monitorthisprocess, gprocscreated, 0, NULL);
        CloseHandle(threadcreated);
        gprocscreated++;
    }
    gprocscreated = 0;

    while(gthreadsrunning)
    {
        Sleep(100);
    }

    gprocscreated = 0;
    while(gprocscreated < gthreadscreated)
    {
        if(!gprocess_crashed)
        {
            DeleteFileA(h_mutated_input_filename[gprocscreated]);
        }
        gprocscreated++;
    }
    gprocscreated = 0;

    build_log_screen(afi, afr, afo);



loopend:

    free(mcontent);
    mcontent = NULL;

    sprintf(afo->logstatuscontent, "returning from mutate_bytes_storm() function.");
    build_log_status(afo);

    return 1;
}

int mutate_bytes_lightning(struct fuzzinput *afi, struct fuzzoutput *afo)
{
    sprintf(afo->logstatuscontent, "entered into mutate_bytes_lightning() function.");
    build_log_status(afo);

    int i = 0;
    int mutation_values = 0;
    int bytes = 0;
    HANDLE out;
    int errorcode = 0;
    gthreadscreated = 0;
    HANDLE threadcreated = 0;

    struct fuzzrunning *afr = (struct fuzzrunning *) calloc(1, sizeof(struct fuzzrunning));
    ZeroMemory(afr, (1 * sizeof(struct fuzzrunning)));

    unsigned char *mcontent = (unsigned char *) calloc(afi->seed_size, sizeof(unsigned char));
    afr->process_commandline_name = (unsigned char *) calloc(384, sizeof(unsigned char));

    afr->mutated_input_filename = (unsigned char *) calloc(256, 1);

    afr->process_current_directory = (unsigned char *) calloc(128, 1);
    GetCurrentDirectory(127, afr->process_current_directory);

    afr->process_cooling_seconds = afi->process_cooling_seconds;

    afr->process_crashed = 0;
    afr->process_clean_exits = 0;
    afr->process_safe_exits = 0;
    afr->process_non_safe_exits = 0;

    afr->continue_debugging = 0;
    afr->exception_occured = 0;
    afr->process_lowmem = 0;

    gprocscreated = 0;

    build_log_screen(afi, afr, afo);

    for(i = 0; i< afi->seed_size; i++)
    {
        mcontent[i] = afi->seed_content[i];
    }

    for(afi->mutation_location = afi->mutation_start_offset; afi->mutation_location <= afi->mutation_end_offset; afi->mutation_location++)
    {
        for(mutation_values = 0; mutation_values < 256; mutation_values+=afi->mutation_hexjump)
        {
            ZeroMemory(afr->mutated_input_filename, 256);
            sprintf(afr->mutated_input_filename,"%s\\forest\\%.4X_%.2X.%s", afr->process_current_directory, afi->mutation_location, mutation_values, afi->seed_fileextension);

            ZeroMemory(h_mutated_input_filename[gthreadscreated], 256);
            sprintf(h_mutated_input_filename[gthreadscreated],"%s\\forest\\%.4X_%.2X.%s", afr->process_current_directory, afi->mutation_location, mutation_values, afi->seed_fileextension);

            out = CreateFileA(afr->mutated_input_filename, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
            if(out == -1)
            {
                printf("error in CreateFileA in mutate_bytes_lightning() function for mutation file %s.\n", afr->mutated_input_filename);
                errorcode = GetLastError();
                printf("LastError Code: %d\n", errorcode);
                sprintf(afo->logstatuscontent, "error in CreateFileA in mutate_bytes_lightning() function for mutation file %s. LastError Code: %d.",
                        afr->mutated_input_filename, errorcode);
                build_log_status(afo);
                return -1;
            }
            mcontent[afi->mutation_location] = mutation_values;
            WriteFile(out, mcontent, afi->seed_size, &bytes, NULL);
            CloseHandle(out);

            ZeroMemory(afr->process_commandline_name, sizeof(unsigned char) * 384);
            sprintf(afr->process_commandline_name, "%s %s", afi->target_application_filename, afr->mutated_input_filename);

            trigger(afr, afo);

            gthreadscreated = gprocscreated;
        }

comeout:
        // update only after one mutation location is completed
        afi->saplings_created += (256 / afi->mutation_hexjump);
        afi->saplings_pending -= (256 / afi->mutation_hexjump);
        afi->overall_saplings_created += (256 / afi->mutation_hexjump);
        afi->overall_saplings_pending -= (256 / afi->mutation_hexjump);

        afi->process_clean_exits = afr->process_clean_exits;
        afi->process_safe_exits = afr->process_safe_exits;
        afi->process_non_safe_exits = afr->process_non_safe_exits;

        afo->progress_cur_bar += (afo->progress_cur_increment * (256.0 / afi->mutation_hexjump));

        GlobalMemoryStatusEx (&afo->statex);
        if((afo->statex.ullAvailPhys/1048576) < 1000)
        {
            sprintf(afo->logstatuscontent, "LOMEM in mutate_bytes_lightning() function. cooling for 30 seconds.");
            build_log_status(afo);
            Sleep(30 * 1000);
        }

        if(gthreadscreated >= afi->max_threads)
        {
            //build_log_screen(afi, afr, afo);
            gthreadsrunning = gthreadscreated;
            gthreadscreated = 0;

            gprocscreated = 0;
            while(gprocscreated < afi->max_threads)
            {
                threadcreated = CreateThread(NULL, 0, monitorthisprocess, gprocscreated, 0, NULL);
                CloseHandle(threadcreated);
                gprocscreated++;
            }
            gprocscreated = 0;

            while(gthreadsrunning)
            {
                Sleep(100);
            }

            gprocscreated = 0;
            while(gprocscreated < afi->max_threads)
            {
                if(!gprocess_crashed)
                {
                    DeleteFileA(h_mutated_input_filename[gprocscreated]);
                }
                gprocscreated++;
            }
            gprocscreated = 0;
            build_log_screen(afi, afr, afo);
        }


        if(gprocess_crashed)
        {
            build_log_screen_crash(afi, afo);
            sprintf(afo->logstatuscontent, "stopped because of process CRASH. inside mutate_bytes_lightning() function.");
            build_log_status(afo);
            printf("stopped because of process CRASH. inside mutate_bytes_lightning() function.\n");
            goto loopend;
        }
        if(afr->process_lowmem > 3)
        {
            sprintf(afo->logstatuscontent, "stopped because of low mem. inside mutate_bytes_lightning() function.");
            build_log_status(afo);
            printf("stopped because of low mem. inside mutate_bytes_lightning() function.");
            afi->process_lowmem = afr->process_lowmem;
            goto loopend;
        }

        mcontent[afi->mutation_location] = afi->seed_content[afi->mutation_location];
    }

    // doing this, so that it looks correct in log screen exited normally function
    afi->mutation_location--;

    gthreadsrunning = gthreadscreated;
    gprocscreated = 0;
    while(gprocscreated < gthreadscreated)
    {
        threadcreated = CreateThread(NULL, 0, monitorthisprocess, gprocscreated, 0, NULL);
        CloseHandle(threadcreated);
        gprocscreated++;
    }
    gprocscreated = 0;

    while(gthreadsrunning)
    {
        Sleep(100);
    }

    gprocscreated = 0;
    while(gprocscreated < gthreadscreated)
    {
        if(!gprocess_crashed)
        {
            DeleteFileA(h_mutated_input_filename[gprocscreated]);
        }
        gprocscreated++;
    }
    gprocscreated = 0;

    build_log_screen(afi, afr, afo);



loopend:

    free(mcontent);
    mcontent = NULL;

    sprintf(afo->logstatuscontent, "returning from mutate_bytes_lightning() function.");
    build_log_status(afo);

    return 1;
}

int mutate_bytes_minimalistic(struct fuzzinput *afi, struct fuzzoutput *afo)
{
    sprintf(afo->logstatuscontent, "entered into mutate_bytes_minimalistic() function.");
    build_log_status(afo);

    int i = 0;
    int mutation_values = 0;
    int bytes = 0;
    HANDLE out;
    int errorcode = 0;
    gthreadscreated = 0;
    HANDLE threadcreated = 0;

    struct fuzzrunning *afr = (struct fuzzrunning *) calloc(1, sizeof(struct fuzzrunning));
    ZeroMemory(afr, (1 * sizeof(struct fuzzrunning)));

    unsigned char *mcontent = (unsigned char *) calloc(afi->seed_size, sizeof(unsigned char));
    afr->process_commandline_name = (unsigned char *) calloc(384, sizeof(unsigned char));

    afr->mutated_input_filename = (unsigned char *) calloc(256, 1);

    afr->process_current_directory = (unsigned char *) calloc(128, 1);
    GetCurrentDirectory(127, afr->process_current_directory);

    afr->process_cooling_seconds = afi->process_cooling_seconds;

    afr->process_crashed = 0;
    afr->process_clean_exits = 0;
    afr->process_safe_exits = 0;
    afr->process_non_safe_exits = 0;

    afr->continue_debugging = 0;
    afr->exception_occured = 0;
    afr->process_lowmem = 0;

    gprocscreated = 0;

    build_log_screen(afi, afr, afo);

    for(i = 0; i< afi->seed_size; i++)
    {
        mcontent[i] = afi->seed_content[i];
    }

    for(afi->mutation_location = afi->mutation_start_offset; afi->mutation_location <= afi->mutation_end_offset; afi->mutation_location++)
    {
        for(mutation_values = 0; mutation_values < 256; mutation_values+=afi->mutation_hexjump)
        {
            ZeroMemory(afr->mutated_input_filename, 256);
            sprintf(afr->mutated_input_filename,"%s\\forest\\%.4X_%.2X.%s", afr->process_current_directory, afi->mutation_location, mutation_values, afi->seed_fileextension);

            ZeroMemory(h_mutated_input_filename[gthreadscreated], 256);
            sprintf(h_mutated_input_filename[gthreadscreated],"%s\\forest\\%.4X_%.2X.%s", afr->process_current_directory, afi->mutation_location, mutation_values, afi->seed_fileextension);

            out = CreateFileA(afr->mutated_input_filename, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
            if(out == -1)
            {
                printf("error in CreateFileA in mutate_bytes_minimalistic() function for mutation file %s.\n", afr->mutated_input_filename);
                errorcode = GetLastError();
                printf("LastError Code: %d\n", errorcode);
                sprintf(afo->logstatuscontent, "error in CreateFileA in mutate_bytes_minimalistic() function for mutation file %s. LastError Code: %d.",
                        afr->mutated_input_filename, errorcode);
                build_log_status(afo);
                return -1;
            }
            mcontent[afi->mutation_location] = mutation_values;
            WriteFile(out, mcontent, afi->seed_size, &bytes, NULL);
            CloseHandle(out);

            ZeroMemory(afr->process_commandline_name, sizeof(unsigned char) * 384);
            sprintf(afr->process_commandline_name, "%s %s", afi->target_application_filename, afr->mutated_input_filename);

            trigger(afr, afo);

            gthreadscreated = gprocscreated;
        }

comeout:
        // update only after one mutation location is completed
        afi->saplings_created += (256 / afi->mutation_hexjump);
        afi->saplings_pending -= (256 / afi->mutation_hexjump);
        afi->overall_saplings_created += (256 / afi->mutation_hexjump);
        afi->overall_saplings_pending -= (256 / afi->mutation_hexjump);

        afi->process_clean_exits = afr->process_clean_exits;
        afi->process_safe_exits = afr->process_safe_exits;
        afi->process_non_safe_exits = afr->process_non_safe_exits;

        afo->progress_cur_bar += (afo->progress_cur_increment * (256.0 / afi->mutation_hexjump));

        GlobalMemoryStatusEx (&afo->statex);
        if((afo->statex.ullAvailPhys/1048576) < 1000)
        {
            sprintf(afo->logstatuscontent, "LOMEM in mutate_bytes_minimalistic() function. cooling for 30 seconds.");
            build_log_status(afo);
            Sleep(30 * 1000);
        }

        if(gthreadscreated >= afi->max_threads)
        {
            //build_log_screen(afi, afr, afo);
            gthreadsrunning = gthreadscreated;
            gthreadscreated = 0;

            gprocscreated = 0;
            while(gprocscreated < afi->max_threads)
            {
                threadcreated = CreateThread(NULL, 0, monitorthisprocess, gprocscreated, 0, NULL);
                CloseHandle(threadcreated);
                gprocscreated++;
            }
            gprocscreated = 0;

            while(gthreadsrunning)
            {
                Sleep(100);
            }

            gprocscreated = 0;
            while(gprocscreated < afi->max_threads)
            {
                if(!gprocess_crashed)
                {
                    DeleteFileA(h_mutated_input_filename[gprocscreated]);
                }
                gprocscreated++;
            }
            gprocscreated = 0;
            build_log_screen(afi, afr, afo);
        }

        if(gprocess_crashed)
        {
            build_log_screen_crash(afi, afo);
            sprintf(afo->logstatuscontent, "stopped because of process CRASH. inside mutate_bytes_minimalistic() function.");
            build_log_status(afo);
            printf("stopped because of process CRASH. inside mutate_bytes_minimalistic() function.\n");
            goto loopend;
        }
        if(afr->process_lowmem > 3)
        {
            sprintf(afo->logstatuscontent, "stopped because of low mem. inside mutate_bytes_minimalistic() function.");
            build_log_status(afo);
            printf("stopped because of low mem. inside mutate_bytes_minimalistic() function.");
            afi->process_lowmem = afr->process_lowmem;
            goto loopend;
        }

        mcontent[afi->mutation_location] = afi->seed_content[afi->mutation_location];
    }
    // doing this, so that it looks correct in log screen exited normally function
    afi->mutation_location--;

    gthreadsrunning = gthreadscreated;
    gprocscreated = 0;
    while(gprocscreated < gthreadscreated)
    {
        threadcreated = CreateThread(NULL, 0, monitorthisprocess, gprocscreated, 0, NULL);
        CloseHandle(threadcreated);
        gprocscreated++;
    }
    gprocscreated = 0;

    while(gthreadsrunning)
    {
        Sleep(100);
    }

    gprocscreated = 0;
    while(gprocscreated < gthreadscreated)
    {
        if(!gprocess_crashed)
        {
            DeleteFileA(h_mutated_input_filename[gprocscreated]);
        }
        gprocscreated++;
    }
    gprocscreated = 0;

    build_log_screen(afi, afr, afo);



loopend:

    free(mcontent);
    mcontent = NULL;

    sprintf(afo->logstatuscontent, "returning from mutate_bytes_minimalistic() function.");
    build_log_status(afo);

    return 1;
}


int mutate_bytes_bareminimalistic(struct fuzzinput *afi, struct fuzzoutput *afo)
{
    sprintf(afo->logstatuscontent, "entered into mutate_bytes_bareminimalistic() function.");
    build_log_status(afo);

    int i = 0;
    int mutation_values = 0;
    int bytes = 0;
    HANDLE out;
    int errorcode = 0;
    gthreadscreated = 0;
    HANDLE threadcreated = 0;

    unsigned char *mcontent = (unsigned char *) calloc(afi->seed_size, sizeof(unsigned char));

    struct fuzzrunning *afr = (struct fuzzrunning *) calloc(1, sizeof(struct fuzzrunning));
    ZeroMemory(afr, (1 * sizeof(struct fuzzrunning)));

    afr->process_commandline_name = (unsigned char *) calloc(384, sizeof(unsigned char));
    afr->mutated_input_filename = (unsigned char *) calloc(256, 1);

    afr->process_current_directory = (unsigned char *) calloc(128, 1);
    GetCurrentDirectory(127, afr->process_current_directory);

    afr->process_cooling_seconds = afi->process_cooling_seconds;

    afr->process_crashed = 0;
    afr->process_clean_exits = 0;
    afr->process_safe_exits = 0;
    afr->process_non_safe_exits = 0;

    afr->continue_debugging = 0;
    afr->exception_occured = 0;
    afr->process_lowmem = 0;

    gprocscreated = 0;

    build_log_screen(afi, afr, afo);

    for(i = 0; i< afi->seed_size; i++)
    {
        mcontent[i] = afi->seed_content[i];
    }

    for(afi->mutation_location = afi->mutation_start_offset; afi->mutation_location <= afi->mutation_end_offset; afi->mutation_location++)
    {
        for(mutation_values = 0; mutation_values < 256; mutation_values+=afi->mutation_hexjump)
        {
            ZeroMemory(afr->mutated_input_filename, 256);
            sprintf(afr->mutated_input_filename,"%s\\forest\\%.4X_%.2X.%s", afr->process_current_directory, afi->mutation_location, mutation_values, afi->seed_fileextension);

            ZeroMemory(h_mutated_input_filename[gthreadscreated], 256);
            sprintf(h_mutated_input_filename[gthreadscreated],"%s\\forest\\%.4X_%.2X.%s", afr->process_current_directory, afi->mutation_location, mutation_values, afi->seed_fileextension);

            out = CreateFileA(afr->mutated_input_filename, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
            if(out == -1)
            {
                printf("error in CreateFileA in mutate_bytes_bareminimalistic() function for mutation file %s.\n", afr->mutated_input_filename);
                errorcode = GetLastError();
                printf("LastError Code: %d\n", errorcode);
                sprintf(afo->logstatuscontent, "error in CreateFileA in mutate_bytes_bareminimalistic() function for mutation file %s. LastError Code: %d.",
                        afr->mutated_input_filename, errorcode);
                build_log_status(afo);
                return -1;
            }
            mcontent[afi->mutation_location] = mutation_values;
            WriteFile(out, mcontent, afi->seed_size, &bytes, NULL);
            CloseHandle(out);

            ZeroMemory(afr->process_commandline_name, sizeof(unsigned char) * 384);
            sprintf(afr->process_commandline_name, "%s %s", afi->target_application_filename, afr->mutated_input_filename);

            trigger(afr, afo);

            gthreadscreated = gprocscreated;
        }

comeout:

        // update only after one mutation location is completed
        afi->saplings_created += (256 / afi->mutation_hexjump);
        afi->saplings_pending -= (256 / afi->mutation_hexjump);
        afi->overall_saplings_created += (256 / afi->mutation_hexjump);
        afi->overall_saplings_pending -= (256 / afi->mutation_hexjump);

        afi->process_clean_exits = afr->process_clean_exits;
        afi->process_safe_exits = afr->process_safe_exits;
        afi->process_non_safe_exits = afr->process_non_safe_exits;

        afo->progress_cur_bar += (afo->progress_cur_increment * (256.0 / afi->mutation_hexjump));

        GlobalMemoryStatusEx (&afo->statex);
        if((afo->statex.ullAvailPhys/1048576) < 1000)
        {
            sprintf(afo->logstatuscontent, "LOMEM in mutate_bytes_bareminimalistic() function. cooling for 30 seconds.");
            build_log_status(afo);
            Sleep(30 * 1000);
        }

        if(gthreadscreated >= afi->max_threads)
        {
            //build_log_screen(afi, afr, afo);
            EnterCriticalSection(&critical_section);
            gthreadsrunning = gthreadscreated;
            LeaveCriticalSection(&critical_section);
            gthreadscreated = 0;

            gprocscreated = 0;
            while(gprocscreated < afi->max_threads)
            {
                threadcreated = CreateThread(NULL, 0, monitorthisprocess, gprocscreated, 0, NULL);
                CloseHandle(threadcreated);
                gprocscreated++;
            }

            gprocscreated = 0;

            while(gthreadsrunning)
            {
                Sleep(100);
            }

            gprocscreated = 0;
            while(gprocscreated < afi->max_threads)
            {
                if(!gprocess_crashed)
                {
                    DeleteFileA(h_mutated_input_filename[gprocscreated]);
                }
                gprocscreated++;
            }
            gprocscreated = 0;
            build_log_screen(afi, afr, afo);
        }

        if(gprocess_crashed)
        {
            build_log_screen_crash(afi, afo);
            sprintf(afo->logstatuscontent, "stopped because of process CRASH. inside mutate_bytes_bareminimalistic() function.");
            build_log_status(afo);
            printf("stopped because of process CRASH. inside mutate_bytes_bareminimalistic() function.\n");
            goto loopend;
        }
        if(afr->process_lowmem > 3)
        {
            sprintf(afo->logstatuscontent, "stopped because of low mem. inside mutate_bytes_bareminimalistic() function.");
            build_log_status(afo);
            printf("stopped because of low mem. inside mutate_bytes_bareminimalistic() function.");
            afi->process_lowmem = afr->process_lowmem;
            goto loopend;
        }

        mcontent[afi->mutation_location] = afi->seed_content[afi->mutation_location];
    }
    // doing this, so that it looks correct in log screen exited normally function
    afi->mutation_location--;

    gthreadsrunning = gthreadscreated;
    gprocscreated = 0;
    while(gprocscreated < gthreadscreated)
    {
        threadcreated = CreateThread(NULL, 0, monitorthisprocess, gprocscreated, 0, NULL);
        CloseHandle(threadcreated);
        gprocscreated++;
    }
    gprocscreated = 0;

    while(gthreadsrunning)
    {
        Sleep(100);
    }

    gprocscreated = 0;
    while(gprocscreated < gthreadscreated)
    {
        if(!gprocess_crashed)
        {
            DeleteFileA(h_mutated_input_filename[gprocscreated]);
        }
        gprocscreated++;
    }

    build_log_screen(afi, afr, afo);

loopend:

    free(mcontent);
    mcontent = NULL;

    sprintf(afo->logstatuscontent, "returning from mutate_bytes_bareminimalistic() function.");
    build_log_status(afo);

    return 1;
}

int mutate_bytes_tiny(struct fuzzinput *afi, struct fuzzoutput *afo)
{
    sprintf(afo->logstatuscontent, "entered into mutate_bytes_tiny() function.");
    build_log_status(afo);

    int i = 0;
    int mutation_values = 0;
    int bytes = 0;
    HANDLE out;
    int errorcode = 0;
    gthreadscreated = 0;
    HANDLE threadcreated = 0;

    struct fuzzrunning *afr = (struct fuzzrunning *) calloc(1, sizeof(struct fuzzrunning));
    ZeroMemory(afr, (1 * sizeof(struct fuzzrunning)));

    unsigned char *mcontent = (unsigned char *) calloc(afi->seed_size, sizeof(unsigned char));
    afr->process_commandline_name = (unsigned char *) calloc(384, sizeof(unsigned char));

    afr->mutated_input_filename = (unsigned char *) calloc(256, 1);

    afr->process_current_directory = (unsigned char *) calloc(128, 1);
    GetCurrentDirectory(127, afr->process_current_directory);

    afr->process_cooling_seconds = afi->process_cooling_seconds;

    afr->process_crashed = 0;
    afr->process_clean_exits = 0;
    afr->process_safe_exits = 0;
    afr->process_non_safe_exits = 0;

    afr->continue_debugging = 0;
    afr->exception_occured = 0;
    afr->process_lowmem = 0;

    gprocscreated = 0;

    build_log_screen(afi, afr, afo);

    for(i = 0; i< afi->seed_size; i++)
    {
        mcontent[i] = afi->seed_content[i];
    }

    for(afi->mutation_location = afi->mutation_start_offset; afi->mutation_location <= afi->mutation_end_offset; afi->mutation_location++)
    {
        for(mutation_values = 0; mutation_values < 256; mutation_values+=afi->mutation_hexjump)
        {
            ZeroMemory(afr->mutated_input_filename, 256);
            sprintf(afr->mutated_input_filename,"%s\\forest\\%.4X_%.2X.%s", afr->process_current_directory, afi->mutation_location, mutation_values, afi->seed_fileextension);

            ZeroMemory(h_mutated_input_filename[gthreadscreated], 256);
            sprintf(h_mutated_input_filename[gthreadscreated],"%s\\forest\\%.4X_%.2X.%s", afr->process_current_directory, afi->mutation_location, mutation_values, afi->seed_fileextension);

            out = CreateFileA(afr->mutated_input_filename, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
            if(out == -1)
            {
                printf("error in CreateFileA in mutate_bytes_tiny() function for mutation file %s.\n", afr->mutated_input_filename);
                errorcode = GetLastError();
                printf("LastError Code: %d\n", errorcode);
                sprintf(afo->logstatuscontent, "error in CreateFileA in mutate_bytes_tiny() function for mutation file %s. LastError Code: %d.",
                        afr->mutated_input_filename, errorcode);
                build_log_status(afo);
                return -1;
            }
            mcontent[afi->mutation_location] = mutation_values;
            WriteFile(out, mcontent, afi->seed_size, &bytes, NULL);
            CloseHandle(out);

            ZeroMemory(afr->process_commandline_name, sizeof(unsigned char) * 384);
            sprintf(afr->process_commandline_name, "%s %s", afi->target_application_filename, afr->mutated_input_filename);

            trigger(afr, afo);

            gthreadscreated = gprocscreated;
        }

comeout:
        // update only after one mutation location is completed
        afi->saplings_created += (256 / afi->mutation_hexjump);
        afi->saplings_pending -= (256 / afi->mutation_hexjump);
        afi->overall_saplings_created += (256 / afi->mutation_hexjump);
        afi->overall_saplings_pending -= (256 / afi->mutation_hexjump);

        afi->process_clean_exits = afr->process_clean_exits;
        afi->process_safe_exits = afr->process_safe_exits;
        afi->process_non_safe_exits = afr->process_non_safe_exits;

        afo->progress_cur_bar += (afo->progress_cur_increment * (256.0 / afi->mutation_hexjump));

        GlobalMemoryStatusEx (&afo->statex);
        if((afo->statex.ullAvailPhys/1048576) < 1000)
        {
            sprintf(afo->logstatuscontent, "LOMEM in mutate_bytes_tiny() function. cooling for 30 seconds.");
            build_log_status(afo);
            Sleep(30 * 1000);
        }

        if(gthreadscreated >= afi->max_threads)
        {
            build_log_screen(afi, afr, afo);
            gthreadsrunning = gthreadscreated;
            gthreadscreated = 0;

            gprocscreated = 0;
            while(gprocscreated < afi->max_threads)
            {
                threadcreated = CreateThread(NULL, 0, monitorthisprocess, gprocscreated, 0, NULL);
                CloseHandle(threadcreated);
                gprocscreated++;
            }
            gprocscreated = 0;

            while(gthreadsrunning)
            {
                Sleep(100);
            }

            gprocscreated = 0;
            while(gprocscreated < afi->max_threads)
            {
                if(!gprocess_crashed)
                {
                    DeleteFileA(h_mutated_input_filename[gprocscreated]);
                }
                gprocscreated++;
            }
            gprocscreated = 0;
            build_log_screen(afi, afr, afo);
        }


        if(gprocess_crashed)
        {
            build_log_screen_crash(afi, afo);
            sprintf(afo->logstatuscontent, "stopped because of process CRASH. inside mutate_bytes_tiny() function.");
            build_log_status(afo);
            printf("stopped because of process CRASH. inside mutate_bytes_tiny() function.\n");
            goto loopend;
        }
        if(afr->process_lowmem > 3)
        {
            sprintf(afo->logstatuscontent, "stopped because of low mem. inside mutate_bytes_tiny() function.");
            build_log_status(afo);
            printf("stopped because of low mem. inside mutate_bytes_tiny() function.");
            afi->process_lowmem = afr->process_lowmem;
            goto loopend;
        }

        mcontent[afi->mutation_location] = afi->seed_content[afi->mutation_location];
    }

    // doing this, so that it looks correct in log screen exited normally function
    afi->mutation_location--;

    gthreadsrunning = gthreadscreated;
    gprocscreated = 0;
    while(gprocscreated < gthreadscreated)
    {
        threadcreated = CreateThread(NULL, 0, monitorthisprocess, gprocscreated, 0, NULL);
        CloseHandle(threadcreated);
        gprocscreated++;
    }
    gprocscreated = 0;

    while(gthreadsrunning)
    {
        Sleep(100);
    }

    gprocscreated = 0;
    while(gprocscreated < gthreadscreated)
    {
        if(!gprocess_crashed)
        {
            DeleteFileA(h_mutated_input_filename[gprocscreated]);
        }
        gprocscreated++;
    }
    gprocscreated = 0;

    build_log_screen(afi, afr, afo);



loopend:

    free(mcontent);
    mcontent = NULL;

    sprintf(afo->logstatuscontent, "returning from mutate_bytes_tiny() function.");
    build_log_status(afo);

    return 1;
}

int mutate_bytes_damn_tiny(struct fuzzinput *afi, struct fuzzoutput *afo)
{
    sprintf(afo->logstatuscontent, "entered into mutate_bytes_damn_tiny() function.");
    build_log_status(afo);

    int i = 0;
    int mutation_values = 0;
    int bytes = 0;
    HANDLE out;
    int errorcode = 0;
    gthreadscreated = 0;
    HANDLE threadcreated = 0;

    struct fuzzrunning *afr = (struct fuzzrunning *) calloc(1, sizeof(struct fuzzrunning));
    ZeroMemory(afr, (1 * sizeof(struct fuzzrunning)));

    unsigned char *mcontent = (unsigned char *) calloc(afi->seed_size, sizeof(unsigned char));
    afr->process_commandline_name = (unsigned char *) calloc(384, sizeof(unsigned char));

    afr->mutated_input_filename = (unsigned char *) calloc(256, 1);

    afr->process_current_directory = (unsigned char *) calloc(128, 1);
    GetCurrentDirectory(127, afr->process_current_directory);

    afr->process_cooling_seconds = afi->process_cooling_seconds;

    afr->process_crashed = 0;
    afr->process_clean_exits = 0;
    afr->process_safe_exits = 0;
    afr->process_non_safe_exits = 0;

    afr->continue_debugging = 0;
    afr->exception_occured = 0;
    afr->process_lowmem = 0;

    gprocscreated = 0;

    build_log_screen(afi, afr, afo);

    for(i = 0; i< afi->seed_size; i++)
    {
        mcontent[i] = afi->seed_content[i];
    }

    for(afi->mutation_location = afi->mutation_start_offset; afi->mutation_location <= afi->mutation_end_offset; afi->mutation_location++)
    {
        for(mutation_values = 0; mutation_values < 256; mutation_values+=afi->mutation_hexjump)
        {
            ZeroMemory(afr->mutated_input_filename, 256);
            sprintf(afr->mutated_input_filename,"%s\\forest\\%.4X_%.2X.%s", afr->process_current_directory, afi->mutation_location, mutation_values, afi->seed_fileextension);

            ZeroMemory(h_mutated_input_filename[gthreadscreated], 256);
            sprintf(h_mutated_input_filename[gthreadscreated],"%s\\forest\\%.4X_%.2X.%s", afr->process_current_directory, afi->mutation_location, mutation_values, afi->seed_fileextension);

            out = CreateFileA(afr->mutated_input_filename, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
            if(out == -1)
            {
                printf("error in CreateFileA in mutate_bytes_damn_tiny() function for mutation file %s.\n", afr->mutated_input_filename);
                errorcode = GetLastError();
                printf("LastError Code: %d\n", errorcode);
                sprintf(afo->logstatuscontent, "error in CreateFileA in mutate_bytes_damn_tiny() function for mutation file %s. LastError Code: %d.",
                        afr->mutated_input_filename, errorcode);
                build_log_status(afo);
                return -1;
            }
            mcontent[afi->mutation_location] = mutation_values;
            WriteFile(out, mcontent, afi->seed_size, &bytes, NULL);
            CloseHandle(out);

            ZeroMemory(afr->process_commandline_name, sizeof(unsigned char) * 384);
            sprintf(afr->process_commandline_name, "%s %s", afi->target_application_filename, afr->mutated_input_filename);

            trigger(afr, afo);

            gthreadscreated = gprocscreated;
        }

comeout:
        // update only after one mutation location is completed
        afi->saplings_created += (256 / afi->mutation_hexjump);
        afi->saplings_pending -= (256 / afi->mutation_hexjump);
        afi->overall_saplings_created += (256 / afi->mutation_hexjump);
        afi->overall_saplings_pending -= (256 / afi->mutation_hexjump);

        afi->process_clean_exits = afr->process_clean_exits;
        afi->process_safe_exits = afr->process_safe_exits;
        afi->process_non_safe_exits = afr->process_non_safe_exits;

        afo->progress_cur_bar += (afo->progress_cur_increment * (256.0 / afi->mutation_hexjump));

        GlobalMemoryStatusEx (&afo->statex);
        if((afo->statex.ullAvailPhys/1048576) < 1000)
        {
            sprintf(afo->logstatuscontent, "LOMEM in mutate_bytes_damn_tiny() function. cooling for 30 seconds.");
            build_log_status(afo);
            Sleep(30 * 1000);
        }

        if(gthreadscreated >= afi->max_threads)
        {
            //build_log_screen(afi, afr, afo);
            gthreadsrunning = gthreadscreated;
            gthreadscreated = 0;

            gprocscreated = 0;
            while(gprocscreated < afi->max_threads)
            {
                threadcreated = CreateThread(NULL, 0, monitorthisprocess, gprocscreated, 0, NULL);
                CloseHandle(threadcreated);
                gprocscreated++;
            }
            gprocscreated = 0;

            while(gthreadsrunning)
            {
                Sleep(100);
            }

            gprocscreated = 0;
            while(gprocscreated < afi->max_threads)
            {
                if(!gprocess_crashed)
                {
                    DeleteFileA(h_mutated_input_filename[gprocscreated]);
                }
                gprocscreated++;
            }
            gprocscreated = 0;
            build_log_screen(afi, afr, afo);
        }

        if(gprocess_crashed)
        {
            build_log_screen_crash(afi, afo);
            sprintf(afo->logstatuscontent, "stopped because of process CRASH. inside mutate_bytes_damn_tiny() function.");
            build_log_status(afo);
            printf("stopped because of process CRASH. inside mutate_bytes_damn_tiny() function.\n");
            goto loopend;
        }
        if(afr->process_lowmem > 3)
        {
            sprintf(afo->logstatuscontent, "stopped because of low mem. inside mutate_bytes_damn_tiny() function.");
            build_log_status(afo);
            printf("stopped because of low mem. inside mutate_bytes_damn_tiny() function.");
            afi->process_lowmem = afr->process_lowmem;
            goto loopend;
        }

        mcontent[afi->mutation_location] = afi->seed_content[afi->mutation_location];
    }
    // doing this, so that it looks correct in log screen exited normally function
    afi->mutation_location--;

    gthreadsrunning = gthreadscreated;
    gprocscreated = 0;
    while(gprocscreated < gthreadscreated)
    {
        threadcreated = CreateThread(NULL, 0, monitorthisprocess, gprocscreated, 0, NULL);
        CloseHandle(threadcreated);
        gprocscreated++;
    }
    gprocscreated = 0;

    while(gthreadsrunning)
    {
        Sleep(100);
    }

    gprocscreated = 0;
    while(gprocscreated < gthreadscreated)
    {
        if(!gprocess_crashed)
        {
            DeleteFileA(h_mutated_input_filename[gprocscreated]);
        }
        gprocscreated++;
    }
    gprocscreated = 0;

    build_log_screen(afi, afr, afo);



loopend:

    free(mcontent);
    mcontent = NULL;

    sprintf(afo->logstatuscontent, "returning from mutate_bytes_damn_tiny() function.");
    build_log_status(afo);

    return 1;
}


int mutate_bytes_microscopic(struct fuzzinput *afi, struct fuzzoutput *afo)
{
    sprintf(afo->logstatuscontent, "entered into mutate_bytes_microscopic() function.");
    build_log_status(afo);

    int i = 0;
    int mutation_values = 0;
    int bytes = 0;
    HANDLE out;
    int errorcode = 0;
    gthreadscreated = 0;
    HANDLE threadcreated = 0;

    unsigned char *mcontent = (unsigned char *) calloc(afi->seed_size, sizeof(unsigned char));

    struct fuzzrunning *afr = (struct fuzzrunning *) calloc(1, sizeof(struct fuzzrunning));
    ZeroMemory(afr, (1 * sizeof(struct fuzzrunning)));

    afr->process_commandline_name = (unsigned char *) calloc(384, sizeof(unsigned char));
    afr->mutated_input_filename = (unsigned char *) calloc(256, 1);

    afr->process_current_directory = (unsigned char *) calloc(128, 1);
    GetCurrentDirectory(127, afr->process_current_directory);

    afr->process_cooling_seconds = afi->process_cooling_seconds;

    afr->process_crashed = 0;
    afr->process_clean_exits = 0;
    afr->process_safe_exits = 0;
    afr->process_non_safe_exits = 0;

    afr->continue_debugging = 0;
    afr->exception_occured = 0;
    afr->process_lowmem = 0;

    gprocscreated = 0;

    build_log_screen(afi, afr, afo);

    for(i = 0; i< afi->seed_size; i++)
    {
        mcontent[i] = afi->seed_content[i];
    }

    for(afi->mutation_location = afi->mutation_start_offset; afi->mutation_location <= afi->mutation_end_offset; afi->mutation_location++)
    {
        for(mutation_values = 0; mutation_values < 256; mutation_values+=afi->mutation_hexjump)
        {
            ZeroMemory(afr->mutated_input_filename, 256);
            sprintf(afr->mutated_input_filename,"%s\\forest\\%.4X_%.2X.%s", afr->process_current_directory, afi->mutation_location, mutation_values, afi->seed_fileextension);

            ZeroMemory(h_mutated_input_filename[gthreadscreated], 256);
            sprintf(h_mutated_input_filename[gthreadscreated],"%s\\forest\\%.4X_%.2X.%s", afr->process_current_directory, afi->mutation_location, mutation_values, afi->seed_fileextension);

            out = CreateFileA(afr->mutated_input_filename, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
            if(out == -1)
            {
                printf("error in CreateFileA in mutate_bytes_microscopic() function for mutation file %s.\n", afr->mutated_input_filename);
                errorcode = GetLastError();
                printf("LastError Code: %d\n", errorcode);
                sprintf(afo->logstatuscontent, "error in CreateFileA in mutate_bytes_microscopic() function for mutation file %s. LastError Code: %d.",
                        afr->mutated_input_filename, errorcode);
                build_log_status(afo);
                return -1;
            }
            mcontent[afi->mutation_location] = mutation_values;
            WriteFile(out, mcontent, afi->seed_size, &bytes, NULL);
            CloseHandle(out);

            ZeroMemory(afr->process_commandline_name, sizeof(unsigned char) * 384);
            sprintf(afr->process_commandline_name, "%s %s", afi->target_application_filename, afr->mutated_input_filename);

            trigger(afr, afo);

            gthreadscreated = gprocscreated;
            gthreadscreated = gprocscreated;
        }

comeout:

        // update only after one mutation location is completed
        afi->saplings_created += (256 / afi->mutation_hexjump);
        afi->saplings_pending -= (256 / afi->mutation_hexjump);
        afi->overall_saplings_created += (256 / afi->mutation_hexjump);
        afi->overall_saplings_pending -= (256 / afi->mutation_hexjump);

        afi->process_clean_exits = afr->process_clean_exits;
        afi->process_safe_exits = afr->process_safe_exits;
        afi->process_non_safe_exits = afr->process_non_safe_exits;

        afo->progress_cur_bar += (afo->progress_cur_increment * (256.0 / afi->mutation_hexjump));

        GlobalMemoryStatusEx (&afo->statex);
        if((afo->statex.ullAvailPhys/1048576) < 1000)
        {
            sprintf(afo->logstatuscontent, "LOMEM in mutate_bytes_microscopic() function. cooling for 30 seconds.");
            build_log_status(afo);
            Sleep(30 * 1000);
        }

        if(gthreadscreated >= afi->max_threads)
        {
            //build_log_screen(afi, afr, afo);
            EnterCriticalSection(&critical_section);
            gthreadsrunning = gthreadscreated;
            LeaveCriticalSection(&critical_section);
            gthreadscreated = 0;

            gprocscreated = 0;
            while(gprocscreated < afi->max_threads)
            {
                threadcreated = CreateThread(NULL, 0, monitorthisprocess, gprocscreated, 0, NULL);
                CloseHandle(threadcreated);
                gprocscreated++;
            }

            gprocscreated = 0;

            while(gthreadsrunning)
            {
                Sleep(100);
            }

            gprocscreated = 0;
            while(gprocscreated < afi->max_threads)
            {
                if(!gprocess_crashed)
                {
                    DeleteFileA(h_mutated_input_filename[gprocscreated]);
                }
                gprocscreated++;
            }
            gprocscreated = 0;
            build_log_screen(afi, afr, afo);
        }

        if(gprocess_crashed)
        {
            build_log_screen_crash(afi, afo);
            sprintf(afo->logstatuscontent, "stopped because of process CRASH. inside mutate_bytes_microscopic() function.");
            build_log_status(afo);
            printf("stopped because of process CRASH. inside mutate_bytes_microscopic() function.\n");
            goto loopend;
        }
        if(afr->process_lowmem > 3)
        {
            sprintf(afo->logstatuscontent, "stopped because of low mem. inside mutate_bytes_microscopic() function.");
            build_log_status(afo);
            printf("stopped because of low mem. inside mutate_bytes_microscopic() function.");
            afi->process_lowmem = afr->process_lowmem;
            goto loopend;
        }

        mcontent[afi->mutation_location] = afi->seed_content[afi->mutation_location];
    }
    // doing this, so that it looks correct in log screen exited normally function
    afi->mutation_location--;

    gthreadsrunning = gthreadscreated;
    gprocscreated = 0;
    while(gprocscreated < gthreadscreated)
    {
        threadcreated = CreateThread(NULL, 0, monitorthisprocess, gprocscreated, 0, NULL);
        CloseHandle(threadcreated);
        gprocscreated++;
    }
    gprocscreated = 0;

    while(gthreadsrunning)
    {
        Sleep(100);
    }

    gprocscreated = 0;
    while(gprocscreated < gthreadscreated)
    {
        if(!gprocess_crashed)
        {
            DeleteFileA(h_mutated_input_filename[gprocscreated]);
        }
        gprocscreated++;
    }

    build_log_screen(afi, afr, afo);

loopend:

    free(mcontent);
    mcontent = NULL;

    sprintf(afo->logstatuscontent, "returning from mutate_bytes_microscopic() function.");
    build_log_status(afo);

    return 1;
}
