#include<stdlib.h>
#include<string.h>
#include "diffstore.h"
extern void init(diffstore *d){
	*d = NULL;
}
extern void store(diffstore *d, char *line, int posx, int posy, int deleteflag, int insertflag, int equalflag){
	node *temp = malloc(sizeof(node));
	temp->line = malloc(strlen(line) * sizeof(char));
	strcpy(temp->line, line);
	temp->posx = posx;
	temp->posy = posy;
	temp->deleteflag = deleteflag;
	temp->insertflag = insertflag;
	temp->equalflag = equalflag;
	if(*d == NULL){
		*d = temp;
		temp->next = NULL;
	}
	else{
		temp->next = *d;
		*d = temp;
	}
}

extern node retrieve(diffstore *d){
	node temp;
	node *ptr = *d;
	temp.line = malloc(strlen((*d)->line) * sizeof(char));
	strcpy(temp.line, (*d)->line);
	temp.posx = (*d)->posx;
	temp.posy = (*d)->posy;
	temp.deleteflag = (*d)->deleteflag;
	temp.insertflag = (*d)->insertflag;
	temp.equalflag = (*d)->equalflag;
	*d = (*d)->next;
	free(ptr);
	return temp;	
}

extern int isempty(diffstore *d){
	return *d == NULL;
}
