project: patch.o patch-me.o
	gcc -Wall patch.o patch-me.o -o project
patch.o: patch.c patch.h
	cc -c -Wall patch.c
patch-me.o: patch-me.c patch.h
	cc -c -Wall patch-me.c
