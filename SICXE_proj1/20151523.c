#include "20151523.h"

void help_(){														// help
	printf("\nh[elp]\n");
	printf("d[ir]\n");
	printf("q]uit\n");
	printf("hi[story]\n");
	printf("du[mp] [start, end]\n");
	printf("e[dit] address, value\n");
	printf("f[ill] start, end, value\n");
	printf("reset\n");
	printf("opcode mnemonic\n");
	printf("opcodelist\n\n");
}


void dir_(){														// dir
	
	DIR* dp = opendir(".");
	struct dirent* dd = NULL;
	struct stat fs;
	int cnt = 0;

	while((dd = readdir(dp)) != NULL){

		cnt++;
		stat(dd->d_name, &fs);

		if(S_ISDIR(fs.st_mode))
			printf("%s/		", dd->d_name);
		else
			printf("%s*		", dd->d_name);
		
		if(cnt%4 == 0)
			printf("\n");
	}
	
	printf("\n");
	closedir(dp);
	
}


void history_(node *curr){													// history print

	while (curr != NULL) {
		printf("	%d		%s\n", curr->idx, curr->data);
		curr = curr->next;
	}
}

node* add_node(node *target, char *cmd)										// add node on history linked-list
{
	node *new = malloc(sizeof(node)); 
	new->data = (char*)malloc(sizeof(char) * 100);
	new->next = target->next;  
    strcpy(new->data, cmd);
	new->idx = target->idx + 1;
	target->next = new;

	return new;
}


