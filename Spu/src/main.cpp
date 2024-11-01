
#include <stdio.h>
#include <stdlib.h>

#include "../../command.h"
#include "spu.h"
#include"../../Bad_stack/headers/stack_implementation.h"
#include"../../Bad_stack/headers/utilits.h"


int Code_array_construct(processor * proc, FILE * file_f) {
     int arg_1;
     for (int i = 0; i< proc->code_size; i++) {
          if (! fscanf(file_f, "%d", &arg_1)) {
               printf("ERORR: in file scanf, param: %d\n", i);
               return 0;
          } else {
               *(proc->code_array + i) = arg_1;
          }

     }
     return 1;
}


int main(int argc, char const *argv[])
{
     
     FILE *  file_f  = fopen((char *)CODE_FILE_NAME, "rt"); 
     
     if (file_f == NULL) {                                         
          return 0;
     }

     FILE * logfile_f = fopen((char *)LOGFILE, "w");
     
     char signature[10] = "";
     int number_of_ver = 0;
     int n_arguments = 0;
     int start_ip = 0;

     if (fscanf(file_f, "%s %d %d %d",signature, &number_of_ver, &n_arguments, &start_ip) != 4) {
          printf("ERROR: scanf start parameters of file!");
          return 0;
     }
     
     if (signature != SIGNATURE && number_of_ver != PROC_VERSION) {
          printf("ERROR: scanf start parameters of file!");
          return 0;
     }
     
     processor proc = {};

     proc.code_size = n_arguments;
     proc.code_array = (int *)calloc(n_arguments + 1, sizeof(int));
     proc.rand_access_memory = (int *)calloc(RAM_SIZE, sizeof(int));
     proc.IP = start_ip + 1;
     proc.code_size = n_arguments;

     bad_stack stack = {};   //TODO: constract_file(); 

     const char * stack_name = "stack_1";
     stack.logFile = logfile_f;
     Stack_ctor(&stack, stack_name);
     Stack_ctor(&proc.stack_function, "stack_function");

     //int a = fread(proc.code_array, sizeof(int), n_arguments, file_f);
     //printf("n: %d a: %d\n", n_arguments, a);

     //if (a != n_arguments) {
       //   printf("error in machine code file\n");
    // }

     if (! Code_array_construct(&proc, file_f)){
          return 0;
     }

     Run_code(&proc, &stack);

     /*
     if (*proc.code_array == START_CODE) {
          proc.IP++;
          
          Run_code(&proc, &stack);  

     } else {
          printf("Program must start from START\n");
     }
     */

     //Dumper(&stack,"stack_1", "main" );
     Stack_dtor(&proc.stack_function);
     Stack_dtor(&stack);
     free(proc.code_array);
     free(proc.rand_access_memory);

     return 0;
}


