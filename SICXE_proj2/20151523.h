#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>


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

typedef struct opinst {
	int line;
	int loc;
	char label[30];
	char symb[30];
	char target[30];
	char target2[30];
	int obj;
	int label_flag;
	int format;
	int m_flag;

}opinst;

opinst inst[100];
int line_ctr = 0;
opinst symb_tmp[100];
int symb_cnt;
