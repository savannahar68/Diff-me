#include<stdio.h>
#include<stdlib.h>
#include "diffstore.h"
diffstore func(diffstore a);
int main(){
	diffstore a, b;
	node n;
	init(&a);
	store(&a, "savan is my line", 2, 3, 0, 1);
	store(&a, "thisis line 2", 4, 31, 1, 0);
	store(&a, "test this lines", 2, 3, 1, 0);
	store(&a, "what about this", 5, 2, 0, 1);
	store(&a, "hi", 6, 3, 1, 0);
	b = func(b);
	while(!isempty(&a)){
		n = retrieve(&a);
		printf("%s %d %d %d %d\n", n.line, n.posx, n.posy, n.deleteflag, n.insertflag);
	}
	return 0;
}
diffstore func(diffstore a){
	diffstore c;
	return c;
}
