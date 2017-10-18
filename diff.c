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
//this fuction does the main logic of mayers alogrithm ,
//it makes the edit graph which will give us the shortest path
//from the top to the end
void shortest_edit_graph(file_data *a){
	int n = a[0].totallines;
	int m = a[1].totallines;
	int max = m + n;
	int size = 2 * max + 1;
	int d, k, x ,y, i;// d = depth k = current x = xpos y = ypos
	int **trace; //this matrix will contains all the x value for each depth
				    // 2 * max + 1 - for stroing both negitive and positive index	
	int arr[size]; // this array contains the current x values for that K,further it would be copied into trace(in each loop)
	int *v;
	v = arr + max;
	memset(arr, 0, size*sizeof(int));
	trace = (int **)malloc(max*sizeof *trace);
	for(i = 0; i < max; i++)
		trace[i] = (int *)malloc(size*sizeof *trace[i]);
	for(d = 0; d <= max; d++){
		for(k = -d; k <= d; k = k + 2){
			if(k == -d || (k != d && v[k - 1] < v[k + 1]))
				x = v[k + 1];
			else
				x = v[k - 1] + 1;
			y = x - k;
			while(x < n && y < m && (strcmp(a[0].lines[x], a[1].lines[y]) == 0)){
				x = x + 1;	
				y = y + 1;
			}
			v[k] = x;
			if(x >= n && y >= m ){
				memcpy(trace[d], &arr, sizeof(arr));
			
				backtrack(trace, d, x, y);
				return;
			}
		}	
		memcpy(trace[d], &arr, sizeof(arr));
	}
	
}
//to find the shortest path of all the paths in trace array
//backtrack throught all the d in reverse order and and save the path into struct array
void backtrack(int **trace, int d, int x, int y){
	int i = 0, j;
	int size = 2 * (x + y) + 1;
	for(i = d; i >= 0; i--){
		for(j = 0; j < size; j++){
			printf(" %d ", trace[i][j]);
		}
		printf("\n");
	}
}