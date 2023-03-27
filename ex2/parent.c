#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

//array for state of children
int* state;
char* str_state;

//array for pid of children
int* children;
//len of argv[1] aka number of children
int len;

void sigusr1_handler(int signum){
    for(int i=0;i<len;i++){
        kill(children[i],SIGUSR1);
    }
}

void sigusr2_handler(int signum){
    for(int i=0;i<len;i++){
        kill(children[i],SIGUSR2);
    }
}

void sigterm_handler(int signum){
    for(int i=0;i<len;i++){
        kill(children[i],SIGTERM);
    }
    //wait for all children
    int status;
    pid_t curr;
    for(int i=0;i<len;i++){
        fprintf(stdout,"Waiting for %d children to exit\n",len-i);
        curr=waitpid(children[i],&status,0);
        if(curr==-1){
            fprintf(stderr,"Error while waiting for terminated children!\n");
            exit(1);
        }
    }
    fprintf(stdout, "All children exited, terminating as well\n");

}

int initialize(char argv[], int len){
    state = (int*)malloc(len*sizeof(int));
    str_state = (char*)malloc(len*sizeof(char));
    if(state==NULL){
        fprintf(stderr,"Failed to allocate memory!\n");
        return 1;
    }
    if(str_state==NULL){
        fprintf(stderr,"Failed to allocate memory!\n");
        return 1;
    }
    for(int i=0;i<len;i++){
        if(argv[i]=='t'){
            state[i] = 0;
            str_state[i] = 't';
        }
        else if(argv[i]=='f'){
            state[i] = 1;
            str_state[i] = 'f';

        }
        else{
            fprintf(stderr, "Error! Argument gates should either be f or t\n");
            free(state);
            return 1;
        }
    }
    for(int i=0;i<len;i++){
            fprintf(stdout,"%d",state[i]);
        }
        fprintf(stdout,"\n");
    return 0;
}

int main(int argc, char* argv[]){
    if(argc!=2){
        fprintf(stderr,"Wrong number of arguments\nInput example: ./gates ttff!\n");
        exit(1);
        }
    //dynamically allocate memory to state array
    len = strlen(argv[1]);

    if(initialize(argv[1],len))
        exit(1);
    children = (int*)malloc(len*sizeof(int));
    if(children==NULL){
        fprintf(stderr,"Failed to allocate memory for children array!\n");
        exit(1);
    }
    //SIGUSR1
    struct sigaction usr1_action,usr2_action,sigterm_action;
    sigset_t sigset_usr1, sigset_usr2, sigset_term;
    /* Set up the structure to specify the new action. */
    usr1_action.sa_handler = sigusr1_handler;
    sigemptyset(&sigset_usr1);
    sigaddset(&sigset_usr1, SIGUSR2);
    sigaddset(&sigset_usr1, SIGTERM);
    usr1_action.sa_mask = sigset_usr1;
    /*If you use sigaction to establish a signal handler, you can specify how that handler should behave.
     If you specify the SA_RESTART flag, return from that handler will resume a primitive;
    otherwise, return from that handler will cause EINTR. */
    usr1_action.sa_flags = SA_RESTART;
    usr2_action.sa_handler = sigusr2_handler;
    sigemptyset(&sigset_usr2);
    sigaddset(&sigset_usr2, SIGUSR1);
    sigaddset(&sigset_usr2, SIGTERM);
    usr2_action.sa_mask = sigset_usr2;
    sigterm_action.sa_handler = sigterm_handler;
    sigemptyset(&sigset_term);
    sigaddset(&sigset_term, SIGUSR1);
    sigaddset(&sigset_term, SIGUSR2);
    sigterm_action.sa_mask = sigset_term;
    if (0 != sigaction(SIGUSR1, &usr1_action, NULL)){
        perror("sigaction () failed installing SIGUSR1 handler");
        exit(1);
    }
    if (0 != sigaction(SIGUSR2, &usr2_action, NULL)){
        perror("sigaction () failed installing SIGUSR2 handler");
        exit(1);
    }
    if (0 != sigaction(SIGTERM, &sigterm_action, NULL)){
        perror("sigaction () failed installing SIGTERM handler");
        exit(1);
    }

    int curr_pid;
    for(int i=0;i<len;i++){
        children[i]=fork();
        if(children[i]<0){
            fprintf(stderr,"Error while forking!\n");
            exit(1);
        }
        //child code
        if(children[i]==0){
            //needed char* to pass to argv
            char arg[1];
            arg[0]=str_state[i];
            char *const argv[] = {"./child",arg,NULL};
            int status = execv("./child",argv);
        }
        else {
            fprintf(stdout,"[PARENT/PID=%d] Created child %d (PID=%d) and initial state %c\n",getpid(),i,children[i],str_state[i]);

        }
    }
    waitpid(-1, NULL,0);
    

    
    
    
}