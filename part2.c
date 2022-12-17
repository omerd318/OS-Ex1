#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>

#define GODEL 50

int main(int argc, char* argv[]) {
    if(argc != 2) {
        printf("Missing arguments\n");
        exit (-1);
    }

    char* config = argv[1];
    int fd_config, fd_grades;
    int stat;
    int position = 0, arr_position = 0;
    char buf[GODEL+1];
    char dir[GODEL], in[GODEL], out[GODEL];
    pid_t pid;

    fd_config = open(config, O_RDONLY);

    if (fd_config < 0) {
        printf("Error with opening config file\n");
        exit(-1);
    }

    if (read(fd_config, &buf, GODEL*3) < 0) {
        printf("Error with reading config file\n");
        exit(-1);
    }

    /* TODO: Think of a more efficient way... */
    while(buf[position] != '\n') {
        dir[arr_position] = buf[position];
        position++;
        arr_position++;
    }

    position++;
    arr_position = 0;

    while(buf[position] != '\n') {
        in[arr_position] = buf[position];
        position++;
        arr_position++;
    }

    position++;
    arr_position = 0;

    while(buf[position] != '\n') {
        out[arr_position] = buf[position];
        position++;
        arr_position++;
    }

    close(fd_config);

    int fd_students = open("./studentsList.txt", O_RDWR | O_CREAT, 0666);

    if((pid = fork()) < 0) {
        printf("fork error\n");
    } else {
        if (pid == 0) {
            /* child */
            close(1);
            dup(fd_students);
            close(fd_students);
            char* argument_list[] = {"ls", dir, NULL};
            exit(execvp("ls", argument_list));
        } else {
            /* parent */
            int waited = wait(&stat);
            char* currStudent;
            position = 0;

            lseek(fd_students, 0, SEEK_SET);
            memset(buf,0,GODEL+1); 
            while((read(fd_students, &buf, 1) > 0) && (buf[0] != '\n')) {
                currStudent[position] = buf[0];
                position++;
            }

            currStudent[position] = '\0';
    
            if((pid = fork()) < 0) {
                printf("fork error\n");
            } else {

                int fd_studentOut = open("out.txt", O_RDWR | O_CREAT, 0666);
            
                if (pid == 0) {
                    /* child */
                    char* path = strcat(strcat(dir, "/"), strcat(currStudent, "/main.exe"));

                    // TODO: Add reading of the arguments from the input file, into the argument array
                    //int fd_in = open(in, O_RDONLY);
                    //memset(buf,0,GODEL+1);
                    //read(fd_in, &buf, GODEL);
                    //printf("%s\n", buf);
            
                    close(1);
                    dup(fd_studentOut);
                    close(fd_studentOut);
                    char* argument_list[] = {path, "6", "12", NULL};
                    exit(execvp(path, argument_list));
                } else {
                    /* parent */
                    waited = wait(&stat);

                    if((pid = fork()) < 0) {
                        printf("fork error\n");
                        } else {
                            if (pid == 0) {
                                /* child */
                                char* argument_list[] = {"./comp.out", "./out.txt", out, NULL};
                                exit(execvp("./comp.out", argument_list));
                            } else {
                                /* parent */
                                waited = wait(&stat);

                                // TODO: check why return status is 512?

                                char* writtenTxt = strcat(currStudent, ", 0\n");

                                if (stat == '2') {    /* The files match */
                                    char* writtenTxt = strcat(currStudent, ", 100\n");
                                }

                                writtenTxt = strcat(currStudent, ",");
                                fd_grades = open("./result.csv", O_WRONLY | O_CREAT, 0666);

                                if(write(fd_grades, writtenTxt, GODEL+5) < 0) {
                                    printf("Error with writing to grades file");
                                    exit(-1);
                                }
                            }
                        }
                    }      
                }
            }
        }

    close(fd_students);
    //remove("./out.txt");
    //remove("./studentsList");

    return 0;
}
