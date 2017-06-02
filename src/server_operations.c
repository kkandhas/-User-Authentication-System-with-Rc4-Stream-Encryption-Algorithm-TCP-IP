#include "server_header.h"

static void swap(int *, int *);
/*************************************************************************************************
*
*	Function Name : check_availability
*
*       Description  : checks whether the username is already available in the hash table or not
*
*       Returns      : Success or Failure
*
***************************************************************************************************/

int check_availability(
                       char *userid, /* username */
                       node *detail[] /* pointer to node */
                      )
{
        int 		hash = 0;
        node 		*temp = NULL;

        hash = hashing(userid[0]);
        /* if the hash table is empty */
        if (NULL == detail[hash])
        {
                return FAILURE;
        }
        /* if the hash table is not empty */
        else
        {
                temp = detail[hash];
                while (NULL != temp)
                {
                        if (0 == strcmp(userid, temp->username))
                        {
                                return SUCCESS;
                        }
                        temp = temp->next;
                }
                return FAILURE;
        }
}

/*******************************************************************************************************
*
*       Function Name    : retrieve_pwd
*
*       Description  : retrieves the encrypted password from the hash table
*
*       Returns      : Success or Failure
*
***********************************************************************************************************/

int retrieve_pwd(
                 char *userid,  /* username */
                 node *detail[],/* pointer to node */
                 char *pwd_db  /* char pointer */
                )
{
	
        int 		hash = hashing(userid[0]);
        node 		*temp;
        temp = detail[hash];

        /* retrieves the corresponding password which matches the username
           from the hash table
         */
        while (NULL != temp)
        {
                if (0 == strcmp(userid, temp->username))
                {
                        strncpy(pwd_db, temp->password, strlen(temp->password));
                }
                temp = temp->next;

	}
        return SUCCESS;
}


/**********************************************************************************************************
*
*       Function Name     : encrypt
*
*       Description   : encryptes the password using RC4 algorithm
*
*       Returns       : Success or Failure
*
**************************************************************************************************************/

int encrypt(
            char *pwd,    /* password */
            char *cipher, /* cipher key */
            char key[]    /* array of character key */
           )
{

        char 		*buf;
        int 		s[MAXLEN];
	int		t1[MAXLEN];
        int 		keylen = 0;
	int		i = 0;
	int		j = 0;
	int		k = 0;
	int		t = 0;
	int		len = 0;

        buf= (char *)malloc(MAX * sizeof(char));
	if(NULL == buf)
	{
		S_ERROR(ERROR_MAJOR, ERROR_MEMORY_ALLOCATION_FAIL);
		exit(EXIT_FAILURE);
	}

        len = strlen(pwd);
        //printf("len = %d\n", len);

        keylen = strlen(key);

        for (i = 0; i < MAXLEN; i++)  /*initialization of state vector and temporary vector with key values */
        {
                s[i] = i;
                t1[i] = key[i % keylen];
        }

        j=0;
        for (i=0; i < MAXLEN; i++)  /* initial permutation of state vector */
        {
                j = (j + s[i] + t1[i]) % MAXLEN;
                swap(&s[i], &s[j]);
        }
        for (k=0; k<len; k++)  /*stream generation with key values */
        {
                i =(i + 1) % MAXLEN;
                j=(j + s[i]) % MAXLEN;
                swap(&s[i], &s[j]);
                t=(s[i] + s[j]) % MAXLEN;
                buf[k] = s[t];
        }

        for (i = 0; i < len; i++)  /* forming ciper by performing XOR with key stream */
        {
                cipher[i] = (buf[i]) ^ (pwd[i]);
        }
        
	free(buf);
        return SUCCESS;
}

/************************************************************************************************
*
*       Function name     : swap
*
*       Description   : swaps two values
*
*       Returns       : nothing
*
*************************************************************************************************/
void swap(
          int *x, /* x value */
          int *y /* y value */
         )
{
        int t;
        t = *x;
        *x = *y;
        *y = t;
}

/*************************************************************************************************
*
*       Function Name    : decrypt
*
*       Description  : decryptes the password
*
*       Returns      : Success or Failure
*
*************************************************************************************************/

int decrypt(
            char *cipher, /* cipher key */
            char *pwd_db, /* pointer to character */
            char key[]    /* array of character key */
           )
{
        char 		*buf = NULL;
        int 		s[MAXLEN];
	int		t1[MAXLEN];
        int 		keylen = 0;
	int		i = 0;
	int		j = 0;
	int		k = 0;
	int		t = 0;
	int		len = 0;

        buf = (char *)calloc(MAX, sizeof(char));
	if(NULL == buf)
	{
		S_ERROR(ERROR_MAJOR, ERROR_MEMORY_ALLOCATION_FAIL);
		exit(EXIT_FAILURE);
	}

        len = strlen(cipher);
        //printf("len = %d\n", len);

        keylen = strlen(key);
        for (i = 0; i < MAXLEN; i++)  /* initializing state vector */
        {
                  s[i] = i;
                  t1[i] = key[i % keylen];
        }
        for (i = 0; i < MAXLEN; i++) /* permutation of state vector */
        {
                   j = (j + s[i] + t1[i]) % MAXLEN;
                   swap(&s[i], &s[j]);
        }

        
        for (k = 0; k <len; k++) /* stream generation */
        {

                   i = (i + 1) % MAXLEN;
                   j=(j + s[i]) % MAXLEN;
                   swap(&s[i], &s[j]);
                   t = (s[i] + s[j]) % MAXLEN;

                   buf[k] = s[t];
        }
        for (i = 0; i < len; i++)
        {
                pwd_db[i] = (cipher[i]) ^ (buf[i]);
        }
	free(buf);
      	return SUCCESS;
}

      


