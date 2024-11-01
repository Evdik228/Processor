
#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include <cstdio>

#include "../include/assembler.h"
#include "../include/utilitis.h"
#include "../../command.h"

int Word_count(char * instruction, int instruction_size) {
    if(instruction_size <= 0) {
        return 0;
    }
    int counter = 1;
    
    for (int i = 1; i < instruction_size - 1; i++) {
        if (isspace(*(instruction + i)) && !isspace(*(instruction + i + 1))) {
            counter ++; 
        }
    }

    return counter;
}

void Output_machine_code(assembler * assm_code, FILE * output_f) {

    fprintf(output_f, "%s %d %d %d\n",SIGNATURE, ASSEMBLER_VERSION, assm_code->IP, assm_code->start_ip);

    for (int n_line = 0; n_line < assm_code->IP; n_line ++) {
        
        int * code_line = (assm_code->code_array + n_line);

        if((( *code_line & IMO_CODE) && (*code_line & RAM_CODE) && (*code_line & REG_CODE)) ||
             (*code_line & IMO_CODE) && (*code_line & REG_CODE)) { 

            fprintf(output_f, "%d %d %d\n", *code_line, 
                                            *(code_line + 1), 
                                            *(code_line + 2));
            n_line++;
            n_line++;
        }
        else if ( ( (*code_line & REG_CODE))                            ||
                  ( (*code_line & IMO_CODE))                            ||
                  ( (*code_line & IMO_CODE) && (*code_line & RAM_CODE)) || 
                  ( (*code_line & RAM_CODE) && (*code_line & REG_CODE)) ) {

            fprintf(output_f, "%d %d \n", *code_line, 
                                          *(code_line + 1));
            n_line++;
        }

        else {

            fprintf(output_f, "%d\n", *(assm_code->code_array + n_line));
        }
    }
}



void Analyze_parameter_bracket(assembler * assm_code, char * text_parameter, int * code_mask) {
    char reg [2] = "";

    reg[0] = *(strchr(text_parameter, '[') + 1);                 //many many copypast    
    reg[1] = *(strchr(text_parameter, '[') + 2);  

    int n_reg = Check_register(reg);
    if(n_reg != -1) {

        *code_mask = *code_mask | RAM_CODE;                           //it,s bad
        *code_mask = *code_mask | REG_CODE;

        *(assm_code->code_array + assm_code->IP) = *code_mask;
        assm_code->IP++; 

        *(assm_code->code_array + assm_code->IP) = n_reg;
        assm_code->IP++;

        return;

    }else {
        int imo = 0;
        char * start_bracket = strchr(text_parameter, '[');
        int len_imo = (size_t)strchr(text_parameter, ']') - (size_t)start_bracket - 1;

        for (int i = 0; i < len_imo; i++ ) {
            imo += (int)(*(start_bracket + 1+i) - '0');
            imo*=10;
        }  
        imo/=10;

        *code_mask = *code_mask | RAM_CODE;                           //it,s bad
        *code_mask = *code_mask | IMO_CODE;

        *(assm_code->code_array + assm_code->IP) = *code_mask;
        assm_code->IP++; 

        *(assm_code->code_array + assm_code->IP) = imo;
        assm_code->IP++;
    }
}


void Analyze_parameter_plus(assembler * assm_code, char * text_parameter, int * code_mask) {

    char reg [2] = "";
    char * plus = strchr(text_parameter, '+');

    reg[1] = *(plus - 1);                 //many many copypast    
    reg[0] = *(plus - 2);  

    int n_reg = Check_register(reg);
    int len_imo = strlen(text_parameter) - (int)(plus - text_parameter) - 1;
    int imo = 0;
                                                          //TODO:A lot of copypast
    printf("len_imo: %d\n", len_imo);

    for (int i = 0; i < len_imo; i++ ) {
        imo += (int)(*(plus + 1+i) - '0');
        imo*=10;
    }  
    imo/=10;

    if(n_reg != -1) {

        *code_mask = *code_mask | IMO_CODE;                           
        *code_mask = *code_mask | REG_CODE;

        *(assm_code->code_array + assm_code->IP) = *code_mask;
        assm_code->IP++; 

        *(assm_code->code_array + assm_code->IP) = n_reg;
        assm_code->IP++;

        *(assm_code->code_array + assm_code->IP) = imo;
        assm_code->IP++;

        return;

    }else {
        printf("Register error!\n");
    }
}

