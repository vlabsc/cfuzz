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

/*
int triggerthread(LPVOID lpParam)
{
    struct afrafothreaddata *threaddata = (struct afrafothreaddata *) lpParam;

    trigger(threaddata->afr, threaddata->afo);

    return 1;
}
*/

int trigger(struct fuzzrunning *afr, struct fuzzoutput *afo)
{
    SYSTEMTIME tstatuslogsystem;

    DWORD ThreadID = 0;
    HANDLE threadcreated = 0;
    int resumethread = 0;
    int trigger_waitforsingleobject_ret = 0;
    int errorcode = 0;
    int trigger_createprocess_ret = 0;


    ZeroMemory(&afr->si, sizeof(afr->si));
    afr->si.dwFlags = afr->si.dwFlags | STARTF_USESHOWWINDOW;
    afr->si.wShowWindow = SW_HIDE;
    afr->si.cb = sizeof(afr->si);
    ZeroMemory(&afr->pi, sizeof(afr->pi));

    trigger_createprocess_ret = CreateProcessA(
                         NULL,                                                  //lpApplicationName
                         afr->process_commandline_name,                         //lpCommandLine
                         NULL,                                                  //lpProcessAttributes
                         NULL,                                                  //lpThreadAttributes
                         FALSE,                                                 //bInheritHandles
                         //CREATE_SUSPENDED | PROCESS_QUERY_INFORMATION,          //dwCreationFlags
                         CREATE_SUSPENDED | DETACHED_PROCESS | CREATE_NEW_PROCESS_GROUP | PROCESS_SET_INFORMATION,          //dwCreationFlags
                         //DETACHED_PROCESS | CREATE_NEW_PROCESS_GROUP,          //dwCreationFlags
                         NULL,                                                  //lpEnvironment
                         NULL,                                                  //lpCurrentDirectory
                         &afr->si,                                              //lpStartupInfo
                         &afr->pi                                               //lpProcessInformation
                         );



    if(trigger_createprocess_ret == 0)
    {
        afr->process_lowmem++;
        printf("Process creation failed! by %d times. error in CreateProcessA in trigger() function for %s. Waiting for 10 seconds.\n",
               afr->process_lowmem, afr->process_commandline_name);
        printf("LastError Code: %d\n", errorcode);
        sprintf(afo->logstatuscontent, "Process creation failed! by %d times. error in CreateProcessA in trigger() function for %s. Waiting for 10 seconds.",
                afr->process_lowmem, afr->process_commandline_name, errorcode);
        build_log_status(afo);

        Sleep(1000* 10);
        return 0;
    }
    else if(trigger_createprocess_ret == 1)
    {
        afr->process_lowmem = 0;
        hts[gprocscreated] = afr->pi.hThread;
        hps[gprocscreated] = afr->pi.hProcess;

        gprocscreated++;

        //if(gprocessoraffinitymaskshift == (afo->SysInfo.dwNumberOfProcessors - 1))
        if(gprocessoraffinitymaskshift == afo->SysInfo.dwNumberOfProcessors)
        {
            gprocessoraffinitymaskshift = 1;
            // all cores start from 1. 0 core is left for other applications
            gprocessoraffinitymask = 1;
        }
        gprocessoraffinitymask = 1 << gprocessoraffinitymaskshift;
        gprocessoraffinitymaskshift++;

        SetProcessAffinityMask(afr->pi.hProcess, gprocessoraffinitymask);
    }

    return 1;
}



