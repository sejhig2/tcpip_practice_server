/*
 * fork.c
 *
 *  Created on: 2023. 5. 3.
 *      Author: jhhwang
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char **argv)
{
	pid_t pid = 0;
	int data = 10;

	pid = fork();
	printf("pid : %d \n", pid);

	if (pid == -1) {
		printf("fork 실패, 프로세스 ID : %d \n", pid);
	}

	printf("fork() 성공, 프로세스 ID : %d \n", pid);

	if (pid == 0) {
		data += 10;
	} else {
		data -= 10;
	}

	printf("data : %d \n", data);

	//fork() 이후, parent process말고 child process 는 어디에서 뭘하나?
	return 0;
}
