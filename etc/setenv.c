#include<stdio.h>
#include<stdlib.h>

extern char **environ;

int main()
	{
		setenv("MYDIR","/home/bit204",0);
		setenv("KERNEL_SRC","/usr/src/linux",0);

		printf("MYDIR=%s\n",getenv("KERNEL_SRC"));
		while(*environ)
			printf("%s\n",*environ++);

		return 0;
	}
