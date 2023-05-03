/*
 * becho_client.c
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
	char message[BUFSIZE] = { 0 };
	int str_len = 0;
	int addr_size = 0;
	int i = 0;

	char MSG1[] = "GOOD ";
	char MSG2[] = "Evening ";
	char MSG3[] = "Everybody";

	struct sockaddr_in serv_addr = { 0 };
	struct sockaddr_in from_addr = { 0 };

	if (argc != 3) {
		printf("Usage : %s <IP> <port> \n", argv[0]);
		exit(1);
	}

	//소캣 생성
	sock = socket(PF_INET, SOCK_DGRAM, 0);
	if (sock == -1) {
		error_handling("UDP 소캣 생성 오류");
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	sendto(sock, MSG1, strlen(MSG1), 0, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
	sendto(sock, MSG2, strlen(MSG2), 0, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
	sendto(sock, MSG3, strlen(MSG3), 0, (struct sockaddr*) &serv_addr, sizeof(serv_addr));

	for (i = 0; i < 3; i++) {
		addr_size = sizeof(from_addr);
		str_len = recvfrom(sock, message, BUFSIZE, 0, (struct sockaddr*) &from_addr, &addr_size);
		message[str_len] = 0;
		printf("서버에서 수신된  %d 차 메시지 : %s \n", i, message);
	}

	close(sock);

	return 0;
}
