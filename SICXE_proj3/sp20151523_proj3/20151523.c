#include "20151523.h"

Symbol_Table* Dec_Table;
Link_History* Used_Command = NULL;																	//linked list for history
Opcode_Table* Op_Table[20];																			//linked list for opcode
Symbol_Table* Sym_Table[20];

Estab_Table* Es_Table[20];

int History_Num=1;																					//history variable
unsigned char memory[max_memory]={0,};																//memory variable
int addr=0;																							//address variable
int LOCCTR;
int loc_length;
int op_length[100];
int loc_save[10000];
int mod_list[50]={0,};

int firsttry;
int progaddr;
int runstartaddr;
int runendaddr;
int runfinaladdr;

Symbol_Table *bp_list=NULL;

int main(void){
	char command[100],first_str[100],second_str[100],comma,fcomma,str_error,com_error;
	char filename1[30],filename2[30],filename3[30]; 

	int start,end;											
	int dump_check,com_check;
	int eaddr, evalue;
	int fstart,fend,fvalue;
	int blank_flag,opflag;
	int i;
	Symbol_Table *bptmp;

	bptmp = bp_list;

	runstartaddr = 0;
	runendaddr = 0;
	runfinaladdr = 0;

	opflag = Read_Op();
	progaddr = 0;
	firsttry=0;

	while(opflag){
		/*Initializing. */
		blank_flag=0;
		comma='c';

		fstart = -1;
		fend = -1;
		fvalue = -1;
		fcomma = '\0';

		eaddr=-1;
		evalue=-1;

		start=-1;
		end=-1;
		
		com_check=-1;
		str_error='\0';
		dump_check=-1;

		first_str[0] = '\0';
		second_str[0] = '\0';

		filename1[0] = '\0';
		filename2[0] = '\0';
		filename3[0] = '\0';

		
		Basic_State();
		fgets(command, 100, stdin);
		com_check = sscanf(command,"%s %c",first_str,&com_error);


		/*Input command and determine whether the command is valid or not. */
		for(i=0; i<(int)strlen(command); i++){
			if(command[i] == '\0' || command[i] == ' ' || command[i] == '\t')
				blank_flag += 1;
		}
		if(blank_flag == (int)strlen(command))
			continue;

		if((strcmp(first_str,"q") == 0 || strcmp(first_str,"quit") == 0) && com_check == 1){				//quit program
			break;
		}
		else if((strcmp(first_str,"h") == 0 || strcmp(first_str,"help") == 0) && com_check == 1){			//help
			Make_History(command);
			Helplist();
		}
		else if((strcmp(first_str,"d") == 0 || strcmp(first_str,"dir") == 0) && com_check == 1){			//directory
			Make_History(command);
			Print_Dir();
		}
		else if((strcmp(first_str,"hi") == 0 || strcmp(first_str,"history") == 0) && com_check == 1){		//history
			Make_History(command);
			Print_History();
		}
		else if(strcmp(first_str,"reset") == 0 && com_check == 1){
			Make_History(command);
			Reset_Dump();																					//reset all the memories
		}

		else if(strcmp(first_str,"du") == 0 || strcmp(first_str,"dump") == 0){								//dump
			dump_check = sscanf(command,"%s %x %c %x %c",first_str,&start,&comma,&end,&str_error);
			if(dump_check == 1 && start == -1 && comma == 'c' && end == -1 && str_error == '\0' && sscanf(command,"%s %s",first_str,second_str) == 1){
				Make_History(command);
				Dump_Print(addr,addr+159);
			}
			else if(dump_check == 2  && comma == 'c' && end == -1 && str_error == '\0'){
				if(start<0 || start>=max_memory)
					printf("dump range check!\n");
				else{
					Dump_Print(start,start+159);
					Make_History(command);
				}
			}
			else if(dump_check == 4 && comma == ',' && str_error == '\0' ){
				if(start<0 || start>=max_memory || end>=max_memory || start > end)
					printf("dump range check!\n");
				else{
					Dump_Print(start,end);
					Make_History(command);
				}
			}
			else{
				printf("invalid command!\n");
			}
		}

		else if(strcmp(first_str,"e") == 0 || strcmp(first_str,"edit") == 0){								//edit
			dump_check = sscanf(command,"%s %x %c %x %c",first_str,&eaddr,&comma,&evalue,&str_error);
			if(dump_check == 4 && comma == ',' && evalue != -1 && eaddr != -1 && str_error == '\0'){
				if(evalue <= 255 && evalue >= 0 && eaddr >= 0 && eaddr < max_memory){
					Dump_Edit(eaddr,evalue);
					Make_History(command);
				}
				else{
					printf("Edit range check!\n");
				}
			}
			else{
				printf("invalid command!\n");
			}
		}

		else if(strcmp(first_str,"f") == 0 || strcmp(first_str,"fill") == 0){								//fill
			dump_check = sscanf(command,"%s %x %c %x %c %x %c",first_str,&fstart,&comma,&fend,&fcomma,&fvalue,&str_error);
			if(dump_check == 6 && comma == ',' && fcomma == ',' && str_error == '\0'){
				if(fstart <= fend && fstart >= 0 && fend < max_memory && fvalue >= 0 && fvalue <= 255){
					Dump_Fill(fstart,fend,fvalue);
					Make_History(command);
				}
				else{
					printf("fill range check!\n");
				}
			}
			else{
				printf("invalid command!\n");
			}
		}

		else if(strcmp(command,"opcodelist") == 0 && com_check == 1){										//opcode list
			Op_List();
			Make_History(command);
		}

		else if(strcmp(first_str,"opcode") == 0){															//opcode mnemonic
			dump_check = sscanf(command,"%s %s %c",first_str,second_str,&str_error);
			if(dump_check == 2 && str_error == '\0'){
				Op_Mnemonic(second_str);
				Make_History(command);
			}
			else{
				printf("invalid command!\n");
			}
		}
		else if(strcmp(first_str,"type") == 0){
			dump_check = sscanf(command,"%s %s %c",first_str,second_str,&str_error);
			if(dump_check == 2 && str_error == '\0'){
				Type_Print(second_str);
				Make_History(command);
			}
		}
		else if(strcmp(first_str,"symbol") == 0 && com_check == 1){
			Print_Symtab();
			Make_History(command);
		}
		else if(strcmp(first_str,"assemble") == 0){
			dump_check = sscanf(command,"%s %s %c",first_str,second_str,&str_error);
			if(dump_check == 2 && str_error == '\0'){
				Free_Symtab();
				Reset_Symbol();
				Assemble(second_str);
			}
			else{
				printf("Invalid command!\n");
			}
			Make_History(command);
		}
		else if(strcmp(first_str,"progaddr") == 0){
			//progaddr꼴
			if(com_check == 1){
				progaddr = 0;
				Make_History(command);
			}
			else{
				dump_check = sscanf(command,"%s %s %c",first_str,second_str,&str_error);
				if(dump_check == 2 && str_error == '\0'){
					dump_check = sscanf(command,"%s %X %c",first_str,&eaddr,&str_error);
					if(dump_check == 2 && str_error == '\0'){
						progaddr = eaddr;
						Make_History(command);
					}
					else{
						printf("Invalid command!\n");
					}
				}
				else{
					printf("Invalid command!\n");
				}
			}
			//progaddr이랑 progaddr address 꼴 두개 처리 해야함
		}
		else if(strcmp(first_str,"loader") == 0){
			dump_check = sscanf(command,"%s %s %s %s %c",first_str,filename1,filename2,filename3,&str_error);
			if(dump_check >= 2 && dump_check <= 4 && str_error == '\0'){
				Free_Estab();
				runstartaddr = progaddr;								//run 시작주소 저장
				//printf("%X\n",runstartaddr);
				loader_pass1(filename1, filename2, filename3);
				runfinaladdr = progaddr;									//run 끝난주소 저장
				//printf("%X\n",runendaddr);
				Make_History(command);
			}
			else{
				printf("Invalid command!\n");
			}
			
			//최소 한개에서 최대 3개까지 입력가능
		}
		else if(strcmp(first_str,"run") == 0 && com_check == 1){
			Make_History(command);
			if(runfinaladdr == 0){
				printf("Nothing loaded!\n");
				continue;
			}
			bptmp = bp_list;
			if(bptmp != NULL){
				runendaddr = bptmp->locctr;
				Run_Prog();
				runstartaddr = runendaddr;
				printf("\tStop at checkpoint[%X]\n",bptmp->locctr);
				bptmp = bp_list;
				bp_list = bp_list->link;
				free(bptmp);
			}
			else{
				runendaddr = runfinaladdr;
				Run_Prog();
				firsttry = 0;
				printf("\tEnd Program\n");
			}
			/*
			bptmp = bp_list;
			while(bptmp != NULL){
				runendaddr = bptmp->locctr;
				bptmp = bptmp->link;
				Run_Prog();
				runstartaddr = runendaddr;
			}
			runendaddr = runfinaladdr;
			Run_Prog();
			*/
			//run함수 구현 아직 안함
		}
		else if(strcmp(first_str,"bp") == 0){
			dump_check = sscanf(command,"%s %s %c",first_str,second_str,&str_error);
			if(dump_check == 1 && str_error == '\0'){
				Print_Bp();
				Make_History(command);
			}
			else if(dump_check == 2 && (strcmp(second_str,"clear") == 0) && str_error == '\0'){
				Clear_Bp();
				Make_History(command);
			}
			else if(dump_check == 2){
				dump_check = sscanf(command,"%s %x",first_str,&eaddr);
				printf("\t[");
				printf("%c[1;32m", 27);
				printf("ok");
				printf("%c[0m", 27);
				printf("] create breakpoint %X\n",eaddr);
				Make_Bplist(eaddr);
				//bplist[bpnum++] = eaddr;
				Make_History(command);
			}
			else{
				continue;
			}
			//bp address랑 bp clear 두개 짜야함
		}
		else{
			printf("invalid command!\n");
		}
	}
	Free_History();
	Op_Free();
	Free_Symtab();
	Free_Estab();

	return 0;
}

/* Printing the basic state. */
void Basic_State(void){
	printf("sicsim> ");
}

/* Reading "opcode.txt". */
int Read_Op(void){
	FILE *fp;
	int numtmp;
	char mnemonictmp[20];
	char typetmp[10];
	int ret;
	int i;
	for(i=0; i<20; i++)
		Op_Table[i] = NULL;
	fp = fopen("opcode.txt","r");
	if(fp == NULL){
		printf("There is no opcode list in the directory\n");
		return 0;
	}
	while(ret != EOF){
		ret = fscanf(fp,"%x%s%s",&numtmp,mnemonictmp,typetmp);
		if(ret == 3)
			Op_Make(numtmp,mnemonictmp,typetmp);
	}
	return 1;
}

/* Print the helplist. */
void Helplist(void){
	printf("h[elp]\n");
	printf("d[ir]\n");
	printf("q[uit]\n");
	printf("hi[story]\n");
	printf("du[mp] [start, end]\n");
	printf("e[dit] address, value\n");
	printf("f[ill] start, end, value\n");
	printf("reset\n");
	printf("opcode mnemonic\n");
	printf("opcodelist\n");
	printf("assemble filename\n");
	printf("type filename\n");
	printf("symbol\n");
	printf("progaddr [address]\n");
	printf("loader [object filename1][object filename2][...]\n");
	printf("run\n");
	printf("bp [address]\n");
	printf("bp clear\n");
}

/* Insert new node into the history list. */
void Make_History(char *command){
	Link_History* tmp;
	Link_History* new;

	new = (Link_History*)malloc(sizeof(Link_History));
	new->num = History_Num++;
	strcpy(new->name, command);
	new->link = NULL;

	if(Used_Command == NULL){
		Used_Command = new;
	}
	else{
		for(tmp = Used_Command; tmp->link != NULL; tmp = tmp->link)
			;
		tmp->link = new;
	}	
}

/* Free history linked list. */
void Free_History(void){
	Link_History *tmp;
	while((tmp = Used_Command) != NULL){
		Used_Command = Used_Command->link;
		free(tmp);
	}
}

/* Print the history list. */
void Print_History(){
	Link_History* tmp;
	for(tmp = Used_Command; tmp != NULL; tmp = tmp->link){
		printf("%7d\t%s\n",tmp->num,tmp->name);
	}
}

