// 0.40

#define WINVER 0x0500

#include <stdio.h>
#include <unistd.h>
#include <windows.h>
#include <winbase.h>
#include <time.h>

int exited_normally();
int begin_files(unsigned char *title,
                unsigned char *target_application_name,
                unsigned char *target_application_version,
                unsigned char *seed_file_type,
                unsigned char *seed_file_max_size_in_bytes,
                unsigned char *seed_file_extension,
                unsigned char *mutation_type,
                unsigned char *mutation_start_offset,
                unsigned char *mutation_end_offset,
                unsigned char *waitforsingleobject_milliseconds,
                unsigned char *process_cooling_milliseconds,
                unsigned char *initial_sleep_time,
                unsigned char *max_threads,
                unsigned char *chamber_folder_name);




int main(int argc, unsigned char *argv[])
{
    int i = 0;

    puts("CFuzz ver 0.40");

    /*
    unsigned long long v1 = 9999999999;
    unsigned long long v2 = 999;

    printf("| %llu | \n", v1);
    printf("| %*llu | \n", 10, v2);

    return 0;
    */




    if(argc != 15)
    {
        printf("proper syntax.\n");
        printf("cfuzz.exe < title > < target app name > < target app version > < seed file type >\
 < seed file max size in bytes > < seed file extension > < mutation start offset > < mutation end offset >\
 < previous crashed file number > < log name > < waitforsingleobject milliseconds > < initial sleep time >\
 < chamber_folder_name >\n");
        printf("argv[0]  - cfuzz.exe\n");
        printf("argv[1]  - < title >\n");
        printf("argv[2]  - < target app name >\n");
        printf("argv[3]  - < target app version >\n");
        printf("argv[4]  - < seed file type >\n");
        printf("argv[5]  - < seed file max size in bytes >\n");
        printf("argv[6]  - < seed file extension >\n");
        printf("argv[7]  - < mutation type > - (1, 4, 8, 16, 32, 64, 128) \n");
        printf("              - 1  - byte (storm), 2 - word (storm), 3 - dword (storm)\n");
        printf("              - 4  - byte (deterministic), 5 - word (deterministic), 6 - dword (deterministic)\n");
        printf("              - 7  - byte (minimalistic), 8 - word (minimalistic), 9 - dword (minimalistic)\n");
        printf("              - 10 - byte (bare minimal), 11 - word (bare minimal), 12 - dword (bare minimal)\n");
        printf("              - 13 - byte (tiny), 14 - word (tiny), 15 - dword (tiny)\n");
        printf("              - 16 - byte (damn tiny), 17 - word (damn tiny), 18 - dword (damn tiny)\n");
        printf("              - 19 - byte (microscopic), 20 - word (microscopic), 21 - dword (microscopic)\n");
        printf("argv[8]  - < mutation start offset >\n");
        printf("argv[9]  - < mutation end offset >\n");
        printf("argv[10] - < waitforsingleobject milliseconds >\n");
        printf("argv[11] - < process cooling milliseconds >. default: 20 milliseconds.\n");
        printf("argv[12] - < initial sleep minutes - guru meditation >\n");
        printf("argv[13] - < max threads >\n");
        printf("argv[14] - < chamber_folder_name >\n");
        return 0;
    }

    begin_files(argv[1], argv[2], argv[3], argv[4], argv[5], argv[6], argv[7], argv[8], argv[9],
                argv[10], argv[11], argv[12], argv[13], argv[14]);

    exited_normally();

    printf("Fuzzing completed. \n");
}
