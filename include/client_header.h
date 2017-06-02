
#ifndef _CLIENT_H
#define _CLIENT_H
/******************************************************************************
*                       HEADER FILES
******************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<string.h>

/******************************************************************************
*                       MACROS
******************************************************************************/
#define MAX		10
#define SUCCESS		0	
#define FAILURE 	1
#define DIGIT		2
#define ATTEMPTS 	3
#define MIN_LEN 	4
#define MAX_LEN 	10
#define MAXLEN 		1024
#define STATUS_LEN 	7
#define ERROR 		-1
#define REGISTER 	1
#define	LOGIN 		2



/* error details */

#define ERROR_NONE     1
#define ERROR_CRITICAL 2
#define ERROR_MAJOR    3
#define ERROR_MINOR    4

/* ang_error is defined in anagram_error.c */
#define C_ERROR(err_level, err_code) ang_error(err_level, err_code)

/* Error Codes */
#define ERROR_UNEXPECTED_EOF	  	0
#define ERROR_READ_FAILED  	  	1
#define ERROR_WRITE_FAILED		2
#define ERROR_INVALID_USERNAME		3
#define ERROR_INCORRECT_PASSWORD   	4
#define ERROR_MEMORY_ALLOCATION_FAIL  	5
#define ERROR_ATTEMPTS_FAILED		6
#define ERROR_CONNECT_FAILED		7
#define ERROR_SOCKET_FAIL 		8


/******************************************************************
*		Function Protypes
*******************************************************************/

extern int myread(int, void*, int );
extern void ang_error(int, int);
extern int registration(int, char *, char *);
extern int validation(int, char *, char *,int);
extern int write_string(int, char *);
extern int check_length(char *);
#endif
