
/*****************************************************************************
*                       HEADER FILES
******************************************************************************/

#include "server_header.h"

/******************************************************************************
*			LOCAL FUNCTION DECLARATION
******************************************************************************/

static int mywrite(int, void*,int );

/**********************************************************************************
*
*	Funciton Name    : authenctication
*
*	Description      : According to the choice given, performs authentication for
		           new user and existing user. 
*
*	Returns          : Success or Failure
*
**************************************************************************************/

int authentication(
		    char *userid,   /* username */
		    char *pwd,      /* password */
		    int choice,     /* choice */
		    node *detail[], /* pointer to node */
		    int fd          /* socket descriptor */
		   )
{
	int 		ret = 0;
	char 		cipher[MAX];
	char 		pwd_db[MAX];
	char 		key[MAX];
	int 		aut_status = 0;
	int 		n = ATTEMPTS;
	memset(key, 0, MAX);
	memset(cipher, 0, MAX);
	memset(pwd_db, 0, MAX);
	strcpy(key, "abcd"); 

	switch (choice)
	{
		/* if the user is a new user */
		case 1:
		{
			while (1)
			{
				
				/* checks if the username is already present or not */
				ret = check_availability(userid, detail);
				/* if the username is not present */
				if (FAILURE == ret)
				{
					printf("username is new\n");
					/* writes FAILURE to the socket */
					write(fd, "FAILURE", STATUS_LEN);					
					break;
				}
				/* If the username is already present */
				else
				{
					memset(userid, 0, MAX);
					/* writes SUCCESS to the socket */
					write(fd, "SUCCESS", STATUS_LEN);			
					S_ERROR(ERROR_MINOR, ERROR_INVALID_USERNAME);
					read_string(fd, userid);
				}
			}
		
			/* calls encrypt function to encrypt the password */
			ret = encrypt(pwd, cipher, key);
			printf("cipher=%s\n",cipher);

			/* error handling for encrypt function */
			if (FAILURE == ret)
			{
				printf("encryption failure");
				exit(1);
			}

	
			/* if return value is SUCCESS , then the username and encrypted	
		 	   password is stored in the hash table 
			*/

			ret = store_db(userid, cipher, detail);
			/* error handling for store_db function */

			if (FAILURE == ret)
			{
				printf("storing in hashtable failure");
				exit(1);
			}
			aut_status = SUCCESS;
			break;			
		}/* end of case 1 */ 	
		/* if the user is a existing user */

		case 2:
		{	
			/* loops for n number of times , n is 3 */

			while (n)
			{	
				/*calls retrieve_pwd function to retrieve encrypted passwod from hash table */
				ret = retrieve_pwd(userid, detail, cipher);
		
				/* calls decrypt function to decrypte the encrypted password */
				
				ret = decrypt(cipher, pwd_db, key);
				printf("pwd_db=%s\n", pwd_db);

				/* If the password matches */
				if (0 == strncmp(pwd, pwd_db, strlen(pwd)))
				{
					write(fd,"SUCCESS", STATUS_LEN);
					printf("authentication SUCCESS\n");	
					aut_status = SUCCESS;
					break;
				}

				/* If the password doesnot match */
				else
				{
					memset(pwd,0,MAX);
					write(fd,"FAILURE",STATUS_LEN);
					n--;
					printf("INCORRECT PASSWORD");
					S_ERROR(ERROR_MAJOR, ERROR_INCORRECT_PWD);
					read_string(fd, userid);
					read_string(fd, pwd);
					aut_status = FAILURE;
				} /* end of else */
			} /* end of while */
			break;

		}/* end of case 2 */
	}/* end of switch case */
	printf("END AUTHENTICATION\n");
	return aut_status;
} 




/*****************************************************************************************************
*
*	Function Name     : access_file
*
*	Description	  : retrieves the file content from the specified filename	and writes the content
*			to the socket
*
*	Returns      	  : Success or Failure
*
********************************************************************************************************/

