/*
 * zombie.c
 *
 *  Created on: 2023. 5. 9.
 *      Author: jhhwang
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

#include "error_handling.h"

int main(int argc, char **argv)
{
	pid_t pid = 0;
	int data = 0;

	pid = fork();
	if (pid < 0) {
		printf("fork 실패 프로세스 id : %d \n", pid);
	}

	printf("fork 성공 프로세스 id : %d \n", pid);

	if (pid == 0) {
		//in case of parent process
		data += 10;
	} else {
		//in case of child process
		data -= 10;
		sleep(20);
	}

	printf("data : %d\n", data);

	return 0;
}