/* Print all the files in the directory. */
void Print_Dir(void){
	DIR *dirp;
	struct dirent *direntp;
	struct stat file_info;
	int line=0;
	char nametmp[100];

	dirp = opendir(".");

	while((direntp = readdir(dirp)) != NULL){
		strcpy(nametmp, direntp->d_name);
		if(strcmp(nametmp,".") == 0 || strcmp(nametmp,"..") == 0)
			continue;
		lstat(nametmp,&file_info);
		printf("%20s",nametmp);
		line++;
		if(S_ISDIR(file_info.st_mode))
			printf("/");
		else if(S_IEXEC & file_info.st_mode)
			printf("*");
		if(line % 3 == 0)
			printf("\n");
	}
	printf("\n");
	closedir(dirp);
}

/* Reset all the memory into 0. */
void Reset_Dump(void){
	int i;
	for(i=0; i<max_memory; i++)
		memory[i] = 0;
	return ;
}

/* Print dump. */
void Dump_Print(int start,int end){														
	int i,j,k,addrtmp,ascii_start;
	if(end >= max_memory)
		end = max_memory-1;
	addrtmp = end+1;
	printf("%05X ",(start/16)*16);
	for(i=0; i<start%16; i++)
		printf("   ");
	for(i=start; i<=end; i++){
		printf("%02X ",memory[i]);
		if(i == end){
			for(j=0; j<( 16-((end+1) % 16) ) % 16; j++){
				printf("   ");
			}
		}
		if((i+1) % 16 == 0 || i == end){
			printf("; ");
			ascii_start = i - (i%16);
			for(k=ascii_start; k<ascii_start+16; k++){
				if(memory[k] >= 32 && memory[k] <= 127)
					printf("%c",memory[k]);
				else
					printf(".");
			}
			printf("\n");
			if(i < end)
				printf("%05X ",((i+1)/16) * 16);
		}
	}
	addr = addrtmp % max_memory;
}

/* Edit the specific address's value. */
void Dump_Edit(int eaddr,int evalue){
	memory[eaddr] = evalue;
}

/* Fill some value of address. */
void Dump_Fill(int fstart,int fend,int fvalue){
	int i;
	for(i=fstart; i<=fend; i++){
		memory[i] = fvalue;
	}
}

/* Make the linked list of a opcode. */
void Op_Make(int numtmp,char mnemonictmp[],char typetmp[]){
	int idxtmp;
	Opcode_Table *new;
	Opcode_Table *tmp;
	/* Memory Allocation. */
	new = (Opcode_Table*)malloc(sizeof(Opcode_Table));
	new->op_num = numtmp;
	strcpy(new->op_mnemonic, mnemonictmp);
	strcpy(new->type, typetmp);
	new->link = NULL;

	idxtmp = opcodetable_func_make_hash_given(mnemonictmp);
	if(Op_Table[idxtmp] == NULL){
		Op_Table[idxtmp] = new;
	}
	else{
		for(tmp = Op_Table[idxtmp]; tmp->link != NULL; tmp = tmp->link)
			;
		tmp->link = new;
	}
}

/* Free the linked list of a opcode. */
void Op_Free(void){
	int i;
	Opcode_Table *tmp;
	for(i=0; i<20; i++){
		while((tmp = Op_Table[i]) != NULL){
			Op_Table[i] = Op_Table[i]->link;
			free(tmp);
		}
	}
}

/* Find the opcode of specific mnemonic. */
void Op_Mnemonic(char mnemonic[]){
	int ret;
	Opcode_Table *tmp;
	ret = opcodetable_func_make_hash_given(mnemonic);
	for(tmp = Op_Table[ret]; tmp != NULL; tmp = tmp->link){
		if(strcmp(tmp->op_mnemonic,mnemonic) == 0){
			printf("opcode is %02X\n",tmp->op_num);
			return ;
		}
	}
	printf("There is no mnemonic such as %s\n",mnemonic);
}

/* Print the linked list of a opcodelist. */
void Op_List(void){
	int i;
	Opcode_Table *tmp;
	for(i=0; i<20; i++){
		printf("%d : ",i); 
		if(Op_Table[i] != NULL){
			printf("[%s,%X] ",Op_Table[i]->op_mnemonic,Op_Table[i]->op_num);
			if(Op_Table[i]->link != NULL){
				for(tmp = Op_Table[i]->link; tmp != NULL; tmp = tmp->link){
					printf("-> [%s,%X] ",tmp->op_mnemonic,tmp->op_num);
				}
			}
		}
		printf("\n");
	}
}

/* Make the hash function. */
int opcodetable_func_make_hash_given(char s[])
{
	int i,sum;
	int len=strlen(s);

	sum=0;	
	for(i=0; i<len; i++)
	{
		sum+=s[i];
	}
	return sum%20;
}

/* Return the type of the opcode. */
int Type_Print(char filename[]){
	FILE *fp = fopen(filename,"r");
	char buffer[200];
	if(fp == NULL){
		printf("%s does not exist!\n",filename);
		return 0;
	}
	while(fgets(buffer,200,fp)){
		printf("%s",buffer);
	}
	fclose(fp);
	return 1;
}
/* Symbol Table 만들고 LOCCTR 첨가. */
int Make_Symbol_And_Loc(char filename[]){
	FILE *fp = fopen(filename,"r");
	char buffer[200];
	char tmp1[40],tmp2[40],tmp3[40];
	int hexnum;
	int scan_check=0,loctmp,byte_num;
	int error_flag=0,startloc;
	int op_cnt=0,loc_cnt=0,op_tmp=0;
	int line_cnt=0;
	if(fp == NULL){
		printf("%s does not exist!\n",filename);
		return -1;
	}

	/* 첫줄에 start 들어오기 전에 다른 instruction 들어오면 오류처리. */
	while(fgets(buffer,200,fp)){
		scan_check = sscanf(buffer,"%s %s %x",tmp1,tmp2,&hexnum);
		if(strcmp(tmp2,"START") == 0 && scan_check == 3){
			LOCCTR = hexnum;
			loc_save[loc_cnt++] = LOCCTR;
			loctmp = LOCCTR;
			startloc = LOCCTR;
			line_cnt += 5;
			break;
		}
		else if(tmp1[0] == '.')
			continue;
		else{
			printf("Other instructions cannot come before than start!\n");
			error_flag=1;
		}
	}
	/* 둘째줄부터 */
	while(fgets(buffer,200,fp)){
		tmp1[0] = '\0';
		tmp2[0] = '\0';
		tmp3[0] = '\0';
		scan_check = -1;
		byte_num = -1;
		sscanf(buffer,"%s",tmp1);
		if(tmp1[0] == '.')
			continue;
		line_cnt += 5;
		scan_check = sscanf(buffer,"%s %s %s",tmp1,tmp2,tmp3);
		if(Is_Used_Sym(tmp1) != -1 && scan_check == 3){						//이미 쓴 symbol
			printf("Error! Line: %d Symbol already used!\n",line_cnt);
			error_flag=1;
		}
		if(strcmp(tmp1,"END") == 0)
			break;
		if(Is_Op(tmp2) == 1){							//opcode의 형식이 맞은 경우
			loctmp = LOCCTR;
			Make_Symtab(tmp1,loctmp);
			loctmp = Type_Return(tmp2);
			loc_save[loc_cnt++] = LOCCTR;
			if(op_tmp+loctmp>30){
				op_length[op_cnt++] = op_tmp;
				op_tmp=0;
			}
			op_tmp += loctmp;
			LOCCTR += loctmp;
		}
		else if(Is_Op(tmp2) == 2){						//+opcode의 형식
			loctmp = LOCCTR;
			Make_Symtab(tmp1,loctmp);
			loc_save[loc_cnt++] = LOCCTR;
			if(op_tmp+4>30){
				op_length[op_cnt++] = op_tmp;
				op_tmp=0;
			}
			op_tmp += 4;
			LOCCTR += 4;
		}
		else if(strcmp(tmp2,"BYTE") == 0){				//BYTE가 들어왔을 경우
			loctmp = LOCCTR;
			if(tmp3[0] == 'X'){
				byte_num = Byte_Check(tmp3);
			}
			else if(tmp3[0] == 'C'){
				byte_num = Check_C(buffer);
			}
			if(byte_num == -1){							//syntax 오류
				printf("Error! Line: %d BYTE syntax error!\n",line_cnt);
				error_flag = 1;
				continue;
			}
			Make_Symtab(tmp1,loctmp);
			loc_save[loc_cnt++] = LOCCTR;
			if(op_tmp + byte_num>30){
				op_length[op_cnt++] = op_tmp;
				op_tmp=0;
			}
			op_tmp += byte_num;
			LOCCTR += byte_num;	// 
		}
		else if(strcmp(tmp2,"RESW") == 0){				//RESW가 들어왔을 경우
			if(Is_Dec(tmp3) == -1){
				printf("Error! Line: %d RESW syntax error!\n",line_cnt);
				error_flag = 1;
				continue;
			}
			loctmp = LOCCTR;
			Make_Symtab(tmp1,loctmp);
			loc_save[loc_cnt++] = LOCCTR;
			if(op_tmp != 0){
				op_length[op_cnt++] = op_tmp;
				op_tmp=0;
			}
			LOCCTR += 3 * atoi(tmp3);
		}
		else if(strcmp(tmp2,"RESB") == 0){				//RESB가 들어왔을 경우
			if(Is_Dec(tmp3) == -1){
				printf("Error! Line: %d RESB syntax error!\n",line_cnt);
				error_flag = 1;
				continue;
			}
			loctmp = LOCCTR;
			Make_Symtab(tmp1,loctmp);
			loc_save[loc_cnt++] = LOCCTR;
			if(op_tmp != 0){
				op_length[op_cnt++] = op_tmp;
				op_tmp=0;
			}
			LOCCTR += atoi(tmp3);
		}
		else if(strcmp(tmp2,"WORD") == 0){				//WORD가 들어왔을 경우
			if(Is_Dec(tmp3) == -1 || atoi(tmp3) > 0xFFFFFF){
				printf("Error! Line: %d WORD syntax error!\n",line_cnt);
				error_flag = 1;
				continue;
			}
			loctmp = LOCCTR;
			Make_Symtab(tmp1,loctmp);
			loc_save[loc_cnt++] = LOCCTR;
			if(op_tmp + 3>30){
				op_length[op_cnt++] = op_tmp;
				op_tmp=0;
			}
			op_tmp += 3;
			LOCCTR += 3;
		}
		else if(strcmp(tmp2,"BASE") == 0){
			continue;
		}
		/*여기까지는 LABEL이 들어온 case, 즉 symbol table에 추가해야할 상황*/

		/* 여기부터는 LABEL이 없는 경우*/
		else{
			if(Is_Op(tmp1) == 1){							//opcode의 형식이 맞은 경우
				loctmp = Type_Return(tmp1);
				loc_save[loc_cnt++] = LOCCTR;
				if(op_tmp + loctmp > 30){
					op_length[op_cnt++] = op_tmp;
					op_tmp=0;
				}
				op_tmp += loctmp;
				LOCCTR += loctmp;
			}
			else if(Is_Op(tmp1) == 2){						//+opcode의 형식
				loc_save[loc_cnt++] = LOCCTR;
				if(op_tmp + 4 > 30){
					op_length[op_cnt++] = op_tmp;
					op_tmp=0;
				}
				op_tmp += 4;
				LOCCTR += 4;
			}
			else if(strcmp(tmp1,"BASE") == 0){
				continue;
			}
			else{
				printf("%s\n",tmp1);
				printf("Error! Line: %d Opcode doesn't exist!\n",line_cnt);
				error_flag = 1;
				continue;
			}
		}
	}
	op_length[op_cnt++] = op_tmp;
	loc_length = LOCCTR - startloc;
	fclose(fp);
	if(error_flag == 1)
		return -1;
	else
		return 1;
}
void Make_Symtab(char symbol[], int locctr){
	int idx;
	Symbol_Table *new;
	Symbol_Table *tmp = NULL;

	new = (Symbol_Table*)malloc(sizeof(Symbol_Table));
	strcpy(new->symbol,symbol);
	new->locctr = locctr;
	new->link = NULL;

	idx = opcodetable_func_make_hash_given(symbol);
	//printf("symbol2: %s %04X\n",symbol,locctr);
	if(Sym_Table[idx] == NULL){
		Sym_Table[idx] = new;
	}
	else{
		for(tmp = Sym_Table[idx]; tmp->link != NULL; tmp = tmp->link)
			;
		tmp->link = new;
	}
}

