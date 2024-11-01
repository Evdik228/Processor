#ifndef SPU_H 
#define SPU_H

inline const char* ASSM_FILE_NAME = "../assm.asm"; 
inline const char* CODE_FILE_NAME = "../machine_code.asm"; 
inline const char* LOGFILE = "logfile.txt"; 

#include"../Bad_stack/headers/stack_implementation.h"
#include"../Bad_stack/headers/utilits.h"

inline const int RAM_SIZE = 32;
inline const int N_REGISTERS = 4;
inline const char * SIGNATURE = "DED_LOX";
inline const int PROC_VERSION = 8;

enum assembler_code{

    START_CODE =  1 ,        //00000001
    PUSH_CODE  =  2 ,        //00000010
    POP_CODE   =  3 ,        //00000011
    ADD_CODE   =  4 ,        //00000100    
    SUB_CODE   =  5 ,        //00000101           
    MUL_CODE   =  6 ,        //00000110
    DIV_CODE   =  7 ,        //00000111     
    SQRT_CODE  =  8 ,        //00001000 
    OUT_CODE   =  9 ,        //00001001
    HLT_CODE   =  0 ,        //00000000 
    JMP_CODE   =  10,        //00001010     
    JA_CODE    =  11,        //00001011
    JB_CODE    =  12,        //00001100
    RET_CODE   =  13,        //00001101
    CALL_CODE  =  14,        //00001110
    IN         =  15,        //00001111    

    RAM_CODE   = 128,        //10000000   
    REG_CODE   =  64,        //01000000
    IMO_CODE   =  32,        //00100000

};

struct processor {

    int * code_array;  //it`s norm, that name of code array like in structure assembler?
    int code_size;
    int IP;
    int reg_array[N_REGISTERS];
    int * rand_access_memory;
    bad_stack stack_function = {};

};

#endif