#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h> 
#include <unistd.h>

#include "build.h"

int main(int argc, char *argv[]){
    
    char cwd[256];

    if (getcwd(cwd, sizeof(cwd)) ==  NULL){
        perror("getcwd() error");
    }

    int count = 1;
    int dirCount = 0;
    int i;
    for(i = 0; i < argc; i++){
        if (argv[i][0] != '-'){
            dirCount += 1;
        }
    }
    
    int argVal = arg(argc, argv);
    
    if (dirCount == 1){
        printf(".\n");
        build(".", count, argVal); 
    }
    else{
        for(i = 1; i < argc; i++){
            /* runs the traversal on all arguments that aren't the executeable or a switch (-a, -l) */
            if (argv[i][0] != '-'){
                int valid = chdir(argv[i]);
                if (valid < 0) {
                    printf("%s [error opening dir]\n", argv[i]);
                    
                } 
                else {
                    chdir(cwd);
                    printf("%s\n", argv[i]);
                    build(argv[i], count, argVal);
                    /* changes back to direcotry when build was called */
                    if (argv[0] != "..") {
                        chdir(cwd);
                    }
                }

            }
        }
    }
    return 0; 

}