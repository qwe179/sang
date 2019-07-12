#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <sys/vfs.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>



void deleteold();
const char *MMOUNT = "/proc/mounts";

struct f_size
{
    long blocks;
    long avail; 
};

typedef struct _mountinfo 
{
    FILE *fp;                // 파일 스트림 포인터    
    char devname[80];        // 장치 이름
    char mountdir[80];        // 마운트 디렉토리 이름
    char fstype[12];        // 파일 시스템 타입
    struct f_size size;        // 파일 시스템의 총크기/사용율 
} MOUNTP;

MOUNTP *dfopen()
{
    MOUNTP *MP;

    // /proc/mounts 파일을 연다.
    MP = (MOUNTP *)malloc(sizeof(MOUNTP));
    if(!(MP->fp = fopen(MMOUNT, "r")))
    {
        return NULL;
    }
    else
        return MP;
}

MOUNTP *dfget(MOUNTP *MP)
{
    char buf[256];
    char *bname;
    char null[16];
    struct statfs lstatfs;
    struct stat lstat; 
    int is_root = 0;

    // /proc/mounts로 부터 마운트된 파티션의 정보를 얻어온다.
    while(fgets(buf, 255, MP->fp))
    {
        is_root = 0;
        sscanf(buf, "%s%s%s",MP->devname, MP->mountdir, MP->fstype);
         if (strcmp(MP->mountdir,"/") == 0) is_root=1;
        if (stat(MP->devname, &lstat) == 0 || is_root)
        {
            if (strstr(buf, MP->mountdir) && S_ISBLK(lstat.st_mode) || is_root)
            {
                // 파일시스템의 총 할당된 크기와 사용량을 구한다.        
                statfs(MP->mountdir, &lstatfs);
                MP->size.blocks = lstatfs.f_blocks * (lstatfs.f_bsize/1024); 
                MP->size.avail  = lstatfs.f_bavail * (lstatfs.f_bsize/1024); 
                return MP;
            }
        }
    }
    rewind(MP->fp);
    return NULL;
}

int dfclose(MOUNTP *MP)
{
    fclose(MP->fp);
}

int main()
{
	int available,total_v;
	double AvailPer;
	
    MOUNTP *MP;
    if ((MP=dfopen()) == NULL)
    {
        perror("error");
        return 1;
    }

    
	while(1){
   
     		while(dfget(MP))
     		{
			if(!strcmp(MP->mountdir,"/"))
			{
            //	printf("%-14s%-20s%10lu%10lu\n", MP->mountdir, MP->devname, 
                               // MP->size.blocks,
                              //  MP->size.avail);
				available=(MP->size.avail);
				total_v=(MP->size.blocks);
			}
		}
		AvailPer=((double)available/(double)total_v);
		AvailPer = AvailPer* 100;
		printf("%d\t %d\t%.2f%%\n",available,total_v,AvailPer);
		if (AvailPer<60.00)
		{
			printf("%f", AvailPer);
			deleteold();
		}
    		sleep(1);
		}
}



void deleteold()
{
	FILE *read_fp;
	char buffer[200];
	int chars_read;
        char result[80];
        char s1[80]="rm -rf ";
	char s2[80]="/home/bit204/user/blackbox/";
	char buffstring[1000];
        int n=0;
        //char * remain,*available;
	chdir(s2);
       	memset(buffer,'\0', sizeof(buffer));
       	memset(buffstring,'\0', sizeof(buffstring));
       	memset(result,'\0', sizeof(result));
	read_fp = popen("ls -d */ -ltr", "r");
    	if (read_fp != NULL)
    	{
        	chars_read = fread(buffer, sizeof(char), BUFSIZ, read_fp);
        	if (chars_read > 0)
        	{

                        printf("%s\n",buffer);

       		}	


		for (int i=0;i<10;i++)
        	{
        		result[n+0]=buffer[n+47];

			//printf("%c",result[i]);

               		n++;
       		 }

		sprintf(buffstring,"%s%s%s",s1,s2,result);
		printf("%s\n",buffstring);

        	int retaval= system(buffstring);
		printf("delete folder!\n");

        	pclose(read_fp);

    	}	
}

