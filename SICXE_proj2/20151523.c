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
	printf("opcodelist\n");
	printf("assemble filename\n");
	printf("type filename\n");
	printf("symbol\n\n");
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

void type_(FILE *file) {
	char letter;
	while ((letter = getc(file)) != EOF) {
		putchar(letter);
	}

}

void obj_(hash* table[20]) {
	int i = 0;
	int j = 0;
	int k = 0;
	int l = 0;
	char tmp[30];
	char tmp_c[30];
	hash *curr = (hash*)malloc(sizeof(hash));
	int targ;
	int ni = 3;
	int xbpe = 2;
	int op, loc = 0;

	while (i<line_ctr) {
		strcpy(tmp, "\0");
		int hash_idx = hash_func((unsigned char*)inst[i].symb);	//find symbol from opcode table
		curr = table[hash_idx];
		targ = 0;
		loc = 0;


		if (inst[i].format != 0 && inst[i].format != 5) {

			if (inst[i].format != 4) { //format #4's opcode checking (because of '+')

				while (1) { //format 2, 3
					if (strcmp((char*)curr->name, inst[i].symb) == 0) {
						break;
					}

					else
						curr = curr->next;

					if (curr == NULL) {
						printf("error : Can't find opcode you typed in.\n");
						break;
					}
				}
			}

			else { //format 4
				for (j = 0; j < strlen(inst[i].symb); j++) {
					tmp[j] = inst[i].symb[j + 1];
				}

				while (1) {
					if (strcmp((char*)curr->name, tmp) == 0) {
						break;
					}

					else
						curr = curr->next;

					if (curr == NULL) {
						printf("error : Can't find opcode you typed in.\n");
						break;
					}
				}

			}

		}

		if (inst[i].format == 0) //no object code
			inst[i].obj = 0;

		else if (inst[i].format == 5) { //constant

			tmp_c[0] = '\0';
			tmp_c[1] = '\0';
			tmp_c[2] = '\0';

			for (j = 0; j < strlen(inst[i].target) - 3; j++)
				tmp_c[j] = inst[i].target[j + 2];

			if (inst[i].target[0] == 'C') {
					inst[i].obj = 16 * 16 * 16 * 16 * tmp_c[0] + 16* 16 * tmp_c[1] + tmp_c[2];
			}

			else if (inst[i].target[0] == 'X') {
				if((int)strtoul(tmp_c, NULL, 16) >= 16*16*16*16)
					inst[i].obj = (int)strtoul(tmp_c, NULL, 16);

				else if((int)strtoul(tmp_c, NULL, 16) < 16*16)
					inst[i].obj = 16*16*16*16*(int)strtoul(tmp_c, NULL, 16);

				else
					inst[i].obj = 16 * 16 * (int)strtoul(tmp_c, NULL, 16);
			}
		}

		else if (inst[i].format == 2) { //format #2

			switch (inst[i].target[0]) //1st argument
			{
			case 'A':
				targ = 0 * 16;
				break;

			case 'X':
				targ = 1 * 16;
				break;

			case 'L':
				targ = 2 * 16;
				break;

			case 'B':
				targ = 3 * 16;
				break;

			case 'S':
				targ = 4 * 16;
				break;

			case 'T':
				targ = 5 * 16;
				break;

			case 'F':
				targ = 6 * 16;
				break;

			default :
				targ = 0 * 16;
				break;
			}

			switch (inst[i].target2[0])//2nd argument
			{
			case 'A':
				targ += 0;
				break;

			case 'X':
				targ += 1;
				break;

			case 'L':
				targ += 2;
				break;

			case 'B':
				targ += 3;
				break;

			case 'S':
				targ += 4;
				break;

			case 'T':
				targ += 5;
				break;

			case 'F':
				targ += 6;
				break;

			default:
				targ += 0;
				break;
			}
				inst[i].obj = (int)strtoul(curr->code, NULL, 16) * 16 * 16;
				inst[i].obj += targ;
		}

		else if (inst[i].format == 4) { //format #4

			if (inst[i].target[0] == '#') { //immediate address (n,i) = (0,1)
				ni = 1;
			
				op = (int)strtoul(curr->code, NULL, 16) - ((int)strtoul(curr->code, NULL, 16) % 4);

				for (j = 0; j < strlen(inst[i].target) -1; j++) 
					tmp[j] = inst[i].target[j + 1];

				if((tmp[0] >= '0' && tmp[0] <= '9') || (tmp[0] >= 'A' && tmp[0] <= 'F')){
					xbpe = 1;
					inst[i].obj = 16 * 16 * 16 * 16 * 16 * 16 * (op + ni) + 16 * 16 * 16 * 16 * 16 * xbpe + (int)strtoul(tmp, NULL, 10); //constant
				}

				else {
					xbpe = 3;
				}

			}

			else if (inst[i].target[0] == '@') { //indirect address (n,i) - (1,0)
				ni = 2;
				xbpe = 3;
				op = (int)strtoul(curr->code, NULL, 16) - ((int)strtoul(curr->code, NULL, 16) % 4);

				for (j = 0; j < strlen(inst[i].target) - 1; j++)
					tmp[j] = inst[i].target[j + 1];

				inst[i].obj = 16 * 16 * 16 * 16 * 16 * 16 * (op + ni) + 16 * 16 * 16 * 16 * 16 * xbpe + (int)strtoul(tmp, NULL, 10);
			}

			else {//absolute address (n,i = 1,1 / e = 1)
				op = (int)strtoul(curr->code, NULL, 16) - ((int)strtoul(curr->code, NULL, 16) % 4);
				
				for(j=0;j<line_ctr;j++) {
					if (strcmp(inst[i].target, inst[j].label) == 0) {
						loc = inst[j].loc;
						break;
					}
				}

				ni = 3;
				xbpe = 1;

				inst[i].obj = 16 * 16 * 16 * 16 * 16 * 16 * (op + ni) + 16 * 16 * 16 * 16 * 16 * xbpe + loc;
			}
		}

		else { //format #3
			if (inst[i].target[0] == '#') {//immediate address
				ni = 1;
			
				op = (int)strtoul(curr->code, NULL, 16) - ((int)strtoul(curr->code, NULL, 16) % 4);

				for (j = 0; j < strlen(inst[i].target) - 1; j++)
					tmp[j] = inst[i].target[j + 1];
	
				tmp[strlen(inst[i].target) - 1] = '\0';

				if ((tmp[0] >= '0' && tmp[0] <= '9') || (tmp[0] >= 'A' && tmp[0] <= 'F')) {
					xbpe = 0;
					inst[i].obj = 16 * 16 * 16 * 16 * (op + ni) + 16 * 16 * 16 * xbpe + (int)strtoul(tmp, NULL, 10); //constant
				}

				else {
					for (j = 0; j < line_ctr; j++) { //location counter calculate
						if (strcmp(tmp, inst[j].label) == 0) {
							loc = inst[j].loc - (inst[i].loc + 3);
							break;
						}
					}
					xbpe = 2;

					if (inst[i].target2[0] == 'X') //x b p e = 1 0 0 0
						xbpe += 8;

					inst[i].obj = 16 * 16 * 16 * 16 * (op + ni) + 16 * 16 * 16 * xbpe + loc; //constant
				}

			}

			else if (inst[i].target[0] == '@') {//indirect address
				ni = 2;
				xbpe = 2;
				op = (int)strtoul(curr->code, NULL, 16) - ((int)strtoul(curr->code, NULL, 16) % 4);

				for (j = 0; j < strlen(inst[i].target) - 1; j++)
					tmp[j] = inst[i].target[j + 1];

				tmp[strlen(inst[i].target) - 1] = '\0';

					for (j = 0; j < line_ctr; j++) {
						if (strcmp(tmp, inst[j].label) == 0) {
							loc = inst[j].loc - (inst[i].loc + 3);
							break;
						}
					}

					if (inst[i].target2[0] == 'X') // x b p e = 1 0 0 0
						xbpe += 8;

					inst[i].obj = 16 * 16 * 16 * 16 * (op + ni) + 16 * 16 * 16 * xbpe + loc; //constant
			}

			else {
				if (inst[i].target[0] == ' ') {//no target ex) RSUB
					op = (int)strtoul(curr->code, NULL, 16) - ((int)strtoul(curr->code, NULL, 16) % 4);

					ni = 3;
					xbpe = 0;

					inst[i].obj = 16 * 16 * 16 * 16 * (op + ni) + 16 * 16 * 16 * xbpe;
				}

				else { 
					op = (int)strtoul(curr->code, NULL, 16) - ((int)strtoul(curr->code, NULL, 16) % 4);
					ni = 3;
					xbpe = 2;

					for (j = 0; j < line_ctr; j++) {
						if (strcmp(inst[i].target, inst[j].label) == 0) 
							break;
					}

					if (inst[j].loc >= (inst[i].loc + 3)) { // accessing under label
						if (inst[j].loc - (inst[i].loc + 3) < 4096) //not above buffer
							loc = inst[j].loc - (inst[i].loc + 3);

						else { //above buffer
							xbpe = 4;
							for (k = 0; k < line_ctr; k++) {
								if (strcmp(inst[k].symb, "BASE") == 0) {
									for (l = 0; l < line_ctr; l++) {
										if (strcmp(inst[k].target, inst[l].label) == 0)
										break;
									}
								}
							}
							loc = inst[j].loc - inst[l].loc;
						}

					}

					else {//accessing over label
						if ((inst[i].loc + 3) - inst[j].loc < 4096) { //not above buffer
							loc = 4096 + inst[j].loc - (inst[i].loc + 3);
						}

						else {//above buffer
							xbpe = 4;
							for (k = 0; k < line_ctr; k++) {
								if (strcmp(inst[k].symb, "BASE") == 0) {
									for (l = 0; l < line_ctr; l++) {
										if (strcmp(inst[k].target, inst[l].label) == 0)
											break;
									}
								}
							}
							loc = inst[j].loc - inst[l].loc;
						}
					}

					if (inst[i].target2[0] == 'X')
						xbpe += 8;

					inst[i].obj = 16 * 16 * 16 * 16 * (op + ni) + 16 * 16 * 16 * xbpe + loc;
				}
			}
		}
		i++;
	}
}

