/*
 * main.c
 *
 *  Created on: April 13th, 2021
 *      Author: Harrison Chebuk
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "command.h"
#include "string_parser.h"

int main(int argc, char const *argv[])
{
	//opening file to read
	//FILE *inFPtr;
	//inFPtr = fopen (argv[1], "r");
	//declear line_buffer
	size_t len = 128;
	char* line_buf = (char *) malloc(len);

	command_line large_token_buffer;
	command_line small_token_buffer;
	char *controlError = "Error! Incorrect syntax. No control code found.\n";
	char *argsError = "Error! Unsupported parameters for command: ";
	char *syntaxError = "Error! Unrecognized command: ";
	char *flagError = "Error! Unrecognized flag: ";
	char *inputFileError = "Unable to open file: ";
	char *commandError = "Error! Wrong command structure. Proper command structure is ./pseudo-shell [flags] <filename>\n";
	char *tempBuf;
	int args;
	int exit = 0;
	int fileMode = 0;
	FILE *source = stdin;
	FILE *redirect;

	//check what mode we are in (interactive or file) and do the proper things
	if (argc == 3 && strcmp(argv[1], "-f") == 0){
		//redirect to an output file when -f is specefied
		fileMode = 1; //so we don't close a file that's not open
		source = fopen(argv[2], "r"); //the file containing commands
		redirect = freopen("output.txt", "w+", stdout); //redirect output
		if (source == NULL){
			//error is still written to output.txt
			write(STDOUT_FILENO, inputFileError, strlen(inputFileError));
			write(STDOUT_FILENO, argv[1], strlen(argv[1]));
			write(STDOUT_FILENO, "\n", 2);
			return 1; //there was an error executing, we need to quit
		}
	}
	else if (argc > 1){		
		if (argv[1][0] == '-'){  //given flag, but not proper
			write(STDOUT_FILENO, flagError, strlen(flagError));
			write(STDOUT_FILENO, argv[1], strlen(argv[1]));
			write(STDOUT_FILENO, "\n", 2);
		}
		else //wrong structure to the command
			write(STDOUT_FILENO, commandError, strlen(commandError));
	}
	else
		write(STDOUT_FILENO, ">>> ", 4); //only do this if we aren't in file mode

	//loop until the file is over
	while (getline (&line_buf, &len, source) != -1)
	{
		//tokenize line buffer
		//large token is seperated by ";"
		
		//check for lack of control code
		tempBuf = strdup(line_buf); //because count_token modifies the original string
		if ((count_token(tempBuf, ";") - 1) == 0){
			write(STDOUT_FILENO, controlError, strlen(controlError));
			free(tempBuf);
			if (fileMode == 0)
				write(STDOUT_FILENO, ">>> ", 4); //only write this if we are in interactive mode
			continue; //there was control code so we skip the rest of this execution and wait for another input
		}
		else{
			//tokenize large buffer
			free(tempBuf);
			large_token_buffer = str_filler(line_buf, ";");
		}

		//iterate through each large token
		for (int i = 0; large_token_buffer.command_list[i] != NULL; i++)
		{
			//smaller token is seperated by " "(space bar)
			tempBuf = strdup(large_token_buffer.command_list[i]); //another copy so we don't gotta worry about modifying the string
			args = count_token(tempBuf, " ");
			free(tempBuf);
			small_token_buffer = str_filler(large_token_buffer.command_list[i], " ");
			
			//check for a matching command at the first spot
			if (strcmp(small_token_buffer.command_list[0], "ls") == 0){
				if (args > 1){
					write(STDOUT_FILENO, argsError, strlen(argsError));
					write(STDOUT_FILENO, small_token_buffer.command_list[0], strlen(small_token_buffer.command_list[0]));
					write(STDOUT_FILENO, "\n", 2);
				}
				else
					listDir();
			}
			else if (strcmp(small_token_buffer.command_list[0], "pwd") == 0){
				if (args > 1){
					write(STDOUT_FILENO, argsError, strlen(argsError));
					write(STDOUT_FILENO, small_token_buffer.command_list[0], strlen(small_token_buffer.command_list[0]));
					write(STDOUT_FILENO, "\n", 2);
				}
				else
					showCurrentDir(); 
			}
			else if (strcmp(small_token_buffer.command_list[0], "mkdir") == 0){
				if (args > 2 || args < 2){
					write(STDOUT_FILENO, argsError, strlen(argsError));
					write(STDOUT_FILENO, small_token_buffer.command_list[0], strlen(small_token_buffer.command_list[0]));
					write(STDOUT_FILENO, "\n", 2);
				}
				else
					makeDir(small_token_buffer.command_list[1]);
			}
			else if (strcmp(small_token_buffer.command_list[0], "cd") == 0){
				if (args > 2 || args < 2){
					write(STDOUT_FILENO, argsError, strlen(argsError));
					write(STDOUT_FILENO, small_token_buffer.command_list[0], strlen(small_token_buffer.command_list[0]));
					write(STDOUT_FILENO, "\n", 2);
				}
				else
					changeDir(small_token_buffer.command_list[1]); 
			}
			else if (strcmp(small_token_buffer.command_list[0], "cp") == 0){
				if (args > 3 || args < 3){
					write(STDOUT_FILENO, argsError, strlen(argsError));
					write(STDOUT_FILENO, small_token_buffer.command_list[0], strlen(small_token_buffer.command_list[0]));
					write(STDOUT_FILENO, "\n", 2);
				}
				else{
					copyFile(small_token_buffer.command_list[1], small_token_buffer.command_list[2]); 
				}
			}
			else if (strcmp(small_token_buffer.command_list[0], "mv") == 0){
				if (args > 3 || args < 3){
					write(STDOUT_FILENO, argsError, strlen(argsError));
					write(STDOUT_FILENO, small_token_buffer.command_list[0], strlen(small_token_buffer.command_list[0]));
					write(STDOUT_FILENO, "\n", 2);
				}
				else
					moveFile(small_token_buffer.command_list[1], small_token_buffer.command_list[2]); 
			}
			else if (strcmp(small_token_buffer.command_list[0], "rm") == 0){
				if (args > 2 || args < 2){
					write(STDOUT_FILENO, argsError, strlen(argsError));
					write(STDOUT_FILENO, small_token_buffer.command_list[0], strlen(small_token_buffer.command_list[0]));
					write(STDOUT_FILENO, "\n", 2);
				}
				else
					deleteFile(small_token_buffer.command_list[1]); 
			}
			else if(strcmp(small_token_buffer.command_list[0], "cat") == 0){
				if (args > 2 || args < 2){
					write(STDOUT_FILENO, argsError, strlen(argsError));
					write(STDOUT_FILENO, small_token_buffer.command_list[0], strlen(small_token_buffer.command_list[0]));
					write(STDOUT_FILENO, "\n", 2);
				}
				else
					displayFile(small_token_buffer.command_list[1]);
			}
			else if (strcmp(small_token_buffer.command_list[0], "exit") == 0){
				exit = 1; //for the case where -f is used, but there is no exit; specefied at the end
				goto cleanup;
			}
			else{
				write(STDOUT_FILENO, syntaxError, strlen(syntaxError));
				write(STDOUT_FILENO, small_token_buffer.command_list[0], strlen(small_token_buffer.command_list[0]));
				write(STDOUT_FILENO, "\n", 2);
			}

			//avoid writing anything that's not output	
			//free smaller tokens and reset variable
			free_command_line(&small_token_buffer);
			memset (&small_token_buffer, 0, 0);
		}
		//free smaller tokens and reset variable
		free_command_line (&large_token_buffer);
		memset (&large_token_buffer, 0, 0);
		if (fileMode == 0)
			write(STDOUT_FILENO, ">>> ", 4); //again, only write this if -f is specefied
	}

	cleanup:
		if (fileMode){
			fclose(source);
			fclose(redirect); 
		}
		if (exit){ //if we exited early we need to free, otherwise if exit was never specefied then this will cause double free
			free_command_line(&small_token_buffer);
			free_command_line (&large_token_buffer);
		}
		free(line_buf);
		return 0; //no errors
}
