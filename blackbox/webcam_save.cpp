#include <opencv2/opencv.hpp>
#include<string>
#include<time.h>
#include<sys/stat.h>
#include<iostream>
#include<stdio.h>
#include<pthread.h>
#include<errno.h>
#include<unistd.h>
#include<sys/types.h>
#include<pwd.h>
#include<libgen.h>
#include<sys/vfs.h>


using namespace cv;
using namespace std;
void* thread_save(void * arg);
void* thread_delete(void * arg);
void deleteold();
void MakeDirectory(char *path);

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
	char chacha[]="/user/blackbox/";
	MakeDirectory(chacha);
	//Mat img;

	pthread_t t_id;
	pthread_t delete_dir_id;

	pthread_create(&t_id,NULL,thread_save,NULL);
	pthread_create(&delete_dir_id,NULL,thread_delete,NULL);

        while(1);
	pthread_detach(t_id);////detatch
	pthread_detach(delete_dir_id);
   	return 0;
}

void* thread_save(void *arg)
{
	//void* thread_time(void *arg);
	chdir("/home/bit204/user/blackbox/");

	//pthread_t time_id
	//pthread_create(&time_id,NULL,thread_time,NULL);

        int compmin,hour,min,sec;
        time_t timer;
        struct tm *t;
	char buf_name[100];
	char buf_dirname[100];
        Mat img_color;
        string gst = "nvarguscamerasrc ! video/x-raw(memory:NVMM), width=(int)800, height=(int)600,format=(string)NV12, framerate=(fraction)24/1 ! nvvidconv flip-method=2 ! video/x-raw, format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";     
        VideoCapture cap(gst);

        Size size = Size((int)cap.get(CAP_PROP_FRAME_WIDTH),
             (int)cap.get(CAP_PROP_FRAME_HEIGHT));
        VideoWriter writer;
        double fps=24.0;
	int a,b,c,d;
	char rooot[100]="/";
	char root[100]="./";

        while(1){
		timer = time(NULL);
		t=localtime(&timer);
		hour=t->tm_hour, min= t->tm_min, sec=t->tm_sec;
		a=t->tm_year+1900,b=t->tm_mon+1,c=t->tm_mday,d=t->tm_hour;
		sprintf(buf_dirname,"%d%d%d_%d",a,b,c,d);
		mkdir(buf_dirname,0777);
		sprintf(buf_name,"%s%s%s%d_%d_%d.avi",root,buf_dirname,rooot,hour,min,sec);
       		writer.open(buf_name,VideoWriter::fourcc('M','P','E','G'),fps,size,true);

		if(!writer.isOpened())
       		{
        		cout<<"error to save initializing";
                
      	 	}

      		compmin=min;
        	while(1)
        	{
                	cap.read(img_color);
			if(img_color.empty()){
        	                cerr<<"empty \n";
                	        break;
			}
                


	        	        writer.write(img_color);
				imshow("Color",img_color);
	      			timer = time(NULL);
        			t=localtime(&timer);
				min=t->tm_min;
				if (min!=compmin)
					break;
				waitKey(1);

        

        	}
			

	}
	return NULL;
	
}




void MakeDirectory(char *path)
{
	uid_t user_id;
	struct passwd *user_pw;

	user_id=getuid();
	user_pw = getpwuid(user_id);

	char home[256],*sp;
	strcpy(home,(user_pw->pw_dir));
	

	strcat(home, path); // 경로문자열을 복사
	sp = home; // 포인터를 문자열 처음으로

	while((sp = strchr(sp, '/'))) 
	{ // 디렉토리 구분자를 찾았으면
		if(sp > home && *(sp - 1) != ':') 
		{ // 루트디렉토리가 아니면
			*sp = '\0'; // 잠시 문자열 끝으로 설정
			mkdir(home, S_IFDIR | S_IRWXU | S_IRWXG | S_IXOTH | S_IROTH);
				// 디렉토리를 만들고 (존재하지 않을 때)
			*sp = '/'; // 문자열을 원래대로 복귀
		}
		sp++; // 포인터를 다음 문자로 이동
	}
}







void* thread_delete(void* arg)
{
        int available,total_v;
        double AvailPer;

    MOUNTP *MP;
    if ((MP=dfopen()) == NULL)
    {
        perror("error");
        
    }


        while(1)
	{

                while(dfget(MP))
                {
                        if(!strcmp(MP->mountdir,"/"))
                        {
            //  printf("%-14s%-20s%10lu%10lu\n", MP->mountdir, MP->devname, 
                               // MP->size.blocks,
                              //  MP->size.avail);
                                available=(MP->size.avail);
                                total_v=(MP->size.blocks);
                        }
                }
                AvailPer=((double)available/(double)total_v);
                AvailPer = AvailPer* 100;
                printf("Total Volume : %d\t Available Volume: %d\t Available(%%)%.2f%%\n",available,total_v,AvailPer);
                if (AvailPer<30.00)
		{
                        deleteold();
                	printf("%d\t %d\t%.2f%%\n",available,total_v,AvailPer);
                }
        	sleep(1);
        }
	return NULL;
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
                        n++;
                 }

                sprintf(buffstring,"%s%s%s",s1,s2,result);
                printf("%s\n",buffstring);
		int retaval= system(buffstring);
		if (retaval!=0)
		{
			perror("system func error");
		}
                printf("delete \"%s \"folder! \n",buffstring);

                pclose(read_fp);
		

        }
	else 
		perror("file open error!!");
}

