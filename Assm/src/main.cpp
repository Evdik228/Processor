
#include <stdio.h>
#include <stdint.h>

#include "../include/assembler.h"
#include "../../command.h"
#include "../include/utilitis.h"


int main(int argc, char const *argv[])
{                               

                                    //reding code
    int number_of_symbols = Get_file_size((char *)ASSM_FILE_NAME); 
    printf("Number of symbols in file: %d\n",number_of_symbols);

    FILE * file_f = fopen((char *)ASSM_FILE_NAME, "rt"); 
    if (file_f == NULL) {\
        printf("ERR in open assm file\n");                                         
         return 0;
    }

    FILE * output_f = fopen((char *)CODE_FILE_NAME,"w");     
    if (output_f == NULL) {
        printf("ERR in open code file\n");
        return 0;
    }
    

    char * data = (char *)calloc(number_of_symbols, sizeof(char));  

    if (number_of_symbols != fread(data, sizeof(char), number_of_symbols, file_f)) {

        printf("error in reading data from the file!\n");
        return 0;
    }

    int number_of_lines = First_n_lines(data, number_of_symbols); 
    line * array_of_lines = (line *)calloc(number_of_lines + 1, sizeof(line)); // in 64b_os one pointer like double - 8bite
    
    line firs_line = {0, data};

    *(array_of_lines) = firs_line;

    number_of_lines = Fill_array_pointer(array_of_lines, number_of_lines, number_of_symbols);

    assembler assm_code = {};
    assm_code.code_size = number_of_lines;
    assm_code.code_array = (int *)calloc(number_of_lines * 3, sizeof(double));
    
    assm_code.metkas = (metka *)calloc(N_METKAS, sizeof(metka));


    Make_machine_code(&assm_code, array_of_lines, number_of_lines);
    
    Output_machine_code(&assm_code, output_f);
    

    free(data);     //clean memory 
    free(array_of_lines);
    free(assm_code.code_array);
    free(assm_code.metkas);
    fclose(file_f);               //TODO: one funck; 
    fclose(output_f);

    return 0;
}





