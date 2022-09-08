#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h> 
#include <getopt.h>
#include <errno.h>
#include <unistd.h>

#include "build.h"

void build(char *pathname, int count, int argVal){
        chdir(pathname);
        
        int x = 0;
        int i = 0;
        int j;
        struct dirent **namelist;
        x = scandir(".", &namelist, NULL, treeComp);
        /* traverses through whole inode list */
        while(x--){

                if (strcmp(namelist[i]->d_name, ".") != 0 && strcmp(namelist[i]->d_name, "..") != 0 && 
                        /* skips over hidden fiels unless argVal indicates a -a */
                        ((namelist[i]->d_name[0] != '.') || argVal >= 2)){
                        if (count){
                                for(j = count; j > 0; j--){
                                        if (j == 1){
                                                printf("|-- ");
                                        }
                                        else{
                                                printf("|   ");
                                        }
                                }
                        }
                        /* 1 = -l, 2 = -a, 3 = combo */
                        if(argVal == 1 || argVal == 3){
                                permissions(namelist[i]->d_name);
                        }
                        /* Check access if no permissions throw error but no exit. */
                        int rw = access(namelist[i]->d_name, R_OK);
                        if (rw < 0 && namelist[i]->d_type != DT_LNK) {
                                printf("%s [error opening dir]\n", namelist[i]->d_name);
                        }
                        else {
                                printf("%s\n", namelist[i]->d_name);
                                if (namelist[i]->d_type == DT_DIR){
                                        build(namelist[i]->d_name, count + 1, argVal);
                                } 
                        }
   
                }
        free(namelist[i]);
        i++;
        }    
        free(namelist);
        chdir("..");
        return; 
}

void permissions(char *file){
        struct stat sb;
        lstat(file, &sb);
        printf("[");

        /* First bit file type */
        switch (sb.st_mode & S_IFMT) {
        case S_IFBLK:  printf("b");                           break;
        case S_IFCHR:  printf("c");                           break;
        case S_IFDIR:  printf("d");                           break;
        case S_IFIFO:  printf("p");                           break;
        case S_IFLNK:  printf("l");                           break;
        default:       printf("-");                           break;   
        }

         /* User Permissions */
        switch (sb.st_mode & (S_IRUSR+S_IWUSR)) {
            case S_IRUSR:           printf("r-");               break;
            case S_IWUSR:           printf("-w");               break;
            case S_IRUSR+S_IWUSR:   printf("rw");               break;
            default:                printf("--");               break;
        }

        if ((sb.st_mode & S_ISUID) && (sb.st_mode & S_IXUSR)) {
            printf("s"); 
        }
        else if ((sb.st_mode & S_ISUID) && !(sb.st_mode & S_IXUSR)) {
            printf("S"); 
        }
        else if (!(sb.st_mode & S_ISUID) && (sb.st_mode & S_IXUSR)) {
            printf("x");
        }
        else {
            printf("-");
        }

        /* Group Permissions */
        switch (sb.st_mode & (S_IRGRP+S_IWGRP)) {
        case S_IRGRP:            printf("r-");              break;
        case S_IWGRP:            printf("-w");              break;
        case S_IRGRP+S_IWGRP:        printf("rw");          break;
        default:                 printf("--");              break;
        }

        if ((sb.st_mode & S_ISGID) && (sb.st_mode & S_IXGRP)) {
            printf("s"); 
        }
        else if ((sb.st_mode & S_ISGID) && !(sb.st_mode & S_IXGRP)) {
            printf("S"); 
        }
        else if (!(sb.st_mode & S_ISGID) && (sb.st_mode & S_IXGRP)) {
            printf("x");
        }
        else {
            printf("-");
        }

        /* Global Permissions */
        switch (sb.st_mode & S_IRWXO) {
        case S_IROTH:            printf("r--");               break;
        case S_IWOTH:            printf("-w-");              break;
        case S_IXOTH:            printf("--x");               break;
        case S_IROTH+S_IWOTH:    printf("rw-");               break;
        case S_IROTH+S_IXOTH:    printf("r-x");               break;
        case S_IWOTH+S_IXOTH:    printf("-wx");               break;
        case S_IRWXO:            printf("rwx");               break;
        default:                 printf("---");               break;
        }
        printf("]");
}

int treeComp(const struct dirent **a, const struct dirent **b){
        char *s1 = (char*) malloc(sizeof((*a)->d_name));
        char *s2 = (char*) malloc(sizeof((*b)->d_name));
        
        /* transfers the string for comparison without dot and first letter is lowercase */
        memcpy(s1, (*a)->d_name, sizeof((*a)->d_name));
        memcpy(s2, (*b)->d_name, sizeof((*b)->d_name));

        /* changes letter to lowercase depending on the file (hidden of not) */
        if((s1[0] == '.') && (s2[0] == '.') ){
                s1[1] = tolower(s1[1]);
                s2[1] = tolower(s2[1]);
                int val = strcmp(s1 + 1, s2 + 1);
                free(s1);
                free(s2);
                return val;
        }
        else if (s1[0] == '.'){
                s1[1] = tolower(s1[1]);
                s2[0] = tolower(s2[0]);
                int val = strcmp(s1 + 1, s2);
                if (val == 0){ 
                /* if files both the same w/o the ., then sort . file first */
                        free(s1);
                        free(s2);
                        return -1;
                }
                else{
                        free(s1);
                        free(s2);
                        return val;
                }
        }
        else if (s2[0] == '.'){
                s1[0] = tolower(s1[0]);
                s2[1] = tolower(s2[1]);
                int val = strcmp(s1, s2 + 1);
                if (val == 0){
                /* if files both the same w/o the ., then sort . file first */
                        free(s1);
                        free(s2);
                        return 1;
                }
                else{
                        free(s1);
                        free(s2);
                        return val;
                }
        }
        else{
                s1[0] = tolower(s1[0]);
                s2[0] = tolower(s2[0]);
                int val = strcmp(s1, s2);
                free(s1);
                free(s2);
                return val;
        }  
} 