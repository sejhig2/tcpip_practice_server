/*
 * ipcgame_serv.c
 *
 *  Created on: 2023. 5. 10.
 *      Author: jhhwang
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>

#include "error_handling.h"

#define BUFSIZE 															   (30)

void z_handler(int sig);
int who_win(int a, int b);

int main(int argc, char **argv)
{
	int fd1[2] = { 0 };
	int fd2[2] = { 0 };

	char buffer[BUFSIZE] = { 0 };
	/* 표준 출력 또는 클라이언트에 전달할 메시지 정의 */
	char intro[] = "입력하세요 (가위 : 0, 바위:1, 보:2) : \n"; /*바이트 비교시 가위:48,바위:49,보:50*/
	char win[] = "이겼습니다 \n";
	char lose[] = "졌습니다\n";
	char no_winner[] = "비겼습니다 \n";

	int serv_sock = 0;
	int clnt_sock = 0;
	struct sockaddr_in serv_addr = { 0 };
	struct sockaddr_in clnt_addr = { 0 };
	struct sigaction act = { 0 };
	int str_len = 0;
	int state = 0; // signal handler 의 상태
	int addr_size = 0;
	pid_t pid = 0;

	if (argc != 2) {
		printf("Usage : %s <port> \n", argv[0]);
		exit(1);
	}

	/* 서버의 부모 프로세스 <->자식 프로세스의 통신을 위한 파이프 생성 */
	if ((pipe(fd1) == -1) || (pipe(fd2) == -1)) {
		error_handling("pipe() error");
	}

	act.sa_handler = z_handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	state = sigaction(SIGCHLD, &act, 0);
	if (state != 0) {
		error_handling("sigaction() error");
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); /* 시스템의 IP 주소를 자동으로 가져옴 */
	serv_addr.sin_port = htons(atoi(argv[1]));

	if (bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr))) {
		error_handling("bind() error");
		/*
		 * 에러 발생시 리턴값을 받아서 해당 리턴값이 왜 발생했는지 확인해보도록
		 */
	}
	if (listen(serv_sock, 5)) {
		error_handling("listen() error");
	}

	while(1) {
		addr_size = sizeof(clnt_addr);

		/* 클라이언트의 파일 디스크립터 가져오기 */
		clnt_sock = accept(serv_sock, (struct sockaddr*) &clnt_addr, &addr_size);
		if (clnt_sock == -1) {
			/* 클라이언트의 수신이 안 되면 계속 accept */
			continue;
		}

		/* 클라이언트 연결을 위해 프로세스 생성.
		 * 부모 프로세스의 역할 :  서버의 입력을 받고, 클라이언트의 입력을 전달 받아서 서버와 클라이언트의 입력을 비교 => 누가 이겼는지 결정
		 * 자식 프로세스의 역할 :
		 */
		if ((pid = fork()) == -1) {
			close(clnt_sock); /* 자식 프로세스 생성이 실패했을 경우, 현재 연결된 소캣을 닫고 다시 accep해서 연결하기 위해 연결된 소캣을 close */
			continue;
		} else if (pid > 0) {
			/* 부모 프로세스의 작업 내용
			 * 1. 서버에게 가위바위보 입력을 요구하고 받기
			 * 2. 자식프로세스에게서 클라이언트의 입력을 받음
			 * 3. 서버의 입력과 클라이언트의 입력 비교해서 누가 이겼는지 확인
			 * 4. 가위바위보 결과를 자식 프로세스에게 전달(자식 프로세스가 결과를 클라이언트에게 전달할 것)
			 */
			int result = 0;
			puts("연결 생성\n");
			close(clnt_sock);

			/* 가위바위보 시작*/
			write(1, intro, sizeof(intro)); /* 서버의 입력을 요구 */
			read(0, &buffer[0], BUFSIZE); /* 서버의 입력을 커맨드 라인을 통해서  받음 buffer[0] : 서버의 선택, buffer[1] 클라이언트의 선택*/
			read(fd1[0], &buffer[1], BUFSIZE - 1); /* 자식 프로세스(클라이언트에게 전달 받은 데이터)의 데이터를 읽어들임  */

			result = who_win(buffer[0], buffer[1]); /* 서버의 입력(서버-부모 프로세스)과 클라이언트(서버-자식 프로세스)의 값을 비교-> 승자 결정 */
			/* 가위바위보 결과 처리 */
			if (result == 0) {
				/* 비긴 경우 */
				write(1, no_winner, sizeof(no_winner)); /* 승패 결과 표준 출력 */
				write(fd2[1], no_winner, sizeof(no_winner)); /* 승패 결과를 자식 프로세스에게 전달(자식 프로세스가 클라이언트에게 전달할 예정) */
			} else if (result == 1) {
				/* 서버가 이긴 경우 */
				write(1, win, sizeof(win)); /* 승패 결과를 표준 출력 */
				write(fd2[1], lose, sizeof(lose)); /* 승패 결과를 자식 프로세스에게 전달(자식 프로세스가 클라이언트에게 전달할 예정) . 서버의 승리는 클라이언트의 패배*/
			} else {
				/* 서버가 패배한 경우 */
				write(1, lose, sizeof(lose));
				write(fd2[1], win, sizeof(win));
			}

		} else {
			close(serv_sock);
			/* 자식 프로세스의 작업 내용 */
			/* 1. 클라이언트에게 가위바위보 선택하라고 요청( socket 이용)*/
			write(clnt_sock, intro, sizeof(intro));
			/* 2. 클라이언트의 가위바위보 선택 수신(socket 이용)*/
			read(clnt_sock, buffer, BUFSIZE);
			/* 3. 클라이언트의 가위바위보를 서버의 부모 프로세스로 전달 (pipe 이용)*/
			write(fd1[1], buffer, 1);
			/* 4. 서버와 클라이언트의 가위바위보 결과를 서버의 부모 프로세스에게 수신 ( pipe 이용) */
			str_len = read(fd2[0], buffer, BUFSIZE);
			/* 5. 가위바위보 결과를 클라이언트에게 전달.(socket 이용) */
			write(clnt_sock, buffer, str_len);

			puts("연결 종료");
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

	pid = waitpid(-1, &rtn, WNOHANG); /* WNOHAGN : 기다릴 자식 프로세스가 없다면 기다리지 않고 종료. 무한 blocking 상태에 빠지지 않음 */
	printf("소멸된 좀비 프로세스 ID : %d \n", pid);
	printf("리턴된 데이터 : %d \n\n", WEXITSTATUS(rtn));
}

int who_win(int a, int b)
{
	if (a == b) {
		return 0;
	} else if (a % 3 == (b + 1) % 3) {
		return 1;
	} else {
		return -1;
	}
}