void make_obj(char* name) {
	int i = 0;
	int j = 0;
	int cnt = 1;
	name = strtok(name, ".");
	strcat(name, ".obj");

	FILE* obj;
	obj = fopen(name, "w");

	fprintf(obj, "H"); //header record
	while (j < 6) {
		fprintf(obj, "%c", inst[0].label[j]);
		j++;
	}
	fprintf(obj, "%.6X%.6X\n", inst[0].loc, inst[line_ctr-1].loc - inst[0].loc + 1);
	

	
	j = 1;
	while (j < line_ctr-3) {
		cnt = 0;
		while (inst[j].format == 0) //delete variable line
			j++;

		fprintf(obj, "T"); //text record
		fprintf(obj, "%.6X", inst[j].loc);

		while(cnt<10) {
			cnt++;
			if (inst[j + cnt - 1].format == 0) { //cut variable
				cnt -= 1;
				break;
			}

			if (inst[j + cnt - 1].format == 4) { //modify flag
				if (inst[j + cnt - 1].target[0] != '#') 
					inst[j + cnt - 1].m_flag = 1;
			}
		}

		fprintf(obj, "%.2X", (cnt) * 3); // line length

		for (i = 0; i < cnt; i++) {
			if (inst[i + j].format == 0) {
				break;
			}

			if(inst[i + j].format == 2)
				fprintf(obj, "%.4X", inst[i+j].obj);
			else
			fprintf(obj, "%.6X", inst[i+j].obj);
		}

		fprintf(obj, "\n");
		j += cnt;
	}

	for (j = 0; j < line_ctr; j++) { //modified record
		if (inst[j].m_flag == 1) 
			fprintf(obj, "M%.6X05\n", inst[j].loc + 1);
	}

	for (j = 0; j < line_ctr; j++) { //end record
		if (strcmp(inst[line_ctr-1].target, inst[j].label) == 0)
			break;
	}

	fprintf(obj, "E%.6X\n", inst[j].loc);

	fclose(obj);
}

