#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "patch.h"

void init(filestore *f){
	f->head = NULL;
	f->tail = NULL;
}
//to append node one after another
//here pos variable is to be stored in the node and not the position of node
void append(filestore *f, char *line, int pos){
	node *temp;
	temp = (node *)malloc(sizeof(node));
	temp->line = (char *)malloc(2*strlen(line)*sizeof(char));
	strcpy(temp->line, line);
	temp->pos = pos;
	temp->flag = -1;
	temp->next = NULL;
	if(f->head == NULL){
		f->head = f->tail = temp;
	}
	else{
		f->tail->next = temp;
		f->tail = temp;
	}
}
//to node to be inserted at specific position given by pos
int insert(filestore *f, char *line, int pos){
	node *temp;
	node *ptr, *prev;
	temp = (node *)malloc(sizeof(node));
	temp->line = (char *)malloc(2*strlen(line)*sizeof(char));
	strcpy(temp->line, line);
	temp->pos = -1;
	temp->flag = -1;
	ptr = f->head;
	prev = ptr;
	while(ptr != NULL && ptr->pos != pos){
		prev = ptr;
		ptr = ptr->next;
	}
	if(ptr == NULL && (prev->pos < pos)){//when we want to insert at the end 
		temp->next = NULL;
		f->tail->next = temp;
		f->tail = temp;
	}
	else if(ptr == prev){
		temp->next = ptr;
		f->head = temp;
	}
	else if(ptr->pos == pos){
		temp->next = prev->next;
		prev->next = temp;
	}
	else
		return 0;//invalid patch
	return 1;
}
//set the delete flag of the node at given position
int deleteset(filestore *f, int pos){
	node *ptr;
	ptr = f->head;
	while(ptr != NULL && ptr->pos != pos)
		ptr = ptr->next;
	if(ptr == NULL)
		return 0;
	ptr->flag = 1;
	return 1;
}
//read and return a line, if empty return NULL
char *readline(filestore *f){
	node *ptr = NULL;
	char *line = NULL;
	if(f->head == NULL){
		return line;
	}
	line = (char *)malloc(2*128*sizeof(char));
	while(f->head != NULL && f->head->flag == 1){//of the node has delete flag set
		ptr = f->head;	  //the free that node and move the pointer ahead
		f->head = f->head->next;
		free(ptr->line);
		free(ptr);		
	}
	if(f->head == NULL){
		free(line);
		line = NULL;
		return line;
	}
	strcpy(line, f->head->line);
	ptr = f->head;
	f->head = f->head->next;
	free(ptr->line);
	free(ptr);
	return line;
}
//read line
filestore readlinesfromfiles(char *filename){
	FILE *fp;
	filestore store;
	char line[256];
	int pos = 1;
	fp = fopen(filename, "r");	
	init(&store);
	if(fp == NULL)
		return store;
	while(1){
		if(fgets(line, 256, fp) == NULL)
			break;
		//malloc each lines
		line[strlen(line) - 1] = '\0'; //to prevent storing of \n at the end
		append(&store, line, pos);
		pos += 1;
	}
	return store; 
}
