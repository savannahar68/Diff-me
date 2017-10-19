#ifndef DIFFSTORE_H
#define DIFFSTORE_H

typedef struct node{
	char *line;
	int posx, posy;
	int deleteflag, insertflag;
	struct node *next;
}node;

typedef node* diffstore;

extern void init(diffstore *d);
extern void store(diffstore *d, char *line, int posx, int posy, int deleteflag, int insertflag);
extern node retrieve(diffstore *d);
extern int isempty(diffstore *d);

#endif