void make_lst(char* name) {
	
	int j;

	name = strtok(name, ".");
	strcat(name, ".lst");

	FILE* lst;
	lst = fopen(name, "w");

	for (j = 0; j < line_ctr; j++) {
		if ((strcmp(inst[j].symb, "END") == 0) || (strcmp(inst[j].symb, "BASE") == 0))
			fprintf(lst, "%d	%s	%s	%s	\n", inst[j].line, inst[j].label, inst[j].symb, inst[j].target);

		else if (strcmp(inst[j].symb, "START") == 0)
			fprintf(lst, "%d	%.4X	%s	%s	%s	\n", inst[j].line, inst[j].loc, inst[j].label, inst[j].symb, inst[j].target);

		else if ((strcmp(inst[j].symb, "RESW") == 0) || (strcmp(inst[j].symb, "RESB") == 0))
			fprintf(lst, "%d	%.4X	%s	%s	%s	\n", inst[j].line, inst[j].loc, inst[j].label, inst[j].symb, inst[j].target);

		else
			fprintf(lst, "%d	%.4X	%s	%s	%s	%.6X\n", inst[j].line, inst[j].loc, inst[j].label, inst[j].symb, inst[j].target, inst[j].obj);

	}
	fclose(lst);
}

int assemble_(FILE *file, hash* table[20]) {
	int i = 0;
	int line = 5;
	int locctr = 0;
	char ins[100];
	char ins_tmp[100];
	char format[10];
	char* n1,* n2,* n3;
	hash *curr = (hash*)malloc(sizeof(hash));
	int hash_idx;
	int second_argu = 0;
	
	int k = 0;
	int l = 0;

	n1 = (char*)malloc(sizeof(char) * 10);
	n2 = (char*)malloc(sizeof(char) * 10);
	n3 = (char*)malloc(sizeof(char) * 10);


	while (!feof(file)) {

		second_argu = 0;//two-variables flag

		fgets(ins, 100, file);
		strcpy(ins_tmp, ins);
		ins_tmp[strlen(ins_tmp) - 1] = '\0';

		n1 = strtok(ins_tmp, " ");
		n2 = strtok(NULL, " ");
		n3 = strtok(NULL, " ");

		if (n1 == NULL)
			break;

		if (strcmp(n1, ".") == 0)
			line += 5;//exception

		else {
			if(strcmp(n1, "RSUB") == 0);

			else if (n2[strlen(n2) - 1] == ',') //two-variable flag
				second_argu = 1;

			if (n3 == NULL) { // no label


				if (n2 == NULL) { //[symbol] no target  ex)RSUB
					inst[i].line = line;
					inst[i].loc = locctr;
					inst[i].label_flag = 0;
					strcpy(inst[i].label, " ");
					strcpy(inst[i].symb, n1);
					strcpy(inst[i].target, " ");
					i++;
				}

				else { // [symbol target1]
					inst[i].line = line;
					inst[i].loc = locctr;
					inst[i].label_flag = 0;
					strcpy(inst[i].label, " ");
					strcpy(inst[i].symb, n1);
					strcpy(inst[i].target, n2);
					strcpy(inst[i].target2, "00");
					i++;
				}
			}

			else { 
				if (second_argu == 0) { //[label symbol target]

					for (l = 0; l < k; l++) {
						if (strcmp(symb_tmp[l].label, n1) == 0) {
							printf("error : label overlapped : line %d\n", i+1);
							return 1;
						}
					}

					inst[i].line = line;
					inst[i].loc = locctr;
					inst[i].label_flag = 1;
					strcpy(inst[i].label, n1);
					strcpy(inst[i].symb, n2);
					strcpy(inst[i].target, n3);
					strcpy(inst[i].target2, "00");

					strcpy(symb_tmp[k].label, n1);
					symb_tmp[k].loc = inst[i].loc;
					k++;
				}

				else { // two variables [symbol target1 target2]
					inst[i].line = line;
					inst[i].loc = locctr;
					inst[i].label_flag = 0;
					strcpy(inst[i].label, " ");
					strcpy(inst[i].symb, n1);
					strcpy(inst[i].target, n2);
					inst[i].target[strlen(inst[i].target) - 1] = '\0';
					strcpy(inst[i].target2, n3);
				}
				i++;
			}

			line += 5;
			//locctr, format store
			//format 0 = no object code line
			//format 5 = constant line
				if (strcmp(inst[i - 1].symb, "WORD") == 0) {
					locctr += 3;
					inst[i - 1].format = 5;
				}

				else if (strcmp(inst[i - 1].symb, "RESW") == 0) {
					locctr += 3 * atoi(inst[i - 1].target);
					inst[i - 1].format = 0;
				}
				else if (strcmp(inst[i - 1].symb, "RESB") == 0) {
					locctr += atoi(inst[i - 1].target);
					inst[i - 1].format = 0;
				}

				else if (strcmp(inst[i - 1].symb, "BYTE") == 0) {
					if(inst[i - 1].target[0] == 'C')
					locctr += strlen(inst[i - 1].target) - 3;

					else if (inst[i - 1].target[0] == 'X')
						locctr += (strlen(inst[i - 1].target) - 3) / 2;

					inst[i - 1].format = 5;
				}

				else if (strcmp(inst[i - 1].symb, "BASE") == 0)
					inst[i - 1].format = 0;

				else if (strcmp(inst[i - 1].symb, "START") == 0)
					inst[i - 1].format = 0;

				else if (strcmp(inst[i - 1].symb, "END") == 0)
					inst[i - 1].format = 0;

				else if (inst[i - 1].symb[0] == '+') {//format 4
					locctr += 4;
					inst[i - 1].format = 4;
				}
				else {
					hash_idx = hash_func((unsigned char*)inst[i - 1].symb);//find symbol from opcode table
					curr = table[hash_idx];

					while (1) {
						if (strcmp((char*)curr->name, inst[i - 1].symb) == 0) {
							strcpy(format, curr->idk);
							break;
						}

						else
							curr = curr->next;

						if (curr == NULL) {
							printf("error : Can't find opcode you typed in. : line %d\n", i);
							return 1;
						}
					}

					if (strcmp(format, "2") == 0) {//format 2
						locctr += 2;
						inst[i - 1].format = 2;
					}

					else {//format 3
						locctr += 3;
						inst[i - 1].format = 3;
					}
				}
					
		}
	}
	line_ctr = i;
	obj_(table);
	symb_cnt = k;
	return 0;
}

