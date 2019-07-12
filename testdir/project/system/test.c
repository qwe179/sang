#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<linux/unistd.h>
#include<linux/sched.h>

int glob=6;
char buf[]="a write to stdout\n";

int main(void)
{
	int var; 
	pid_t pid;

	var = 88;
	write(STDOUT_FILENO,buf,sizeof(buf)-1);
	printf("before fork\n");

	if((pid=fork())==0){
		glob++,var++;
	}else
		sleep(2);
	printf("pid=%d,glob=%d,var=%d\n",getpid(),glob,var);exit(0);
}
