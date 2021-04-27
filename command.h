/*
* Description: header file for command.c, includes stub commands meant to emulate unix commands
*
* Author: Harrison Chebuk
*
* Date: April 13th, 2021
*
*/

void listDir(); /*for the ls command*/

void showCurrentDir(); /*for the pwd command*/

void makeDir(char *dirName); /*for the mkdir command*/

void changeDir(char *dirName); /*for the cd command*/

void copyFile(char *sourcePath, char *destinationPath); /*for the cp command*/

void moveFile(char *sourcePath, char *destinationPath); /*for the mv command*/

void deleteFile(char *filename); /*for the rm command*/

void displayFile(char *filename); /*for the cat command*/