int monitorthisprocess(LPVOID lpParam)
{
    unsigned int lprocscreated = (unsigned int *) lpParam;
    DWORD rtret = 0;

    rtret = ResumeThread(hts[lprocscreated]);
    if(rtret == -1)
    {
        printf("ResumeThread Failed for %d error: %d \n", lprocscreated, GetLastError());
    }
    CloseHandle(hts[lprocscreated]);

    HANDLE hProcess = hps[lprocscreated];

    DWORD trigger_waitforsingleobject_ret = 0;
    int errorcode = 0;
    int stillrunning = 1;
    BOOL safe_bSuccess;
    BOOL safe_bDup;
    HANDLE safe_hRT = 0;
    DWORD trigger_ExitCode = 0;

    SYSTEMTIME start, end;
    GetSystemTime(&start);

    //unsigned long int lwaitforsingleobject_seconds = 1000 * gwaitforsingleobject_seconds;
    //printf("start time (m, s): %d, %d \n", start.wMinute, start.wSecond);

startagain:

    trigger_waitforsingleobject_ret = WaitForSingleObject(hProcess, gwaitforsingleobject_seconds * 1000);
    GetSystemTime(&end);

    //printf("1. end time: %d. gwaitforsingleobject_seconds: %u \n", end.wSecond, gwaitforsingleobject_seconds);
    //WAIT_TIMEOUT reached
    if(trigger_waitforsingleobject_ret == 0x00000102)
    {
        //printf("1. TIMEOUT. waited for %d seconds \n", end.wSecond - start.wSecond);
        safe_hRT = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)safe_pfnExitProc, (PVOID)0, 0, NULL);
        if (safe_hRT == NULL)
        {
            //printf("safe exit failed.\n");
            //TerminateProcess(hProcess, 0);
        }
        CloseHandle(safe_hRT);
        goto monitorthisprocessend;
    }
    else
    {
        //printf("trigger_waitforsingleobject_ret: 0x%.8X. \n", trigger_waitforsingleobject_ret);
        //something interesting has happened
        trigger_ExitCode = 0;
        GetExitCodeProcess(hProcess, &trigger_ExitCode);

        if(trigger_ExitCode == STILL_ACTIVE) // STILL_ACTIVE 259
        {
            stillrunning = 1;
        }
        else
        {
            stillrunning = 0;
            if(trigger_ExitCode == EXCEPTION_ACCESS_VIOLATION ||
                trigger_ExitCode == EXCEPTION_STACK_OVERFLOW ||
                trigger_ExitCode == EXCEPTION_PRIV_INSTRUCTION ||
                trigger_ExitCode == EXCEPTION_INT_DIVIDE_BY_ZERO ||
                trigger_ExitCode == EXCEPTION_ILLEGAL_INSTRUCTION ||
                trigger_ExitCode == STATUS_STACK_BUFFER_OVERRUN ||
                trigger_ExitCode == STATUS_HEAP_CORRUPTION ||
                trigger_ExitCode == STATUS_FATAL_APP_EXIT)
            {
                gprocess_crashed = 1;
                printf("\n CRASH CRASH CRASH CRASH CRASH \n");
                char logstatuscontent[64];
                ZeroMemory(logstatuscontent, 64);
                errorcode = GetLastError();
                sprintf(logstatuscontent, "CRASH CRASH CRASH CRASH CRASH");
                build_log_status_crash(logstatuscontent);

                CloseHandle(hProcess);
                EnterCriticalSection(&critical_section);
                gthreadsrunning--;
                LeaveCriticalSection(&critical_section);
                return 1;
            }
            else
            {
                errorcode = GetLastError();

                if(errorcode == 0x6)
                {
                    /*
                    // ERROR_INVALID_HANDLE
                    if(nooftimesinvalidhandleerrorcode == 1024)
                        printf("exitcode is 0x6. Invalid Handle. more than 1000 times.\n");

                    if(nooftimesinvalidhandleerrorcode < 1025)
                        nooftimesinvalidhandleerrorcode++;
                    */
                }
                else if(errorcode == 0x57)
                {
                    /*
                    // ERROR_INVALID_PARAMETER
                    if(nooftimesinvalidparametererrorcode == 1024)
                        printf("exitcode is 0x57. Invalid Parameter. more than 1000 times.\n");

                    if(nooftimesinvalidparametererrorcode < 1025)
                        nooftimesinvalidparametererrorcode++;
                    */
                }
                else

                {
                    printf("possible crash. exitcode is: %.8X \n", errorcode);
                    char logstatuscontent[64];
                    ZeroMemory(logstatuscontent, 64);

                    sprintf(logstatuscontent, "possible crash. exitcode is: %.8X", errorcode);
                    build_log_status_crash(logstatuscontent);
                }
            }
        }
        // GetExitCodeProcess also catches this
        // The exception value for an unhandled exception that caused the process to terminate.
    }

    //printf("2. end time (m, s): %d, %d\n", end.wMinute, end.wSecond);

    if(stillrunning == 1)
    {
        if(end.wMinute > start.wMinute)
        {
            if(( (end.wSecond + 60) - start.wSecond) <= gwaitforsingleobject_seconds)
            {
                goto startagain;
            }
        }
        else if(end.wMinute == start.wMinute)
        {
            if((end.wSecond - start.wSecond) <= gwaitforsingleobject_seconds)
            {
                goto startagain;
            }
        }
    }



    trigger_waitforsingleobject_ret = WaitForSingleObject(hProcess, 0);
    // no crash WAIT_TIMEOUT = 0x00000102
    if(trigger_waitforsingleobject_ret == 0x00000102)
    {
        //printf("2. TIMEOUT. waited for %d seconds \n", end.wSecond - start.wSecond);
        safe_hRT = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)safe_pfnExitProc, (PVOID)10, 0, NULL);
        if (safe_hRT == NULL)
        {
            //printf("safe exit failed.\n");
            //TerminateProcess(hProcess, 0);
        }
        CloseHandle(safe_hRT);
    }
    else
    {
        //something interesting has happened
        trigger_ExitCode = 0;
        GetExitCodeProcess(hProcess, &trigger_ExitCode);
        // GetExitCodeProcess also catches this
        // The exception value for an unhandled exception that caused the process to terminate.

        if(trigger_ExitCode == EXCEPTION_ACCESS_VIOLATION ||
           trigger_ExitCode == EXCEPTION_STACK_OVERFLOW ||
           trigger_ExitCode == EXCEPTION_PRIV_INSTRUCTION ||
           trigger_ExitCode == EXCEPTION_INT_DIVIDE_BY_ZERO ||
           trigger_ExitCode == EXCEPTION_ILLEGAL_INSTRUCTION ||
           trigger_ExitCode == STATUS_STACK_BUFFER_OVERRUN ||
           trigger_ExitCode == STATUS_HEAP_CORRUPTION ||
           trigger_ExitCode == STATUS_FATAL_APP_EXIT)
        {
            gprocess_crashed = 1;
            printf("\n CRASH CRASH CRASH CRASH CRASH \n");
            char logstatuscontent[64];
            ZeroMemory(logstatuscontent, 64);
            errorcode = GetLastError();
            sprintf(logstatuscontent, "CRASH CRASH CRASH CRASH CRASH");
            build_log_status_crash(logstatuscontent);

            CloseHandle(hProcess);
            EnterCriticalSection(&critical_section);
            gthreadsrunning--;
            LeaveCriticalSection(&critical_section);
            return 1;
        }
    }

monitorthisprocessend:
    if(TerminateProcess(hProcess, 0) == 0)
    {
        //printf("Terminate failed. error: %d \n", GetLastError());
    }

    CloseHandle(hProcess);
    CloseHandle(hps[lprocscreated]);

    EnterCriticalSection(&critical_section);
    gthreadsrunning--;
    LeaveCriticalSection(&critical_section);
    return 1;
}


int resumethisprocess(LPVOID lpParam)
{
    unsigned int lprocscreated = (unsigned int *) lpParam;
    DWORD rtret = 0;

    rtret = ResumeThread(hts[lprocscreated]);
    if(rtret == -1)
    {
        printf("ResumeThread Failed for %d error: %d \n", lprocscreated, GetLastError());
    }
    CloseHandle(hts[lprocscreated]);

}
