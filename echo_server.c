/*
 * echo_server.c
 *
 *  Created on: 2023. 4. 28.
 *      Author: jhhwang
 */

#include <stdio.h>
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
	char message[BUFSIZ] = { 0 };
	int str_len = 0;

	struct sockaddr_in serv_addr = { 0 };
	struct sockaddr_in clnt_addr = { 0 };
	int clnt_addr_size = 0;

	if (argc != 2) {
		printf("Usage : %s <port> \n", argv[0]);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1) {
		error_handling("socket() error \n");
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	if (bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1) {
		error_handling("bind() error\n");
	}

	if (listen(serv_sock, 5) == -1) {
		error_handling("listen() error \n");
	}

	clnt_addr_size = sizeof(clnt_addr);

	for (int i = 0; i < 10; i++) {
		clnt_sock = accpt(serv_sock, (struct sockaddr*) &clnt_addr, &clnt_addr_size);
		if (clnt_sock == -1) {
			error_handling("accept() error\n");
		}

		while((str_len = read(clnt_sock, message, BUFSIZE)) != 0) {
			wirte(clnt_sock, message, str_len);
			write(1, message, str_len);
		}
		close(clnt_sock);
	}
	1
	return 0;
}

