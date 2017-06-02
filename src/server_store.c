#include "server_header.h"

/******************************************************************************
*
*       Function Name   : store_path
*
*       Description     : stores the path of the file in a linked list.
*
*       Returns         : Success or Failure
*
*******************************************************************************/

int store_path(
                list **head /* head pointer */
              )
{
        
	while(1)
	{
		list	*newnode;
	        char	buf[30];

	        /* allocates memory to new node */
	        newnode = (list *)malloc(sizeof(list));
		if(NULL == newnode)
		{
			S_ERROR(ERROR_MAJOR, ERROR_MEMORY_ALLOCATION_FAIL);
			exit(EXIT_FAILURE);
		}

	        printf("enter filename\n");
	        scanf("%s", buf);
		newnode->filename = NULL;
        	/* allocates memory to filename */
	        newnode->filename = (char *)calloc((strlen(buf) + 1),sizeof(char));
		if(NULL == newnode->filename)
		{	
			S_ERROR(ERROR_MAJOR, ERROR_MEMORY_ALLOCATION_FAIL);
			exit(EXIT_FAILURE);
		}
	
	        strncpy(newnode->filename, buf, strlen(buf));
	        printf("enter filepath\n");
	        scanf("%s", buf);
		newnode->filepath = NULL;
        	/* allocates memory to filepath */
	        newnode->filepath= (char *)calloc((strlen(buf) + 1), sizeof(char));
		if(NULL == newnode->filepath)
		{	
			S_ERROR(ERROR_MAJOR, ERROR_MEMORY_ALLOCATION_FAIL);
			exit(EXIT_FAILURE);
		}
	        strncpy(newnode->filepath, buf, strlen(buf));
	        /* stores filename and filepath in the linked list till end is given */
	       // while (1)
	        //{
		if(NULL == (*head))
        	{
			(*head) = newnode;
			newnode->next = NULL;
        	}
        	else
        	{
			newnode->next = (*head);
		        (*head) = newnode;
        	}

                printf("enter continue or end to exit\n");
               	scanf("%s", buf);
	        if ( 0 == strncmp(buf,"end",3))
                {
			break;
                }
               		
        } /* end of while */
        return SUCCESS;
}


/*********************************************************************************************
*
*       Function Name    : hashing
*
*       Description  : finds hash key
*
*       Returns      : key value
*
***********************************************************************************************/

int hashing(
            int key /* key value */
           )
{
        return (key % MAX);
}

/***********************************************************************************************
*
*       Function Name    : store_db
*
*       Description	 : stores username and password in hash table
*
*       Returns          : Success and Failure
*
*************************************************************************************************/

int store_db(
             char *userid,  /* username */
             char *pwd,     /* password */
             node *detail[] /* pointer to node */
            )
{
        int	hash;
        hash = hashing(userid[0]);
        node *newnode;
        /* allocates memory to newnode */
        newnode = (node *)malloc(sizeof(node));
	if(NULL == newnode)
	{
		S_ERROR(ERROR_MAJOR, ERROR_MEMORY_ALLOCATION_FAIL);
		exit(EXIT_FAILURE);
	}
        /* allocates memory to username */
	
        newnode->username = (char *)calloc((strlen(userid) + 1), sizeof(char));
	if(NULL == newnode->username)
	{
		S_ERROR(ERROR_MAJOR, ERROR_MEMORY_ALLOCATION_FAIL);
		exit(EXIT_FAILURE);
	}
        strncpy(newnode->username, userid, strlen(userid));
        /* allocates memory to password */
        newnode->password = (char *)calloc((strlen(pwd) + 1), sizeof(char));
	if(NULL == newnode->password)
	{
		S_ERROR(ERROR_MAJOR, ERROR_MEMORY_ALLOCATION_FAIL);
		exit(EXIT_FAILURE);
	}
        strncpy(newnode->password, pwd, strlen(pwd));
        newnode->next = NULL;
        /* if the hash table is empty */
        if (NULL == detail[hash])
        {
                detail[hash] = newnode;
        }
        /* if the hash table is not empty */
        else
        {
                newnode->next = detail[hash];
                detail[hash] = newnode;
        }
        
        return SUCCESS;
}

/***********************************************************************************************
*
*       Function Name    : deallocate_hash
*
*       Description  : deallocates the hash table
*
*       Returns      : Success and Failure
*
*************************************************************************************************/
int deallocate_hash(node *detail[MAX])
{
        int	i;
        node	*temp;
        node	*temp2;

        for (i = 0; i < MAX; i++)
        {
                temp = detail[i];
                while(NULL != temp)
                {
                        temp2 = temp;
                        temp = temp ->next;
			
                        free(temp2->username);
                        temp2->username = NULL;
                        free(temp2->password);
                        temp2->password = NULL;
                        free(temp2);
                        temp2 = NULL;
                }
		detail[i] = NULL;
        }
        return SUCCESS;
}

/***********************************************************************************************
*
*       Function Name    : deallocate_list
*
*       Description  : deallocates the hash table
*
*       Returns      : Success and Failure
*
*************************************************************************************************/


int deallocate(list **head)
{
        list	*temp2;
        list	*temp;
	temp = *head;
	while(NULL != temp)
	{
		temp2 = temp;
		temp = temp->next;
		free(temp2->filename);
		temp2->filename = NULL;
		free(temp2->filepath);
		temp2->filepath = NULL;
		free(temp2);
		temp2 = NULL;
	}
		
        return SUCCESS;
}

