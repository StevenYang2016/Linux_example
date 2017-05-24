#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

static pid_t pid;

static void wakeup(int dummy){

printf(" I (PID=%d) am up now \n",pid);
}

static void handler(int dummy){

printf("I (PID=%d) got an interrupt,will exit\n",pid);
exit(0);
}

static void trapper(int i){

  if(i==SIGUSR1){
	printf("I (PID=%d) got SIGUSR1,will exit\n",pid);
	exit(0);
  }
  else{
	printf("I (PID=%d) got signal %d,will continue\n",pid,i);
  }
}

void parent (int sig){

	printf("Signal (%d) received by parent (%d)\n",sig,pid);
}

int main(int argc,char *argv[]){

	int i,cpid1,cpid2;
	printf("Number of signal is %d \n",NSIG);
	if(!(cpid1=fork())){
	    pid=cpid1=getpid();
	    printf("CPID1=%d\n",cpid1);
	    for(i=1;i<NSIG;i++)
 		signal(i,SIG_IGN);
	    signal(SIGINT,handler);
	    signal(SIGALRM,wakeup);
            alarm(2);
	    for(;;){
	       pause();
	    }
	   printf("--------CPID1(%d) terminates\n",cpid1);
           exit(0);
         }
         else if(!(cpid2=fork())){
		pid=cpid2=getpid();
		printf("CPID2=%d\n",cpid2);
		for(i=1;i<NSIG;i++){
		   signal(i,trapper);
		}
		for(;;){
		   pause();
		}
		printf("----------CPID2(%d) terminates\n",cpid2);
		exit(0);
	}
	pid=getpid();
	sleep(3);
	printf("This is parent process (pid=%d)\n",pid);
	for(i=1;i<NSIG;i++)
	    signal(i,parent);
	printf(" Send SIGUSR1(%d) to CPID1(%d)\n",SIGUSR1,cpid1);
	kill(cpid1,SIGUSR1);
	printf(" Send SIGINT(%d) to CPID1(%d)\n",SIGINT,cpid1);
	kill(cpid1,SIGINT);

	printf(" Send SIGINT(%d) to CPID2(%d)\n",SIGINT,cpid2);
	kill(cpid2,SIGINT);

	printf(" Send SIGUSR1(%d) to CPID2(%d)\n",SIGUSR1,cpid2);
	kill(cpid2,SIGUSR1);

	for(;wait((int*)0)>0;);
	return 0;
}







