#include<stdio.h>   
#include<stdlib.h>   
  
int main(int arg,char *arr[])   
{   
	FILE *fs,*ft;   
	char ch;   

	if(arg!=3)   
	{   
		printf("Argument Missing !");   
		exit(0);   
	}   
  
	fs = fopen(arr[1],"r");   

	if(fs==NULL)   
	{   
		printf("Cannot open source file !");   
		exit(0);   
	}
  
	ft = fopen(arr[2],"w");   

	if(ft==NULL)   
	{   
		printf("Cannot copy file !");   
		fclose(fs);   
		exit(0);   
	}   
  
	while(1)   
	{   
		ch = getc(fs);   
		if(ch==EOF)   
		{
			break;   
		}
		else  
		putc(ch,ft);   
	}   
  
	printf("File copied succesfully!");   

	fclose(fs);   
	
	fclose(ft);   
	
	return 0;
}  
