#include<stdio.h>
#include<sys/types.h>
#include<time.h>
#include<errno.h>
#include<sys/stat.h>
#include<string.h>
int main()

{
	time_t timer;
	struct tm *t;
	timer = time(NULL);
	t=localtime(&timer);

	int a=t->tm_year+1900;
	int b=t->tm_mon+1;
	int c=t->tm_mday;
	int d=t->tm_hour;
	
	char buf[1024];
	sprintf(buf,"%d%d%d_%d",a,b,c,d);
	printf("%s",buf);



	
	if(mkdir(buf,0777)==-1&&errno!=EEXIST){
		fprintf(stderr,"%s directory create error:%s\n", strerror(errno));
		return -1;

	return 0;

	}
}

	







