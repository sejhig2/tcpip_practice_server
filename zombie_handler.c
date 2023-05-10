/*
 * zombie_handler.c
 *
 *  Created on: 2023. 5. 9.
 *      Author: jhhwang
 */

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

void z_handler(int sig);

int main(int argc, char **argv)
{
	pid_t pid = 0;
	int state = 0;
	int i = 0;

	struct sigaction act = { 0 };
	act.sa_handler = z_handler;
	act.sa_flags = 0;
	sigemptyset(&act.sa_mask);

	/*signal handler 등록*/
	state = sigaction(SIGCHLD, &act, 0);
	if (state != 0) {
		printf("sigaction() error \n");
		exit(1);
	}

	// child processe  생성
	pid = fork();
	if (pid == 0) {
		printf("child process was created. pid : %d \n", getpid());
		exit(3);
		/*child process 가 종료되는 시점에서 z_handler() 가 동작할 것이다. */
		printf("exit(3) 이 실행됨. <- 정상동작이라면 해당  printf() 구문은 동작하지 않아야 한다.\n");
	} else {
		sleep(3);
	}

	return 0;
}

void z_handler(int sig)
{
	pid_t pid = 0;
	int rtn = 0;

	while((pid = waitpid(-1, &rtn, WNOHANG)) > 0) {
		printf("소면된 좀비의 프로세스 ID : %d \n", pid);
		printf("리턴된 데이터 : %d \n\n", WEXITSTATUS(rtn));
	}
}

