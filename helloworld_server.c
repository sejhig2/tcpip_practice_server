/*
 * helloworld_server.c
 *
 *  Created on: 2023. 4. 27.
 *      Author: jhhwang
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

/*
 *1. 소캣 생성
 *2. 소캣에 주소 할당
 *3. 대기 상태
 *4. 요청 수락
 *5. 데이터 전송
 */

void error_handling(char *message);

int main(int argc, char **argv)
{
	int serv_sock = 0;
	int clnt_sock = 0;
	struct sockaddr_in serv_addr = { 0 };
	struct sockaddr_in clnt_addr = { 0 };
	int clnt_addr_size = 0;
	char message[] = "hello ranix!\n";

	if (argc != 2) {
		printf("usage : %s <port> \n", argv[0]);
		exit(1);
	}

	//소캣 생성
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1) {
		error_handling("socket() error");
	}
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	//서버 주소 할당
	if (bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1) {
		error_handling("bind() error\n");
	}

	//연결 대기 상태
	if (listen(serv_sock, 5) == -1) {
		error_handling("listen() error \n");
	}
	clnt_addr_size = sizeof(clnt_addr);

	//연결 요청 수락하기
	clnt_sock = accept(serv_sock, (struct sockaddr*) &clnt_addr, &clnt_addr_size);
	if (clnt_sock == -1) {
		error_handling("accept() error \n");
	}

	//데이터 전송
	write(clnt_sock, message, sizeof(message));
	close(clnt_sock);

	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

