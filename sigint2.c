/*
 * sigint2.c
 *
 *  Created on: 2023. 5. 9.
 *      Author: jhhwang
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void handler(int sig);

int main(int argc, char **argv)
{
	int state = 0;
	int num = 0;

	struct sigaction act = { 0 }; // 시그널 헨들러 구조체 선언 -> 선언 후 구조체의 맴버를 꼭 할당해줘야한다.
	act.sa_handler = handler;
	act.sa_flags = 0;
	sigemptyset(&act.sa_mask); /* sa_mask 의 모든 비트를 0으로 설정 */

	state = sigaction(SIGINT, &act, 0); /* 시그널 헨들러 등록 */

	if (state != 0) {
		puts("sigaction() error\n");
		exit(1);
	}

	while(1) {
		printf("%d : 대기중 \n", num++);
		sleep(2);
		if (num > 5)
			break;
	}

	return 0;
}

void handler(int sig)
{
	printf("전달된 시그널은 %d \n", sig);
}

