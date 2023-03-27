#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

int status;

int main(int argc, char* argv[]){
    fprintf(stdout,"%s\n",argv[1]);
    //SIGUSR1
    struct sigaction usr1_action,usr2_action,sigterm_action;
    sigset_t sigset_usr1, sigset_usr2, sigset_term;
    /* Set up the structure to specify the new action. */
    //usr1_action.sa_handler = sigusr1_handler;
    sigemptyset(&sigset_usr1);
    sigaddset(&sigset_usr1, SIGUSR2);
    sigaddset(&sigset_usr1, SIGTERM);
    /*If you use sigaction to establish a signal handler, you can specify how that handler should behave.
     If you specify the SA_RESTART flag, return from that handler will resume a primitive;
    otherwise, return from that handler will cause EINTR. */
    usr1_action.sa_flags = SA_RESTART;
    //usr2_action.sa_handler = sigusr2_handler;
    sigemptyset(&sigset_usr2);
    sigaddset(&sigset_usr2, SIGUSR1);
    sigaddset(&sigset_usr2, SIGTERM);
    
    while(1);

    
    
    
}