void Free_Symtab(void){
	int i;
	Symbol_Table *tmp;
	for(i=0; i<20; i++){
		while((tmp = Sym_Table[i]) != NULL){
			Sym_Table[i] = Sym_Table[i]->link;
			free(tmp);
		}
	}
}

/* 안겹치면 -1 겹치면 locctr 리턴*/
int Is_Used_Sym(char symbol[]){
	int idx;
	Symbol_Table *tmp;
	idx = opcodetable_func_make_hash_given(symbol);
	for(tmp = Sym_Table[idx]; tmp != NULL; tmp = tmp->link){
		if(tmp==NULL)
			return -1;
		if(strcmp(tmp->symbol,symbol) == 0)
			return tmp->locctr;
	}
	return -1;
}	

/* 안겹치면 0 겹치면 1 +꼴일경우 2 리턴*/
int Is_Op(char name[]){
	int idx;
	int flag=0;
	char plus_deleted[20];
	Opcode_Table *tmp;

	/*+뺀 mnemonic 저장.*/
	if(name[0] == '+'){
		strcpy(plus_deleted,name+1);

		idx = opcodetable_func_make_hash_given(plus_deleted);
		for(tmp = Op_Table[idx]; tmp != NULL; tmp = tmp->link){
			if(strcmp(tmp->op_mnemonic,plus_deleted) == 0)
				flag = 2;
		}
	}
	else{
		idx = opcodetable_func_make_hash_given(name);
		for(tmp = Op_Table[idx]; tmp != NULL; tmp = tmp->link){
			if(strcmp(tmp->op_mnemonic,name) == 0)
				flag = 1;
		}
	}
	if(flag == 0)
		return 0;
	else if(flag == 1)
		return 1;
	else if(flag == 2)
		return 2;
	else
		return 0;
}

/* 내림차순으로 출력. */
void Print_Symtab(void){
	int i,count=0;
	Symbol_Table *tmp;
	for(i=0; i<20; i++){
		if(Sym_Table[i] != NULL){
			for(tmp = Sym_Table[i]; tmp != NULL; tmp = tmp->link)
				count += 1;
		}
	}
	for(i=0; i<count; i++)
		printf("\t%s\t%04X\n",Dec_Table[i].symbol,Dec_Table[i].locctr);
}

/*assemble 함
  loc_length < 프로그램 길이 */
