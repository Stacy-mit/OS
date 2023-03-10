#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <argp.h>
#include <string.h>

int main(int argc, char *argv[]){
    //error checking for insufficient #arguments
    if(argc!=2){
        fprintf(stdout, "Usage: ./a.out filename\n");
        exit(1);
    }
    static char args_doc[] = "filename";


    /*
    The options, parser, and
        args fields are zero because we have neither options or
        a program documentation;
    */
    //static struct argp argp = { 0, 0, args_doc, 0 };
    //argp_parse (&argp, argc, argv, 0, 0, 0);

    //struct to store the properties of the file processed by stat()

    struct stat sfile;
    /*
     The stat() function reads all the properties of a 
     file including the file size, creation date, and modified date. 
     The stat() function return -1 if the file doesn't exist or zero ( 0 ) otherwise. 
     The stat() function is included in the sys/stat.h standard library.
    */
    if(stat(argv[1], &sfile)==0){
        fprintf(stderr, "Error: %s already exists\n" , argv[1]);
        exit(1);
    }
    int fd;
    fd = open(argv[1], O_RDWR | O_APPEND | O_CREAT, S_IRWXU);
    
    if (fd == -1) {
        fprintf(stderr,"Error opening file\n");
        exit(1);
     }
    
    int status;
    pid_t child;
    child = fork();

    if (child<0){
        fprintf(stderr,"Error while forking!\n");
        exit(1);
    }

    //child code
    if(child == 0){
        pid_t mypid = getpid();
        pid_t ppid  = getppid();
        char msg[100];
        sprintf(msg,"[CHILD] getpid() = %d, getppid()= %d\n",mypid,ppid);
        int wr = write(fd,msg,strlen(msg)); 
        if(wr<0){
            fprintf(stderr,"error while writing!\n");
            exit(1);
        }
        exit(0);

    }

    else {
        int wt = wait(&status);
        if(wt < 0){
            perror("Error while waiting!\n");
            exit(1);
        }
        pid_t mypid = getpid();
        pid_t ppid  = getppid();
        char msg[100];
        sprintf(msg,"[PARENT] getpid() = %d, getppid()= %d\n",mypid,ppid);
        int wr = write(fd,msg,strlen(msg)); 
        if(wr<0){
            sprintf(stderr,"error while writing!\n");
            exit(1);
        }
        exit(0);
    }


}