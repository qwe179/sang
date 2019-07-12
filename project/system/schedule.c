#include<stdio.h>
#include<unistd.h>
#include<sched.h>

int main(void)
{
	struct sched_param param;
	int i,j;

	sched_getparam(0,&param);

	printf("\nBefore set\n");
	printf("Param.priority = %d\n",param.sched_priority);
	printf("Sched poliucy = %d\n",sched_getscheduler(0));
	for(i=0;i<100000;i++)
		for(j=0;j<100000;j++);
	

	param.sched_priority = 10;
	sched_setscheduler(0,SCHED_FIFO,&param);
	sched_getparam(0,&param);



	printf("\nFIFO set\n");
	printf("Param.priority = %d\n",param.sched_priority);
	printf("Sched policy = %d\n",sched_getscheduler(0));

	param.sched_priority=20;
	sched_setscheduler(0,SCHED_RR,&param);
	sched_getparam(0,&param);

	printf("\nRR set\n");
	printf("Param.priority = %d\n",param.sched_priority);
	printf("Sched policy = %d\n",sched_getscheduler(0));

	return 0;
}
