#include"server_header.h"

/********************************************************************
*                       Global variables
********************************************************************/
int g_error_level = ERROR_MAJOR;

char *g_error_code_to_str[] =
{
  "Unexpected End of File",     //ERROR_UNEXPECTED_EOF
  "Read Failed",           	//ERROR_READ_FAILED
  "Write Failed",		//ERROR_WRITE_FAILED
  "Invalid Username",		//ERROR_INVALID_USERNAME
  "Incorrect username and Password",         // ERROR_INCORRECT_PASSWORD
  "Memory Allocation Failure ", // ERROR_MEMORY_ALLOCATION
  "Attempts Exceeded",		//ERROR_ATTEMPS_EXCEEDED
  "Bind Failed",			//ERROR_BIND_FAILED
  "Accept Failed",		//ERROR_ACCEPT_FAILED
  "socket creation failure"	//ERROR_SOCKET
};

/********************************************************************
*
* FUNCTION NAME: ang_error
*
* DESCRIPTION: Prints the error message along with the error number
*
* RETURNS: Returns void
*********************************************************************/
void ang_error(int err_level,
               int err_code)
{
    /* compare the err_level parameter against the global flag to see
       whether the input error level is suppressed or not. If not then
       the input error message is printed */
	printf("error code %d\n",err_code);
    if(err_level <= g_error_level)
    {
        printf("Error : (%s)\n", g_error_code_to_str[err_code]);
    }
}

