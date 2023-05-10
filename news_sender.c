/*
 * news_sender.c
 *
 *  Created on: 2023. 5. 10.
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

#define TTL (64)
#define BUFSIZE (30)

int main(int argc, char **argv)
{
	int send_sock = 0;
	struct sockaddr_in multi_addr = { 0 };
	int multi_TTL = TTL;
	int state = 0;
	FILE *fp = NULL;
	char buf[BUFSIZE] = { 0 };

	if (argc != 3) {
		printf("Usage : %s <ip> <port> \n", argv[0]);
		exit(1);
	}

	send_sock = socket(PF_INET, SOCK_DGRAM, 0);
	if (send_sock == -1) {
		error_handling("socket() error");
	}

	memset(&multi_addr, 0, sizeof(multi_addr));
	multi_addr.sin_addr.s_addr = inet_addr(argv[1]);
	multi_addr.sin_family = AF_INET;
	multi_addr.sin_port = htons(atoi(argv[2]));

	state = setsockopt(send_sock, IPPROTO_IP, IP_MULTICAST_TTL, (void*) &multi_TTL,
	            sizeof(multi_TTL));
	if (state) {
		error_handling("setsockopt() error");
	}

	if ((fp = fopen("News.txt", 'r')) == NULL) {
		error_handling("fopen() error");
	}

	while(!feof(fp)) {
		fgets(buf, BUFSIZE, fp);
		sendto(send_sock, buf, strlen(buf), 0, (struct sockaddr*) &multi_addr, sizeof(multi_addr));
		sleep(2);
	}

	close(send_sock);

	return 0;
}
