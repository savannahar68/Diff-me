project: diff-me.o diff.o diffstore.o
	gcc -Wall diff-me.o diff.o diffstore.o -o project
diff-me.o: diff-me.c diff.h diffstore.h
	cc -Wall -c diff-me.c
diff.o: diff.c diff.h diffstore.h
	cc -Wall -c diff.c
diffstore.o: diffstore.c diffstore.h
	cc -Wall -c diffstore.c
