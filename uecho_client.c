/*
 * uecho_client.c
 *
 *  Created on: 2023. 5. 2.
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

#define BUFSIZE (30)

int main(int argc, char **argv)
{
	int sock = 0;
	char message[BUFSIZ] = { 0 };
	int str_len = 0;
	int addr_size = 0;

	struct sockaddr_in serv_addr = { 0 };
	struct sockaddr_in from_addr = { 0 };

	if (argc != 3) {
		printf("Usage : %s <ip> <port>\n", argv[0]);
		exit(1);
	}

	//클라이언트 소켓 생성 및 protocol family, data type, protocol 설정
	sock = socket(PF_INET, SOCK_DGRAM, 0);

	//서버 인스턴스 생성
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	while(1) {
		fputs("전송할 메시지를 입력하세요. (q to quit) : ", stdout);
		fgets(message, sizeof(message), stdin);
		if (!strcmp(message, "q\n")) {
			break;
		}

		//전송
		sendto(sock, message, strlen(message), 0, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
		addr_size = sizeof(from_addr);

		//수신
		str_len = recvfrom(sock, message, BUFSIZ, 0, (struct sockaddr*) &from_addr, &addr_size);
		message[str_len] = 0;
		printf("서버에서 전송된 메시지 : %s \n", message);
	}

	return 0;
}
