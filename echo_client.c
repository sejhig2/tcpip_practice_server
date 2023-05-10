/*
 * echo_client.c
 *
 *  Created on: 2023. 4. 28.
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
	int sock = 0;
	char message[BUFSIZE] = { 0 };
	int str_len = 0;
	struct sockaddr_in serv_addr = { 0 };

	if (argc != 3) {
		printf("Usage : %s <ip> <port> \n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1) {
		error_handling("socket() error");
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	if (connect(sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1) {
		error_handling("connetc() error\n");
	}

	printf("next connect\n");
	while(1) {
		fputs("전송할 메시지를 입력하시오 (q to quit) : ", stdout);
		fgets(message, BUFSIZE, stdin);

		if (!strcmp(message, "q\n")) {
			break;
		}
		write(sock, message, strlen(message));

		str_len = read(sock, message, BUFSIZE - 1);
		message[str_len] = 0;
		printf("서버에서 전송된 메시지 : %s \n", message);
	}

	close(sock);
	return 0;
}
