#include<string.h>
#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<malloc.h>
#include<setjmp.h>
#include<sys/time.h>
#include "scheduler.h"

struct frame{
	unsigned long flags;
	unsigned long ebp;
	unsigned long edi;
	unsigned long esi;
	unsigned long edx;
	unsigned long ecx;
	unsigned long ebx;
	unsigned long eax;
	unsigned long retaddr;
	unsigned long retaddr2;
	unsigned long data;
};

typedef struct sch_handle_tag
{
	int child_task;

	TaskInfo running_task;
	TaskInfo root_task;
}SchHandle;

SchHandle gh_sch;

TaskInfo task_get_runningtask();
void task_insert(TaskInfo taskinfo);
void task_delete(TaskInfo taskinfo);
void task_next();
void scheduler();
void parent_task();
TaskInfo thread_create(TaskFunc callback,void *context)
{
	TaskInfo taskinfo;
	taskinfo=malloc(sizeof(*taskinfo));
	memset(taskinfo,0x00,sizeof(*taskinfo));

	{
		struct frame *f = (struct frame *)&taskinfo->stack[THREAD_STACKSIZE-sizeof(struct frame)/4];

		int i;
		for(i=0;i<THREAD_STACKSIZE;++i){
			taskinfo->stack[i]=i;
		}
		memset(f,0,sizeof(struct frame));
		f->retaddr=(unsigned long)callback;
		f->retaddr2=(unsigned long)thread_kill;
		f->data=(unsigned long)context;
		taskinfo->sp=(unsigned long)&f->flags;
		f->ebp=(unsigned long)&f->eax;
	}

	taskinfo->task_id=gh_sch.child_task;
	taskinfo->status=TASK_READY;
	task_insert(taskinfo);

	return taskinfo;
}

void thread_init()
{
	gh_sch.root_task=NULL;
	gh_sch.running_task=NULL;
	gh_sch.child_task=0;

	thread_create(parent_task,NULL);
}

static unsigned long spsave,sptmp;
void thread_switch()
{
	asm("push %%rax\n\t"
		"push %%rbx\n\t"
		"push %%rcx\n\t"
		"push %%rdx\n\t"
		"push %%rsi\n\t"
		"push %%rdi\n\t"
		"push %%rbp\n\t"
		"push %%rbp\n\t"
		"mov %%rsp,%0"
		:"=r"(spsave)
	   );

	gh_sch.running_task->sp=spsave;

		scheduler();
	sptmp=gh_sch.running_task->sp;

		asm("mov %0,%%rsp\n\n"
				"pop %%rbp\n\t"
				"pop %%rbp\n\t"
				"pop %%rdi\n\t"
				"pop %%rsi\n\t"
				"pop %%rdx\n\t"
				"pop %%rcx\n\t"
				"pop %%rbx\n\t"
				"pop %%rax\n\t"
				::"r"(sptmp)
		   );
}

void scheduler(void)
{
	TaskInfo task;
	task = task_get_runningtask();

	switch (task->status){
		case TASK_RUN:
		case TASK_SLEEP:
			break;
		case TASK_KILL:
			task_delete(task);
			scheduler();
			break;
		case TASK_YIELD:
			task->status = TASK_RUN;
			break;
		case TASK_READY:
			task->status =TASK_RUN;
			break;
	}
	task_next();
}

void thread_wait(void)
{
	return ;
}
void thread_kill(void)
{
	TaskInfo task;
	task=task_get_runningtask();
	task->status=TASK_KILL;
	thread_switch();
}
void thread_uninit(void)
{
	return;
}
void parent_task(void *context)
{

	struct sigaction act;
	sigset_t masksets;
	pid_t pid;


	sigemptyset(&masksets);

	act.sa_handler=thread_switch;
	act.sa_mask=masksets;
	act.sa_flags=SA_NODEFER;

	sigaction(SIGUSR1,&act,NULL);

	if((pid=fork())==0){
			while(1){
				sleep(1);
				kill(getppid(),SIGUSR1);
			}
	}else{
		while(1){
			if(gh_sch.child_task==1){
				kill(pid,SIGINT);
				break;
			}
		};
	}
}

void task_insert(TaskInfo taskinfo)
{
	if(gh_sch.root_task==NULL){
		gh_sch.root_task=taskinfo;
		gh_sch.running_task=taskinfo;
	}else {
		TaskInfo temp;
		temp=gh_sch.root_task;
		while(temp->next!=NULL){
			temp=temp->next;
		}
		temp->next=taskinfo;
		taskinfo->prev=temp;
	}
}

TaskInfo task_get_runningtask(void)
{
	return gh_sch.running_task;
}

void task_next(void)
{
	TaskInfo temp;
	temp=gh_sch.running_task;
	if(temp->next!=NULL){
		gh_sch.running_task=temp->next;
	}
	else{
		gh_sch.running_task=gh_sch.root_task;
	}
}

void task_delete(TaskInfo taskinfo)
{
	TaskInfo temp = taskinfo ->prev;
	if(gh_sch.root_task=taskinfo){
		gh_sch.root_task=NULL;
		gh_sch.running_task=NULL;
		gh_sch.child_task=0;
	}else{
		temp->next=taskinfo->next;

		if(taskinfo=gh_sch.running_task){
			if(temp->next!=NULL){
				(taskinfo->next)->prev=temp;
				gh_sch.running_task=temp->next;
			}else
				gh_sch.running_task=temp;
		}
		gh_sch.child_task--;
	}
	free(taskinfo);
}
		
		


