#include<stdio.h>
#include<stdlib.h>

typedef struct file_data{
	char **lines;
	int *origlinenumber;
	int totallines;

}file_data;

void initfiledata(file_data *a);
int readlinesfromfiles(char *filename, file_data *a);
void shortest_edit_graph(file_data *a);