typedef struct node{
	char *line;
	int pos;//storing position of line
	int flag;//check deletion
	struct node *next;
}node;

typedef struct filestore{
	node *head;	
	node *tail;
}filestore;

void init(filestore *f);
void append(filestore *f, char *line, int pos);
int insert(filestore *f, char *line, int pos);
int deleteset(filestore *f, int pos);
char *readline(filestore *f);
filestore readlinesfromfiles(char *filename);

