#include<stdio.h>
#include<stdlib.h>

extern char **environ;

int main()
	{

		printf("MYDIR=%s\n",getenv("KERNEL_SRC"));
		while(*environ)
			printf("%s\n",*environ++);

		return 0;
	}
