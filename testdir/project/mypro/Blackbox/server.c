#include <stdio.h>

#include <string.h>

#include <unistd.h>

#include <sys/types.h>

#include <sys/ipc.h>

#include <sys/shm.h>

#include <stdlib.h>

#define SIZE 1024

 

int main()

{int a;
	printf("hi");

    struct check_data {

       int check;

       char data[SIZE];

    } *shared_data;

    int shmid;

    void *shmaddr;

    struct shmid_ds shm_stat;

    // 1234 키의 공유메모리 생성

    if((shmid=shmget((key_t)5678, sizeof(struct check_data), IPC_CREAT|0666)) == -1) {

       perror("shmid failed");

       exit(1);

    }

		printf("hi");
 

    // shmid 공유메모리를 호출 프로세스 메모리 영역으로 첨부

    if((shmaddr=shmat(shmid, (void *)0, 0)) == (void *)-1) {

       perror("shmat failed");

       exit(1);

    }
    shared_data = (struct check_data *)shmaddr;
    // check가 0이면 공유메모리에 도착한 데이터가 없다는 의미
		FILE* fp1= fopen("LenaZ.jpg","wb");
	while(1) {
	
       /* 공유메모리에 데이터 도착하면 */
		
      if(shared_data->check==0) {
		   
		 a=fwrite(shared_data->data,1,1024,fp1);
          shared_data->check = 1;
		if (a==0)
			break;
		
          }
    }

    // 공유메모리를 호출 프로세스의 메모리 영역에서 분리

    if(shmdt(shmaddr) == -1) {

       perror("shmdt failed");

       exit(1);

    }

    // shmid 공유메모리 삭제

    if(shmctl(shmid, IPC_RMID, 0) == -1) {

       perror("shmctl failed");

       exit(1);

    }

    exit(0);

}
