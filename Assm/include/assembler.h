
#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>
 
const int ASSEMBLER_VERSION = 8;

inline const char * START  = "START" ;
inline const char * PUSH   = "PUSH"  ;
inline const char * POP    = "POP"   ;
inline const char * JMP    = "JMP"   ;
inline const char * JA     = "JA"    ;
inline const char * ADD    = "ADD"   ;
inline const char * SUB    = "SUB"   ;
inline const char * MUL    = "MUL"   ;
inline const char * DIV    = "DIV"   ;
inline const char * SQRT   = "SQRT"  ;
inline const char * OUT    = "OUT"   ;
inline const char * HLT    = "HLT"   ;
inline const char * AX     = "AX"    ;
inline const char * BX     = "BX"    ;
inline const char * CX     = "CX"    ;
inline const char * DX     = "DX"    ;
inline const char * CALL   = "CALL"  ;
inline const char * RET    = "RET"   ;

enum Register_num {

    AX_NUM =  1,
    BX_NUM =  2,
    CX_NUM =  3,
    DX_NUM =  4,
    NO_REG = -1,

};

const int MAX_REG_SIZE  = 8;
const int MAX_FUNK_SIZE = 8;

const int ONE_PARAMETER    = 1;
const int TWO_PARAMETERS   = 2;

const int N_METKAS = 10; 
const int LEN_METKA = 10;


struct metka 
{
    const char label[10] = "";      
    int address = -1;
    int is_define = -1; 
};

struct assembler
{
    int * code_array;
    int code_size;
    int IP;
    int start_ip = -1;
    metka * metkas;

};

struct line
{
    size_t line_size;
    char* line_index;
};


void Make_machine_code(assembler * assm_code, line * array_of_lines, int number_of_lines);
void Output_machine_code(assembler * assm_code, FILE * output_f);


#endif