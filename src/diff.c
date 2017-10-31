#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<ctype.h>
#include<string.h>
#include<sys/types.h>
#include <dirent.h>
#include<time.h>
#include<errno.h>
#include<sys/stat.h>
#include "diff.h"
#include "diffstore.h"

//variables for flag
extern int flagy;
extern int flagc;
extern int flagw;
extern int flagi;
extern int flagb;
extern int flagt;

//Initialize the structure name file_data and set the linenumber to 1
void initfiledata(file_data *a){
	(*a).origlinenumber = malloc(1000*sizeof(int *));
	//malloc 1000 lines pointer(1000 pointers are made) change the size to read more big file
	(*a).lines = malloc(1000*sizeof(char*));
	a->totallines = 0;
}
void freeinitfiledata(file_data *a){
	free(a->lines);
	free(a->origlinenumber);
}
//readlines open a file, read from the file line by line and saves all the lines int lines array of the struture file_Data
int readlinesfromfiles(char *filename, file_data *a){
	FILE *fp;
	int x = 0;
	char line[256];
	fp = fopen(filename, "r");	
	if(fp == NULL)
		return 0;
	a->lines[x] = "test";
	a->origlinenumber[x] = x;
	x++;
	while(1){
		if(fgets(line, 256, fp) == NULL)
			break;
		//malloc each lines
		line[strlen(line) - 1] = '\0'; //to prevent storing of \n at the end
		a->lines[x] = malloc((1+strlen(line))*sizeof(char));
		strcpy(a->lines[x], line);
	
		a->origlinenumber[x] = x;
		x++;
	}
	a->totallines = x;
	fclose(fp);
	return 1; 
}
//this fuction does the main logic of mayers alogrithm ,
//it makes the edit graph which will give us the shortest path
//from the top to the end
diffstore shortest_edit_graph(file_data *a){
	diffstore diff;
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
	trace = (int **)malloc((1 + (max))*sizeof *trace);
	for(i = 0; i < max + 1; i++)
		trace[i] = (int *)malloc((2*size)*sizeof *trace[i]);
	for(d = 0; d <= max; d++){
		for(k = -d; k <= d; k = k + 2){
			if(k == -d || (k != d && v[k - 1] < v[k + 1]))
				x = v[k + 1];
			else
				x = v[k - 1] + 1;
			y = x - k;
			while(x < n && y < m && (stringcmp(a[0].lines[x], a[1].lines[y]) == 0)){
				x = x + 1;	
				y = y + 1;
			}
			v[k] = x;
			if(x >= n && y >= m ){
				memcpy(trace[d], &arr, sizeof(arr));
			
				diff = backtrack(trace, d, x, y, n, m, a);
				for(i = 0; i < max + 1; i++)
					free(trace[i]);
				free(trace);	
				return diff;
			}
		}	
		memcpy(trace[d], &arr, sizeof(arr));
	}
	init(&diff);
	for(i = 0; i < max + 1; i++)
		free(trace[i]);		
	free(trace);	
	return diff;
	
}
//remove the extra white space and trailing space
void compress_spaces(char *str)
{
	char *dst = str;
	for(; *str; ++str){
        	*dst++ = *str;
		if (isspace(*str)) {
            		do ++str;
            		while (isspace(*str));
            		--str;
        	}
    	}

	*dst = 0;
    	while(isspace(*(dst - 1))){
        	*(dst - 1) = 0;
        	dst--;
        }
}