void Assemble(char filename[]){
	int exe_flag=0,error_flag=0,i;
	int mod_cnt=0;
	int line_cnt=0;										//line 수
	int scan_check=0;
	int loc_cnt=0, op_tmp=0, op_cnt=0;
	int pc_num=0,base_num=0;							//각각 pc counter, base counter
	int obj_code;										//obj 계산시 사용
	int numtmp,baseflag=0;
	int lentmp;											//저장용

	char buffertmp[200];
	char byte_tmp1[40];
	char syn_check;										//syntax 에러 체크시 사용
	char *str,buffer[200],tmp1[40],tmp2[40],tmp3[40],tmp4[2],tmp5[2];
	char str_tmp[40],str_tmp2[40];
	char *file_lst, *file_obj,file_tmp[30];
	char *regtmp1, *regtmp2;

	FILE *fp, *fp2, *fp3;

	for(i=0; i<100; i++)
		op_length[i]=0;
	for(i=0; i<10000; i++)
		loc_save[i]=0;

	fp = fopen(filename,"r");
	exe_flag=Make_Symbol_And_Loc(filename);	
	if(exe_flag == -1)
		return ;

	strcpy(file_tmp,filename);
	file_lst = strtok(file_tmp, ".");
	strcat(file_lst,".lst");
	fp2 = fopen(file_lst,"w");

	file_obj = strtok(file_lst,".");
	strcat(file_obj,".obj");
	fp3 = fopen(file_obj,"w");


	/* 맨 처음 START를 입력 받고 프로그램을 시작. */
	while(fgets(buffertmp,200,fp)){
		tmp1[0]='\0';
		tmp2[0]='\0';
		tmp3[0]='\0';
		tmp4[0]='\0';
		tmp5[0]='\0';

		scan_check = sscanf(buffertmp,"%s %s %s %s %s",tmp1,tmp2,tmp3,tmp4,tmp5);
		line_cnt += 5;
		if(strcmp(tmp2,"START") == 0 && scan_check>=3){
			str = strtok(buffertmp,".\n");
			fprintf(fp2,"%3d\t%04X %-50s\n",line_cnt,loc_save[loc_cnt+1],str);
			fprintf(fp3,"H%-6s%06X%06X",tmp1,loc_save[loc_cnt+1],loc_length);
			loc_cnt++;
			break;
		}
		else if(tmp1[0] == '.'){
			continue;
		}
		else{
			printf("Line 5 : Other instructions cannot come before than start!\n");
			error_flag = 1;
		}
	}

	/* 두 번쨰 줄부터의 실행
	 * object 코드를 실시간으로 생성하여 *.lst파일에 입력한다.
	 * 또한 *.obj 파일에도 실시간으로 입력한다.
	 * 반복문이 도는 동안 계속 저장될 변수로는 line 수, locctr 등이
	 * 저장된다.*/
	while(fgets(buffertmp,200,fp)){
		tmp1[0]='\0';
		tmp2[0]='\0';
		tmp3[0]='\0';
		tmp4[0]='\0';
		tmp5[0]='\0';
		obj_code=0;
		numtmp=-1;
		syn_check='\0';
		pc_num = loc_save[loc_cnt+1];

		scan_check=0;

		/* 주석 제외 */
		sscanf(buffertmp,"%s",tmp1);
		if(tmp1[0] == '.')
			continue;
		strcpy(buffer,buffertmp);
		str = strtok(buffertmp,".\n");
		strtok(buffer,"\n");
		scan_check = sscanf(str,"%s %s %s %s %s",tmp1,tmp2,tmp3,tmp4,tmp5);
		line_cnt += 5;

		/* END가 나올경우 프로그램 종료 */
		if(strcmp(tmp1,"END") == 0){
			fprintf(fp2,"%3d\t     %-50s\n",line_cnt,str);
			break;
		}
		/* 앞에 label 있고 두번째께 opcodelist에 존재할 경우.  */
		else if(Is_Op(tmp2) == 1){

			/* Symbol의 앞에 #이 올 경우 
			 * 순서대로 PC Addressing, Base Relative
			 * 숫자가 들어올 경우 */
			if(tmp3[0] == '#'){
				if(Is_Used_Sym(tmp3+1) != -1){
					numtmp = Is_Used_Sym(tmp3+1);
					/*Inside the range of PC addressing mode. */
					if((numtmp - pc_num) >= -2048 && (numtmp - pc_num) <= 2047){
						obj_code = format3(Opnum_Return(tmp2),0x12,numtmp-pc_num);
						fprintf(fp2, "%3d\t%04X %-50s %06X\n",line_cnt,loc_save[loc_cnt++],str,obj_code);
						if(op_tmp + 3 > 30 || op_tmp == 0){
							fprintf(fp3,"\nT%06X%02X",loc_save[loc_cnt-1],op_length[op_cnt++]);
							op_tmp=0;
						}
						fprintf(fp3,"%06X",obj_code);
						op_tmp += 3;
					}
					else if((numtmp - base_num) >= 0 && (numtmp - base_num) <= 4095 && baseflag == 1){
						obj_code = format3(Opnum_Return(tmp2),0x14,numtmp-base_num);
						fprintf(fp2, "%3d\t%04X %-50s %06X\n",line_cnt,loc_save[loc_cnt++],str,obj_code);
						if(op_tmp + 3 > 30 || op_tmp == 0){
							fprintf(fp3,"\nT%06X%02X",loc_save[loc_cnt-1],op_length[op_cnt++]);
							op_tmp=0;
						}
						fprintf(fp3,"%06X",obj_code);
						op_tmp += 3;
					}
					else{
						printf("Error! Line %d: Invalid operand!\n",line_cnt);
						error_flag = 1;
						continue;
					}
				}
				else{
					sscanf(tmp3+1," %d %c",&numtmp,&syn_check);
					if(numtmp != -1 && syn_check == '\0'){
						if(numtmp >= -2048 && numtmp <= 4095){
							obj_code = format3(Opnum_Return(tmp2),0x10,numtmp);
							fprintf(fp2,"%3d\t%04X %-50s %06X\n",line_cnt,loc_save[loc_cnt++],str,obj_code);
							if(op_tmp + 3 > 30 || op_tmp == 0){
								fprintf(fp3,"\nT%06X%02X",loc_save[loc_cnt-1],op_length[op_cnt++]);
								op_tmp=0;
							}
							fprintf(fp3,"%06X",obj_code);
							op_tmp += 3;
						}
						else{
							printf("Error! Line %d: Invalid operand!\n",line_cnt);
							error_flag = 1;
							continue;
						}
					}
					else{
						printf("Error! Line %d: Invalid operand!\n",line_cnt);
						error_flag = 1;
						continue;
					}
				}

			}
			/* Operand 앞에 @가 붙어
			 * Indirect Addressing일 경우. */
			else if(tmp3[0] == '@'){
				if(Is_Used_Sym(tmp3+1) != -1){
					numtmp = Is_Used_Sym(tmp3+1);
					if((numtmp - pc_num) >= -2048 && (numtmp - pc_num) <= 2047){
						obj_code = format3(Opnum_Return(tmp2),0x22,numtmp-pc_num);
						fprintf(fp2, "%3d\t%04X %-50s %06X\n",line_cnt,loc_save[loc_cnt++],str,obj_code);
						if(op_tmp + 3 > 30 || op_tmp == 0){
							fprintf(fp3,"\nT%06X%02X",loc_save[loc_cnt-1],op_length[op_cnt++]);
							op_tmp=0;
						}
						fprintf(fp3,"%06X",obj_code);
						op_tmp += 3;
					}
					else if((numtmp - base_num) >= 0 && (numtmp - base_num) <= 4095 && baseflag == 1){
						obj_code = format3(Opnum_Return(tmp2),0x24,numtmp-base_num);
						fprintf(fp2, "%3d\t%04X %-50s %06X\n",line_cnt,loc_save[loc_cnt++],str,obj_code);
						if(op_tmp + 3 > 30 || op_tmp == 0){
							fprintf(fp3,"\nT%06X%02X",loc_save[loc_cnt-1],op_length[op_cnt++]);
							op_tmp=0;
						}
						fprintf(fp3,"%06X",obj_code);
						op_tmp += 3;
					}
					else{
						printf("Error! Line %d: Invalid operand!\n",line_cnt);
						error_flag = 1;
						continue;
					}
				}
				else{
					printf("Error! Line %d: Invalid operand!\n",line_cnt);
					error_flag = 1;
					continue;
				}
			}

			/* Operand가 Symbol Table에
			 * 존재하는 Symbol일 경우. */
			else if(Is_Used_Sym(tmp3) != -1){
				/*Operand에 Symbol Table에 존재하는 Label 입력*/
				if(Type_Return(tmp2) == 3){
					numtmp = Is_Used_Sym(tmp3);
					if((numtmp - pc_num) >= -2048 && (numtmp - pc_num) <= 2047){
						obj_code = format3(Opnum_Return(tmp2),0x32,numtmp-pc_num);
						fprintf(fp2, "%3d\t%04X %-50s %06X\n",line_cnt,loc_save[loc_cnt++],str,obj_code);
						if(op_tmp + 3 > 30 || op_tmp == 0){
							fprintf(fp3,"\nT%06X%02X",loc_save[loc_cnt-1],op_length[op_cnt++]);
							op_tmp=0;
						}
						fprintf(fp3,"%06X",obj_code);
						op_tmp += 3;
					}
					else if((numtmp - base_num) >= 0 && (numtmp - base_num) <= 4095 && baseflag == 1){
						obj_code = format3(Opnum_Return(tmp2),0x34,numtmp-base_num);
						fprintf(fp2, "%3d\t%04X %-50s %06X\n",line_cnt,loc_save[loc_cnt++],str,obj_code);
						if(op_tmp + 3 > 30 || op_tmp == 0){
							fprintf(fp3,"\nT%06X%02X",loc_save[loc_cnt-1],op_length[op_cnt++]);
							op_tmp=0;
						}
						fprintf(fp3,"%06X",obj_code);
						op_tmp += 3;
					}
					else{
						printf("Error! Line %d: Invalid operand!\n",line_cnt);
						error_flag = 1;
						continue;
					}
				}
				else{
					printf("Error! Line %d: Invalid operand!\n",line_cnt);
					error_flag = 1;
					continue;
				}
			}
			/* Operand 꼴이 다른 opcode의 경우*/
			else{
				/*레지스터 입력 또는 BUFFER,X의 형태*/
				if(Is_Reg(tmp3) != -1){
					if(Type_Return(tmp2) == 2){
						obj_code = format2(Opnum_Return(tmp2),Is_Reg(tmp3),0);
						fprintf(fp2,"%3d\t%04X %-50s %04X\n",line_cnt,loc_save[loc_cnt++],str,obj_code);
						if(op_tmp + 2 > 30 || op_tmp == 0){
							fprintf(fp3,"\nT%06X%02X",loc_save[loc_cnt-1],op_length[op_cnt++]);
							op_tmp=0;
						}
						fprintf(fp3,"%04X",obj_code);
						op_tmp += 2;	
					}
					else{
						printf("Error! Line %d: Invalid operand!\n",line_cnt);
						error_flag = 1;
						continue;
					}
				}
				/* Operand가 비어있는 경우. */
				else if(tmp3[0] == '\0'){
					if(Type_Return(tmp2) == 1){
						obj_code = format1(Opnum_Return(tmp2));
						fprintf(fp2,"%3d\t%04X %-50s %02X\n",line_cnt,loc_save[loc_cnt++],str,obj_code);
						if(op_tmp + 1 > 30 || op_tmp == 0){
							fprintf(fp3,"\nT%06X%02X",loc_save[loc_cnt-1],op_length[op_cnt++]);
							op_tmp=0;
						}
						fprintf(fp3,"%02X",obj_code);
						op_tmp += 1;
					}
					else if(Type_Return(tmp2) == 2){
						obj_code = format2(Opnum_Return(tmp2),0,0);
						fprintf(fp2,"%3d\t%04X %-50s %04X\n",line_cnt,loc_save[loc_cnt++],str,obj_code);
						if(op_tmp + 2 > 30 || op_tmp == 0){
							fprintf(fp3,"\nT%06X%02X",loc_save[loc_cnt-1],op_length[op_cnt++]);
							op_tmp=0;
						}
						fprintf(fp3,"%04X",obj_code);
						op_tmp += 2;
					}
					else if(Type_Return(tmp2) == 3){
						obj_code = format3(Opnum_Return(tmp2),0x30,0);
						fprintf(fp2,"%3d\t%04X %-50s %06X\n",line_cnt,loc_save[loc_cnt++],str,obj_code);
						if(op_tmp + 3 > 30 || op_tmp == 0){
							fprintf(fp3,"\nT%06X%02X",loc_save[loc_cnt-1],op_length[op_cnt++]);
							op_tmp=0;
						}
						fprintf(fp3,"%06X",obj_code);
						op_tmp += 3;
					}
					else{
						printf("Error! Line %d: Invalid operand!\n",line_cnt);
						error_flag = 1;
						continue;
					}
				}

				/* Symbol, X꼴 또는 register, register꼴. */
				else{
					strcpy(str_tmp,tmp3);
					strcat(str_tmp,tmp4);
					strcat(str_tmp,tmp5);
					strcpy(str_tmp2,str_tmp);
					lentmp = (int)strlen(str_tmp);

					regtmp1 = strtok(str_tmp2,",");
					regtmp2 = strtok(NULL,",");

					if(Type_Return(tmp2) == 2 && Is_Reg(regtmp1) != - 1 && Is_Reg(regtmp2) != -1){
						obj_code = format2(Opnum_Return(tmp2),Is_Reg(regtmp1),Is_Reg(regtmp2));
						fprintf(fp2, "%3d\t%-4X %-50s %04X\n",line_cnt,loc_save[loc_cnt++],str,obj_code);
						if(op_tmp + 2 > 30 || op_tmp == 0){
							fprintf(fp3,"\nT%04X%02X",loc_save[loc_cnt-1],op_length[op_cnt++]);
							op_tmp=0;
						}
						fprintf(fp3,"%06X",obj_code);
						op_tmp += 2;
					}
					else if(str_tmp[lentmp-1] == 'X' && str_tmp[lentmp-2] == ','){
						file_lst = strtok(str_tmp,",");
						numtmp = Is_Used_Sym(file_lst);
						if(numtmp != -1 && Type_Return(tmp2) == 3){
							if((numtmp - pc_num) >= -2048 && (numtmp - pc_num) <= 2047){
								obj_code = format3(Opnum_Return(tmp2),0x3A,numtmp-pc_num);
								fprintf(fp2, "%3d\t%04X %-50s %06X\n",line_cnt,loc_save[loc_cnt++],str,obj_code);
								if(op_tmp + 3 > 30 || op_tmp == 0){
									fprintf(fp3,"\nT%06X%02X",loc_save[loc_cnt-1],op_length[op_cnt++]);
									op_tmp=0;
								}
								fprintf(fp3,"%06X",obj_code);
								op_tmp += 3;
							}
							else if((numtmp - base_num) >= 0 && (numtmp - base_num) <= 4095 && baseflag == 1){
								obj_code = format3(Opnum_Return(tmp2),0x3C,numtmp-base_num);
								fprintf(fp2, "%3d\t%04X %-50s %06X\n",line_cnt,loc_save[loc_cnt++],str,obj_code);
								if(op_tmp + 3 > 30 || op_tmp == 0){
									fprintf(fp3,"\nT%06X%02X",loc_save[loc_cnt-1],op_length[op_cnt++]);
									op_tmp=0;
								}
								fprintf(fp3,"%06X",obj_code);
								op_tmp += 3;
							}
							else{
								printf("Error! Line %d: Invalid operand!\n",line_cnt);
								error_flag = 1;
								continue;
							}
						}
						else{
							printf("Error! Line %d: Invalid operand!\n",line_cnt);
							error_flag = 1;
							continue;
						}

					}
					/*Type2 A,B 형식*/
					else{
						printf("Error! Line %d: Invalid operand!\n",line_cnt);
						error_flag = 1;
						continue;
					}
				}
			}
		}

		/* 앞에 label 있고 두번째께 (+)opcode 형태로 존재할경우 
		 * 각각 앞에 #, @, Symbol이 operand 위치에 올 경우. */
		else if(Is_Op(tmp2) == 2){
			if(tmp3[0] == '#'){
				numtmp = Is_Used_Sym(tmp3+1);
				if(numtmp != -1){
					obj_code = format4(Opnum_Return(tmp2+1), 0x11, numtmp);
					mod_list[mod_cnt++] = loc_save[loc_cnt] - loc_save[0]+1;
					fprintf(fp2, "%3d\t%04X %-50s %08X\n",line_cnt,loc_save[loc_cnt++],str,obj_code);
					if(op_tmp + 4 > 30 || op_tmp == 0){
						fprintf(fp3,"\nT%06X%02X",loc_save[loc_cnt-1],op_length[op_cnt++]);
						op_tmp=0;
					}
					fprintf(fp3,"%08X",obj_code);
					op_tmp += 4;
				}
				else{
					sscanf(tmp3+1,"%d %c",&numtmp,&syn_check);
					if(numtmp != -1 && syn_check == '\0'){
						if(numtmp >= 0 && numtmp <= 0xFFFFF){ 
							obj_code = format4(Opnum_Return(tmp2+1), 0x11, numtmp);
							fprintf(fp2, "%3d\t%04X %-50s %08X\n",line_cnt,loc_save[loc_cnt++],str,obj_code);
							if(op_tmp + 4 > 30 || op_tmp == 0){
								fprintf(fp3,"\nT%06X%02X",loc_save[loc_cnt-1],op_length[op_cnt++]);
								op_tmp=0;
							}
							fprintf(fp3,"%08X",obj_code);
							op_tmp += 4;
						}
						else{
							printf("Error! Line %d: address range check!\n",line_cnt);
							error_flag = 1;
							continue;
						}
					}
					else{
						printf("Error! Line %d: Invalid operand!\n",line_cnt);
						error_flag = 1;
						continue;
					}
				}
			}
			else if(tmp3[0] == '@'){
				numtmp = Is_Used_Sym(tmp3+1);
				if(numtmp != -1){
					obj_code = format4(Opnum_Return(tmp2+1),0x21,numtmp);
					mod_list[mod_cnt++] = pc_num - loc_save[0]+1;
					fprintf(fp2, "%3d\t%04X %-50s %08X\n",line_cnt,loc_save[loc_cnt++],str,obj_code);
					if(op_tmp + 4 > 30 || op_tmp == 0){
						fprintf(fp3,"\nT%06X%02X",loc_save[loc_cnt-1],op_length[op_cnt++]);
						op_tmp=0;
					}
					fprintf(fp3,"%08X",obj_code);
					op_tmp += 4;
				}
				else{
					printf("Error! Line %d: address range check!\n",line_cnt);
					error_flag = 1;
					continue;
				}
			}
			else if(Is_Used_Sym(tmp3) != -1){
				numtmp = Is_Used_Sym(tmp3);
				if(numtmp != -1){
					obj_code = format4(Opnum_Return(tmp2+1),0x31,numtmp);
					mod_list[mod_cnt++] = loc_save[loc_cnt] - loc_save[0]+1;
					fprintf(fp2, "%3d\t%04X %-50s %08X\n",line_cnt,loc_save[loc_cnt++],str,obj_code);
					if(op_tmp + 4 > 30 || op_tmp == 0){
						fprintf(fp3,"\nT%06X%02X",loc_save[loc_cnt-1],op_length[op_cnt++]);
						op_tmp=0;
					}
					fprintf(fp3,"%08X",obj_code);
					op_tmp += 4;
				}
				else{
					printf("Error! Line %d: Invalid operand!\n",line_cnt);
					error_flag = 1;
					continue;
				}
			}	

			/* Symbol, X의 꼴. */
			else{
				strcpy(str_tmp,tmp3);
				strcat(str_tmp,tmp4);
				strcat(str_tmp,tmp5);
				lentmp = (int)strlen(str_tmp);

				if(str_tmp[lentmp-1] == 'X' && str_tmp[lentmp-2] == ','){
					file_lst = strtok(str_tmp,",");
					numtmp = Is_Used_Sym(file_lst);
					if(numtmp != -1 && Type_Return(tmp2+1) == 3){
						obj_code = format4(Opnum_Return(tmp2+1),0x39,numtmp);
						fprintf(fp2, "%3d\t%04X %-50s %08X\n",line_cnt,loc_save[loc_cnt++],str,obj_code);
						if(op_tmp + 4 > 30 || op_tmp == 0){
							fprintf(fp3,"\nT%06X%02X",loc_save[loc_cnt-1],op_length[op_cnt++]);
							op_tmp=0;
						}
						fprintf(fp3,"%08X",obj_code);
						op_tmp += 4;

					}
					else{
						printf("Error! Line %d: Invalid operand!\n",line_cnt);
						error_flag = 1;
						continue;
					}
				}
				else{
					printf("Error! Line %d: Invalid operand!\n",line_cnt);
					error_flag = 1;
					continue;
				}
			}
		}
		/* Byte 꼴. */
		else if(strcmp(tmp2,"BYTE") == 0){
			if(tmp3[0] == 'X'){
				numtmp = Byte_Check(tmp3);
			}
			else if(tmp3[0] == 'C'){
				numtmp = Check_C(buffer);
			}
			else{
				printf("Error! Line %d: Invalid byte syntax!\n",line_cnt);
				error_flag = 1;
				continue;
			}
			if(numtmp != -1){
				fprintf(fp2,"%3d\t%04X %-50s ",line_cnt,loc_save[loc_cnt++],buffer);
				if(tmp3[0] == 'C'){
					sscanf(buffer,"%s %s %[^'] %*c %[^']",tmp1,tmp2,tmp3,byte_tmp1);
					for(i=0; i<numtmp; i++){
						fprintf(fp2,"%2X",byte_tmp1[i]);
					}
					fprintf(fp2,"\n");
					if(op_tmp + numtmp > 30 || op_tmp == 0){
						fprintf(fp3,"\nT%06X%02X",loc_save[loc_cnt-1],op_length[op_cnt++]);
						op_tmp=0;
					}
					for(i=0; i<numtmp; i++){
						fprintf(fp3,"%2X",byte_tmp1[i]);
					}
					op_tmp += numtmp;
				}
				else{
					for(i=2; i<numtmp*2+2; i++){
						fprintf(fp2,"%c",tmp3[i]);
					}
					fprintf(fp2,"\n");
					if(op_tmp + numtmp > 30 || op_tmp == 0){
						fprintf(fp3,"\nT%06X%02X",loc_save[loc_cnt-1],op_length[op_cnt++]);
						op_tmp=0;
					}
					for(i=2; i<numtmp*2+2; i++){
						fprintf(fp3,"%c",tmp3[i]);
					}
					op_tmp += numtmp*2;

				}
			}
			else{
				printf("Error! Line %d: Byte format check!\n",line_cnt);
				error_flag = 1;
				continue;
			}
		}

		else if(strcmp(tmp2, "RESW") == 0){
			fprintf(fp2,"%3d\t%04X %-50s\n",line_cnt,loc_save[loc_cnt++],str);
			if(op_tmp != 0){
				op_tmp=0;
			}
		}

		else if(strcmp(tmp2, "RESB") == 0){
			fprintf(fp2,"%3d\t%04X %-50s\n",line_cnt,loc_save[loc_cnt++],str);
			if(op_tmp != 0){
				op_tmp=0;
			}
		}

		else if(strcmp(tmp2, "WORD") == 0){
			if(tmp3[0] <= '9' && tmp3[0] >= '0'){
				if(Is_Dec(tmp3) == -1){
					printf("Error! Line %d: Word format check!\n",line_cnt);
				}
				sscanf(tmp3,"%d %c",&numtmp,&syn_check);
				if(numtmp != -1 && syn_check == '\0'){
					numtmp = atoi(tmp3);
					if(numtmp >= 0x000000 && numtmp <= 0xFFFFFF){
						fprintf(fp2,"%3d\t%04X %-50s %06X\n",line_cnt,loc_save[loc_cnt++],str,numtmp);
						if(op_tmp + 3 > 30 || op_tmp == 0){
							fprintf(fp3,"\nT%06X%02X",loc_save[loc_cnt-1],op_length[op_cnt++]);
							op_tmp=0;
						}
						fprintf(fp3,"%06X",numtmp);
						op_tmp += 3;
					}
					else{
						printf("Error! Line %d: Word format check!\n",line_cnt);
						error_flag = 1;
						continue;
					}
				}
				else{
					printf("Error! Line %d: Word format check!\n",line_cnt);
					error_flag = 1;
					continue;
				}
			}
			else{		
				printf("Error! Line %d: Word format check!\n",line_cnt);
				error_flag = 1;
				continue;
			}
		}
		/* 여기서부터는 LABEL 없는 경우. 
		 * Operand 위치에 순서대로 #, @
		 * 들어올 경우. */
		else{
			if(Is_Op(tmp1) == 1){
				if(tmp2[0] == '#'){
					if(Is_Used_Sym(tmp2+1) != -1){
						numtmp = Is_Used_Sym(tmp2+1);

						/*Inside the range of PC addressing mode. */
						if((numtmp - pc_num) >= -2048 && (numtmp - pc_num) <= 2047){
							obj_code = format3(Opnum_Return(tmp1),0x12,numtmp-pc_num);
							fprintf(fp2, "%3d\t%04X %-50s %06X\n",line_cnt,loc_save[loc_cnt++],str,obj_code);
							if(op_tmp + 3 > 30 || op_tmp == 0){
								fprintf(fp3,"\nT%06X%02X",loc_save[loc_cnt-1],op_length[op_cnt++]);
								op_tmp=0;
							}
							fprintf(fp3,"%06X",obj_code);
							op_tmp += 3;
						}
						else if((numtmp - base_num) >= 0 && (numtmp - base_num) <= 4095 && baseflag == 1){
							obj_code = format3(Opnum_Return(tmp1),0x14,numtmp-base_num);
							fprintf(fp2, "%3d\t%04X %-50s %06X\n",line_cnt,loc_save[loc_cnt++],str,obj_code);
							if(op_tmp + 3 > 30 || op_tmp == 0){
								fprintf(fp3,"\nT%06X%02X",loc_save[loc_cnt-1],op_length[op_cnt++]);
								op_tmp=0;
							}
							fprintf(fp3,"%06X",obj_code);
							op_tmp += 3;
						}
						else{
							printf("Error! Line %d: Invalid operand!\n",line_cnt);
							error_flag = 1;
							continue;
						}
					}
					else{
						sscanf(tmp2+1," %d %c",&numtmp,&syn_check);
						if(numtmp != -1 && syn_check == '\0'){
							if(numtmp >= -2048 && numtmp <= 4095){
								obj_code = format3(Opnum_Return(tmp1),0x10,numtmp);
								fprintf(fp2,"%3d\t%04X %-50s %06X\n",line_cnt,loc_save[loc_cnt++],str,obj_code);
								if(op_tmp + 3 > 30 || op_tmp == 0){
									fprintf(fp3,"\nT%06X%02X",loc_save[loc_cnt-1],op_length[op_cnt++]);
									op_tmp=0;
								}
								fprintf(fp3,"%06X",obj_code);
								op_tmp += 3;
							}
							else{
								printf("Error! Line %d: Invalid operand!\n",line_cnt);
								error_flag = 1;
								continue;
							}
						}
						else{
							printf("Error! Line %d: Invalid operand!\n",line_cnt);
							error_flag = 1;
							continue;
						}
					}
				}
				else if(tmp2[0] == '@'){
					if(Is_Used_Sym(tmp2+1) != -1){
						numtmp = Is_Used_Sym(tmp2+1);
						//pc_num += 3;
						if((numtmp - pc_num) >= -2048 && (numtmp - pc_num) <= 2047){
							obj_code = format3(Opnum_Return(tmp1),0x22,numtmp-pc_num);
							fprintf(fp2, "%3d\t%04X %-50s %06X\n",line_cnt,loc_save[loc_cnt++],str,obj_code);
							if(op_tmp + 3 > 30 || op_tmp == 0){
								fprintf(fp3,"\nT%06X%02X",loc_save[loc_cnt-1],op_length[op_cnt++]);
								op_tmp=0;
							}
							fprintf(fp3,"%06X",obj_code);
							op_tmp += 3;
						}
						else if((numtmp - base_num) >= 0 && (numtmp - base_num) <= 4095 && baseflag == 1){
							obj_code = format3(Opnum_Return(tmp1),0x24,numtmp-base_num);
							fprintf(fp2, "%3d\t%04X %-50s %06X\n",line_cnt,loc_save[loc_cnt++],str,obj_code);
							if(op_tmp + 3 > 30 || op_tmp == 0){
								fprintf(fp3,"\nT%06X%02X",loc_save[loc_cnt-1],op_length[op_cnt++]);
								op_tmp=0;
							}
							fprintf(fp3,"%06X",obj_code);
							op_tmp += 3;
						}
						else{
							printf("Error! Line %d: Invalid operand!\n",line_cnt);
							error_flag = 1;
							continue;
						}
					}
					else{
						printf("Error! Line %d: Invalid operand!\n",line_cnt);
						error_flag = 1;
						continue;
					}
				}

				/* Operand 없는 opcode의 경우*/
				else if(tmp2[0] == '\0'){
					if(Type_Return(tmp1) == 1){
						obj_code = format1(Opnum_Return(tmp1));
						fprintf(fp2,"%3d\t%04X %-50s %02X\n",line_cnt,loc_save[loc_cnt++],str,obj_code);
						if(op_tmp + 1 > 30 || op_tmp == 0){
							fprintf(fp3,"\nT%06X%02X",loc_save[loc_cnt-1],op_length[op_cnt++]);
							op_tmp=0;
						}
						fprintf(fp3,"%02X",obj_code);
						op_tmp += 1;
					}
					else if(Type_Return(tmp1) == 2){
						obj_code = format2(Opnum_Return(tmp1),0,0);
						fprintf(fp2,"%3d\t%04X %-50s %04X\n",line_cnt,loc_save[loc_cnt++],str,obj_code);
						if(op_tmp + 2 > 30 || op_tmp == 0){
							fprintf(fp3,"\nT%06X%02X",loc_save[loc_cnt-1],op_length[op_cnt++]);
							op_tmp=0;
						}
						fprintf(fp3,"%04X",obj_code);
						op_tmp += 2;
					}
					else if(Type_Return(tmp1) == 3){
						obj_code = format3(Opnum_Return(tmp1),0x30,0);
						fprintf(fp2,"%3d\t%04X %-50s %06X\n",line_cnt,loc_save[loc_cnt++],str,obj_code);
						if(op_tmp + 3 > 30 || op_tmp == 0){
							fprintf(fp3,"\nT%06X%02X",loc_save[loc_cnt-1],op_length[op_cnt++]);
							op_tmp=0;
						}
						fprintf(fp3,"%06X",obj_code);
						op_tmp += 3;
					}
					else{
						printf("Error! Line %d: Invalid operand!\n",line_cnt);
						error_flag = 1;
						continue;
					}
				}
				else if(Is_Used_Sym(tmp2) != -1 && tmp3[0] == '\0'){
					/*Operand에 Symbol Table에 존재하는 Label 입력*/
					if(Type_Return(tmp1) == 3){
						numtmp = Is_Used_Sym(tmp2);
						if((numtmp - pc_num) >= -2048 && (numtmp - pc_num) <= 2047){
							obj_code = format3(Opnum_Return(tmp1),0x32,numtmp-pc_num);
							fprintf(fp2, "%3d\t%04X %-50s %06X\n",line_cnt,loc_save[loc_cnt++],str,obj_code);
							if(op_tmp + 3 > 30 || op_tmp == 0){
								fprintf(fp3,"\nT%06X%02X",loc_save[loc_cnt-1],op_length[op_cnt++]);
								op_tmp=0;
							}
							fprintf(fp3,"%06X",obj_code);
							op_tmp += 3;
						}
						else if((numtmp - base_num) >= 0 && (numtmp - base_num) <= 4095 && baseflag == 1){
							obj_code = format3(Opnum_Return(tmp1),0x34,numtmp-base_num);
							fprintf(fp2, "%3d\t%04X %-50s %06X\n",line_cnt,loc_save[loc_cnt++],str,obj_code);
							if(op_tmp + 3 > 30 || op_tmp == 0){
								fprintf(fp3,"\nT%06X%02X",loc_save[loc_cnt-1],op_length[op_cnt++]);
								op_tmp=0;
							}
							fprintf(fp3,"%06X",obj_code);
							op_tmp += 3;
						}
						else{
							printf("Error! Line %d: Invalid operand!\n",line_cnt);
							error_flag = 1;
							continue;
						}
					}
					else{
						printf("Error! Line %d: Invalid operand!\n",line_cnt);
						error_flag = 1;
						continue;
					}
				}	
				else{
					/*레지스터 입력 또는 BUFFER,X의 형태*/
					if(Is_Reg(tmp2) != -1){
						if(Type_Return(tmp1) == 2){
							obj_code = format2(Opnum_Return(tmp1),Is_Reg(tmp2),0);
							fprintf(fp2,"%3d\t%04X %-50s %04X\n",line_cnt,loc_save[loc_cnt++],str,obj_code);
							if(op_tmp + 2 > 30 || op_tmp == 0){
								fprintf(fp3,"\nT%06X%02X",loc_save[loc_cnt-1],op_length[op_cnt++]);
								op_tmp=0;
							}
							fprintf(fp3,"%04X",obj_code);
							op_tmp += 2;	
						}
						else{
							printf("Error! Line %d: Invalid operand!\n",line_cnt);
							error_flag = 1;
							continue;
						}

					}
					/*Symbol, X의 꼴*/
					else{
						strcpy(str_tmp,tmp2);
						strcat(str_tmp,tmp3);
						strcat(str_tmp,tmp4);
						strcpy(str_tmp2,str_tmp);
						lentmp = (int)strlen(str_tmp);

						regtmp1 = strtok(str_tmp2,",");
						regtmp2 = strtok(NULL,",");
						if(Type_Return(tmp1) == 2 && Is_Reg(regtmp1) != - 1 && Is_Reg(regtmp2) != -1){
							obj_code = format2(Opnum_Return(tmp1),Is_Reg(regtmp1),Is_Reg(regtmp2));
							fprintf(fp2, "%3d\t%-4X %-50s %04X\n",line_cnt,loc_save[loc_cnt++],str,obj_code);
							if(op_tmp + 2 > 30 || op_tmp == 0){
								fprintf(fp3,"\nT%04X%02X",loc_save[loc_cnt-1],op_length[op_cnt++]);
								op_tmp=0;
							}
							fprintf(fp3,"%06X",obj_code);
							op_tmp += 2;
						}
						else if(str_tmp[lentmp-1] == 'X' && str_tmp[lentmp-2] == ','){
							file_lst = strtok(str_tmp,",");
							numtmp = Is_Used_Sym(file_lst);
							if(numtmp != -1 && Type_Return(tmp1) == 3){
								if((numtmp - pc_num) >= -2048 && (numtmp - pc_num) <= 2047){
									obj_code = format3(Opnum_Return(tmp1),0x3A,numtmp-pc_num);
									fprintf(fp2, "%3d\t%04X %-50s %06X\n",line_cnt,loc_save[loc_cnt++],str,obj_code);
									if(op_tmp + 3 > 30 || op_tmp == 0){
										fprintf(fp3,"\nT%06X%02X",loc_save[loc_cnt-1],op_length[op_cnt++]);
										op_tmp=0;
									}
									fprintf(fp3,"%06X",obj_code);
									op_tmp += 3;
								}
								else if((numtmp - base_num) >= 0 && (numtmp - base_num) <= 4095 && baseflag == 1){
									obj_code = format3(Opnum_Return(tmp1),0x3C,numtmp-base_num);
									fprintf(fp2, "%3d\t%04X %-50s %06X\n",line_cnt,loc_save[loc_cnt++],str,obj_code);
									if(op_tmp + 3 > 30 || op_tmp == 0){
										fprintf(fp3,"\nT%06X%02X",loc_save[loc_cnt-1],op_length[op_cnt++]);
										op_tmp=0;
									}
									fprintf(fp3,"%06X",obj_code);
									op_tmp += 3;
								}
								else{
									printf("Error! Line %d: Invalid operand!\n",line_cnt);
									error_flag = 1;
									continue;
								}
							}
							else{
								printf("Error! Line %d: Invalid operand!\n",line_cnt);
								error_flag = 1;
								continue;

							}
						}
						else{
							printf("Error! Line %d: Invalid operand!\n",line_cnt);
							error_flag = 1;
							continue;
						}
					}
				}

			}

			/* (+)opcode의 형태의 입력.
			 * 순서대로 #, @, Symbol 자체
			 * 의 입력이 들어올 경우.*/
			else if(Is_Op(tmp1) == 2){
				if(tmp2[0] == '#'){
					numtmp = Is_Used_Sym(tmp2+1);
					if(numtmp != -1){
						obj_code = format4(Opnum_Return(tmp1+1), 0x11, numtmp);
						mod_list[mod_cnt++] = loc_save[loc_cnt] - loc_save[0] + 1;
						fprintf(fp2, "%3d\t%04X %-50s %08X\n",line_cnt,loc_save[loc_cnt++],str,obj_code);
						if(op_tmp + 4 > 30 || op_tmp == 0){
							fprintf(fp3,"\nT%06X%02X",loc_save[loc_cnt-1],op_length[op_cnt++]);
							op_tmp=0;
						}
						fprintf(fp3,"%08X",obj_code);
						op_tmp += 4;
					}
					else{
						sscanf(tmp2+1,"%d %c",&numtmp,&syn_check);
						if(numtmp != -1 && syn_check == '\0'){
							if(numtmp >= 0 && numtmp <= 0xFFFFF){ 
								obj_code = format4(Opnum_Return(tmp1+1), 0x11, numtmp);
								fprintf(fp2, "%3d\t%04X %-50s %08X\n",line_cnt,loc_save[loc_cnt++],str,obj_code);
								if(op_tmp + 4 > 30 || op_tmp == 0){
									fprintf(fp3,"\nT%06X%02X",loc_save[loc_cnt-1],op_length[op_cnt++]);
									op_tmp=0;
								}
								fprintf(fp3,"%08X",obj_code);
								op_tmp += 4;
							}
							else{
								printf("Error! Line %d: address range check!\n",line_cnt);
								error_flag = 1;
								continue;
							}
						}
						else{
							printf("Error! Line %d: Invalid operand!\n",line_cnt);
							error_flag = 1;
							continue;
						}
					}
				}
				else if(tmp2[0] == '@'){
					numtmp = Is_Used_Sym(tmp3+1);
					if(numtmp != -1){
						obj_code = format4(Opnum_Return(tmp1+1),0x21,numtmp);
						mod_list[mod_cnt++] = loc_save[loc_cnt] - loc_save[0] + 1;
						fprintf(fp2, "%3d\t%04X %-50s %08X\n",line_cnt,loc_save[loc_cnt++],str,obj_code);
						if(op_tmp + 4 > 30 || op_tmp == 0){
							fprintf(fp3,"\nT%06X%02X",loc_save[loc_cnt-1],op_length[op_cnt++]);
							op_tmp=0;
						}
						fprintf(fp3,"%08X",obj_code);
						op_tmp += 4;
					}
					else{
						printf("Error! Line %d: address range check!\n",line_cnt);
						error_flag = 1;
						continue;
					}
				}
				else if(Is_Used_Sym(tmp2) != -1){
					numtmp = Is_Used_Sym(tmp2);
					if(numtmp != -1){
						obj_code = format4(Opnum_Return(tmp1+1),0x31,numtmp);
						mod_list[mod_cnt++] = loc_save[loc_cnt] - loc_save[0] + 1;
						fprintf(fp2, "%3d\t%04X %-50s %08X\n",line_cnt,loc_save[loc_cnt++],str,obj_code);
						if(op_tmp + 4 > 30 || op_tmp == 0){
							fprintf(fp3,"\nT%06X%02X",loc_save[loc_cnt-1],op_length[op_cnt++]);
							op_tmp=0;
						}
						fprintf(fp3,"%08X",obj_code);
						op_tmp += 4;
					}
					else{
						printf("Error! Line %d: Invalid operand!\n",line_cnt);
						error_flag = 1;
						continue;
					}
				}	

				/*Symbol, X의 꼴. */
				else{
					strcpy(str_tmp,tmp2);
					strcat(str_tmp,tmp3);
					strcat(str_tmp,tmp4);
					lentmp = (int)strlen(str_tmp);

					if(str_tmp[lentmp-1] == 'X' && str_tmp[lentmp-2] == ','){
						file_lst = strtok(str_tmp,",");
						numtmp = Is_Used_Sym(file_lst);
						if(numtmp != -1 && Type_Return(tmp1+1) == 3){
							obj_code = format4(Opnum_Return(tmp1+1),0x39,numtmp);
							fprintf(fp2, "%3d\t%04X %-50s %08X\n",line_cnt,loc_save[loc_cnt++],str,obj_code);
							if(op_tmp + 4 > 30 || op_tmp == 0){
								fprintf(fp3,"\nT%06X%02X",loc_save[loc_cnt-1],op_length[op_cnt++]);
								op_tmp=0;
							}
							fprintf(fp3,"%08X",obj_code);
							op_tmp += 4;
						}
						else{
							printf("Error! Line %d: Invalid operand!\n",line_cnt);
							error_flag = 1;
							continue;
						}
					}
					else{
						printf("Error! Line %d: Invalid operand!\n",line_cnt);
						error_flag = 1;
						continue;
					}

				}


			}

			/* Base 명령어가 들어올 경우
			 * Base flag 를 1로 set 하고
			 * 그 옆에있는 LABEL의 LOCCTR을
			 * base register로 저장. */
			else if(strcmp(tmp1,"BASE") == 0){
				baseflag=1;
				if(Is_Used_Sym(tmp2) != -1){
					base_num = Is_Used_Sym(tmp2);
					fprintf(fp2,"%3d\t%4s %-50s\n",line_cnt," ",str);
				}
				else{
					printf("Error! Line %d: Invalid operand!\n",line_cnt);
					error_flag=1;
					continue;
				}
			}
			else{
				printf("Error! Line %d: Object code doesn't exist!\n",line_cnt);
				error_flag=1;
				continue;
			}
		}
	}
	for(i=0; i<mod_cnt; i++){
		fprintf(fp3, "\nM%06X05",mod_list[i]);
	}
	fprintf(fp3,"\nE%06X\n",loc_save[0]);


	Change_Sym();
	fclose(fp);
	fclose(fp2);
	fclose(fp3);

	if(error_flag == 1){
		Free_Symtab();
		Reset_Symbol();
		strcpy(file_tmp,filename);
		file_lst = strtok(file_tmp, ".");
		strcat(file_lst,".lst");
		remove(file_lst);

		file_obj = strtok(file_lst,".");
		strcat(file_obj,".obj");
		remove(file_obj);
	}
	else{
		strcpy(file_tmp,filename);
		file_lst = strtok(file_tmp, ".");
		printf("\toutput file : [%s.lst], [%s.obj]\n",file_lst,file_lst);
	}
}

