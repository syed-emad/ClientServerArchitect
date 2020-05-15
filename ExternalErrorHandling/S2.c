#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/wait.h>
#include <ctype.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#include <signal.h>
 

void sighandler(int sig, siginfo_t *info, void *ucontext){
	
    sigset_t s;
    sigemptyset(&s);
    sigset_t old;
    //sigaddset(&s,SIGINT);
    sigaddset(&s,SIGCHLD);
    sigprocmask(SIG_UNBLOCK, &s, &old);
    write(STDOUT_FILENO, "SIG\n", sizeof("SIG\n"));
    sleep(1);

    char buff[100];
	pid_t mypid;
    mypid = info->si_pid;;
    int signo=info->si_signo;
    int status=info->si_status;
	int count = sprintf(buff, "\nPID:%d \nSIGNO:%d   \nSTATUS:%d", mypid,signo,status);
    write(STDOUT_FILENO, buff, count);
    sleep(1);
}


int main() {
    
 printf("PID: %d\n", getpid());

 	//using sigaction stuct
 	struct sigaction action;

 	action.sa_sigaction=&sighandler;
 	action.sa_flags= SA_SIGINFO|SA_NOCLDWAIT;

 	 

 	
    for (int i = 0; i < 4; ++i)
    {
        int pid=fork();
        if(pid==0){
            execlp("gnome-calculator","gnome-calculator",NULL);
        }
    }
    

 	for (;;)
    pause();



   


    return 0;
}


