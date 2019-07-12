#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <sys/shm.h>
#define SIZE 1024
int main()
{
    struct check_data {

       int check;

       char data[SIZE];
    } *shared_data;
	int a;
    int shmid;
    void *shmaddr;
    // 1234 키의 공유메모리 있으면 접근해서 식별자 얻음

    if((shmid=shmget((key_t)5678, SIZE, IPC_CREAT|0666)) == -1) {
       perror("shmid failed");
       exit(1);
    }
    // shmid 공유메모리를 호출 프로세스 메모리 영역으로 첨부 */
    if((shmaddr=shmat(shmid, (void *)0, 0)) == (void *)-1) {
       perror("shmat failed");
       exit(1);
    }
    shared_data = (struct check_data *)shmaddr;

	
	FILE *fp = fopen("Lena.jpg","rb");
	FILE *fpw=fopen("Lena2.jpg","wb");
      shared_data->check = 1;

	while (1)
	{
		if (shared_data->check==1)
		{
		a=fread(shared_data->data,1,1024,fp); 
		fwrite(shared_data,1,1024,fpw);
		shared_data->check=0;	

	
		}
	   
	if (feof==0)
		break;
	
	}
         
  
   	
    // 공유메모리를 호출 프로세스의 메모리 영역에서 분리
    if(shmdt(shmaddr) == -1) {
       perror("shmdt failed");
       exit(1);
    }
    exit(0);

}