/*name에 알맞는 opcode type을 리턴함 각각 1,2, 3/4*/
int Type_Return(char name[]){
	int idx;
	Opcode_Table *tmp;
	if(name[0] == '\0')
		return 0;
	idx = opcodetable_func_make_hash_given(name);
	for(tmp = Op_Table[idx]; tmp != NULL; tmp = tmp->link){
		if(strcmp(tmp->op_mnemonic,name) == 0){
			if(strcmp(tmp->type,"1") == 0){
				return 1;
			}
			else if(strcmp(tmp->type,"2") == 0){
				return 2;
			}
			else if(strcmp(tmp->type,"3/4") == 0){
				return 3;
			}
		}
	}
	return 0;
}

/* Symbol Table을 리셋한다. */
void Reset_Symbol(void){
	int i;
	for(i=0; i<20; i++){
		Sym_Table[i] = NULL;
	}
}

/* Byte의 길이를 체크. */
int Byte_Check(char str[]){
	int len=-1,i;
	int numlen=0;
	int flag=0;
	len = (int)strlen(str);
	if(str[0] == 'C'){
		if(str[1] == '\'' && str[len-1] == '\''){
			numlen = len - 3;
			return numlen;
		}
	}
	else if(str[0] == 'X'){
		if(str[1] == '\'' && str[len-1] == '\''){
			if(len % 2 == 0){
				return -1;
			}
			for(i=2; i<len-1; i++){
				if((str[i] >= 'A' && str[i] <= 'F') || (str[i] >= '0' && str[i] <= '9')){
				}
				else{
					flag=1;
				}
			}
			if(flag == 0)
				return (len-3)/2;
			else
				return -1;
		}
		else
			return -1;
	}
	return -1;
}