void dump_(int *addr, unsigned char *mem, char *disp, char *arg1, char *arg2, int *cp, int arg1_i, int arg2_i) { // dump

	int cnt = 0, cnt2 = 0;
	int i, j;

	if (arg1 == NULL) {											// dump ( no start, no end )
		if (*cp % MEM_width == 0) {									// no blank case ( start address % 16 = 0 )

			for (i = 0; i < 10; i++) {

				if (i + (*cp / MEM_width) >= MEM_height) {
					printf("you exceeded available space.\n");
					*cp = 0;
					return;
				}

				printf("%.5X ", addr[i + (*cp / MEM_width)]);

				for (j = 0; j < MEM_width; j++)
					printf("%.2X ", mem[MEM_width * i + j + *cp]);

				for (j = 0; j < MEM_width; j++)
					printf("%c", disp[MEM_width * i + j + *cp]);

				printf("\n");
			}
			*cp = *cp + 160;
		}
		

		else {												// blank case ( start address % 16 != 0 )
			printf("%.5X ", addr[(*cp / MEM_width)]);						// first line ( because of printing blank )
			for (i = 0; i < (*cp%MEM_width); i++)
				printf("   ");
			for (i = (*cp % MEM_width); i < MEM_width; i++)
				printf("%.2X ", mem[i + *cp - (*cp % MEM_width)]);
			for (i = 0; i < (*cp % MEM_width); i++)
				printf(".");
			for (i = (*cp % MEM_width); i < MEM_width; i++)
				printf("%c", disp[i + *cp - (*cp % MEM_width)]);
			printf("\n");

			if ((*cp / MEM_width) >= MEM_height - 1) {
				printf("you exceeded available space.\n");
				*cp = 0;
				return;
			}

			for (i = 1; i < 10; i++) {								// middle line 
				printf("%.5X ", addr[(*cp / MEM_width) + i]);
				for (j = 0; j < MEM_width; j++)
					printf("%.2X ", mem[MEM_width * i + *cp - (*cp % MEM_width) + j]);
				for (j = 0; j < MEM_width; j++)
					printf("%c", disp[MEM_width * i + *cp - (*cp % MEM_width) + j]);
				printf("\n");
			}

			printf("%.5X ", addr[(*cp / MEM_width) + 10]);						// last line ( because of printing blank )
			for (i = 0; i < (*cp % MEM_width); i++)
				printf("%.2X ", mem[i + *cp - (*cp % MEM_width) + 160]);
			for (i = (*cp % MEM_width); i < MEM_width; i++)
				printf("   ");
			for (i = 0; i < (*cp % MEM_width); i++)
				printf("%c", disp[i + *cp - (*cp % MEM_width) + 160]);
			for (i = (*cp % MEM_width); i < MEM_width; i++)
				printf(".");
			printf("\n");
			*cp = *cp + 160;
		}
	}

	else if (arg2 == NULL) {										// dump start ( no end )
		if (arg1_i % MEM_width == 0) {									// no blank case ( start address % 16 = 0 )
			for (i = 0; i < 10; i++) {

				if (i + (arg1_i / MEM_width) >= MEM_height) {
					printf("you exceeded available space.\n");
					*cp = 0;
					return;
				}

				printf("%.5X ", addr[i + (arg1_i / MEM_width)]);

				for (j = 0; j < MEM_width; j++)
					printf("%.2X ", mem[MEM_width * i + j + arg1_i]);
				for (j = 0; j < MEM_width; j++)
					printf("%c", disp[MEM_width * i + j + arg1_i]);
				printf("\n");
			}
			*cp = arg1_i + 160;
		}


		else {											// blank case ( start address % 16 != 0 )
			printf("%.5X ", addr[(arg1_i / MEM_width)]);					// first line ( because of printing blank )
			for (i = 0; i < (arg1_i % MEM_width); i++)
				printf("   ");
			for (i = (arg1_i % MEM_width); i < MEM_width; i++)
				printf("%.2X ", mem[i + arg1_i - (arg1_i % MEM_width)]);
			for (i = 0; i < (arg1_i % MEM_width); i++)
				printf(".");
			for (i = (arg1_i % MEM_width); i < MEM_width; i++)
				printf("%c", disp[i + arg1_i - (arg1_i % MEM_width)]);
			printf("\n");

			if ((arg1_i / MEM_width) >= MEM_height - 1) {
				printf("you exceeded available space.\n");
				*cp = 0;
				return;
			}

			for (i = 1; i < 10; i++) {										// middle line
				printf("%.5X ", addr[(arg1_i / MEM_width) + i]);
				for (j = 0; j < MEM_width; j++)
					printf("%.2X ", mem[MEM_width * i + arg1_i - (arg1_i % MEM_width) + j]);
				for (j = 0; j < MEM_width; j++)
					printf("%c", disp[MEM_width * i + arg1_i - (arg1_i % MEM_width) + j]);
				printf("\n");
			}

			printf("%.5X ", addr[(arg1_i / MEM_width) + 10]);				// last line ( because of printing blank )
			for (i = 0; i < (arg1_i % MEM_width); i++)
				printf("%.2X ", mem[i + arg1_i - (arg1_i % MEM_width) + 160]);
			for (i = (arg1_i % MEM_width); i < MEM_width; i++)
				printf("   ");
			for (i = 0; i < (arg1_i % MEM_width); i++)
				printf("%c", disp[i + arg1_i - (arg1_i % MEM_width) + 160]);
			for (i = (arg1_i % MEM_width); i < MEM_width; i++)
				printf(".");
			printf("\n");

			*cp = arg1_i + 160;
		}
	}

	else {												// dump start end
		if (arg1_i % MEM_width == 0) {								// no blank case ( start address % 16 = 0 )

			for (i = 0; i < (arg2_i - arg1_i) / MEM_width + 1; i++) {

				printf("%.5X ", addr[i + (arg1_i / MEM_width)]);

				for (j = 0; j < MEM_width; j++) {
					if (cnt <= (arg2_i - arg1_i)) {
						printf("%.2X ", mem[MEM_width * i + j + arg1_i]);
						cnt++;
					}
					else
						printf("   ");
				}


				for (j = 0; j < MEM_width; j++) {
					if (cnt2 <= (arg2_i - arg1_i)) {
						printf("%c", disp[MEM_width * i + j + arg1_i]);
						cnt2++;
					}
					else
						printf(".");
				}
				printf("\n");
			}
			*cp = arg1_i + arg2_i - arg1_i + 1;
		}

		else {

			if (arg2_i - arg1_i <= MEM_width - (arg1_i % MEM_width)){				// end - start < MEM_width(16)  - (start % MEM_width)
				printf("%.5X ", addr[(arg1_i / MEM_width)]);
				for (i = 0; i < (arg1_i % MEM_width); i++)
					printf("   ");

				for (i = (arg1_i % MEM_width); i < (arg2_i % MEM_width) + 1; i++)
					printf("%.2X ", mem[i + arg1_i - (arg1_i % MEM_width)]);
				for (i = (arg2_i % MEM_width) + 1; i < MEM_width; i++)
					printf("   ");

				for (i = 0; i < (arg1_i % MEM_width); i++)
					printf(".");
				for (i = (arg1_i % MEM_width); i < (arg2_i % MEM_width) + 1; i++)
					printf("%c", disp[i + arg1_i - (arg1_i % MEM_width)]);
				for (i = (arg2_i % MEM_width) + 1; i < MEM_width; i++)
					printf(".");

				printf("\n");
			}

			else {
				printf("%.5X ", addr[(arg1_i / MEM_width)]);					// first line ( because of printing blank )
				for (i = 0; i < (arg1_i % MEM_width); i++)
					printf("   ");
				for (i = (arg1_i % MEM_width); i < MEM_width; i++)
					printf("%.2X ", mem[i + arg1_i - (arg1_i % MEM_width)]);
				for (i = 0; i < (arg1_i % MEM_width); i++)
					printf(".");
				for (i = (arg1_i % MEM_width); i < MEM_width; i++)
					printf("%c", disp[i + arg1_i - (arg1_i % MEM_width)]);
				printf("\n");

				if ((arg1_i / MEM_width) >= MEM_height - 1) {
					printf("you exceeded available space.\n");
					*cp = 0;
					return;
				}

				for (i = 1; i <= (arg2_i - arg1_i) / MEM_width; i++) {				// middle line
					printf("%.5X ", addr[(arg1_i / MEM_width) + i]);
					if (i == (arg2_i - arg1_i) / MEM_width) {
						for (j = 0; j <= arg2_i % MEM_width; j++)
							printf("%.2X ", mem[MEM_width * i + arg1_i - (arg1_i % MEM_width) + j]);
						for (j = (arg2_i % MEM_width) + 1; j < MEM_width; j++)
							printf("   ");
						for (j = 0; j <= arg2_i % MEM_width; j++)
							printf("%c", disp[MEM_width * i + arg1_i - (arg1_i % MEM_width) + j]);
						for (j = (arg2_i % MEM_width) + 1; j < MEM_width; j++)
							printf(".");
					}
					else {
						for (j = 0; j < MEM_width; j++)
							printf("%.2X ", mem[MEM_width * i + arg1_i - (arg1_i % MEM_width) + j]);
						for (j = 0; j < MEM_width; j++)
							printf("%c", disp[MEM_width * i + arg1_i - (arg1_i % MEM_width) + j]);
					}
					printf("\n");
				}
			}
			*cp = arg2_i + 1;
		}
	}

}