int access_file(
                int sd,         /* socket descriptor */
                char *filename, /* pointer to character */
                list *head      /* head pointer */
               )
{
	
        list 		*temp = head;
        int 		fd = 0;
        char 		buf[MAXLEN];
        int 		count = 0;
	int		ret = 0;
	int 		bytesread = 0;
        unsigned short 	slen = 0;
	int 		status = 1;

	memset(buf, 0, MAX);
        /* opens the specified file and writes the content of the file
           to the socket
        */

       
	if(NULL == buf)  /*no file in the server */
	{
		S_ERROR(ERROR_MAJOR, ERROR_MEMORY_ALLOCATION_FAIL);
		exit(EXIT_FAILURE);
	}
        while (NULL != temp)  // loop to find corresponding path of the file 
        {
                if (0 == (status = strncmp(temp->filename, filename, strlen(filename)))) // identifies the path 
                {
                        memset(buf, 0, MAX);

                        fd = open(temp->filepath, O_RDONLY);
			if(ERROR == fd)
			{
				printf("FILE OPENING FAILS\n");
				exit(EXIT_FAILURE);
			}
			count = lseek(fd, 0L, SEEK_END);
			slen = htons(count);
			
			ret = mywrite(sd, &slen, sizeof(slen)); // writes the size of the file into socket
			if(0 > ret)
			{
				printf("ERROR IN WRITING FILE SIZE\n");
				exit(EXIT_FAILURE);
			}	
			ret = lseek(fd, 0L, SEEK_SET);		
                       	while((bytesread = read(fd, buf, BUFSIZE))>0) 
			{
				//printf("bytesread=%d\n",bytesread);
				ret = write(sd, buf, bytesread); // writes the content into the file
				if(ERROR == ret)
				{
					printf("Error in sending size of file\n");
					exit(EXIT_FAILURE);
				}
			}
                        break;
                }
		
		temp = temp->next;
		
        }
	if(NULL == temp && 0 != status)  // if the file is not there in the server
	{
		memset(buf, 0, MAXLEN);
		strcpy(buf,"\nfile not exist\n");
		count = strlen(buf);
		slen = htons(count);
			
		ret = write(sd, &slen, sizeof(slen));
		if(ERROR == ret)
		{
			printf("Error in sending file size\n");
			exit(EXIT_FAILURE);;
		}	
			
           	ret = write(sd, buf, strlen(buf));
		if(ERROR == ret)
		{
			printf("Error in sending size of file\n");
			exit(EXIT_FAILURE);
		}
		
	}
	
	close(fd);
        return SUCCESS;
}

/************************************************************************************************************
*
*		Function Name 	:	 mywrite
*		Description	:	 partial write for writing contents into socket
*		Returns		:	 SUCCESS
*
************************************************************************************************************/
int  mywrite(int sd, void* buffer, int n)
{
        int 		ret = 0;
        int 		pointer = 0;
        int 		totnoofbytes = 0;
        while (1)
        {
                ret = write(sd, buffer + pointer, n - totnoofbytes);
                if (ERROR ==  ret)
                {
                        S_ERROR(ERROR_MAJOR, ERROR_WRITE_FAIL);
                        exit(EXIT_FAILURE);
                }

                totnoofbytes = totnoofbytes + ret;
                pointer = pointer + ret;

                if (totnoofbytes == n)
                {
                        return totnoofbytes;
                }
        }
	return SUCCESS;
}


/********************************************************************************************************
*
*		function Name	:	read_string
*		Description	:	reads the string from file
*		Returns		:	nothing
*
*********************************************************************************************************/
										
int read_string(int fd,char *word)
{
	int 		ret = 0;
	int 		len = 0;
	char 		buf[MAX];

	ret = read(fd, buf, DIGIT);// reads the length of the word from socket
        if (ERROR == ret)// error handling for read call
        {
        	S_ERROR(ERROR_MAJOR, ERROR_READ_FAIL);
                exit(EXIT_FAILURE);
        }
        len = atoi(buf);// converts a string to integer to get the length of the word
        
        ret = read(fd, word, len);// reads the username from socket
        if (ERROR == ret)// error handling for read call
        {
       		S_ERROR(ERROR_MAJOR, ERROR_READ_FAIL);
                exit(EXIT_FAILURE);
        }
	return SUCCESS;
}
								
				
	
	
	