int stringcmp(char *linea, char *lineb){
	char *linex, *liney, *i, *j;
	int res;
	linex = (char *)malloc((2*strlen(linea))*sizeof(char));
	liney = (char *)malloc((2*strlen(lineb))*sizeof(char));
	strcpy(linex, linea);
	strcpy(liney, lineb);
	if(flagb == 1 && flagw == 0){ //b flag - for removing extra and trailing space from the line
		compress_spaces(linex);
		compress_spaces(liney);
	}
	if(flagw == 1){ //w flag set - all the whitespaces are removed
		i = linex;
		j = linex;
		while(*j != 0)
  		{
    			*i = *j++;
    			if(*i != ' ')
      				i++;
  		}
  		*i = 0;
		i = liney;
                j = liney;
                while(*j != 0)
                {
                        *i = *j++;
                        if(*i != ' ')
                                i++;
                }
                *i = 0;
	}

	if(flagi == 1){
		
		res = strcasecmp(linex, liney);
	}
	else{
		res = strcmp(linex, liney);
	}	
	free(linex);
	free(liney);
	return res;//return if  matched or not (0 or not 0)
}
//to find the shortest path of all the paths in trace array
//backtrack throught all the d in reverse order and and save the path into struct array
diffstore backtrack(int **trace, int d, int x, int y, int n, int m, file_data *a){
	int i = 0, btcounter = 0, *v, j;
	int max = n + m;
	int size = max * 2 + 1;
	int arr[size];
	int k, prev_k, prev_x, prev_y;
	btrack bt[max];//for storing result
	diffstore diff;
	for(i = d; i > 0; i--){
		for(j = 0; j < size; j++){
			arr[j] = trace[i][j];
		}
		v = arr + max;//going to middle of array	
		k = x - y;	
		if(k == -d || (k != d && v[k - 1] < v[k + 1]))
			prev_k = k + 1;
		else
			prev_k = k - 1;
		prev_x = v[prev_k];
		prev_y = prev_x - prev_k;
		while(x > prev_x && y > prev_y){
			bt[btcounter].prevx = x - 1;
			bt[btcounter].prevy = y - 1;
			bt[btcounter].x = x;
			bt[btcounter].y = y;
			x = x - 1;
			y = y - 1;
			btcounter++;
		}
		if(d > 0){
			bt[btcounter].prevx = prev_x;
                        bt[btcounter].prevy = prev_y;
                        bt[btcounter].x = x;
                        bt[btcounter].y = y;
			btcounter++;
		}
		x = prev_x;
		y = prev_y;
	}
	//bt structure array has shortest path in reverse order, further we do processing on this
	diff = diffoutput(bt, btcounter, a); //get diffoutput in diff then return diff
	return diff;
}
//run the backtrack output and mark the chanegs of updation deletion into data structure named diffstore
//return diffstore to backtrack
//check the prev_x and prev_y with x and y if they are different then no changes files are euqal
//if prev_X and x are same then there's a deletion  (downward move taken in graph)
//if prev_y and  y are same then there's a insertion (rightward move taken in graph)
diffstore diffoutput(btrack bt[], int btcounter, file_data *a){
	diffstore b;
	init(&b);
	int i;
	char *lineA, *lineB;
	lineA = (char *)malloc(500*sizeof(char));
	lineB = (char *)malloc(500*sizeof(char));
	for(i = 0; i < btcounter; i++){

		if(bt[i].x == bt[i].prevx){ //insert line from 2nd file
			strcpy(lineB, a[1].lines[bt[i].prevy]);
			store(&b, lineB, a[0].origlinenumber[bt[i].prevx], a[1].origlinenumber[bt[i].prevy], 0, 1, 0);						
		}
		else if(bt[i].y == bt[i].prevy){ //delete line from 1st file
			strcpy(lineA, a[0].lines[bt[i].prevx]);
			store(&b, lineA, a[0].origlinenumber[bt[i].prevx], a[1].origlinenumber[bt[i].prevy], 1, 0, 0);						
		}
		else{
			//printf("similar : %s\n", a[0].lines[bt[i].prevx]);
			strcpy(lineA, a[0].lines[bt[i].prevx]);
			store(&b, lineA, a[0].origlinenumber[bt[i].prevx], a[1].origlinenumber[bt[i].prevy], 0, 0, 1);			
			//both lines are equal if prevx != x and prevy != y
		}
		
	}
	free(lineA);
	free(lineB);
	return b;
}
//gives the list of all directory and files
directory list_dir(char * dir_name){
	DIR * d;
	directory dirstruct;
    	/* Open the directory specified by "dir_name". */
    	d = opendir (dir_name);
	// Check it was opened.
    	if (! d) {
        	fprintf (stderr, "Cannot open directory '%s': %s\n", dir_name, strerror (errno));
        	exit (EXIT_FAILURE);
    	}
	dirstruct.filename = (char **)malloc(1000 * sizeof(char*));
	dirstruct.dirname = (char **)malloc(1000 * sizeof(char*));
	dirstruct.totalfiles = 0;
	dirstruct.flag = (int *)malloc(1000*sizeof(int));
    	while (1) {
        	struct dirent * entry;
        	const char * d_name;
		entry = readdir (d);
        	if (! entry) {
            		break;
        	}
        	d_name = entry->d_name;

        	if (! (entry->d_type & DT_DIR)) {
			dirstruct.filename[dirstruct.totalfiles] = malloc((strlen(d_name) + 1)*sizeof(char));
			strcpy(dirstruct.filename[dirstruct.totalfiles], d_name);
			dirstruct.dirname[dirstruct.totalfiles] = malloc((strlen(dir_name) + 1)*sizeof(char));
			strcpy(dirstruct.dirname[dirstruct.totalfiles], dir_name);
			strcat(dirstruct.dirname[dirstruct.totalfiles], "/");
			dirstruct.flag[dirstruct.totalfiles] = 0;//flag initially set to 0
			dirstruct.totalfiles += 1;
		}
        	if (entry->d_type & DT_DIR) {
            
           	if (strcmp (d_name, "..") != 0 &&
                	strcmp (d_name, ".") != 0) {
                	int path_length;
                	char path[PATH_MAX];
               		path_length = snprintf (path, PATH_MAX,"%s/%s", dir_name, d_name);
              //  printf ("%s\n", path);
             	   	if (path_length >= PATH_MAX) {
                   		fprintf (stderr, "Path length has got too long.\n");
                    		exit (EXIT_FAILURE);
                	}
               	 /* Recursively call "list_dir" with the new path. */
                		list_dir (path);
            		}
		}
	}
    	if (closedir (d)) {
        	fprintf (stderr, "Could not close '%s': %s\n",
                	 dir_name, strerror (errno));
        	exit (EXIT_FAILURE);
    	}
	return dirstruct;
}
int is_binary(const void *data, size_t len){
    return memchr(data, '\0', len) != NULL;
}
