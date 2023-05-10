/*
 * sigalrm.c
 *
 *  Created on: 2023. 5. 9.
 *      Author: jhhwang
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>

void timer(int sig);

int main(int argc, char **argv)
{
	int state = 0;

	struct sigaction act = { 0 };
	act.sa_handler = timer;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	//알람 시그널 할당
	state = sigaction(SIGALRM, &act, 0);
	if (state != 0) {
		printf("sigaction() error\n");
		exit(1);
	}

	alarm(5);
	while(1) {
		puts("대기중\n");
		sleep(1);
	}

	return 0;
}

void timer(int sig)
{
	puts("예약한 시간입니다\n");
	exit(0);
}