/* Symbol_Table을 Decreasing Order로 새로운 배열에 저장한다. */
void Change_Sym(void){
	int i,j;
	int count=0;
	Symbol_Table *tmp;
	Symbol_Table dectmp;
	for(i=0; i<20; i++){
		if(Sym_Table[i] != NULL){
			for(tmp = Sym_Table[i]; tmp != NULL; tmp = tmp->link)
				count += 1;
		}
	}
	Dec_Table = (Symbol_Table*)malloc(sizeof(Symbol_Table) * count);
	j=0;
	for(i=0; i<20; i++){
		if(Sym_Table[i] != NULL){
			for(tmp = Sym_Table[i]; tmp != NULL; tmp = tmp->link){
				strcpy(Dec_Table[j].symbol,tmp->symbol);
				Dec_Table[j].locctr = tmp->locctr;
				j+=1;
			}
		}
	}
	for(i=0; i<count-1; i++){
		for(j=0; j<count-i-1; j++){
			if(strcmp(Dec_Table[j].symbol,Dec_Table[j+1].symbol) < 0){
				dectmp = Dec_Table[j];
				Dec_Table[j] = Dec_Table[j+1];
				Dec_Table[j+1] = dectmp;
			}
		}
	}
}
int format1(int opcode){
	return opcode;
}

/* Register에 들어온 값들을 조사하고 이를 통해 format2 형식의 objcode를 계산해준다.*/
int format2(int opcode,char reg1, char reg2){
	int sum=0;
	if(reg1 == 'A'){
		reg1 = 0;
	}
	else if(reg1 == 'X'){
		reg1 = 1;
	}
	else if(reg1 == 'L'){
		reg1 = 2;
	}
	else if(reg1 == 'B'){
		reg1 = 3;
	}
	else if(reg1 == 'S'){
		reg1 = 4;
	}
	else if(reg1 == 'T'){
		reg1 = 5;
	}
	else if(reg1 == 'F'){
		reg1 = 6;
	}

	if(reg2 == 'A'){
		reg2 = 0;
	}
	else if(reg2 == 'X'){
		reg2 = 1;
	}
	else if(reg2 == 'L'){
		reg2 = 2;
	}
	else if(reg2 == 'B'){
		reg2 = 3;
	}
	else if(reg2 == 'S'){
		reg2 = 4;
	}
	else if(reg2 == 'T'){
		reg2 = 5;
	}
	else if(reg1 == 'F'){
		reg1 = 6;
	}

	opcode <<= 8;
	sum |= opcode;
	reg1 <<= 4;
	sum |= reg1;
	sum |= reg2;

	return sum;
}

