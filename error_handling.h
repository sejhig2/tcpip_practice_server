/*
 * error_handling.h
 *
 *  Created on: 2023. 4. 28.
 *      Author: jhhwang
 */

#ifndef ERROR_HANDLING_H_
#define ERROR_HANDLING_H_

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

#endif /* ERROR_HANDLING_H_ */