void Analyze_parameter_plus_and_bracket(assembler * assm_code, char * text_parameter, int * code_mask) {
    char reg [2] = "";
    char * plus = strchr(text_parameter, '+');

    reg[1] = *(plus - 1);                 //many many copypast    
    reg[0] = *(plus - 2);  

    int n_reg = Check_register(reg);
    int len_imo = (int)( strchr(text_parameter, ']') - plus) - 1;
    int imo = 0;
                                                          //TODO:A lot of copypast!!!!!
    printf("len_imo: %d\n", len_imo);                     //TODO:A lot of copypast!!!!!

    for (int i = 0; i < len_imo; i++ ) {
        imo += (int)(*(plus + 1+ i ) - '0');
        imo*=10;
    }  
    imo/=10;

    if(n_reg != -1) {

        *code_mask = *code_mask | IMO_CODE;                           
        *code_mask = *code_mask | REG_CODE;
        *code_mask = *code_mask | RAM_CODE;

        *(assm_code->code_array + assm_code->IP) = *code_mask;
        assm_code->IP++; 

        *(assm_code->code_array + assm_code->IP) = n_reg;
        assm_code->IP++;

        *(assm_code->code_array + assm_code->IP) = imo;
        assm_code->IP++;

        return;

    }else {
        printf("Register error!\n");
    }
    
}


void Metka_analyse(assembler * assm_code, char * metka_code, int is_from_jmp) {
   
    char metka_label[LEN_METKA] = "";

    int metka_len = strchr(metka_code, ':') - metka_code;

    for (int i = 0; i < metka_len; i++) {
        metka_label[i] = metka_code[i];
    }

    printf("metka: %s\n", metka_label);

    for (int n_metka = 0; n_metka < N_METKAS; n_metka++ ) {
        
        //printf("\n%d\n", !strcmp((const char *)metka_label, (assm_code->metkas + n_metka)->label) &&
        //                                      (assm_code->metkas + n_metka)->address != -1 &&
        //                                      (assm_code->metkas + n_metka)->is_define != -1);
        //printf("\nYEA TUT_1!!!\n");
        //printf("\n%d\n", !strcmp((assm_code->metkas + n_metka)->label ,"") && !is_from_jmp);
        //printf("\nYEA TUT_2!!!\n");
        //printf("\n%d\n", !strcmp((assm_code->metkas + n_metka)->label ,"") && is_from_jmp);
        //printf("\nYEA TUT_3!!!\n");
        //printf("\n%d\n", !strcmp((const char *)metka_label,(assm_code->metkas + n_metka)->label) &&
        //                                        assm_code->metkas + n_metka)->is_define == -1);
        //printf("\nYEA TUT_4!!!\n");                                           

        if (!strcmp((const char *)metka_label,(assm_code->metkas + n_metka)->label)&&
                                              (assm_code->metkas + n_metka)->address != -1 &&
                                              (assm_code->metkas + n_metka)->is_define != -1) 
                                              {
    
            *(assm_code->code_array + assm_code->IP) = (assm_code->metkas + n_metka)->address;
            assm_code->IP++;

            break;
        } 
        
        else if (!strcmp((assm_code->metkas + n_metka)->label ,"") && !is_from_jmp) {
            
            memcpy((char *)(assm_code->metkas + n_metka)->label, (const char *)metka_label, strlen((const char *)metka_label));
            (assm_code->metkas + n_metka)->address = assm_code->IP;
            (assm_code->metkas + n_metka)->is_define = 1;

            break;
        } 

        else if (!strcmp((assm_code->metkas + n_metka)->label ,"") && is_from_jmp) {
            
            memcpy((char *)(assm_code->metkas + n_metka)->label, (const char *)metka_label, strlen((const char *)metka_label));
            (assm_code->metkas + n_metka)->address = assm_code->IP;
            *(assm_code->code_array + assm_code->IP) = -1;
            (assm_code->metkas + n_metka)->is_define = -1;
            assm_code->IP++;
            
            break;
        }

        else if (!strcmp((const char *)metka_label,(assm_code->metkas + n_metka)->label) &&
                                                   (assm_code->metkas + n_metka)->is_define == -1) {

            (assm_code->metkas + n_metka)->is_define = 1;
            *(assm_code->code_array + (assm_code->metkas + n_metka)->address) = assm_code->IP;
            (assm_code->metkas + n_metka)->address = assm_code->IP;

            break;
        } 
    }
}


