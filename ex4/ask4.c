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


//setting the default host, port and bool parameters to check if hpst, port and debug are chosen
char def_host[50]="iot.dslab.pub.ds.open-cloud.xyz"
bool check_host = false;
char def_port[10] = "18080";
bool check_port = false;
bool debug = false;

int main(int argc, char *argv[]){

    if(argc<1){
        fprintf(stdout, "Usage: ./a.out [--host HOST] [--port PORT] [--debug]")
        exit(1);
    }

    if(argc>=1){

        for(int i=1; i<argc;i++){
          if(!strcmp(argv[i], "--host")){
            char host[50] = argv[i+1];
            check_host = true;
          }
          else if(!strcmp(argv[i], "--port")){
            char port[10] = argv[i+1];
            check_port = true;
          }
          else if(!strcmp(argv[i], "--debug")){
            debug = true;
          }  
          else{
            fprintf(stdout, "Usage: ./a.out [--host HOST] [--port PORT] [--debug]")
            exit(1);
          }
        } 
    }

    //checking if host and port have been defined. if not they are given the default options
    if (check_host == false) {
        char host[50] = def_host;
    }

    if (check_port == false) {
        char port[10] = def_port;
    }

    //define socket
    int domain = AF_INET;
    int type = SOCK_STREAM;

    int cl_socket = socket(domain, type, 0);

    if (cl_socket < 0){
        perror("Error while creating client socket");
        exit (1);
    }

    //defining the socket-port bind for the client
    struct sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(0); /* Let the system choose */
    sin.sin_addr.s_addr = htonl(INADDR_ANY); /*socket's IP address is the the address of the 
    system in use and is the address that the one in which the socket makes the connections*/

    int cl_bind = bind(cl_socket, &sin, sizeof(sin));

    if (cl_bind < 0 ){
        perror("Error while binding client socket")
        exit (1);
    }



}   