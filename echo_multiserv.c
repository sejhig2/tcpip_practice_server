/*
 * echo_multiserv.c
 *
 *  Created on: 2023. 5. 9.
 *      Author: jhhwang
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>

#include "error_handling.h"

#define BUFSIZE (30)

void z_handler(int sig);

int main(int argc, char **argv)
{
	/* 소캣 관련 변수 및 구조체 */
	int serv_sock = 0;
	int clnt_sock = 0;
	int addr_size = 0;
	int str_len = 0;

	struct sockaddr_in serv_addr = { 0 };
	struct sockaddr_in clnt_addr = { 0 };

	/*프로세스 관련 변수*/
	int pid = 0;
	int state = 0;

	/* 시그널 함수 관련 변수 및 구조체*/
	struct sigaction act = { 0 };

	/*메시지 관련 변수*/
	char message[BUFSIZE] = { 0 };

	/*커맨드 라인 입력시 오류제어*/
	if (argc != 2) {
		printf("Usage : %s <port> \n", argv[0]);
		exit(1); // 에러 발생 후 강제 종료
	}

	act.sa_handler = z_handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	/*시그널 헨들러 등록*/
	state = sigaction(SIGCHLD, &act, 0);

	if (state != 0) {
		puts("sigaction() error\\n");
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); //시스템이 사용중인 IP를 자동으로 할당
	serv_addr.sin_port = htons(atoi(argv[1]));

	/* 소캣 연결*/
	if ((bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr))) == -1) {
		error_handling("bind() error\n");
	}

	if (listen(serv_sock, 5) == -1) {
		error_handling("listen() error\n");
	}

	/* 에코 서버 동작*/
	while(1) {
		addr_size = sizeof(clnt_addr);
		clnt_sock = accept(serv_sock, (struct sockaddr*) &clnt_addr, &addr_size);
		if (clnt_sock == -1) {
			continue;
		}

		/* 클라이언트 연결을 독립적으로 생성*/
		/* 질문 1 : 부모 프로세스는 언제 종료될까?*/
		if ((pid = fork()) == -1) {
			/* fork실패 -> 다시 fork() ->fork실패 -> 다시 fork() -> ...*/
			close(clnt_sock);
			continue;
		} else if (pid > 0) {
			/*부모 프로세스인 경우*/
			puts("연결 생성 : 부모 프로세스\n");
			close(clnt_sock);
			continue;
		} else {
			/* 자식 프로세스인 경우 */
			close(serv_sock);

			/* 자식 프로세스의 처리 영역 : 데이터 수신 및 전송 */
			while((str_len = read(clnt_sock, message, BUFSIZE)) != 0) {
				write(clnt_sock, message, str_len);
				write(1, message, str_len);
			}
			puts("연결 종료\n");
			close(clnt_sock);
			exit(0);
		}
	}

	return 0;
}

void z_handler(int sig)
{
	pid_t pid = 0;
	int rtn = 0;

	pid = waitpid(-1, &rtn, WNOHANG);
	printf("소멸된 좀비 프로세스 ID : %d \n", pid);
	printf("리턴된 데이터 : %d \n\n", WEXITSTATUS(rtn));
}
