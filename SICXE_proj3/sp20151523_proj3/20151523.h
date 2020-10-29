#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#define max_memory 1048576			//It equals to 16 * 65536 which is the whole number of the memory.
#define Header 1
#define Define 2

/* Usage: It is the linked list which saves the information of the history. 
 * Variable num : number of valid command input. 
 *          name: name of valid command input.                             */
typedef struct linkhistory{
	int num;
	char name[100];
	struct linkhistory *link;
}Link_History;

/* Usage: It is the linked list which saves the information of the opcode and mnemonic.
 * Variable op_num      : opcode number of mnemonic.
 *          op_mnemonic : name of the mnemonic.
 *          type        : type of the mnemonic(1~4).                                   */
typedef struct opcode_table{
	int op_num;
	char op_mnemonic[20];
	char type[10];
	struct opcode_table *link;
}Opcode_Table;

/* symbol table 만들 때 사용.
 * 변수 순서대로 입력받음. */
typedef struct sym_tab{
	char symbol[10];
	int locctr;
	struct sym_tab *link;
}Symbol_Table;

/* Pass1에서의 H 와 D Record를 저장시 사용
 * type1일 경우 H 타입, type2일 경우 */
typedef struct l_l{
	int type;
	char name[30];
	int addr;
	int length;
	int startaddr;
	struct l_l *link;
}Estab_Table;

/* Name     : Basic_State()
 * Parameter: None
 * Usage    : Shows the basic state siscim>    */
void Basic_State(void);

/* Name     : Read_Op()
 * Parameter: None
 * Usage    : Read the opcode, and mnemonic
 *            written in "opcode.txt".         */
int Read_Op(void);

/* Name     : Helplist()
 * Parameter: None
 * Usage    : Print the helplist.              */
void Helplist(void);

/* Name     : Make_History()
 * Parameter: char command[]
 * Usage    : Make the linked list which stores
 *            the information of the history.  */
void Make_History(char *);

/* Name     : Free_History()
 * Parameter: None
 * Usage    : Free the linked list which stores
 *            the information of the history.  */
void Free_History(void);

/* Name     : Print_History()
 * Parameter: None
 * Usage    : Print the every history which 
 *            consists valid command input.    */
void Print_History(void);

/* Name     : Print_Dir()
 * Parameter: None
 * Usage    : Print every files which is in
 *            the current directory.           */
void Print_Dir(void);

/* Name     : Reset_Dump()
 * Parameter: None
 * Usage    : Reset every memories into '0'    */
void Reset_Dump(void);

/* Name     : Dump_Print()
 * Parameter: int start, int end 
 * Usage    : Print all the dump memories
 *            having the address from start
 *            to end.                          */
void Dump_Print(int, int);

/* Name     : Dump_Edit()
 * Parameter: int address, int value
 * Usage    : Edit the value of specific
 *			  address.                         */
void Dump_Edit(int, int);

/* Name     : Dump_Fill()
 * Parameter: int start, int end, int value
 * Usage    : Fill the value to address from
 *            start to end.                    */
void Dump_Fill(int, int, int);

/* Name     : Op_Make()
 * Parameter: int op_num,char mnemonic,char type
 * Usage    : Make the linked list of opcode.
 *            consisting opcode number, mnemonic,
 *            name and type of the mnemonic.   */
void Op_Make(int, char [], char []);

/* Name     : Op_Free()
 * Parameter: None
 * Usage    : Free the linked list of opcode.  */
void Op_Free(void);

/* Name     : Op_Mnemonic()
 * Parameter: char Mnemonic
 * Usage    : Print the opcode of the mnemonic.*/
void Op_Mnemonic(char []);

/* Name     : Op_List
 * Parameter: None
 * Usage    : Print the list of opcode.        */
void Op_List(void);

/* Name     : opcodetable_func_make_hash_given()
 * Parameter: char Mnemonic
 * Usage    : return hasing number of given
 *            mnemonic using hash function.    */
int opcodetable_func_make_hash_given(char []);

/* filename에 해당하는 파일을 현재 디렉터리에서 찾아서 한줄씩 출력
 * 만약 없다면 에러출력. 성공시 1출력 실패시 0출력. */
int Type_Print(char []);

/* filename을 입력받아 symbol table을 작성한다. 정상적으로 종료시 1 출력 아닐시 -1*/
int Make_Symbol_And_Loc(char []);

/* symbol과 locctr을 linked list에 추가. */
void Make_Symtab(char [], int);

/* Symbol Table Free한다. */
void Free_Symtab(void);

/* Symbol Table에 이미 들어있는 symbol인지 확인. */
int Is_Used_Sym(char []);

/* Opcode중에 겹치는것이 있나 확인. */
int Is_Op(char []);

/*symbol table을 출력*/
void Print_Symtab(void);

/*filename에 해당하는 asm파일을 읽어서 obj 파일과 lst 파일 생성. */
void Assemble(char []);

/*opcode의 타입을 리턴*/
int Type_Return(char []);

/*Symbol 리셋*/
void Reset_Symbol(void);

/*Byte 다음에 오는 문자열 체크*/
int Byte_Check(char []);

/* Symbol을 내림차순으로 바꿔 새로운 배열에 저장. */
void Change_Sym(void);

/* 각각 format1, format2, format3, format4에 대한
 * objcode를 생성해 준다. */
int format1(int);
int format2(int, char, char);
int format3(int, int, int);
int format4(int, int, int);

/* Mnemonic의 opcode를 출력. */
int Opnum_Return(char []);

/* String이 Decimal인지 검사. */
int Is_Dec(char []);

/* Byte C''타입의 형식을 체크. */
int Check_C(char []);

/* 문자열이 Register인지 검사. */
int Is_Reg(char []);

/* filename1, filename2, filename3을 받아서 각각
 * 내용을 읽고 그 내용을 적절히 저장하여 load map을 화면에 출력하는
 * 함수. 만약 오류가 존재할 경우 에러 내용을 화면에 출력한다. */
void loader_pass1(char [], char [], char []);

/* filename을 읽어 Modification 등을 진행하여 dump에 메모리를 적절하게
 * 변경하여 저장하는 함수. */
void loader_pass2(char []);

/* ESTAB을 만드는 함수. 이름에 따른 Hash function을 사용한다.*/
void Make_Estab(int, char [], int, int);

/* ESTAB에 이미 들어있는 Symbol인지 확인. 만약 존재한다면 */
int Find_Estab(char []);

/* String을 입력받아 이를 16진수 수로 바꿔주는 함수. */
int S_to_Hex(char []);

/* Load Map을 출력하는 함수. */
void Print_Load(void);

/* ESTAB Free해주는 함수. */
void Free_Estab(void);

/* bp list를 print하는 함수. */
void Print_Bp(void);

/* bp list를 clear하는 함수. */
void Clear_Bp(void);

/* run에서 계산할때 넣을 숫자를 계산하는 함수. */
void Make_Num(int [], int, int*);

/* run하는 함수. */
void Run_Prog(void);

/* Format 3,4의 계산을 하는 함수. */
int TA_Format3(int [], int *);

/* Format 2의 계산을 하는 함수. */
int TA_Format2(int [], int*, int*);

/* Bplist 배열을 오름차순으로 만드는 함수. */
void Make_Bplist(int );
