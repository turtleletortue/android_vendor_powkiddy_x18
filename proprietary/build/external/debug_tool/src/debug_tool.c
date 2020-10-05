#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/stat.h>

#ifdef __ANDROID__
#include <cutils/properties.h>
#endif
#include "functions.h"

int main(int argc, char *argv[])
{
        char device[1024];
        __system_property_get("ro.product.device", device);
        printf("\n            DEBUG TOOL for %s\n\n", device);

        /* Check if argc length is 2 */
        if (argc < 2)
        {
           printf("[-] Expected more arguments.\n\n");
           show_help();
           return -1;
        }

        /* Check if user entered more than 1 argument */
        if (argc > 2)
        {
           printf("[-] Only 1 argument is required.\n\n");
           show_help();
           return -1;
        }

        struct stat st = {0};

        if (stat(LOG_DIRECTORY, &st) == -1 && (*argv[2] != '-c')) 
        {
            printf("[?] Logging directory is not present, regenerating...\n");
            mkdir(LOG_DIRECTORY, 0755);
        }

        if(strcmp(argv[1], "-h") == 0)
        {
           show_help();
        }

        if(strcmp(argv[1], "-c") == 0)
        {
           printf("[?] Cleaning up...\n");
           system("rm -rf " LOG_DIRECTORY);
           printf("[+] All OK!\n\n");
        }

        if(strcmp(argv[1], "-l") == 0)
        {
           printf("[?] Taking logcat\n");
           system("timeout 10 logcat > " LOG_DIRECTORY "logcat.log");
           printf("[+] All OK\n\n");
        }

        if(strcmp(argv[1], "-d") == 0)
        {

        if (getuid() != 0) {
            printf("[!] You need to be root to execute this action.\n\n");
            return -1;
        }

        printf("[?] Taking dmesg...\n");
        system("dmesg > " LOG_DIRECTORY "dmesg.log");
        printf("[+] All OK!\n\n");
        }

        if(strcmp(argv[1], "-g") == 0)
        {
           printf("[?] Getting device properties...\n");
           system("getprop > " LOG_DIRECTORY "properties.prop");
           printf("[+] All OK\n\n");
        }

        if(strcmp(argv[1], "-b") == 0)
        {
           printf("[?] Taking bugreport...\n");
           system("timeout 15 bugreport > " LOG_DIRECTORY "bugreport.log");
           printf("[+] All OK!\n\n");
        }

        if(strcmp(argv[1], "-s") == 0)
        {
           int input;

           if (getuid() != 0) {
               printf("[!] You need to be root to execute this action.\n\n");
               return -1;
           }

           printf("[?] Switch SELinux to permissive (0) or enforcing (1)? >> ");
           scanf("%d", &input);

           /* Valid values are 0 and 1 */
           if (input != 0 && input != 1)
           {
               printf("[-] Invalid value: %d\n\n", input);
               return -1;
           }

           /* Permissive */
           if (input == 0) {
             printf("[?] Setting SELinux to permissive...\n");
             write_int(SELINUX, 0);
           }

           /* Enforcing */
           if (input == 1) {
             printf("[?] Setting SELinux to enforcing...\n");
             write_int(SELINUX, 1);
           }

           printf("[+] Done!\n\n");
        }

        if(strcmp(argv[1], "-t") == 0)
        {
            if (getuid() != 0) {
                printf("[!] You need to be root to execute this action.\n\n");
                return -1;
            }

            printf("[?] Searching for tombstones...\n");
            if (!is_dir_empty("/data/tombstones"))
            {
                if (__copy_folder("/data/tombstones", LOG_DIRECTORY) != 0)
                {
                    printf("[-] Failed to copy tombstones folder...\n\n");
                    return -1;
                }
            } else {
                printf("[?] Nothing to do: Tombstones folder is empty.\n\n");
                return 0;
            }

            printf("[+] Done!\n\n");
            return 0;
        }               

        return 0;
}
