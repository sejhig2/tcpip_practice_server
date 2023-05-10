/*
 * sigint.c
 *
 *  Created on: 2023. 5. 9.
 *      Author: jhhwang
 */

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void handler(int sig);

int main(int argc, char **argv)
{
	printf("%d \n", SIGINT);
	int state = 0;
	int num = 0;

	signal(SIGINT, handler);
	while(1) {
		printf("%d : 대기중 \n", num++);
		sleep(2);
		if (num > 5) {
			break;
		}
	}
	return 0;
}

void handler(int sig)
{
	signal(SIGINT, handler);
	printf("전달된 signal : %d\n", sig);
}
