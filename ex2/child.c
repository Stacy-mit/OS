#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

int status;
int total_sec=0;
char* gate_num;

//setting up the alarm function
void alarm_handler (int s){
    if (s=SIGALRM){
        total_sec++;
        alarm(1);
    }
    if (status == 0 ) fprintf(stdout, "[GATE=%s/PID=%d/TIME=%d]The gates are open!\n",gate_num,getpid(),total_sec);
    else if (status == 1) fprintf(stdout, "[GATE=%s/PID=%d/TIME=%d]The gates are closed!\n",gate_num,getpid(),total_sec);
    else{
        fprintf(stderr, "Error while printing the gate status!\n");
        exit(1);
    }    
};

//setting up sigusr1 function !!the parameters need to be changed!!
void usr1_handler (int signum){
    if (status == 0) {
        status = 1;
        fprintf(stdout, "[GATE=%s/PID=%d/TIME=%d]The gates are closed!\n",gate_num,getpid(),total_sec);
    }
    else if (status == 1) {
        status = 0;
        fprintf(stdout, "[GATE=%s/PID=%d/TIME=%d]The gates are open!\n",gate_num,getpid(),total_sec);
    }
    else {
        fprintf(stderr, "Error! Failed to flip the gate's state!\n");
        exit(1);
    }  
}

void usr2_handler (int signum){
    alarm(1);
    if (status == 0 || status == 1) {
        fprintf(stdout, "state: %d, %d seconds\n",status, total_sec);
    }
    else fprintf(stderr, "Error! Not valid gate state!\n");
}

int main(int argc, char* argv[]){

    alarm(15);
    
    fprintf(stdout,"%s\n",argv[1]);
    gate_num=argv[2];
    //SIGUSR1
    struct sigaction usr1_action,usr2_action,sigterm_action, alarm_action;
    sigset_t sigset_usr1, sigset_usr2, sigset_term, sigset_alarm;
    /* Set up the structure to specify the new action. */
    usr1_action.sa_handler = usr1_handler;
    sigemptyset(&sigset_usr1);
    sigaddset(&sigset_usr1, SIGUSR2);
    sigaddset(&sigset_usr1, SIGTERM);
    sigaddset(&sigset_usr1, SIGALRM);
    /*If you use sigaction to establish a signal handler, you can specify how that handler should behave.
     If you specify the SA_RESTART flag, return from that handler will resume a primitive;
    otherwise, return from that handler will cause EINTR. */
    usr1_action.sa_flags = SA_RESTART;
    usr2_action.sa_handler = usr2_handler;
    sigemptyset(&sigset_usr2);
    sigaddset(&sigset_usr2, SIGUSR1);
    sigaddset(&sigset_usr2, SIGTERM);
    sigaddset(&sigset_usr2, SIGALRM);
    //set up the alarm via sigaction
    alarm_action.sa_handler=alarm_handler;
    alarm_action.sa_flags=0; //no special flags are set for the alarm
    sigemptyset(&sigset_alarm);
    sigaddset(&sigset_alarm, SIGUSR1);
    sigaddset(&sigset_alarm, SIGUSR2);
    sigaddset(&sigset_alarm, SIGTERM); 

    if (0 != sigaction(SIGALRM, &alarm_action, NULL)){
        perror("sigaction () failed installing SIGALRM handler");
        exit(1);
    }
    
    while(1);

    
    
    
}