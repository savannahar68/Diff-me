#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<errno.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<time.h>
#include "diff.h"
#include "diffstore.h"

int flagy, flagc, flagw, flagi, flagb, flagt, flagr; //to check which flags are set
int main(int argc, char *argv[]){
	if(argc < 3){
		perror("Invalid arguments");
		return errno;
	}
        char filename[2][250];
        file_data files[2];
	diffstore diff;
	char **alldifflines, *string1, *string2;
	long fsize;
	FILE *f;
	int insert = -1, delete = -1;
	int outputcounter = -1, i, j, m, n, range = 0, option = 0;
	struct stat attr; //for getting timestamp
	node opline; //contains one output line from diff which is returned by shortest edit
	diffout output[1000];
	int max;
        strcpy(filename[0], argv[argc - 2]);
        strcpy(filename[1], argv[argc - 1]);
	//set all flags to 0
	flagy = 0;
        flagc = 0;
        flagw = 0;
        flagi = 0;
        flagb = 0;
        flagt = 0;
	flagr = 0;
	while ((option = getopt(argc, argv,"yctwbir")) != -1) {
        	switch (option) {
             		case 'y' : flagy = 1;
                		break;
            	 	case 'c' : flagc = 1;
            		    	break;
             		case 't' : flagt = 1;
                		break;
	     		case 'w' : flagw = 1;
                		break;
             		case 'b' : flagb = 1; 
                		break;
             		case 'i' : flagi = 1;
				break;
			case 'r' : flagr = 1;
				break;
             		default:
                		break;
        	}
   	}

	if(flagr == 0){
		//init the structure to start the line count of each file from 0
	        initfiledata(&files[0]);
	        initfiledata(&files[1]);
		//if readlinesfromfiles returns 0 then there was problem reading the file so exit giving that error
       		if(readlinesfromfiles(filename[0], &files[0]) == 0 || readlinesfromfiles(filename[1], &files[1]) == 0){
			perror("File cannot be opened");
			return errno;
		}
		max = files[0].totallines + files[1].totallines; //maximum number of possible diff output
	//	diffout output[max];
		diff = shortest_edit_graph(files);
	}
	//following will make all the sequential operations (a , d) go in same structure for easy printing
	//used while printing default diff
	if(flagy == 0 && flagc == 0 && flagr == 0){
		while(!isempty(&diff)){
			opline = retrieve(&diff);
			//insert and delete vaiable will have previous output used to construct range
			if(opline.deleteflag == 1){
				if(opline.posx == (delete + 1)){
					delete++;
					output[outputcounter].step += 1;
					output[outputcounter].endypos = opline.posy;
					output[outputcounter].line[output[outputcounter].linecounter] = (char*)malloc(2*strlen(opline.line)*sizeof(char));
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
					output[outputcounter].line = (char **)malloc(2*(max)*sizeof(char*));
					output[outputcounter].linecounter = 0;
					output[outputcounter].line[output[outputcounter].linecounter] = (char*)malloc(2*strlen(opline.line)*sizeof(char));
					strcpy(output[outputcounter].line[output[outputcounter].linecounter], opline.line);
					output[outputcounter].linecounter += 1;
				}
			}
			else if(opline.insertflag == 1){
				if(opline.posx == insert){
					output[outputcounter].step += 1;
					output[outputcounter].endypos = opline.posy;
		                        output[outputcounter].line[output[outputcounter].linecounter] = (char*)malloc(2*strlen(opline.line)*sizeof(char));
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
					output[outputcounter].line = (char **)malloc(2*(max)*sizeof(char*));
					output[outputcounter].linecounter = 0;
					output[outputcounter].line[output[outputcounter].linecounter] = (char*)malloc(2*strlen(opline.line)*sizeof(char));
					strcpy(output[outputcounter].line[output[outputcounter].linecounter], opline.line);
					output[outputcounter].linecounter += 1;
				}	
			}
			else{
				//both are equal
			}
			free(opline.line);
		}
	}
	if(flagy == 1 && flagc == 0 && flagr == 0){// y flag output
		while(!isempty(&diff)){
			opline = retrieve(&diff);
			if(opline.insertflag == 1){
				if(strlen(opline.line) > 60){ //truncate the line if not fitting in the column
					opline.line[60] = '\0';
				}
				printf("%-60s > %s\n", "", opline.line);
			}
			else if(opline.deleteflag == 1){	
				if(strlen(opline.line) > 60){
					printf("%-.60s < %s\n", opline.line, "");
				}
				else{
					printf("%-60s < %s\n", opline.line, "");
				}
			}
			else{//equal
				if(strlen(opline.line) > 60)
					opline.line[60] = '\0';
				printf("%-60s   %s\n", opline.line, opline.line);
			}
		}
		freeinitfiledata(&files[0]);
		freeinitfiledata(&files[1]);
	}
	//flag c context
	else if(flagc == 1 && flagy == 0 && flagr == 0){
		stat(filename[0], &attr);
		printf("*** %s\t%s", filename[0], ctime(&attr.st_mtime));	
		stat(filename[1], &attr);
		printf("--- %s\t%s", filename[1], ctime(&attr.st_mtime));
		printf("*****************\n");
		printf("\n*** %d,%d ****\n", 1, files[0].totallines);
		alldifflines = (char **)malloc(max*sizeof(char*));
		j = 0;
		flagc = 0;
		while(!isempty(&diff)){
			opline = retrieve(&diff);
			if(opline.equalflag != 1){
				alldifflines[j] = (char *)malloc(2*strlen(opline.line)*sizeof(char));
				strcpy(alldifflines[j], opline.line);
				j++;
			}
		}
		range = j;
		for(i = 1; i < files[0].totallines; i++){
			for(j = 0; j < range; j++){
				if(strcmp(files[0].lines[i], alldifflines[j]) == 0){
					flagc = 1;
					break;
				}
			}
			if(flagc == 1){
				printf("- %s\n", files[0].lines[i]);
			}
			else{
				printf("  %s\n", files[0].lines[i]);
			}
			flagc = 0;
		}
		printf("\n--- %d,%d ---\n", 1, files[1].totallines);
		for(i = 1; i < files[1].totallines; i++){
                        for(j = 0; j < range; j++){
                                if(strcmp(files[1].lines[i], alldifflines[j]) == 0){
                                        flagc = 1;
                                        break;
                                }
                        }
                        if(flagc == 1){
                                printf("+ %s\n", files[1].lines[i]);
                        }
                        else{
                                printf("  %s\n", files[1].lines[i]);
                        }
                        flagc = 0;
                }
		for(j = 0; j < range; j++)
			free(alldifflines[j]);
		free(alldifflines);
		freeinitfiledata(&files[0]);
		freeinitfiledata(&files[1]);
	}
	//default diff output
	else if(flagy == 0 && flagc == 0 && flagr == 0){
		for(i = 0; i <= outputcounter; i++){
			range = 0;
			if(output[i].flag == 1){
				range = output[i].start + output[i].step;
				if(output[i].step == 0)
					printf("%dd%d\n", output[i].start, output[i].ypos);
				else
					printf("%d,%dd%d\n", output[i].start, range, output[i].ypos);
				for(j = 0; j < output[i].linecounter; j++){
		                	printf("< %s\n", output[i].line[j]);
					free(output[i].line[j]);
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
					free(output[i].line[j]);
		        	}	
			}
			free(output[i].line);
		}
		freeinitfiledata(&files[0]);
		freeinitfiledata(&files[1]);
	}
	//code for r flag
	else if(flagr == 1 && flagy == 0 && flagc == 0){

		directory dir1;
		directory dir2;	
		dir1 = list_dir(filename[0]);
		dir2 = list_dir(filename[1]);	
		for(m = 0; m < dir1.totalfiles; m++){
			for(n = 0; n < dir2.totalfiles; n++){
				if((strcmp(dir1.filename[m], dir2.filename[n]) == 0) && dir2.flag[n] == 0){
					memset(filename[0], '\0', 250*sizeof(char));
					memset(filename[1], '\0', 250*sizeof(char));
					strcat(filename[0], dir1.dirname[m]);
					strcat(filename[0], dir1.filename[m]);
					strcat(filename[1], dir2.dirname[n]);
					strcat(filename[1], dir2.filename[n]);
					f = fopen(filename[0], "rb");
					if(f == NULL){
						perror("Cannot open the file");
						return errno;
					}
					fseek(f, 0, SEEK_END);
					fsize = ftell(f);
					fseek(f, 0, SEEK_SET);  //same as rewind(f);
					string1 = malloc(fsize + 1);
					fread(string1, fsize, 1, f);
					fclose(f);
					string1[fsize] = 0;
					//for binary files check
					if(is_binary(string1, fsize) == 1){
						f = fopen(filename[1], "rb");
                                       		if(f == NULL){
                                                	perror("Cannot open the file");
                                               		return errno;
                                        	}
						fseek(f, 0, SEEK_END);
	                                        fsize = ftell(f);
        	                                fseek(f, 0, SEEK_SET);  //same as rewind(f);
                	                        string2 = malloc(fsize + 1);
                        	                fread(string2, fsize, 1, f);
                                	        fclose(f);
                                        	string2[fsize] = 0;
						if(strcmp(string1, string2) == 0)
							printf("Binary Files : %s and %s are similar\n", filename[0], filename[1]);
						else
							printf("Binary Files : %s and %s differ\n", filename[0], filename[1]);
						free(string1);
						free(string2);
						dir2.flag[n] = 1;
						dir1.flag[m] = 1;
						break;
					}
					//files are not binary
					else{
						initfiledata(&files[0]);
                				initfiledata(&files[1]);
               					if(readlinesfromfiles(filename[0], &files[0]) == 0 || readlinesfromfiles(filename[1], &files[1]) == 0){
                      			  		perror("File cannot be opened");
                        				return errno;
                				}
                				max = files[0].totallines + files[1].totallines; //maximum number of possible diff output
                				diff = shortest_edit_graph(files);
						insert = delete = outputcounter = -1;
						range = 0;
						while(!isempty(&diff)){
							opline = retrieve(&diff);
							//insert and delete vaiable will have previous output used to construct range
							if(opline.deleteflag == 1){
								if(opline.posx == (delete + 1)){
									delete++;
									output[outputcounter].step += 1;
									output[outputcounter].endypos = opline.posy;
									output[outputcounter].line[output[outputcounter].linecounter] = (char*)malloc(2*strlen(opline.line)*sizeof(char));
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
									output[outputcounter].line = (char **)malloc(2*(max)*sizeof(char*));
									output[outputcounter].linecounter = 0;
									output[outputcounter].line[output[outputcounter].linecounter] = (char*)malloc(2*strlen(opline.line)*sizeof(char));
									strcpy(output[outputcounter].line[output[outputcounter].linecounter], opline.line);
									output[outputcounter].linecounter += 1;
								}
							}
							else if(opline.insertflag == 1){
								if(opline.posx == insert){
									output[outputcounter].step += 1;
									output[outputcounter].endypos = opline.posy;
									output[outputcounter].line[output[outputcounter].linecounter] = (char*)malloc(2*strlen(opline.line)*sizeof(char));
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
									output[outputcounter].line = (char **)malloc(2*(max)*sizeof(char*));
									output[outputcounter].linecounter = 0;
									output[outputcounter].line[output[outputcounter].linecounter] = (char*)malloc(2*strlen(opline.line)*sizeof(char));
									strcpy(output[outputcounter].line[output[outputcounter].linecounter], opline.line);
									output[outputcounter].linecounter += 1;
								}	
							}
							else{
								//both are equal
							}
							
							free(opline.line);
						}
						printf("Diff: %s : %s\n", filename[0], filename[1]);
						for(i = 0; i <= outputcounter; i++){
							range = 0;
							if(output[i].flag == 1){
								range = output[i].start + output[i].step;
								if(output[i].step == 0)
									printf("%dd%d\n", output[i].start, output[i].ypos);
								else
									printf("%d,%dd%d\n", output[i].start, range, output[i].ypos);
								for(j = 0; j < output[i].linecounter; j++){
									printf("< %s\n", output[i].line[j]);
									free(output[i].line[j]);
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
									free(output[i].line[j]);
								}	
							}
							free(output[i].line);
						}
						freeinitfiledata(&files[0]);
						freeinitfiledata(&files[1]);
						dir2.flag[n] = 1;
                                                dir1.flag[m] = 1;
						free(string1);
						break;
					}
				}
			}
		}
		for(i = 0; i < dir1.totalfiles; i++){
			if(dir1.flag[i] == 0){
				printf("Only in %s file - %s\n", dir1.dirname[i], dir1.filename[i]);
			}
		}
                for(j = 0; j < dir2.totalfiles; j++){
			if(dir2.flag[j] == 0){
				printf("Only in %s file - %s\n", dir2.dirname[j], dir2.filename[j]);
			}
		}		
	}
	else{
		perror("Diff : Conflicting flags");
		return 0;
	}
	return 0;
}

