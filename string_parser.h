/*
 * string_parser.h
 *
 *  Created on: April 13th, 2021
 *      Author: Harrison Chebuk
 *  Description: Used to tokenize strings based on the provided delim in the form of a command_line struct
 *
 */

#ifndef STRING_PARSER_H_
#define STRING_PARSER_H_


#define _GNU_SOURCE


typedef struct
{
    char** command_list;
    int num_token;
}command_line;

//this functions returns the number of tokens needed for the string array
//based on the delimeter
int count_token (char* buf, const char* delim);

//This functions can tokenize a string to token arrays base on a specified delimeter,
//it returns a struct variable
command_line str_filler (char* buf, const char* delim);


//this function safely free all the tokens within the array.
void free_command_line(command_line* command);


#endif /* STRING_PARSER_H_ */