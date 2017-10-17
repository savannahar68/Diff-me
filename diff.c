#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include "diff.h"

//Initialize the structure name file_data and set the linenumber to 1
void initfiledata(file_data *a){
	(*a).origlinenumber = malloc(1000*sizeof(int *));
	//malloc 1000 lines pointer(1000 pointers are made) change the size to read more big file
	(*a).lines = malloc(1000*sizeof(char*));
	a->totallines = 0;
}

//readlines open a file, read from the file line by line and saves all the lines int lines array of the struture file_Data
int readlinesfromfiles(char *filename, file_data *a){
	FILE *fp;
	int x = 0;
	char line[128];
	fp = fopen(filename, "r");	
	if(fp == NULL)
		return 0;
	while(1){
		if(fgets(line, 128, fp) == NULL)
			break;
		//malloc each lines
		line[strlen(line) - 1] = '\0'; //to prevent storing of \n at the end
		a->lines[x] = malloc(strlen(line)*sizeof(char));
		strcpy(a->lines[x], line);
	
		a->origlinenumber[x] = x + 1;
		x++;
	}
	a->totallines = x;
	return 1; 
}
