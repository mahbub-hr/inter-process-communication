#include<unistd.h>
#include<stdio.h>

int main() {
	int a = fork();
	int b = fork();
	printf("%d, %d\n", a, b);
	return 0;
}
