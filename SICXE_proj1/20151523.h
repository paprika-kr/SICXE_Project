#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#define MEM_height 65536
#define MEM_width 16


int hash_func(unsigned char *arg1);



typedef struct node_h {
	struct node_h *next;
	int idx;
	char *data;
}node;

typedef struct hash {
	struct hash *next;
	char code[80];
	unsigned char name[80];
	char idk[80];
}hash;


