#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/dirent.h>
#include <unistd.h> 
#include <fcntl.h> 

// main function of batch program
int main(int argc, char * argv[]) {
    // path to current folder where we are  
    char path[1024];
    getcwd(path, sizeof(path));
    // command and arguments as one string (user input)
    char input[50];
    // neverending (until exit input) loop for batch behaviour imitation
    while (1) {
        /* if (fork() == 0) { */
        printf("[%s]> ", path);
        /* } */
        fgets(input, sizeof input, stdin);
        // extract the first word from user input, this is our command
        char * token = strtok(input, " ");
        // flag for future use 
        char flag = 0;
        // print user input
        if (strstr(token, "echo") != NULL) {
            // create child process (sth goes wrong, it wotrks but, I have double printf("[%s]> ", path); only in next line and can't get rid of it)
            /* fork(); */
            // print all arguments as a string like echo in real batch
            while(token != NULL ) {
                // flag 1 to do not print echo as a first word 
                if (flag == 1) {
                    printf( " %s", token);
                }
                // make token null
                token = strtok(NULL, " ");
                // flag change
                flag = 1;
            }
        } 
        // kill program after user input equals to 'exit'
        else if (strstr(token, "exit") != NULL){
                exit(0);
        } 
        // show files list from current path
        else if (strstr(token, "ls") != NULL){
            // create child process (sth goes wrong, it wotrks but, I have double printf("[%s]> ", path); only in next line and can't get rid of it)
            /* fork(); */
            while(token != NULL ) {
                // structure declaration
                DIR *d;
                struct dirent *dir;
                // take directory from current path and open it
                d = opendir(path);
                // without hidden files
                if (strstr(token, "ls") != NULL){
                    // print every file inside directory (with hidden files)
                    if (d) {
                        while ((dir = readdir(d)) != NULL) {
                            if (*dir->d_name != '.') {
                                if (dir->d_type == DT_REG) {
                                    printf("%s\n", dir->d_name);
                                }
                            }
                        }   
                        closedir(d);
                    }
                }
                if (strstr(token, "-a") != NULL){
                    // if next token is all print additional prints (hidden files)
                    if (d) {
                        while ((dir = readdir(d)) != NULL) {
                            if (*dir->d_name == '.') {
                                if (dir->d_type == DT_REG) {
                                    printf("%s\n", dir->d_name);
                                }
                            }
                        }  
                        closedir(d);
                    } 
                }
                if (strstr(token, "-f") != NULL){
                    // add folders to list
                    if (d) {
                        while ((dir = readdir(d)) != NULL) {
                            if (*dir->d_name != '.') {
                                if (dir->d_type == DT_DIR) {
                                    printf("%s\n", dir->d_name);
                                }
                            }
                        }   
                        closedir(d);
                    }
                }
            // make token null to can take another one
            token = strtok(NULL, " ");
            }
        }
        // cd implementation
        else if (strstr(token, "cd") != NULL){
            while(token != NULL ) {
                if (flag == 1) {
                    // change current path to directory from token (try as a token i.e. .. or folder name)
                    chdir(token);
                    getcwd(path, sizeof(path));
	            }
                token = strtok(NULL, " ");
                flag = 1;
	        }
        }
        // cat implementation
        else if (strstr(token, "cat") != NULL){
            while(token != NULL ) {
                if (flag == 1) {
                    // file declaration
                    FILE *file;
                    int chr;
                    // file open
                    if ((file = fopen(token, "r")) == NULL) {
                        continue;
                    }
                    // new line
                    while ((chr = getc(file)) != EOF){
                        fprintf(stdout, "%c", chr);
                        fclose(file);
                    }
                    // can't save it because cmnd+c buttons kill entire program
                    break;
                }
            token = strtok(NULL, " ");
            flag = 1;
	        }
        }
        // for command which doesn't exist let user know about it
        else {
            printf( "Command not found: %s", token);
        }
    }  
    return 0;  
}