/* format3 형식의 objcode를 계산한다. */
int format3(int opcode,int nixbpe, int dis_addr){
	int sum=0;
	dis_addr &= 0xFFF;
	opcode <<= 16;
	sum |= opcode;
	nixbpe <<= 12;
	sum |= nixbpe;
	sum |= dis_addr;

	return sum;
}

/* Format4 형식의 objcode를 계산한다. */
int format4(int opcode,int nixbpe, int dis_addr){
	int sum=0;
	dis_addr &= 0xFFFFF;
	opcode <<= 24;
	sum |= opcode;
	nixbpe <<= 20;
	sum |= nixbpe;
	sum |= dis_addr;

	return sum;
}

/* Mnemonic의 Opcode를 출력해준다. */
int Opnum_Return(char name[]){
	int idx;
	Opcode_Table *tmp;
	idx = opcodetable_func_make_hash_given(name);
	for(tmp = Op_Table[idx]; tmp != NULL; tmp = tmp->link){
		if(strcmp(name,tmp->op_mnemonic) == 0)
			return tmp->op_num;
	}
	return -1;
}

/* 입력받은 문자열이 Decimal 인지 확인. */
int Is_Dec(char s[]){
	int i;
	int len = strlen(s);
	for(i=0; i<len; i++){
		if(s[i] > '9' || s[i] < '0')
			return -1;
	}
	return 1;
}

/* Byte에서 C''형식에 대한 체크를 수행한다.
 * 만약 틀린 형식일 경우 -1을 리턴한다. */
int Check_C(char s[]){
	int len;
	char tmp1[20],tmp2[20],tmp3[20],check[40];
	char e_check1,e_check2;
	sscanf(s,"%s %s %[^'] %c %[^'] %c",tmp1,tmp2,tmp3,&e_check1,check,&e_check2);
	len = (int)strlen(check);
	if(e_check1 == '\'' && e_check2 == '\''){
		return len;
	}
	else{
		return -1;
	}
}

/* 문자열이 Register인지 검사한다.
 * 아닐 경우 -1을 리턴한다. */
int Is_Reg(char s[]){
	int flag = -1;
	if(strcmp(s,"A") == 0){
		flag = 0;
	}
	else if(strcmp(s,"X") == 0){
		flag = 1;
	}
	else if(strcmp(s,"L") == 0){
		flag = 2;
	}
	else if(strcmp(s,"PC") == 0){
		flag = 8;
	}
	else if(strcmp(s,"SW") == 0){
		flag = 9;
	}
	else if(strcmp(s,"B") == 0){
		flag = 3;
	}
	else if(strcmp(s,"S") == 0){
		flag = 4;
	}
	else if(strcmp(s,"T") == 0){
		flag = 5;
	}
	else if(strcmp(s,"F") == 0){
		flag = 6;
	}
	return flag;
}

/* linking loader에서 pass1 부분을 담당하는 함수. ESTAB을 생성하고 duplicate symbol에
 * 대한 에러체크를 수행한다. ESTAB을 만들 때에는 program symbol과 label등을 구분하여 저장한다. */
void loader_pass1(char filename1[], char filename2[], char filename3[]){
	FILE *fp1, *fp2, *fp3;
	FILE *fp;

	char filename[3][30];
	char strtmp[100];
	char nametmp[10],addrtmp[10],lengthtmp[10];

	int error_flag=0;
	int file_cnt =0 ;
	int i,j,k;
	int proglen=0;					//프로그램 길이
	int startaddr=0;				//시작 주소, 나중에 Define들을 받을 때 뺴줘야함

	fp1 = fopen(filename1,"r");
	if(fp1 != NULL){
		file_cnt += 1;
		strcpy(filename[0],filename1);
	}
	fp2 = fopen(filename2,"r");
	if(fp2 != NULL){
		file_cnt += 1;
		strcpy(filename[1],filename2);
	}
	fp3 = fopen(filename3,"r");
	if(fp3 != NULL){
		file_cnt += 1;
		strcpy(filename[2],filename3);
	}
	for(k=0 ;k<file_cnt; k++){
		fp = fopen(filename[k],"r");
		/* Header 읽기*/
		while(fgets(strtmp,100,fp)){
			memset(nametmp,'\0',10);
			memset(addrtmp,'\0',10);
			memset(lengthtmp,'\0',10);

			nametmp[0] = '\0';
			addrtmp[0] = '\0';
			lengthtmp[0] = '\0';

			if(strtmp[0] == 'H'){
				for(i=1; i<7; i++){
					if(strtmp[i] == ' ')
						break;
					nametmp[i-1] = strtmp[i];
				}
				nametmp[i-1] = '\0';
				for(i=7; i<13; i++)
					addrtmp[i-7] = strtmp[i];
				for(i=13; i<19; i++)
					lengthtmp[i-13] = strtmp[i];
				if(S_to_Hex(addrtmp) == -1 || S_to_Hex(lengthtmp) == -1){
					error_flag = 1;
					printf("Error1\n");
					break;
				}
				if(Find_Estab(nametmp) != -1){
					printf("Duplicated Symbol\n");
					return ;
				}
				/*Make_Estab들어가기 전에 이미 쓰인 Symbol인지 검사 필요. */
				Make_Estab(Header,nametmp,S_to_Hex(addrtmp),S_to_Hex(lengthtmp));
				break;
			}
		}
		startaddr = S_to_Hex(addrtmp);
		proglen = S_to_Hex(lengthtmp);
		while(fgets(strtmp,100,fp)){

			memset(nametmp,'\0',10);
			memset(addrtmp,'\0',10);
			memset(lengthtmp,'\0',10);

			nametmp[0] = '\0';
			addrtmp[0] = '\0';
			lengthtmp[0] = '\0';

			i=1;
			j=0;

			/* D 레코드 읽기. */
			if(strtmp[0] == 'D'){
				while(i+11 <= strlen(strtmp)){
					for(j=0; j<6; j++,i++){
						if(strtmp[i] == ' ')
							nametmp[j] = '\0';
						else
							nametmp[j] = strtmp[i];
					}
					nametmp[j] = '\0';
					for(j=0; j<6; j++,i++){
						addrtmp[j] = strtmp[i];
					}
					/* 유효한 16진수 타입인지 에러체크. */
					if(S_to_Hex(addrtmp) == -1){
						error_flag = 1;
						printf("Error2\n");
						break;
					}
					if(Find_Estab(nametmp) != -1){
						printf("Duplicated Symbol\n");
						return ;
					}
					Make_Estab(Define,nametmp,S_to_Hex(addrtmp) - startaddr,S_to_Hex(lengthtmp));
				}
			}
			/* T레코드 부터는 더이상 pass1에서 읽을 필요가 없음. */
			else if(strtmp[0] == 'T'){
				progaddr += proglen;
				break;
			}
		}
		fclose(fp);
	}
	/* ESTAB 출력. */
	Print_Load();
	if(error_flag == 0){
		for(k=0; k<file_cnt; k++){
			loader_pass2(filename[k]);
		}
	}
}

/* linking loader의 pass2 부분에 해당하는 함수. 에러체크는
 * ESTAB에 존재하지 않는 symbol을 참조하는 경우를 체크하였다.
 * 이후 run등에도 사용될 progaddr나 proglength 등을 저장한다. */
void loader_pass2(char filename[]){
	FILE *fp;
	int csaddr;										//프로그램 시작주소
	int cslength=0;									//프로그램 길이
	int address;									//주소값
	int reference_num;								//reference number에 사용할 배열과 index
	int reference_arr[100];
	int loctmp;
	int len;

	int t_len;
	int obj_code;
	int modifi_num;									//modifi num을 저장한다.
	int immediate_num;								//bit mask 등에 사용할 변수.
	int edit_num;
	int i;
	int error_flag=0;
	int idx;

	char modifi_type;
	char nametmp[10];
	char strtmp[100];
	char *str;
	
	Estab_Table *tmp;

	csaddr = progaddr;
	fp = fopen(filename,"r");
	if(fp == NULL){
		printf("File open error!\n");
		return ;
	}
	while(fgets(strtmp,100,fp)){
		str = strtok(strtmp,"\n");
		if(str[0] == 'E'){
			sscanf(str+1,"%6x",&address);
			break;
		}
		else if(str[0] == '.'){
			continue;
		}
		else if(str[0] == 'H'){
			sscanf(str+1,"%6s",nametmp);
			loctmp = Find_Estab(nametmp);
			if(loctmp == -1){
				error_flag = 1;
				printf("Symbol doesn't exist!\n");
				break;
			}
			idx = opcodetable_func_make_hash_given(nametmp);
			for(tmp = Es_Table[idx]; tmp != NULL; tmp = tmp->link){
				if(strcmp(tmp->name,nametmp) == 0){
					cslength = tmp->length;
					break;
				}
			}
			reference_arr[1] = loctmp;
		}
		else if(str[0] == 'R'){
			if((strlen(str)-1) % 8 == 0)
				len = (strlen(str)-1) / 8;
			else
				len = ((strlen(str)-1) / 8) + 1;
			for(i=0; i<len; i++){
				sscanf(str+i*8+1,"%2x%6s",&reference_num,nametmp);
				loctmp = Find_Estab(nametmp);
				if(loctmp == -1){
					printf("Symbol doesn't exist!\n");
					error_flag = 1;
					break;
				}
				reference_arr[reference_num] = loctmp;
			}
		}
		else if(str[0] == 'T'){
			sscanf(str+1,"%6x%2x",&address,&t_len);
			for(i=0; i<t_len; i++){
				sscanf(str+9+i*2,"%2x",&obj_code);
				memory[i+csaddr+address] = obj_code;
			}
		}

		/* M타입일 경우 Format4의 05bit modify와 06bit modify를 구분해 계산한다.*/
		else if(str[0] == 'M'){
			sscanf(str+1,"%6x%2x%c%x",&address,&modifi_num,&modifi_type,&reference_num);
			if(reference_num < 1){
				printf("Undefined Reference Number!\n");
				error_flag = 1;
				break;
			}
			else{
				immediate_num = 0;
				if(modifi_num % 2 == 1){
					immediate_num += memory[csaddr+address]%16;
				}

				for(i=csaddr + address + modifi_num%2; i<csaddr+address+3; i++){
					immediate_num <<= 4;
					immediate_num += memory[i] / 0x10;
					immediate_num <<= 4;
					immediate_num += memory[i] % 0x10;
				}

				if(modifi_type == '+'){
					immediate_num += reference_arr[reference_num];
				}
				else if(modifi_type == '-'){
					immediate_num -= reference_arr[reference_num];
				}
				else{
					error_flag = 1;
					printf("Syntax Error! '+' or '-' should be used!\n");
					break;
				}

				if(modifi_num == 5){
					edit_num = 0xFFFFF;
				}
				else if(modifi_num == 6){
					edit_num = 0xFFFFFF;
				}
				else{
					error_flag = 1;
					printf("Modification number has to be 5 or 6!\n");
					break;
				}
				immediate_num &= edit_num;

				for(i=2; i>=modifi_num%2; i--){
					memory[i+csaddr+address] = immediate_num%0x100;
					immediate_num >>= 8;
				}
				if(modifi_num%2 == 1){
					memory[csaddr+address] /= 16;
					memory[csaddr+address] *= 16;
					memory[csaddr+address] += immediate_num % 0x10;
				}
			}
		}
		else{
			continue;
		}
	}
	progaddr += cslength;

	fclose(fp);
	if(error_flag == 1){
		printf("Load Fail!\n");
	}
}

/*ESTAB을 생성하는 함수. 이 때 Hash function을 사용해 symbol을 찾기 편하게 한다. */
void Make_Estab(int typetmp, char nametmp[], int addrtmp, int lentmp){
	int idx;
	Estab_Table *tmp, *new;

	new = (Estab_Table*)malloc(sizeof(Estab_Table));
	new->type = typetmp;
	strcpy(new->name,nametmp);
	new->addr = /*S_to_Hex(addrtmp) + */progaddr;
	new->startaddr = addrtmp;
	new->length = lentmp;
	new->link = NULL;
	if(new->type == Define)
		new->addr += addrtmp;

	idx = opcodetable_func_make_hash_given(nametmp);

	if(Es_Table[idx] == NULL){
		Es_Table[idx] = new;
	}
	else{
		for(tmp = Es_Table[idx]; tmp->link != NULL; tmp = tmp->link)
			;
		tmp->link = new;
	}
}