void Text_parameter_detect(assembler * assm_code, char * text_parameter, int * code_mask) {

    if (strchr(text_parameter, '[') != NULL && strchr(text_parameter, '+') == NULL) {

        Analyze_parameter_bracket(assm_code, text_parameter, code_mask);

    } 
    else if(strchr(text_parameter, '+') != NULL && strchr(text_parameter, '[') == NULL) { 
         
        Analyze_parameter_plus(assm_code, text_parameter, code_mask);

    }
    else if (strchr(text_parameter, '+') != NULL && strchr(text_parameter, '[') != NULL) {

        Analyze_parameter_plus_and_bracket(assm_code, text_parameter, code_mask);

    }
    else if(strchr(text_parameter, ':') != NULL) {

        Metka_analyse(assm_code, text_parameter, 1);
    }
        
    else {
        *code_mask = *code_mask | REG_CODE;

        *(assm_code->code_array + assm_code->IP) = *code_mask;
        assm_code->IP++;

        *(assm_code->code_array + assm_code->IP) = Check_register(text_parameter);
        assm_code->IP++;
    }
}

void Instruction_analyze(assembler * assm_code, int * code_mask, char * asm_funk, char * text_parameter, 
                         double immediate_operant, int quantity_params, int n_line) {
      
    if (strcmp(asm_funk, PUSH) == 0) {

        *code_mask = *code_mask | PUSH_CODE;

        if (quantity_params == TWO_PARAMETERS && (*code_mask & IMO_CODE)) {

            *(assm_code->code_array + assm_code->IP) = *code_mask;
            assm_code->IP++;                                            

            *(assm_code->code_array + assm_code->IP) = immediate_operant;
            assm_code->IP++;
            return;

        } else if (quantity_params == TWO_PARAMETERS && text_parameter != "") {
            
            Text_parameter_detect(assm_code, text_parameter, code_mask);

        } else {
            Dumper(assm_code, code_mask, asm_funk, text_parameter, immediate_operant, n_line);
            return;
        }         
    }

    else if (strcmp(asm_funk, POP) == 0) {

        * code_mask = * code_mask | POP_CODE;

        if (quantity_params == ONE_PARAMETER) {
            Dumper(assm_code, code_mask, asm_funk, text_parameter, immediate_operant, n_line);
            return;
        }
        else if (quantity_params == TWO_PARAMETERS && text_parameter != "") {
            Text_parameter_detect(assm_code, text_parameter, code_mask);
        }
    }

    else if (strcmp(asm_funk, JMP) == 0) {

        * code_mask = * code_mask | JMP_CODE;
        *code_mask = *code_mask|IMO_CODE;

        if (quantity_params == ONE_PARAMETER) {
            Dumper(assm_code, code_mask, asm_funk, text_parameter, immediate_operant, n_line);
            return;
        }
        else if (quantity_params == TWO_PARAMETERS && text_parameter != "") {

            *(assm_code->code_array + assm_code->IP) = *code_mask;
            assm_code->IP++;
            Dumper(assm_code, code_mask, asm_funk, text_parameter, immediate_operant, n_line);
            Text_parameter_detect(assm_code, text_parameter, code_mask);

            Dumper(assm_code, code_mask, asm_funk, text_parameter, immediate_operant, n_line);
            
        } else {
            Dumper(assm_code, code_mask, asm_funk, text_parameter, immediate_operant, n_line);
            return;
        }
        
    }

    else if (strcmp(asm_funk, JA) == 0) {

        * code_mask = * code_mask | JA_CODE;
        
        if (quantity_params == ONE_PARAMETER) {
            Dumper(assm_code, code_mask, asm_funk, text_parameter, immediate_operant, n_line);
            return;
        }
        else if (quantity_params == TWO_PARAMETERS && text_parameter != "") {
            
            *code_mask = *code_mask|IMO_CODE;

            *(assm_code->code_array + assm_code->IP) = *code_mask;
              assm_code->IP++;
              
            Text_parameter_detect(assm_code, text_parameter, code_mask);
            
        } else {
            Dumper(assm_code, code_mask, asm_funk, text_parameter, immediate_operant, n_line);
            return;
        }

    }

    else if (strcmp(asm_funk, ADD) == 0) {

        * code_mask = * code_mask | ADD_CODE;
        *(assm_code->code_array + assm_code->IP) = * code_mask;
        assm_code->IP++;

        return;
    }

    else if (strcmp(asm_funk, SUB) == 0) {

        * code_mask = * code_mask | SUB_CODE;    //TODO: make macros 
        *(assm_code->code_array + assm_code->IP) = * code_mask;
        assm_code->IP++;

        return;
    }

    else if (strcmp(asm_funk, MUL) == 0) {

        * code_mask = * code_mask | MUL_CODE;
        *(assm_code->code_array + assm_code->IP) = * code_mask;
        assm_code->IP++;
         return;
    }

    else if (strcmp(asm_funk, DIV) == 0) {

        * code_mask = * code_mask | DIV_CODE;
        *(assm_code->code_array + assm_code->IP) = * code_mask;
        assm_code->IP++;

        return;
    }

    else if (strcmp(asm_funk, SQRT) == 0) {

        * code_mask = * code_mask | SQRT_CODE;       
        *(assm_code->code_array + assm_code->IP) = * code_mask;  //TODO: define 
        assm_code->IP++;

        return;
    }

    else if (strcmp(asm_funk, OUT) == 0) {

         * code_mask = * code_mask | OUT_CODE;
         *(assm_code->code_array + assm_code->IP) = * code_mask;
         assm_code->IP++;

         return;
    }

    else if (strcmp(asm_funk, START) == 0) {

        assm_code->start_ip = assm_code->IP;

        * code_mask = * code_mask | START_CODE;
        *(assm_code->code_array + assm_code->IP) = * code_mask;
        assm_code->IP++;

        return;
    }


        else if (strcmp(asm_funk, HLT) == 0) {

            * code_mask = * code_mask | HLT_CODE;
            *(assm_code->code_array + assm_code->IP) = * code_mask;
            assm_code->IP++;

            return;
        }
        else if (strcmp(asm_funk, CALL) == 0) {

            * code_mask = * code_mask | CALL_CODE;
        
        if (quantity_params == ONE_PARAMETER) {
            Dumper(assm_code, code_mask, asm_funk, text_parameter, immediate_operant, n_line);
            return;
        }
        else if (quantity_params == TWO_PARAMETERS && text_parameter != "") {

            *code_mask = *code_mask | IMO_CODE;
            *(assm_code->code_array + assm_code->IP) = *code_mask;
              assm_code->IP++;
              
            Text_parameter_detect(assm_code, text_parameter, code_mask);
            
        } else {
            Dumper(assm_code, code_mask, asm_funk, text_parameter, immediate_operant, n_line);
            return;
        }

        }
        else if (strcmp(asm_funk, RET) == 0) {

            * code_mask = * code_mask | RET_CODE;
            *(assm_code->code_array + assm_code->IP) = * code_mask;
            assm_code->IP++;

            return;
        }

        else if(strchr(asm_funk, ':') != NULL) {

            Metka_analyse(assm_code, asm_funk, 0);

            return;

        } else {

         Dumper(assm_code, code_mask, asm_funk, text_parameter, immediate_operant, n_line);

        }

    }

