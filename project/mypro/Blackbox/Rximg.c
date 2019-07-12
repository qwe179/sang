#include<string.h>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include<signal.h>


//For IPC 
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHMSIZE	1000 //int * 100

void signalHandler(int signo);
int shmid;

int main(void)
{


	void *shared_Mem = (void*)0;
	int shmid;
	int *shmaddr;
	int i;

	// step0. ftok()

	// step1. shmget
	shmid = shmget((key_t)5678, sizeof(int)*SHMSIZE, 0666 | IPC_CREAT);
	if (shmid == -1)
	{
		fprintf(stderr, "shmget failed\n");
		exit(EXIT_FAILURE);
	}

	// step2. shmat
	shared_Mem = shmat(shmid, (void*)0, 0);
	if (shared_Mem == (void*)-1)
	{
		fprintf(stderr, "shmat failed\n");
		exit(EXIT_FAILURE);
	}
	int count = 0;
	int total=0;

	printf("Memory attached at %X\n", (int)shared_Mem);
	shmaddr = (int*)shared_Mem;

	// step3. memory access
	FILE *fp = fopen("Lena.jpg","rb");
	FILE *fpw=fopen("Lena2.jpg","wb");
	while (feof(fp)==0)
	{
		count= fread(shmaddr,1,1024,fp); 
		fwrite(shmaddr,1,1024,fpw);
		memset(shmaddr,0,5);
		total+=count;
	}



    fclose(fp);
	fclose(fpw);
	
	// step4. shmdt
	if (shmdt(shared_Mem) == -1)
	{
		fprintf(stderr, "shmdt failed\n");
		exit(EXIT_FAILURE);
	}
	signal(SIGINT,signalHandler);

	exit(EXIT_SUCCESS);
}

void signalHandler(int signo)
{
		
}







