#ifndef _SERVER_H
#define _SERVER_H

/******************************************************************************
*			HEADER FILES
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
#include<signal.h>

/******************************************************************************
*			MACROS
******************************************************************************/
#define MAX 		10
#define SUCCESS 	0
#define FAILURE 	1
#define DIGIT 		2
#define ATTEMPTS 	3
#define MAXLEN 		256
#define BUFSIZE 	1024
#define STATUS_LEN 	7
#define ERROR		-1

/* error details */

#define ERROR_NONE     1
#define ERROR_CRITICAL 2
#define ERROR_MAJOR    3
#define ERROR_MINOR    4

/* ang_error is defined in anagram_error.c */
#define S_ERROR(err_level, err_code) ang_error(err_level, err_code)

/* Error Codes */
#define ERROR_UNEXPECTED_EOF 	0
#define ERROR_READ_FAIL 	1
#define ERROR_WRITE_FAIL 	2
#define ERROR_INVALID_USERNAME 	3
#define ERROR_INCORRECT_PWD 	4
#define ERROR_MEMORY_ALLOCATION_FAIL 5
#define ERROR_ATTEMPTS_EXCEEDED 6
#define ERROR_BIND_FAIL 	7
#define ERROR_ACCEPT_FAIL 	8
#define ERROR_SOCKET_FAIL 	9
/*****************************************************************************
*
*			STRUCTURE DEFINITIONS
*
******************************************************************************/
typedef struct node
{
	char *username;
	char *password;
	struct node *next;
}node;

typedef struct list
{
	char *filename;
	char *filepath;
	struct list *next;
}list;


/******************************************************************************
*
*			FUNCTION DECLARATIONS
*
******************************************************************************/
int hashing(int);
extern int authentication(char *,char *,int,node **,int);
extern int accessfile(int, char *);
extern int store_path(list **);
extern int encrypt(char *,char *,char []);
extern int decrypt(char *,char *,char []);
extern int check_availability(char *,node **);
extern int store_db(char *,char *,node **);
extern int retrieve_pwd(char *,node **,char *);
extern int access_file(int,char *,list *);
extern int deallocate_hash(node *detail[]);
extern int deallocate(list **);
extern void ang_error(int, int);
extern int read_string(int, char *);

#endif
