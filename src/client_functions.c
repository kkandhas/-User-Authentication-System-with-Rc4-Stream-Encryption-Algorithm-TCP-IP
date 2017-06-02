
/*****************************************************************
*			HEADER FILE
*****************************************************************/

#include "client_header.h"


/********************************************************************
*
*  FUNCTION NAME :  myread
*  
*  DESCRIPTION   :  Reads the data   
*
*  RETURNS       :  Number of bytes successfully read or -1 on error
*
********************************************************************/

int myread(
	   int sd,       /* socket descriptor */
	   void* buffer, /* buffer */
	   int n         /* integer value */
	  )
{
        int                   ret = 0;
        int                   pointer = 0;
        int                   totnoofbytes = 0;
        while (1)
        {
                ret = read(sd, buffer + pointer, n - totnoofbytes);      /* reads feom the socket */
                if (0 > ret)
                {
                        C_ERROR(ERROR_MAJOR, ERROR_READ_FAILED);
                        exit(EXIT_FAILURE);;
                }

                totnoofbytes = totnoofbytes + ret;
                pointer = pointer + ret;
               
             
                if ((totnoofbytes == n) || (ret == 0))
                {
                        return totnoofbytes;
                }
        }
}




/*********************************************************************
* 
*  FUNCTION NAME :  registration
*
*  DESCRIPTION   :  Registers for new user
*
*  RETURNS       :  SUCCESS OR FAILURE
*
**********************************************************************/

int registration(
		 int sd,             /* socket descriptor */
		 char *username,    /* username */
		 char *password    /* password */
		) 
{
	
	char 	             buf[MAX];
	int 	             ret=0;

	
	memset(buf, 0, MAX);
	
        
	while (1)
        {
        	ret = read(sd, buf, MAX);   /* reads from the socket */
               	if (ERROR == ret)  /*error handling for read*/
                {

                	printf("READING AVAILABILITY STATUS\n");
                        C_ERROR(ERROR_MAJOR, ERROR_READ_FAILED);
                        exit (1);
                }

           	if (0 == strcmp(buf, "SUCCESS"))
                {
                	printf("USERNAME ALREADY AVILABLE\n");
                        printf("\nenter another username\n");
                        exit (1);
                        scanf("%s", username);
			write_string(sd, username);     /* calls write_string to write the username */
		}
		else
		{
			break;
		}
	}
	return SUCCESS;
}

/*********************************************************************
*
*  FUNCTION NAME :  validation
*
*  DESCRIPTION   :  validation is done for existing user
*
*  RETURNS       :  SUCCESS OR FAILURE
*
**********************************************************************/
 
int validation(
		int sd,         /* socket descriptor */
		char *username, /* username */
		char *password, /* password */
		int attempts   /* attempts value */
	      )
{
	char 	           status[MAX];
	int 	           ret=0;	


        while (attempts)
        {
				
		memset(status, 0, MAX);
                ret = read(sd, status, MAX);    /* reads status from socket */
                                
		if (ERROR == ret) 			/*error handling for read*/
                {
               		printf("READING AUTHENTICATION STATUS\n");
			C_ERROR(ERROR_MAJOR, ERROR_READ_FAILED);
                        exit (EXIT_FAILURE);
                }
                                
		printf("\nauthentication status: %s\n", status);
                                
		if (0 == strcmp(status, "FAILURE"))
                {
                        printf("READING AUTHENTICATION STATUS\n");
                        C_ERROR(ERROR_MAJOR, ERROR_INCORRECT_PASSWORD);
			printf("\nenter username\n");
			scanf("%s",username);
                                        
			printf("\nenter password\n");
                        scanf("%s", password);
                        write_string(sd, username);    /* calls write_string to write username */
                        write_string(sd, password);    /* calls write_string to write password */
			attempts--;                    /* decrementing the attempts value */
                }
                                
		else
                {
                        break;
                }
       	}
	
	if(0 == strncmp(status, "SUCCESS", strlen(status)))
	{
		return SUCCESS;
	}
	else
	{
		return FAILURE;
	}

}

/*****************************************************************************
*
*  FUNCTION NAME  :  write_string
*
*  DESCRIPTION    :  writes the string into the socket
*
*  RETURNS        :  SUCCESS OR FAILURE
*
******************************************************************************/

int write_string(
		 int sd,     /* socket descriptor */
		 char *word  
		)
{
	char 	              buffer[DIGIT];
	int 	              len=0;
	int 	              ret=0;	
	
	len = strlen(word);
        memset(buffer, 0, DIGIT);
        
	sprintf(buffer, "%d", len);    /* converts integer to character */
        buffer[strlen(buffer)] = '\0';

        ret = write(sd, buffer, DIGIT);	/*writes length of user name to socket*/
        
	if (ERROR == ret)			 /*error handling for write*/
        {
                printf("WRITING LENGTH OF STRING\n");
                C_ERROR(ERROR_MAJOR, ERROR_WRITE_FAILED);
        
	        exit (EXIT_FAILURE);
        }


        ret = write(sd, word, strlen(word)); 	/*writes user name to socket*/
        
	if (ERROR == ret) 				/*error handling for write*/
        {
                printf("WRITING STRING\n");
                C_ERROR(ERROR_MAJOR, ERROR_WRITE_FAILED);
                exit (EXIT_FAILURE);
        }

	
	return SUCCESS;
}

/*************************************************************************
*
*  FUNCTION NAME  :  check_length
*
*  DESCRIPTION    :  Finds the length of the given word and checks whether
*		     the word length is within the range
*
*  RETURNS        :  SUCCESS OR FAILURE
*
**************************************************************************/

int check_length(
		 char *word      
		)
{
	int len;
	
	while(1)
	{
		len = strlen(word);
	
		if((MIN_LEN > len))
		{
			printf("ENTERED STRING IS NOT WITHIN THE RANGE\n");
	
			printf("PLS ENTER AGAIN\n");
			scanf("%s", word);
		}
	
		if((MAX_LEN < len))
		{
			printf("ENTERED STRING IS NOT WITHIN THE RANGE\n");
	
			printf("PLS ENTER AGAIN\n");
			scanf("%s", word);
		}
		else
		{
			break;
		}
	}
	
	return SUCCESS;
}
