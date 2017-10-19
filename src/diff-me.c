#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<errno.h>
#include "diff.h"
#include "diffstore.h"
int main(int argc, char *argv[]){
        char filename[2][50];
        file_data files[2];
	diffstore diff;
	int insert = -1, delete = -1;
	int outputcounter = -1, i, j, range = 0;
	node opline; //contains one output line from diff which is returned by shortest edit
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
	int max = files[0].totallines + files[1].totallines; //maximum number of possible diff output
	diffout output[max];
	diff = shortest_edit_graph(files);
	while(!isempty(&diff)){
		opline = retrieve(&diff);
		//insert and delete vaiable will have previous output used to construct range
		if(opline.deleteflag == 1){
			if(opline.posx == (delete + 1)){
				delete++;
				output[outputcounter].step += 1;
				output[outputcounter].endypos = opline.posy;
				output[outputcounter].line[output[outputcounter].linecounter] = (char*)malloc(strlen(opline.line)*sizeof(char));
				strcpy(output[outputcounter].line[output[outputcounter].linecounter], opline.line);
				output[outputcounter].linecounter += 1;
			}
			else{
				outputcounter += 1;
				delete = opline.posx;
				output[outputcounter].flag = 1;//delete
				output[outputcounter].start = delete;
				output[outputcounter].ypos = opline.posy;
				output[outputcounter].endypos = 0;
				output[outputcounter].step = 0;
				output[outputcounter].line = (char **)malloc((max/2)*sizeof(char*));
				output[outputcounter].linecounter = 0;
				output[outputcounter].line[output[outputcounter].linecounter] = (char*)malloc(strlen(opline.line)*sizeof(char));
				strcpy(output[outputcounter].line[output[outputcounter].linecounter], opline.line);
				output[outputcounter].linecounter += 1;
			}
		}
		else{
			if(opline.posx == insert){
				output[outputcounter].step += 1;
				output[outputcounter].endypos = opline.posy;
                                output[outputcounter].line[output[outputcounter].linecounter] = (char*)malloc(strlen(opline.line)*sizeof(char));
                                strcpy(output[outputcounter].line[output[outputcounter].linecounter], opline.line);
                                output[outputcounter].linecounter += 1;
			}
			else{
				outputcounter += 1;
				insert = opline.posx;
				output[outputcounter].flag = 0;//insert
				output[outputcounter].ypos = opline.posy;
				output[outputcounter].endypos = 0;
				output[outputcounter].start = insert;
				output[outputcounter].step = 0;//not in use
				output[outputcounter].line = (char **)malloc((max/2)*sizeof(char*));
				output[outputcounter].linecounter = 0;
				output[outputcounter].line[output[outputcounter].linecounter] = (char*)malloc(strlen(opline.line)*sizeof(char));
				strcpy(output[outputcounter].line[output[outputcounter].linecounter], opline.line);
				output[outputcounter].linecounter += 1;
			}	
		}
		//printf("%s %d %d %d %d\n", opline.line, opline.posx, opline.posy, opline.deleteflag, opline.insertflag);
	}
	for(i = 0; i <= outputcounter; i++){
		range = 0;
		//printf("flag = %d start = %d step = %d\n", output[i].flag, output[i].start, output[i].step);
		if(output[i].flag == 1){
			range = output[i].start + output[i].step;
			if(output[i].step == 0)
				printf("%dd%d\n", output[i].start, output[i].ypos);
			else
				printf("%d,%dd%d\n", output[i].start, range, output[i].ypos);
			for(j = 0; j < output[i].linecounter; j++){
                        	printf("< %s\n", output[i].line[j]);
                	}	
		}
		else{
			if(output[i].start == 0){
				range = files[0].totallines;
				if(output[i].endypos != 0)
					printf("%da%d,%d\n", range, output[i].ypos, output[i].endypos);
				else	
					printf("%da%d\n", range, output[i].ypos);
					
			}
			else if(output[i].endypos != 0)
				printf("%da%d,%d\n", output[i].start, output[i].ypos, output[i].endypos);	
			else
				printf("%da%d\n", output[i].start, output[i].ypos);	
			for(j = 0; j < output[i].linecounter; j++){
                        	printf("> %s\n", output[i].line[j]);
                }	
		}
	}
	return 0;
}

