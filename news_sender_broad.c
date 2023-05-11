/*
 * news_sender_broad.c
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

#define BUFSIZE (30)
#define TRUE (1)
#define FALSE (0)

int main(int argc, char **argv)
{
	int send_sock = 0;
	struct sockaddr_in broad_addr = { 0 };
	int state = 0;
	FILE *fp = NULL;
	char buf[BUFSIZE] = { 0 };
	int so_broadcast = TRUE;

	if (argc != 3) {
		printf("Usage : %s <broadcast IP> <port> \n", argv[0]);
		exit(1);
	}

	/* 소켓 생성 */
	send_sock = socket(PF_INET, SOCK_DGRAM, 0);
	if (send_sock == -1) {
		error_handling("socket() error");
	}

	memset(&broad_addr, 0, sizeof(broad_addr));
	broad_addr.sin_family = AF_INET;
	broad_addr.sin_addr.s_addr = inet_addr(argv[1]);
	broad_addr.sin_port = htons(atoi(argv[2]));

	/*소켓 옵션을 브로드캐스트가 가능하도록 설정*/
	state = setsockopt(send_sock, SOL_SOCKET, SO_BROADCAST, (void*) &so_broadcast,
	            sizeof(broad_addr));

	if (state) {
		error_handling("setsockopt() error");
	}

	if ((fp = fopen("/usr/local/JH/project_in_eclipse/tcp_ip_socket_programing_server/src/News.txt",
	            "r")) == NULL) {
		error_handling("fopen() error");
	}

	while(!feof(fp)) {
		fgets(buf, BUFSIZE, fp);
		sendto(send_sock, buf, strlen(buf), 0, (struct sockaddr*) &broad_addr, sizeof(broad_addr));
		sleep(2);
	}

	close(send_sock);

	return 0;
}
