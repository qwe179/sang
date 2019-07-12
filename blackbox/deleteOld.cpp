#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "checkdisk.h"
int main()
{

    FILE *read_fp;
    char buffer[BUFSIZ + 1],*pt1;
    int chars_read;
	char result[BUFSIZ];
	int n=0;
	//char * remain,*available;
    memset(buffer, '\0', sizeof(buffer));
    
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
			result[n+0]=buffer[n+43];

			printf("%c",result[i]);
				
			n++;
		}
		printf("%s\n",result);
		char s1[16]="rm -rf ";
		strcat(s1,result);
		int retaval= system(s1);

        pclose(read_fp);
        exit(EXIT_SUCCESS);
    }
    exit(EXIT_FAILURE);
}
