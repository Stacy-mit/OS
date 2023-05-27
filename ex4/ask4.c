#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>

int socket(int domain, int type, int protocol);

//setting the default host, port and a char debug parameter to check if debug is chosen
char host[50]="iot.dslab.pub.ds.open-cloud.xyz"
char port[10] = "18080";
char debug = N;

int main(int argc, char *argv[]){

    if(argc<1){
        fprintf(stdout, "Usage: ./a.out [--host HOST] [--port PORT] [--debug]")
        exit(1);
    }

    if(argc>=1){
        for(int i=1; i<argc;i++){
          if(!strcmp(argv[i], "--host")){
            host = argv[i+1];
          }
          if(!strcmp(argv[i], "--port")){
            port = argv[i+1];
          }
          if(!strcmp(argv[i], "--debug")){
            debug = Y;
          }  
        }

    }

    //define socket
    int domain = AF_INET;
    int type = SOCK_STREAM;

    int sock_fd = socket(domain, type, 0);

    if (sock_fd < 0){
        perror("Error while creating socket");
        return 1;
    }

}