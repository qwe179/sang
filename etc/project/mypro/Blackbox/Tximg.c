#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include<signal.h>
#include<sys/types.h>
#include<fcntl.h>
//For IPC 
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHMSIZE	1000 //int * 100


int main(void)
{

	void *shared_Mem = (void*)0;
	int shmid;
	int *shmaddr;
	int i;
	struct shmid_ds shm_stat;
	struct check_dat{
		int check;
		char data[SIZE];
	}*shared_data;

	shared_data=(struct check_data *)shmaddr;
	shared_dat->check=0;



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
	

	printf("Memory attached at %X\n", (int)shared_Mem);
	shmaddr = (int*)shared_Mem;

	// step3. memory access

	if(shmctl(shmid,IPC_STAT,&shm_stat)==-1){
		perror("shmctl failed");
		exit(1);
	}

	char *fname="LenaZang.jpg";

	int fd1;
	fd1=open(fname,O_RDWR|O_CREAT|O_TRUNC,0666);
	while(){
	write(fd1,shmaddr,sizeof(shmaddr));
	}
	if (shmaddr==NULL)



	close(fd1);


	// step4. shmdt
	if (shmdt(shared_Mem) == -1)
	{
		fprintf(stderr, "shmdt failed\n");
		exit(EXIT_FAILURE);
	}



	exit(0);
	
}






