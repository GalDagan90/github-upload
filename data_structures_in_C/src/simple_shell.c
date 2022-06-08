/**************************************************************
* Author: Gal Dagan
* Date: 
* Reviewer: 
*
* Description: Implementation of a simple scheduler
*
* InfintyLabs OL108
**************************************************************/
#include <assert.h>     /* assert() */
#include <stdio.h>      /* printf() */  
#include <stdlib.h>     /* system() */ 
#include <sys/types.h>  
#include <unistd.h>

int main ( )
{
    int return_value ;
    return_value = system ( "ls -l /" );
    
    return return_value;
} 
