#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<errno.h>
#include "diff.h"
int main(int argc, char *argv[]){
	int i;
        char filename[2][50];
        file_data files[2];
	//init the structure to start the line count of each file from 0
        initfiledata(&files[0]);
        initfiledata(&files[1]);
        strcpy(filename[0], argv[argc - 2]);
        strcpy(filename[1], argv[argc - 1]);
        //if readlinesfromfiles returns 0 then there was problem reading the file so exit giving that error
        if(readlinesfromfiles(filename[0], &files[0]) == 0 || readlinesfromfiles(filename[1], &files[1]) == 0){
		perror("File cannot be opened");
		return errno;
	}
	printf("%d lines number : \n", files[0].totallines);
	for(i = 0; i < files[0].totallines; i++){
		printf("%d. %s\n", files[0].origlinenumber[i], files[0].lines[i]);
	}
	for(i = 0; i < files[1].totallines; i++){
		printf("%d. %s\n", files[1].origlinenumber[i], files[1].lines[i]);
	}
}

