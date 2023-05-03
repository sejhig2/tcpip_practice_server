/*
 * inet_aton.c
 *
 *  Created on: 2023. 4. 28.
 *      Author: jhhwang
 */

#include <stdio.h>
#include <arpa/inet.h>

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main(int argc, char **argv)
{
	char *addr = "1.2.3.123";
	struct sockaddr_in addr_inet = { 0 };

	if (!inet_aton(addr, &addr_inet.sin_addr)) {
		error_handling("conversion error\n");
	}
	printf("Unsigned long addr(network ordered) : %x \n\n", addr_inet.sin_addr.s_addr);

	return 0;
}
