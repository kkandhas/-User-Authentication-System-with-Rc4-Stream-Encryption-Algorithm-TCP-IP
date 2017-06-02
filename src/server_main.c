/*****************************************************************************
*                       HEADER FILES
******************************************************************************/
#include "server_header.h"
/*****************************************************************************
*                       LOCAL MACROS
******************************************************************************/
#define MYPORT 6000 

/***************************************************************************
*			GLOBAL VARIABLES
****************************************************************************/
node *detail[MAX];
list *head = NULL;

/******************************************************************************
*
*       Function Name   : main
*
*       Description     : Accepts username, password, filename and selection.
*			  And performs authentication process,if the authentication 
*			  is success, it sends the file content to the client.
*
*       Returns         : Success or Failure
*
*******************************************************************************/
static void handler(int);
int main(int argc,char *argv[])
{
        int 		sd = 0;
	int 		con_fd = 0;
        int 		ret = 0;
	int 		aut_status = FAILURE;
        unsigned int    len = 0;
	int    		selection = 0;
        char 		username[MAX];
        char 		password[MAX];
	char 		filename[MAX];
	char 		buffer[20];
        char 		buf[DIGIT];
        struct 		sockaddr_in server;
	struct		sockaddr_in client;

	if(SIG_ERR == signal(SIGINT,handler)) // Registers signal handler for SIGINT signal
	{
		printf("ERROR IN SIGNAL HANGLING\n");
		exit(EXIT_FAILURE);
	}

	if(argc == 2)
	{

	}

     	memset(username, 0, MAX);
	memset(password, 0, MAX);
	memset(filename, 0, MAX);
        memset(&server, 0, sizeof(server));
        memset(&client, 0, sizeof(client));
        server.sin_family = AF_INET;
        server.sin_port = htons(MYPORT); // PORT NO
        server.sin_addr.s_addr = htonl(INADDR_ANY); // IP ADDRRESS 

        ret = store_path(&head);// calls store_path to store the filename and filepath in linked list 

	sd = socket(AF_INET, SOCK_STREAM, 0); // creation of TCP socket
	if(ERROR == sd)
	{
		S_ERROR(ERROR_MAJOR, ERROR_SOCKET_FAIL);
		exit(EXIT_FAILURE);
	}

        ret = bind(sd, (struct sockaddr *)&server, sizeof(server)); //binding server with local address
        if (ERROR == ret)// error handling for bind 
        {
               	S_ERROR(ERROR_MAJOR, ERROR_BIND_FAIL); 
                exit(EXIT_FAILURE);
        }	

        listen(sd, 5);// listens for request from client
        while (1)   // Beginning of Main Server Processing Loop
	{
                printf("SERVER : WAITING IN LOOP\n");
                len = sizeof(client);
		// accept system call to accept the connection from client

          	con_fd = accept(sd, (struct sockaddr *)&client, &len);
                if (ERROR == con_fd)// error handling for accept call 
                {
                        S_ERROR(ERROR_MAJOR, ERROR_ACCEPT_FAIL);
                        exit(EXIT_FAILURE);
                }
                printf("connection from %s",inet_ntop(AF_INET, &client.sin_addr, buffer, sizeof(buffer)));

		read_string(con_fd, username);//reads username from socket
		read_string(con_fd, password); //reads password from socket
		ret = read(con_fd, buf, DIGIT);// reads the selection from socket 

                if (ERROR == ret)// error handling for read call
                {
                        S_ERROR(ERROR_MAJOR, ERROR_READ_FAIL);
                        exit(EXIT_FAILURE);
                }

                selection = atoi(buf);// converts a string into integer to get the selection number 
	
		printf("selection %d\n", selection);
		// calls authentication function to perform authentication

		aut_status = authentication(username, password, selection, detail, con_fd);
		fflush(stdout);
		/* If the Authentication Status is SUCCESS, the file content is given to the client */
		if (SUCCESS == aut_status)
		{
			printf("AUTHENTICATION SUCCESS\n");				
			read_string(con_fd, filename);
			/*calls access_file function to retrieve contents of the specified file from client */

			ret = access_file(con_fd, filename, head);
		}
		else
		{
			printf("AUTHENTICATION FAILURE\n");
		
		}
		close(con_fd);//closes the socket descriptor
	}
	return SUCCESS;
}

/****************************************************************************
*
*	Function Name	:	handler
*	Description	:	handles the signal when ctrl-c is pressed
*	Returns		:	nothing
*
****************************************************************************/



void handler(int sig_no)
{
	if(sig_no == SIGINT)
	{
		printf("CTRL C pressed\n");
		deallocate_hash(detail); //dellocates the memory allocated for hash table 
		deallocate(&head); //deallocated the list
		exit(EXIT_SUCCESS);
	}
	
}