void Make_machine_code(assembler * assm_code, line * array_of_lines, int number_of_lines) {
                                                   
    for (int n_line = 0; n_line < number_of_lines; n_line++ ) {
    
        char * instruction   = ((array_of_lines + n_line)->line_index);
        int instruction_size = ((array_of_lines + n_line)->line_size);
        char asm_funk[MAX_FUNK_SIZE] = "";
        char text_parameter[MAX_REG_SIZE] = "";   
        int immediate_operant = 0;

        int code_mask = 0;    

        int quantity_params = Word_count(instruction, instruction_size);

        printf("quantity_params: %d\n", quantity_params);

        switch (quantity_params) {   //TODO: funk for make new argument

        case ONE_PARAMETER:

            if (sscanf(instruction, "%s", asm_funk) != 1) {
                printf("Error in code, line number %i", n_line);
                return;
            }

            break;

        case TWO_PARAMETERS:  //TODO: new font

            if (sscanf(instruction, "%s %d", asm_funk, &immediate_operant) == 2){ // PUSH 6
                code_mask += IMO_CODE;
            }

            else if (sscanf(instruction, "%s %s", asm_funk, text_parameter) == 2) { //  PUSH [AX+5]    PUSH [7]

            } else {
                printf("Error in code, line number %i", n_line + 1);
                return;
            }

            break;   
    /*
        case THREE_PARAMETERS:   

        if (sscanf(instruction, "%s, %s, %lf", asm_funk, text_parameter, immediate_operant) != 3){
                printf("Error in code, line number %i", n_line);
                return;
            }
             code_mask += IMO_CODE;

        if (sscanf(instruction, "%s, %s, %s", asm_funk, text_parameter, immediate_operant) != 3){
                printf("Error in code, line number %i", n_line);
                return;
            }             

            break;     
    */
        default:

            printf("More or less words in instruction, line: %i", n_line + 1);
            return;
        }
        
        Instruction_analyze(assm_code, &code_mask, asm_funk, text_parameter, immediate_operant, quantity_params, n_line);
        Dumper(assm_code, &code_mask, asm_funk, text_parameter, immediate_operant, n_line);
    }   

}
