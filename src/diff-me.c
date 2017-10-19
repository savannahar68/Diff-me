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
	diff = shortest_edit_graph(files);
	while(!isempty(&diff)){
		opline = retrieve(&diff);
		printf("%s %d %d %d %d\n", opline.line, opline.posx, opline.posy, opline.deleteflag, opline.insertflag);
	}
	return 0;
}

