/*
 * gethostbyname.c
 *
 *  Created on: 2023. 5. 3.
 *      Author: jhhwang
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "error_handling.h"

int main(int argc, char **argv)
{
	int i = 0;
	struct hostent *host = NULL;

	if (argc != 2) {
		printf("Usage : %s <addr> \n", argv[0]);
		exit(1);
	}

	//url을 이용해서 host 구조체의 맴버 채우기
	host = gethostbyname(argv[1]);
	if (!host) {
		error_handling("gethostbyname() error\n");
	}

	printf("Officially name : %s \n\n", host->h_name);

	puts("Aliases ------------");
	if(i = 0; host->h_aliases[i]; i++
				)
				{
					pust(host->h_aliases[i]);
				}
				printf("Address Type : %s \n",
				            host->h_addrtype == AF_INET ? "AF_INET" : "AF_INET6");

				return 0;
			}
