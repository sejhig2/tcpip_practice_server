/*
 * bind.sock.c
 *
 *  Created on: 2023. 4. 28.
 *      Author: jhhwang
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "error_handling.h"

int main(int argc, char **argv)
{
	int serv_sock = 0;
	char *serv_ip = "127.123.123.123";
	char *serv_port = "9190";
	struct sockaddr_in serv_addr = { 0 };

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sock == 0) {
		error_handling("socket () error\n");
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_addr.s_addr = inet_addr(serv_ip);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(atoi(serv_port));

	if (bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1) {
		error_handling("bind() error\n");
	}

	printf("파일 디스크립터 %d의 소캣에 주소 할당까지 완료 \n", serv_sock);
	return 0;
}
