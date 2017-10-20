#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<errno.h>
#include<ctype.h>
#include "patch.h"

int main(int argc, char *argv[]){
	FILE *fp;
	long val;
	char *line, *p;
	int i = 0, pos, j, size;
	int num[4];
	filestore store;
	if(argc != 3){
		perror("Passing arguments should be a file and a patch");
		return errno;
	}
	if((fp = fopen(argv[1], "r")) == NULL ){
		perror("Problem while opening file :");
		return errno;
	}
	fclose(fp);
	store = readlinesfromfiles(argv[1]);
	fp = fopen(argv[2], "r");
	
	if(fp == NULL){
		perror("patch cannot be done");	
		return errno;
	}
	line = (char *)malloc(128*sizeof(char));
	while(fgets(line, 128, fp) != NULL){
		line[strlen(line) - 1] = '\0';
		p = line;
		while(*p && *p != 'a' && *p != 'd'){
			if (isdigit(*p)) {
				val = strtol(p, &p, 10);
				num[i++] = val;
			}else{
            			p++;
        		}
    		}
		i = 2;	
		while(*p){
                        if (isdigit(*p)) {
                                val = strtol(p, &p, 10);
                                num[i++] = val;
                        }else{
                                p++;
                        }
                }
		p = line;
		while(*p != 'a' && *p != 'd')
			p++;
		if(*p == 'a'){
			pos = num[0];
			if(num[3] == 0){
				p = NULL;
				fgets(line, 128, fp);
				line[strlen(line) - 1] = '\0';
				p = line;
				p += 2;
				if(insert(&store, (line + 2), pos) != 1){
					perror("Problem in patch file");
					return errno;
				}
			}
			else{
				size = num[3] - num[2] + 1;
				for(j = 0; j < size; j++){
					p = NULL;
					fgets(line, 128, fp);
					line[strlen(line) - 1] = '\0';
					p = line;
					p += 2;
					if(insert(&store, (line + 2), pos) != 1){
						perror("Problem in patch file");
						return errno;
					}
				}
			}	
		}
		else if(*p == 'd'){
			if(num[1] == 0){
				if(deleteset(&store, num[0]) != 1){
					perror("Problem in patch file");
					return errno;
				}
			}
			else{
				for(j = num[0]; j <= num[1]; j++){
					if(deleteset(&store, j) != 1){
						perror("Problem in patch file");
						return errno;
					}
				}
			}
		}
		else{
			perror("Problem in patch file");
			return 0;
		}
		//some code
		i = 0;
		j = 0;
		size = 0;
	}
	fclose(fp);
	fp = fopen(argv[1], "w");
	if(fp == NULL){
		perror("Cannot open file");
		return errno;
	}
	while((line = readline(&store)) != NULL){
		fwrite(line, 1, strlen(line)*sizeof(char), fp);
		fwrite("\n", 1, sizeof(char), fp);
	}
	return 0;
}
