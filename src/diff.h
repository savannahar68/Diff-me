#include<stdio.h>
#include<stdlib.h>
#include "diffstore.h"
//structure to save file
typedef struct file_data{
	char **lines;
	int *origlinenumber;
	int totallines;

}file_data;

//to save edit graph coordiinates
typedef struct btrack{
	int x, y, prevx, prevy;
}btrack;

//structure to save diff output
typedef struct diffout{
	int flag; //for delete it is set to 1
		 // for insert set the flag to 0;
	int start; //this coordinate will have the starting line
	int step; //have number of lines ahead of it
	int ypos; //file number of 2nd file
	int endypos; 
	char **line;
	int linecounter; //init it to 0 in else with each node
}diffout;

//this structure is for -recursively operating on folders
typedef struct directory{
	char **filename;
	char **dirname;
	int totalfiles;
	int *flag;
}directory;

//prototype
void initfiledata(file_data *a);
int readlinesfromfiles(char *filename, file_data *a);
diffstore shortest_edit_graph(file_data *a);
diffstore backtrack(int **trace, int d, int x, int y, int n, int m, file_data *a);
diffstore diffoutput(btrack bt[], int btcounter, file_data *a);
void compress_spaces(char *line);
int stringcmp(char *linex, char *liney);
directory list_dir(char *dir_name);
