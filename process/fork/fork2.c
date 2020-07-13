#include<sys/types.h>
#include <stdio.h>
#include<unistd.h>
int num = 0;
int main(int argc, char*argv[]){
    int pid;
    pid = fork();
    printf("%d\n", num);  
    if(pid == 0){       /*child*/
	printf("Child Start\n");
        num = 1;
	printf("Child End\n");
    }else if(pid > 0){  /*parent*/
	printf("Parent Start\n");
        num = 2;
	printf("Parent End\n");
    }
    printf("%d\n", num);
}
