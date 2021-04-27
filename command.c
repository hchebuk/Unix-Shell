/*
 * command.c
 *
 *  Created on: April 13, 2021
 *      Author: Harrison Chebuk
 *
 */
#define _GNU_SOURCE
#include <stdlib.h>
#include <string.h>
#include "command.h"
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void listDir(){ 
	/*for the ls command*/
	char *buf = (char *) malloc(sizeof(char) * 256); //255 is the maximum length file name + 1 for null terminating
	getcwd(buf, 256);
	DIR *cwd = opendir(buf);
	free(buf);
	char *name;
	int len;
	struct dirent *curFile = readdir(cwd); //iterate the first time to to the first file in the directory
	while(curFile != NULL){
		name = curFile->d_name; 
		len = strlen(name);
		//listing files
		write(STDOUT_FILENO, name, len);
		write(STDOUT_FILENO, "\n", 1);
		curFile = readdir(cwd);
	}
	closedir(cwd);
}

void showCurrentDir(){ 
	/*for the pwd command*/
	char *pwd = (char *) malloc(sizeof(char) * 256); //255 is the maximum length file name + 1 for null terminating
	getcwd(pwd, 256);
	write(STDOUT_FILENO, pwd, strlen(pwd));
	write(STDOUT_FILENO, "\n", 1);
	free(pwd);
}

void makeDir(char *dirName){ 
	/*for the mkdir command*/
	char *error = "There was an error making the directory\n";
	if(mkdir(dirName, 0777))
		write(STDOUT_FILENO, error, strlen(error));
}

void changeDir(char *dirName){ 
	/*for the cd command*/
	char *error = "There was an error changing directories\n";
	if(chdir(dirName))
		write(STDOUT_FILENO, error, strlen(error));
}

void copyFile(char *sourcePath, char *destinationPath){ 
	/*for the cp command*/
	char *error1 = "The source file failed to open\n";
	char *error2 = "Failed to open or create the destination file\n";
	
	//set up our directory id for openat
	char *pwd = (char *) malloc(sizeof(char) * 256); //255 is the maximum length file name + 1 for null terminating
	getcwd(pwd, 256);
	//write(STDOUT_FILENO, sourcePath, strlen(sourcePath));
	char *buf;
	DIR *cwd = opendir(pwd);

	struct stat size;
	int file = openat(dirfd(cwd), sourcePath, O_RDONLY);	
	if(file == -1){ //file failed to open
		write(STDOUT_FILENO, error1, strlen(error1));
	}
	else{	
		fstat(file, &size);
		buf = (char *) malloc(size.st_size);
		read(file, buf, size.st_size);
		close(file);
	}

	if(file != -1){
		//write to the destinationPath
		file = openat(dirfd(cwd), destinationPath, O_CREAT | O_WRONLY, 00777); //open with full permissions, creates if file doesn't exist or a directory with the name exists
		if(file == -1){
			if(chdir(destinationPath)){ //couldn't open as a file or directory
				write(STDOUT_FILENO, error2, strlen(error2));
			}
			else{
				file = openat(dirfd(cwd), sourcePath, O_CREAT | O_WRONLY, 00777); //create an exact copy of source path in new directory with full permissions
				write(file, buf, size.st_size);
				close(file);
			}
			chdir(pwd);
		}
		else{
			write(file, buf, size.st_size);
			close(file);
		}
		free(pwd);
		free(buf);
	}

	closedir(cwd);
}

void moveFile(char *sourcePath, char *destinationPath){ 
	/*for the mv command*/
	char *error = "Unable to get rid of old file\n";
	copyFile(sourcePath, destinationPath); //copy the old file to the new location 
	if(unlink(sourcePath)){ //delete the old file
		write(STDOUT_FILENO, error, strlen(error));
	}
}

void deleteFile(char *filename){ 
	/*for the rm command*/
	char *error = "Unable to remove file\n";
	if(unlink(filename)){
		write(STDOUT_FILENO, error, strlen(error));
	}
}

void displayFile(char *filename){ 
	/*for the cat command*/
	char *error = "The file you are trying to cat does not exist\n";
	struct stat size;
	int file = open(filename, O_RDONLY);
	if(file == -1){ //file failed to open
		write(STDOUT_FILENO, error, strlen(error));
	}
	else{
		fstat(file, &size);
		char *buf = (char *) malloc(size.st_size);
		read(file, buf, size.st_size);
		close(file);
		write(STDOUT_FILENO, buf, size.st_size);
		free(buf);
	}
}
