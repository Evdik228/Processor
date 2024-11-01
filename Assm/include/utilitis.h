#ifndef UTILITIS_H
#define UTILITIS_H

#include<stdlib.h>
#include <cstdio>

#include "assembler.h"


void Dumper(assembler * assm_code, int * code_mask, char * asm_funk, char * parameter_one, int parameter_two, int n_line);

int Check_register(char * parameter_one);
int Fill_array_pointer(line* array_of_lines, int number_of_lines, int number_of_symbols);
size_t Get_file_size(const char * SCAN_FILE_NAME);
int First_n_lines(char* text, int number_of_symbols);


#endif