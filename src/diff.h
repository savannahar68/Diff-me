#include<stdio.h>
#include<stdlib.h>
#include "diffstore.h"
typedef struct file_data{
	char **lines;
	int *origlinenumber;
	int totallines;

}file_data;

typedef struct btrack{
	int x, y, prevx, prevy;
}btrack;

void initfiledata(file_data *a);
int readlinesfromfiles(char *filename, file_data *a);
diffstore shortest_edit_graph(file_data *a);
diffstore backtrack(int **trace, int d, int x, int y, int n, int m, file_data *a);
diffstore diffoutput(btrack bt[], int btcounter, file_data *a);