void symbol_() {
	int i, j, tmp2;
	char tmp[30];


	if (line_ctr == 0)
		return;

	for (i = 0; i < symb_cnt - 1; i++)
	{
		for (j = 0; j < symb_cnt - 1 - i; j++)
		{
			if (strcmp(symb_tmp[j].label, symb_tmp[j+1].label) > 0) //sorting by dictionary 
			{
				strcpy(tmp, symb_tmp[j].label);
				strcpy(symb_tmp[j].label, symb_tmp[j + 1].label);
				strcpy(symb_tmp[j + 1].label, tmp);

				tmp2 = symb_tmp[j].loc;
				symb_tmp[j].loc = symb_tmp[j + 1].loc;
				symb_tmp[j + 1].loc = tmp2;
			}
		}
	}

	for (i = 0; i < symb_cnt; i++)
		printf("\t%s\t%.4X\n", symb_tmp[i].label, symb_tmp[i].loc);

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

	FILE *tp = NULL;
	FILE *as = NULL;

	struct stat buf;

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

		else if (strcmp(command, "symbol") == 0) {
			head_tmp = add_node(head_tmp, command);
			symbol_();
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

			else if (strcmp(cmd_tok, "type") == 0) {
				if (argv1 == NULL)
					printf("there's no filename\n");


				else if (argv2 == NULL) {
					tp = fopen(argv1, "r");
					stat(argv1, &buf);

					if (S_ISDIR(buf.st_mode)) {
						printf("Can't read directory '%s' \n", argv1);
					}

					else if (tp == NULL)
						printf("Can't read '%s' file\n", argv1);

					else {
						head_tmp = add_node(head_tmp, command);
						type_(tp);
					}
				}

				else
					printf("too many arguments\n");
			}

			else if (strcmp(cmd_tok, "assemble") == 0) {
				if (argv1 == NULL)
					printf("there's no filename\n");


				else if (argv2 == NULL) {
					as = fopen(argv1, "r");
					if (as == NULL)
						printf("Can't read '%s' file\n", argv1);

					else {
						head_tmp = add_node(head_tmp, command);
						if (assemble_(as, hash_table) == 0) {
							printf("%c[1;32m", 27);
							printf("Successfully");
							printf("%c[0m", 27);
							printf(" assemble %s.\n", argv1);
							make_lst(argv1);
							make_obj(argv1);
						}
						
					}
				}

				else
					printf("too many arguments\n");
			}

			else
				printf("unavailable command\n");
		}
		
	}	
	if(as != NULL)
		fclose(as);

	if(tp != NULL)
		fclose(tp);

	fclose(fp);
	free(argv3);
	free(argv2);
	free(ascii_disp);
	free(memory);
	free(mem_addr);
	free(cmd_tok);
	free(command);



	return 0;
}