/* ESTAB에서 적절한 주소값을 찾아 이를 리턴해주는 함수. */
int Find_Estab(char name[]){
	int idx;
	Estab_Table *tmp;

	idx = opcodetable_func_make_hash_given(name);
	for(tmp = Es_Table[idx]; tmp != NULL; tmp = tmp->link){
		if(strcmp(tmp->name,name) == 0){
			return tmp->addr;
		}
	}
	return -1;
}

/* 문자열을 입력받아 적절한 format인지 확인하고, 이를 16진수로 바꿔주는 함수. */
int S_to_Hex(char strtmp[]){
	int i;
	int hex=0;
	int digit=1;
	for(i=5; i>=0; i--){
		if(strtmp[i] <= '9' && strtmp[i] >= '0'){	
			hex += (strtmp[i]-'0') * digit;
			digit *= 16;
		}
		else if(strtmp[i] <= 'F' && strtmp[i] >= 'A'){
			hex += (strtmp[i]-55) * digit;
			digit *= 16;
		}
		else
			return -1;
	}
	return hex;
}

/* Loading한 후 ESTAB을 출력해주는 함수. */
void Print_Load(void){
	int i,j;
	int count=0;
	int totallen=0;
	Estab_Table *tmp,ordered[300],forswap;
	for(i=0; i<20; i++){
		if(Es_Table[i]!=NULL){
			for(tmp = Es_Table[i]; tmp != NULL; tmp = tmp->link){
				ordered[count].type = tmp->type;
				strcpy(ordered[count].name,tmp->name);
				ordered[count].addr = tmp->addr;
				ordered[count].length = tmp->length;
				count++;
			}
		}
	}
	for(i=0; i<count-1; i++){
		for(j=0; j<count-i-1; j++){
			if(ordered[j].addr > ordered[j+1].addr){
				forswap = ordered[j];
				ordered[j] = ordered[j+1];
				ordered[j+1] = forswap;
			}
		}
	}
	printf("%-15s%-15s%-15s%-15s\n","control","symbol","address","length");
	printf("%-15s%-15s%-15s%-15s\n","section","name"," "," ");
	printf("-----------------------------------------------------------\n");
	for(i=0; i<count; i++){
		if(ordered[i].type == Header){
			printf("%-15s%-15s%04X%-11s%04X\n",ordered[i].name," ",ordered[i].addr," ",ordered[i].length);
			totallen += ordered[i].length;
		}
		else if(ordered[i].type == Define){
			printf("%-15s%-15s%04X%-11s%15s\n"," ",ordered[i].name,ordered[i].addr," "," ");
		}
		else
			printf("Print Error!\n");
	}
	printf("-----------------------------------------------------------\n");
	printf("%-30s%-15s%04X\n"," ","total length",totallen);
	progaddr -= totallen;
}

/* ESTAB을 Free해주는 함수. */
void Free_Estab(void){
	int i;
	Estab_Table *tmp;
	for(i=0; i<20 ;i++){
		while((tmp = Es_Table[i]) != NULL){
			Es_Table[i] = Es_Table[i]->link;
			free(tmp);
		}
	}
}

/* Bp를 출력해주는 함수. */
void Print_Bp(void){
	Symbol_Table *tmp;
	printf("breakpoint\n");
	printf("----------\n");
	for(tmp = bp_list; tmp != NULL; tmp = tmp->link){
		printf("%X\n",tmp->locctr);
	}
}

/* Bp를 비워주는 함수. */
void Clear_Bp(void){
	Symbol_Table *tmp;
	while((tmp = bp_list) != NULL){
		bp_list = bp_list -> link;
		free(tmp);
	}
	printf("\t[");
	printf("%c[1;32m", 27);
	printf("ok");
	printf("%c[0m", 27);
	printf("] clear all breakpoints\n");
}

/*A: 0
 *X: 1
 *L: 2
 *B: 3
 *S: 4
 *T: 5
 *F: 6
 *PC: 8
 *SW: 9
 위의 레지스터 번호를 사용한다. */

/* memory에 레지스터에 들어있는 값을 적절하게 넣어주는 함수. */
void Make_Num(int reg[], int num, int *ta){
	memory[*ta] = (reg[num] & 0x00FF0000) >> 16;
	memory[*ta+1] = (reg[num] & 0x0000FF00) >> 8;
	memory[*ta+2] = (reg[num] & 0x000000FF);
}

/* 프로그램을 break point까지 실제로 돌리는 함수. if else 문장 안에 들어있는 조건들은
 * 각각 opcode들이며, 해당 opcode에 따라 명령을 수행한다. */
void Run_Prog(void){
	//t loc;
	int op_number;
	static int reg[10]={0,};					//register들을 저장하는 배열, 각각 index가 register의 번호를 의미한다.
	int ta=0, reg1=0, reg2=0;			//각각 target address, register1, register2를 저장한다.
	int i;
	int swtmp;							//sw register에 들어갈 값을 임시로 저장하는 변수.
	if(firsttry == 0){
		for(i=0; i<10; i++){
			reg[i]=0;
		}
		reg[8] = runstartaddr;
		reg[2] = runfinaladdr;
		firsttry = 1;
	}
	while(reg[8] < runendaddr){
		op_number = memory[reg[8]] & 0xfc;
		if(op_number == 0x14){
			TA_Format3(reg,&ta);
			Make_Num(reg,2,&ta);
		}
		else if(op_number == 0x68){
			reg[3] = TA_Format3(reg,&ta);
		}
		else if(op_number == 0x48){
			TA_Format3(reg,&ta);
			reg[2] = reg[8];
			reg[8] = ta;
		}
		else if(op_number == 0x00){
			reg[0] = TA_Format3(reg,&ta);
		}
		else if(op_number == 0x28){
			swtmp = TA_Format3(reg,&ta);
			if(reg[0] > swtmp){
				reg[9] = 1;
			}
			else if(reg[0] == swtmp){
				reg[9] = 0;
			}
			else{
				reg[9] = -1;
			}
		}
		else if(op_number == 0x30){
			TA_Format3(reg,&ta);
			if(reg[9] == 0){
				reg[8] = ta;
			}
		}
		else if(op_number == 0x3c){
			TA_Format3(reg,&ta);
			reg[8] = ta;
		}
		else if(op_number == 0x0c){
			TA_Format3(reg,&ta);
			Make_Num(reg,0,&ta);
		}
		else if(op_number == 0xb4){
			TA_Format2(reg,&reg1,&reg2);
			reg[reg1]=0;
		}
		else if(op_number == 0x74){
			reg[5] = TA_Format3(reg,&ta);
		}
		else if(op_number == 0xe0){
			TA_Format3(reg,&ta);
			reg[9] = 1;
		}
		else if(op_number == 0xd8){
			TA_Format3(reg,&ta);
			reg[9] = 0;
		}
		else if(op_number == 0xa0){
			TA_Format2(reg,&reg1,&reg2);
			if(reg[reg1] > reg[reg2]){
				reg[9] = 1;
			}
			else if(reg[reg1] == reg[reg2]){
				reg[9] = 0;
			}
			else{
				reg[9] = -1;
			}
		}
		else if(op_number == 0x54){
			TA_Format3(reg,&ta);
			memory[ta] = reg[0] % 256;
		}
		else if(op_number == 0xb8){
			reg[1] += 1;
			TA_Format2(reg,&reg1,&reg2);
			if(reg[1] > reg[reg1]){
				reg[9] = 1;
			}
			else if(reg[1] == reg[reg1]){
				reg[9] = 0;
			}
			else{
				reg[9] = -1;
			}
		}
		else if(op_number == 0x38){
			TA_Format3(reg,&ta);
			if(reg[9] == -1){
				reg[8] = ta;
			}
		}
		else if(op_number == 0x10){
			TA_Format3(reg,&ta);
			Make_Num(reg,1,&ta);
		}
		else if(op_number == 0x4c){
			TA_Format3(reg,&ta);
			reg[8] = reg[2];
		}
		else if(op_number == 0x50){
			reg[0] &= 0xFFFFFF00;
			TA_Format3(reg,&ta);
			reg[0] += memory[ta];
		}
		else if(op_number == 0xdc){
			TA_Format3(reg,&ta);
			reg[9]=0;
		}
		else{
			reg[8] += 1;
		}
	}
	printf("A : %06X X : %06X\n",reg[0],reg[1]);
	printf("L : %06X PC: %06X\n",reg[2],reg[8]);
	printf("B : %06X S : %06X\n",reg[3],reg[4]);
	printf("T : %06X\n",reg[5]);
}

/* Format 3형식의 Target Address를 계산해주는 함수. */
int TA_Format3(int reg[],int *ta){
	int nixbpe,disp=0;
	nixbpe = ((int)(memory[reg[8]] & 0x03) << 4) + ((int)(memory[reg[8]+1] & 0x0F0) >> 4); 
	disp = (int)((memory[reg[8]+1] & 0x0F) << 8) + (int)(memory[reg[8]+2] & 0xFF);
	if((nixbpe & 0x01) != 0){
		disp <<= 8;
		disp |= memory[reg[8]+3];
	}
	//3형식과 4형식의 경우 구분해서 disp 만들어줌
	if(((nixbpe & 0x01) == 0) && ((disp & 0x800) != 0)){
		disp = (~disp);
		disp+=1;
		disp &= 0x07FF;
		disp *= -1;
	}
	else if(((nixbpe & 0x01) != 0) && ((disp & 0x80000) != 0)){
		disp = (~disp);
		disp+=1;
		disp &= 0x07FFFF;
		disp *= -1;
	}
	*ta = disp;
	//X Register가 1로 set된 경우.
	if((nixbpe & 0x08) != 0){
		*ta += reg[1];
	}
	//n,i 비트가 둘다 0 일경우 -> SIC
	if((nixbpe & 0x20) == 0 && (nixbpe & 0x10) == 0){
		*ta += (int)((memory[reg[8]] & 0xF0) << 8);
		reg[8] += 3;
		return (memory[*ta]<<16) + (memory[*ta+1]<<8) + (memory[*ta+2]);
	}
	else{//각각 3형식 4형식.
		if((nixbpe & 0x01) == 0){
			reg[8] = reg[8] + 3;
		}
		else{
			reg[8] = reg[8] + 4;
		}
		//PC relative와 base relative 구분.
		if((nixbpe & 0x02) != 0){
			*ta += reg[8];
		}
		else if((nixbpe & 0x04) != 0){
			*ta += reg[3];
		}
		//ni가 11 -> simple
		if(((nixbpe & 0x20) != 0) && ((nixbpe & 0x10) != 0)){
			return (memory[*ta]<<16) + (memory[*ta+1]<<8) + (memory[*ta+2]);
		}
		//ni가 01 -> immediate
		else if(((nixbpe & 0x20) == 0) && ((nixbpe & 0x10) != 0)){
			return *ta;
		}
		//ni가 10 -> indirect
		else{
			*ta = (memory[*ta]<<16) + (memory[*ta+1]<<8) + (memory[*ta+2]);
			return (memory[*ta]<<16) + (memory[*ta+1]<<8) + (memory[*ta+2]);
		}
	}
}

/* Format2의 Target Address를 계산해준다. */
int TA_Format2(int reg[],int *reg1, int *reg2){
	*reg1 = (memory[reg[8]+1] & 0xf0)>>4;
	*reg2 = (memory[reg[8]+1] & 0x0f);
	reg[8] += 2;
	return 1;
}

void Make_Bplist(int baddr){
	Symbol_Table *new, *tmp;
	
	new = (Symbol_Table*)malloc(sizeof(Symbol_Table));
	new->link = NULL;
	new->locctr = baddr;

	if(bp_list == NULL){
		bp_list = new;
	}
	else if(bp_list->link == NULL){
		if(bp_list->locctr < new->locctr){
			bp_list->link = new;
		}
		else{
			new->link = bp_list;
			bp_list = new;
		}
	}
	else{
		for(tmp = bp_list; tmp->link != NULL; tmp = tmp->link){
			if(baddr < tmp->link->locctr){
				break;
			}
		}
		new->link = tmp->link;
		tmp->link = new;
	}
}
