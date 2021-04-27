/*
 * string_parser.c
 *
 *  Created on: April 13, 2021
 *      Author: Harrison Chebuk
 *
 */
#define _GNU_SOURCE
#include <stdio.h>
/*
 * string_parser.c
 *
 *  Created on: April 9th, 2021
 *      Author: Harrison Chebuk
 */

#include <stdlib.h>
#include <string.h>
#include "string_parser.h"


int count_token (char* buf, const char* delim)
{
	/*
	*	#1.	Check for NULL string
	*	#2.	iterate through string counting tokens
	*		Cases to watchout for
	*			a.	string start with delimeter
	*			b. 	string end with delimeter
	*			c.	account NULL for the last token
	*	#3. return the number of token (note not number of delimeter)
	*       Note: This will modify the buf up to the delimeter, so pass in a copy
	*/
	int num_token = 0;
	char *token = strtok(buf, delim); //create a token to the next delim
	while (token != NULL){
		num_token++; //count the token
		token = strtok(NULL, delim); //create the next token
	}
	return num_token;
}

command_line str_filler (char* buf, const char* delim)
{
	/*
	*	#1.	create command_line variable to be filled
	*	#2.	use function strtok_r to take out \n character then count
	*			number of tokens with count_token function, set num_token.
	*	#3. malloc memory for token array inside command_line variable
	*			base on the number of tokens.
	*	#4. malloc each index of the array with the length of tokens,
	*			fill array with tokens, and fill last spot with NULL.
	*	#5. return the variable.
	*/
	command_line filled;
	char *save; //our save point for strtok_r
	char *bufCopy = strdup(buf); //a duplicate of the buf so we don't modify it
	strtok_r(bufCopy, "\n", &save); //remove the new line
	int count = count_token(bufCopy, delim);
	filled.num_token = count;
	filled.command_list = (char **) malloc(sizeof(char *) * (count + 1)); //+1 for the final NULL
	strtok_r(buf, "\n", &save); //remove the new line from buff to prevent weird print.
	
	char *token = strtok(buf, delim);
	for (int i = 0; i < count; i++){
		filled.command_list[i] = (char *) malloc(sizeof(char) * (strlen(token) + 1)); //+1 for the NULL
		strcpy(filled.command_list[i], token); //copy over the token
		token = strtok(NULL, delim); //iterate to next token
	}
	filled.command_list[count] = NULL; //set last pos to NULL
	free(bufCopy); //free our duplicate
	return filled;
}


void free_command_line(command_line* command)
{
	/*
	*	#1.	free the array base num_token
	*/
	for (int i = 0; i <= command->num_token; i++)
		free(command->command_list[i]); //free internal
	free(command->command_list); //free outer
}
