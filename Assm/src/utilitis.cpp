
#include "../include/assembler.h"
#include "../../command.h"
#include <string.h>
#include <assert.h>

void Varificate() {
//
}

void Dumper(assembler * assm_code, int * code_mask, char * asm_funk, char * parameter_one, int parameter_two, int n_line)  {

    printf("Check assembler code, line: %d\n", n_line  + 1);  //maybe make a copy, read about faster
    printf("Assm funk: %s\n", asm_funk);
    if (*code_mask & IMO_CODE && parameter_one == "") {
        printf("immediate operant %d\n", parameter_two);
    }

    else if (parameter_one != "") {
        printf("parametr: %s\n", parameter_one);
    }

    printf("code array:\n");
    printf("{");
    for (int i = 0; i < assm_code->IP; i++) {
        if(i == assm_code->IP - 1) {
            printf(" [%d] ", *(assm_code->code_array + i ));
        } else {
            printf(" [%d],", *(assm_code->code_array + i ));
        }
        
    }
    printf("}\n\n");
            

}

int Check_register(char * parameter_one) {
    if (!strcmp(parameter_one, AX)) {
        return AX_NUM;
    }

    else if (!strcmp(parameter_one, BX)) {
        return BX_NUM;
    }

    else if (!strcmp(parameter_one, CX)) {
        return CX_NUM;
    } 
    
    else if (!strcmp(parameter_one, DX)) {
        return DX_NUM;
        
    } else {
        return -1;
    }   
}

int First_n_lines(char* text, int number_of_symbols){
    int n_lines = 0;
    for (size_t elem = 0; elem < number_of_symbols; elem++) {
        if(*(text + elem) == '\n' && *(text + elem + 1) != '\n') {
            n_lines++;
        }
    }
    return n_lines;
}

int Fill_array_pointer(line* array_of_lines, int number_of_lines, int number_of_symbols) {
    int n_lines = 0;
    assert( array_of_lines != NULL );

    for (size_t elem = 1; elem < (size_t)number_of_symbols; elem++) {     

        if (*(array_of_lines->line_index + elem)     == '\n' &&
            *(array_of_lines->line_index + elem + 1) != '\n') {

            line i_line = {0 , array_of_lines->line_index};
            *(array_of_lines + n_lines + 1) = i_line;
            (array_of_lines + n_lines + 1)->line_index = array_of_lines->line_index + elem; 
            (array_of_lines + n_lines)->line_size = size_t((array_of_lines + n_lines + 1)->line_index) 
                                                  - size_t((array_of_lines + n_lines)    ->line_index);
            n_lines++;
        }
    }
    return n_lines;
}

size_t Get_file_size(const char * SCAN_FILE_NAME) {
    assert(SCAN_FILE_NAME != NULL);

    FILE * file = fopen(SCAN_FILE_NAME, "rb");

    int file_size = 0;                         //what int64_t?
              
    if (file == NULL) {  
		file_size = -1;
	} else {

		fseek(file, 0, SEEK_END);    //open file in binary mode, go to last element and get number of last bite
		file_size = ftello(file);     
		fclose(file);
	}
	return file_size;    
}