void hextoAscii(unsigned char *mem, char *disp) {						// convert memory data to display data
	int i;										// hexadicimal -> ASCII 
		for (i = 0; i < MEM_height * MEM_width; i++) {
			if (mem[i] >= 32 && mem[i] <= 126)
				disp[i] = mem[i];
			else
				disp[i] = 46;
		}
}

void edit_(unsigned char *mem, int addr, int val) {					// edit addr val
	mem[addr] = val;
}

void fill_(unsigned char *mem, int start, int end, int val) {				// fill start end val
	int i;
	for (i = start; i <= end; i++) 
		mem[i] = val;
}

void reset_(unsigned char *mem) {								// reset memory
	int i;
	for (i = 0; i < MEM_height * MEM_width; i++) 
		mem[i] = 0;
}

void hash_node(hash *target[20], FILE *oplist) {					// create hash table linked-list

	hash *new;
	hash *curr;
	int idx, feop_flag;								// feop_flag : end of file(=0) or not (<0)
	
	while (!feof(oplist)) {
		new = (hash*)malloc(sizeof(hash));
		fscanf(oplist, "%s %s %s", new->code, new->name, new->idk);
		feop_flag = (int)strtoul(new->idk, NULL, 16);
		if (feop_flag <= 0) 
			break;
		new->next = NULL;
		idx = hash_func(new->name);
		curr = target[idx];
		if (target[idx] == NULL) 
			target[idx] = new;
		else {
			while (curr->next != NULL)
				curr = curr->next;
			curr->next = new;
		}
	}
	return;
}

int hash_func(unsigned char *arg1) {							// hash function																					// key = (opcode name / 3)'s rest divided by 20
	int key;
	key = (int)strtoul((char*)arg1, NULL, 16) / 3;
	key %= 20;

	return key;
}

