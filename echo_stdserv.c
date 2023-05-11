/*
 * echo_stdserv.c
 *
 *  Created on: 2023. 5. 11.
 *      Author: jhhwang
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "error_handling.h"

#define BUFSIZE (1024)

int main(int argc, char **argv)
{
	int serv_sock = 0;
	int clnt_sock = 0;
	FILE *read_fp = NULL;
	FILE *write_fp = NULL;
	char message[BUFSIZE] = { 0 };

	struct sockaddr_in serv_addr = { 0 };
	struct sockaddr_in clnt_addr = { 0 };
	int clnt_addr_size = 0;

	if (argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1) {
		error_handling("socket() error");
	}
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	if (bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1) {
		error_handling("bind() error");
	}

	if (listen(serv_sock, 5) == -1) {
		error_handling("listen() error");
	}

	clnt_addr_size = sizeof(clnt_addr);
	clnt_sock = accept(serv_sock, (struct sockaddr*) &clnt_addr, &clnt_addr_size);
	if (clnt_sock == -1) {
		error_handling("accept() error");
	}

	/* 파일 디스크립터 clnt_sock 을 이용해서 입출력 파일 포인터를 생성한다 */
	read_fp = fdopen(clnt_sock, "r");
	write_fp = fdopen(clnt_sock, "w");

	/* 표준 입출력 함수를 확용한 데이터 송수신 */
	while(feof(read_fp)) {
		fgets(message, BUFSIZE, read_fp);
		fputs(message, write_fp);

	}
	fclose(read_fp);
	fclose(write_fp);

	return 0;
}
