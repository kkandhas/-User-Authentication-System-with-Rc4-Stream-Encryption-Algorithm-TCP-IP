
/******************************************************************************
*                       HEADER FILES
******************************************************************************/

#include "client_header.h"


/******************************************************************************
*
*  FUNCTION NAME   : main
*       
*  DESCRIPTION     : Connects to the server and sends username, password,
*                    filename and selection.
*                    Displays the content of file for Authenticated User.
*       
*  RETURNS         : SUCCESS or FAILURE
*
******************************************************************************/

int main(
		int argc,	 	/*command line argument count*/
		char *argv[] 		/*command line argument vector*/
	)
{

        struct sockaddr_in 	server;
        int 			sd = 0; 		/*socket descriptor*/
        int 			selection = 0; 	/*switch case control variable*/
        char 			username[MAX];
        char			*password;
        char 			filename[MAX];
        int 			aut_status = 0;
        int 			attempts = ATTEMPTS;
        char 			buf[MAX];
        int 			ret=0; 		/*return status*/
	int			count = 0;
        char 			buffer[DIGIT];
	unsigned short 		rlen = 0;
       
	if (3 < argc)   /* extra data provided */
	{
		printf("\n Syntax to be followed is : ./client.exe <server IP-address> <Port-No>");
		exit(EXIT_FAILURE);
	}

	
        if (NULL == argv[1])   /* checks whether the ip address is entered or not*/

        {
                printf("Enter the Server Address\n");
                exit (EXIT_FAILURE);
        }

        if (NULL == argv[2])   /* checks whether the port no. is entered or not */

        {
		printf("Enter the Port number\n");
                exit (EXIT_FAILURE);
        }

        memset(&server, 0, sizeof(server));
        
	server.sin_family = AF_INET;
        server.sin_port = htons(atoi(argv[2]));
        server.sin_addr.s_addr = inet_addr(argv[1]);
        
	sd = socket(AF_INET, SOCK_STREAM, 0);    /* calls socket call to establish the socket */
        ret = connect(sd, (struct sockaddr *)&server, sizeof(server));    /* calls connect call to establish the connection */
        
	if (ERROR == ret)     /*checks whether the sever is connected or not*/
        {
		printf("ret value of connect is %d\n", ret);
		C_ERROR(ERROR_MAJOR, ERROR_CONNECT_FAILED);
                exit (EXIT_FAILURE);
        }
        
	printf("1. Register\n");
        printf("2. Login\n");

        printf("\n Enter your choice:\t");
        scanf("%d", &selection);
	

	memset(username, 0, MAX);
	memset(password, 0, MAX);
	memset(filename, 0, MAX);
        
	printf("\n Enter username\n");
        scanf("%s", username);
	
	ret = check_length(username);      /* calls check_length function to check the length of the username */
        
//	printf("\nenter password\n");
//      scanf("%s", password);

	password = getpass("\n Enter password :");	
	
	ret = check_length(password);      /* calls check_length function to check the length of the password */

        


        ret = write_string(sd, username); 	/*writes user name to socket*/
        if (FAILURE == ret) 			/*error handling for write*/
        {
		printf("\n On WRITING USERNAME"); 
		C_ERROR(ERROR_MAJOR, ERROR_WRITE_FAILED);
                exit (EXIT_FAILURE);
        }




        ret = write_string(sd, password); 	/*writes password to socket*/
        
	if (FAILURE == ret) 			/*error handling for write*/
        {
		printf("WRITING PASSWORD\n");
		C_ERROR(ERROR_MAJOR, ERROR_WRITE_FAILED);
                exit (EXIT_FAILURE);
        }

	memset(buffer, 0, DIGIT);
        sprintf(buffer, "%d", selection);        /* converts integer to character */
        buffer[strlen(buffer)] = '\0';
        ret = write(sd, buffer, DIGIT);          /* writes the selection to socket */
        if (ERROR == ret) 				/*error handling for write*/
        {
		printf("WRITING SELECTION\n");
		C_ERROR(ERROR_MAJOR, ERROR_WRITE_FAILED);
                exit (EXIT_FAILURE);
        }


	switch (selection)
        {
                case REGISTER: /*REGISTRATION*/
                {
			aut_status = registration(sd, username, password);    /* calls registration function to re									gister username and password */
                        break;
                }

                case LOGIN: /*LOGIN*/
                {
			aut_status = validation(sd, username, password, attempts);    /* calls validation function									 to validate the username and password */
			break;
                }
                default:
                {
                        printf("\nINVALID SELECTION\n");
                        exit (EXIT_FAILURE);
                }
        }
	
        if (SUCCESS == aut_status)
        {
        	printf("\nenter filename\n");
        	scanf("%s", filename);

		check_length(filename);   /* calls check_length function to check the length of the filename */
        	        
               	ret = write_string(sd, filename);   /* calls write_string function to write the filename */
                if (FAILURE == ret) 		/*error handling for write*/
                {
			printf("ERROR IN SENDING FILENAME\n");
			C_ERROR(ERROR_MAJOR, ERROR_WRITE_FAILED);            
			exit (EXIT_FAILURE);
                }
               
              	memset(buf, 0, MAX);
        	
		ret = myread(sd, &rlen, sizeof(rlen));     /* reads the length from socket */

                if (ERROR == ret)   /* error handling for read */
                {
			printf("READING SIZE OF FILE\n");
			C_ERROR(ERROR_MAJOR, ERROR_READ_FAILED);            
                        exit (EXIT_FAILURE);
                }

		count = ntohs(rlen);
		printf("size of file ret = %d\n",ret);
                ret = read(sd,buf,count);   /* reads the size of the file from socket */
    		if (ERROR == ret)   /*error handling for read*/
		{
			printf("READING CONTENTS OF FILE\n");
			C_ERROR(ERROR_MAJOR, ERROR_READ_FAILED);            
			exit(EXIT_FAILURE);
		}

                printf("\ncontent of the file:\n");
                write(1, buf, ret);     /* displays the content of the file in the screen */

	}
        else
        {
                printf("AUTHENTICATION FAILURE\n");
        }
 	return SUCCESS;

}