int opcode_(unsigned char *arg1, hash* table[20]){							//opcode name
	int i;
	int h_flag = 0;										//flag : add history list(1) or not(0)
	hash *curr = (hash*)malloc(sizeof(hash));

	i = hash_func(arg1);
	curr = table[i];

	while (1) {
		if (strcmp((char*)curr->name, (char*)arg1) == 0) {
			h_flag = 1;
			printf("opcode is %s\n", curr->code);
			break;
		}
			
		else
			curr = curr->next;

		if (curr == NULL) {
			printf("Can't find opcode you typed in.\n");
			break;
		}
	}

	return h_flag;
}

void opcodelist_(hash *table[20]) {								// print all opcode list
	hash *curr = (hash*)malloc(sizeof(hash));
	int i = 0;


	for (i = 0; i < 20; i++) {
		curr = table[i];
		printf("%d : ", i);


		while(curr != NULL) {
			printf("[%s, %s]", curr->name, curr->code);
			curr = curr->next;
			if (curr != NULL)
				printf(" -> ");
		}
		if (curr == NULL) 
			printf("\n");
	}
}


int main(){

	int i;

	char *command = (char*)malloc(sizeof(char)*80);
	char *cmd_tmp = (char*)malloc(sizeof(char) * 80);
	char *cmd_tok = (char*)malloc(sizeof(char) * 80);

	char *argv1 = (char*)malloc(sizeof(char) * 80);
	char *argv2 = (char*)malloc(sizeof(char) * 80);
	char *argv3 = (char*)malloc(sizeof(char) * 80);
	char *argv4 = (char*)malloc(sizeof(char) * 80);
	argv4 = NULL;

	int *mem_addr = (int*)malloc(sizeof(int) * MEM_height);					// memory address assignment 

	mem_addr[0] = 0;
	for (i = 1; i < MEM_height; i++) {
		mem_addr[i] = mem_addr[i - 1] + 16;
	}


	unsigned char *memory = (unsigned char*)malloc(sizeof(char) * MEM_height * MEM_width);				// memory assignment
		
		for (i = 0; i < MEM_height * MEM_width; i++) {
			memory[i] = 0;
		}
	
	char *ascii_disp = (char*)malloc(sizeof(char) * MEM_height * MEM_width);			// ascii display assignment

	node *head = (node*)malloc(sizeof(node));								// history linked-list assignment
	node *head_tmp = (node*)malloc(sizeof(node));
	head->next = NULL;
	head->idx = 0;
	head_tmp = head;

	int argv1_dec = 0, argv2_dec = 0, argv3_dec = 0;
	int *checkpoint = (int*)malloc(sizeof(int));							// dump address
	*checkpoint = 0;

	hash *hash_table[20];										//	hash table linked-list assignment
	
	for (i = 0; i < 20; i++) {
		hash_table[i] = NULL;
	}

	FILE *fp;											// open "opcode.txt" file
	fp = fopen("opcode.txt", "r");
	if (fp == NULL)
		printf("Can't read 'opcode.txt' file\n");

	hash_node(hash_table, fp);									// make hash table

	while(1){
		printf("sicsim> ");
		fgets(command,100,stdin);
		command[strlen(command)-1] = '\0';

		if ((strcmp(command, "help") == 0) || (strcmp(command, "h") == 0)) {				// commands with no arguments
			head_tmp = add_node(head_tmp, command);
			help_();
		}

		else if ((strcmp(command, "dir") == 0) || (strcmp(command, "d") == 0)) {
			head_tmp = add_node(head_tmp, command);
			dir_();
		}

		else if((strcmp(command,"quit") == 0) || (strcmp(command, "q") == 0))
			break;

		else if ((strcmp(command, "history") == 0) || (strcmp(command, "hi") == 0)) {
			head_tmp = add_node(head_tmp, command);
			history_(head->next);
		}

		else if ((strcmp(command, "dump") == 0) || (strcmp(command, "du") == 0)) {
			head_tmp = add_node(head_tmp, command);
			hextoAscii(memory, ascii_disp);
			dump_(mem_addr, memory, ascii_disp, NULL, NULL, checkpoint, 0, 0);
		}

		else if (strcmp(command, "reset") == 0) {
			head_tmp = add_node(head_tmp, command);
			hextoAscii(memory, ascii_disp);
			reset_(memory);
		}

		else if (strcmp(command, "opcodelist") == 0) {
			head_tmp = add_node(head_tmp, command);
			opcodelist_(hash_table);
		}

		else {											// commands with argument(s)
			strcpy(cmd_tmp, command);
			cmd_tok = strtok(cmd_tmp, " ");
			argv1 = strtok(NULL, ",");
			argv2 = strtok(NULL, ",");
			argv3 = strtok(NULL, ",");
			argv4 = strtok(NULL, " ");						// argv4 : check argument 4 ( ex. fill A, A, A, A ...)
			
			if (argv1 != NULL)
				argv1_dec = (int)strtoul(argv1, NULL, 16);
			if(argv2 != NULL)
				argv2_dec = (int)strtoul(argv2, NULL, 16);
			if (argv3 != NULL)
				argv3_dec = (int)strtoul(argv3, NULL, 16);

			if ((strcmp(cmd_tok, "dump") == 0) || (strcmp(cmd_tok, "du") == 0)) {
				if ((argv1_dec >= 0 && argv1_dec <= MEM_height * MEM_width)) {			// argv1's range check
					if (argv2 == NULL) {							// dump start ( no end )
						head_tmp = add_node(head_tmp, command);
						hextoAscii(memory, ascii_disp);
						dump_(mem_addr, memory, ascii_disp, argv1, argv2, checkpoint, argv1_dec, 0);
					}

					else if (argv1_dec <= argv2_dec) {
						if (argv3 == NULL) {						// dump start end
							if ((argv2_dec >= 0 && argv2_dec <= MEM_height * MEM_width)) {			// argv2's range check
								head_tmp = add_node(head_tmp, command);
								hextoAscii(memory, ascii_disp);
								dump_(mem_addr, memory, ascii_disp, argv1, argv2, checkpoint, argv1_dec, argv2_dec);
							}
							else
								printf("argument2's range is unavailable\n");
						}
						else
							printf("too many arguments\n");
					}
					else
						printf("argument1 is less than argument2\n");
				}
				else
					printf("argument1's range is unavailable\n");
			}

			else if ((strcmp(cmd_tok, "edit") == 0) || (strcmp(cmd_tok, "e") == 0)) {
				if ((argv1_dec >= 0 && argv1_dec <= MEM_height * MEM_width)) {
					if ((argv2_dec >= 0 && argv2_dec <= 255) && argv2 != NULL) {
						if (argv3 == NULL) {
							head_tmp = add_node(head_tmp, command);
							edit_(memory, argv1_dec, argv2_dec);
						}
						else
							printf("too many arguments\n");
					}
					else
						printf("argument2's range is unavailable\n");
				}
				else
					printf("argument1's range is unavailable\n");
			}

			else if ((strcmp(cmd_tok, "fill") == 0) || (strcmp(cmd_tok, "f") == 0)) {
				if ((argv1_dec >= 0 && argv1_dec <= MEM_height * MEM_width)) {
					if ((argv2_dec >= 0 && argv2_dec <= MEM_height * MEM_width) && argv2 != NULL) {
						if ((argv3_dec >= 0 && argv3_dec <= 255) && argv3 != NULL) {
							if (argv4 == NULL) {
								head_tmp = add_node(head_tmp, command);
								fill_(memory, argv1_dec, argv2_dec, argv3_dec);
							}
							else
								printf("too many arguments\n");
						}
						else
							printf("argument3's range is unavailable\n");
					}
					else
						printf("argument2's range is unavailable\n");
				}
				else
					printf("argument1's range is unavailable\n");
			}

			else if (strcmp(cmd_tok, "opcode") == 0) {
				if (argv1 == NULL) 
					printf("there's no arguments\n");


				else if (argv2 == NULL) {
					if( opcode_((unsigned char*)argv1, hash_table) == 1)
						head_tmp = add_node(head_tmp, command);
				}
				
				else
					printf("too many arguments\n");
			}

			else
				printf("unavailable command\n");
		}
		
	}	


/*	free(checkpoint);
	free(ascii_disp);
	free(memory);
	free(argv4);
	free(argv3);
	free(argv2);
	free(argv1);
	free(cmd_tok);
	free(cmd_tmp);
	free(command);
*/	return 0;
}
