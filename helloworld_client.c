/*
 * helloworld_client.c
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

void error_handling(char *message);

int main(int argc, char **argv)
{
	int sock = 0;
	struct sockaddr_in serv_addr = { 0 };
	char message[30] = { 0 };
	int str_len = 0;

	if (argc != 3) {
		printf("Usage : %s <IP> <port> \n", argv[0]);
	}

	/*1. 서버 접속을 위한 소켓 생성*/
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1) {
		error_handling("socket() error \n");
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	/* 서버에 연결 요청 */
	if (connect(sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1) {
		error_handling("connect() error \n");
	}

	/* 데이터 수신 */
	str_len = read(sock, message, sizeof(message) - 1);
	if (str_len == -1) {
		error_handling("read() error!\n");
	}
	message[str_len] = 0;
	printf("Message from server : %s \n", message);

	/* 소켓 닫기 */
	close(sock);

	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
