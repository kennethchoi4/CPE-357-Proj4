#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h> 
#include <getopt.h>

int arg(int argc, char *argv[]){
    int optIndex = 0;
    int argVal = 0;

    /* indicates if an a has already been reached */
    int aFlag = 0;

    /* indicates if an l has already been reached */
    int lFlag = 0;
    while ((optIndex = getopt(argc, argv, "al")) != -1){
        switch(optIndex){
            case 'a':
                if (aFlag){
                    break;
                }
                if (lFlag){
                    argVal = 3;
                    break;
                }
                aFlag = 1;
                argVal = 2;
                break;
            case 'l':
                if (lFlag){
                    break;
                }
                if (aFlag){
                    argVal = 3;
                    break;
                }
                argVal = 1;
                lFlag = 1;
                break;
            default:
                perror("Invalid Switch");
                exit(-1);   
                break;      

        }
    }
    return argVal;
}