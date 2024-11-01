


#include"../../Bad_stack/headers/stack_implementation.h"
#include"../../Bad_stack/headers/utilits.h"

#include"../../command.h"
#include "spu.h"

#include<math.h>


int Count_parameter(int instruction_code) {

    if      ((instruction_code & IMO_CODE) &&
             (instruction_code & REG_CODE) &&
             (instruction_code & RAM_CODE)) {

        return 3;
    }
    
    else if ((instruction_code & IMO_CODE) && (instruction_code & RAM_CODE) || 
             (instruction_code & IMO_CODE) && (instruction_code & REG_CODE) ||
             (instruction_code & RAM_CODE) && (instruction_code & REG_CODE)) {

        return 2; 
    }
             
    else if ((instruction_code & REG_CODE) || (instruction_code & IMO_CODE)) {

        return 1;
    
    } else {

        return 0;
    }
}

int Pop(processor * proc,  bad_stack * stack, int n_param) {
    int instruction_code = *(proc->code_array + proc->IP);
    if (n_param == 0) {
        Stack_pop(stack);
        proc->IP++;
    } 
    else if(n_param == 1) {

        if (instruction_code & REG_CODE) {
            Stack_pop(stack);
            *(proc->reg_array + *(proc->code_array + proc->IP + 1)) = stack->last_pop_element;
            proc->IP++;
            proc->IP++;
        } else {
            return -1;
        }
    }
    else if(n_param == 2) {
                                        //RAM & REG || RAM & IMO || REG & IMO  
        if ((instruction_code & REG_CODE) && (instruction_code & IMO_CODE)) {
            Stack_pop(stack);
            *(proc->reg_array + *(proc->code_array + proc->IP + 1)) = stack->last_pop_element + *(proc->code_array + proc->IP + 2);
            proc->IP++;
            proc->IP++; 
            proc->IP++;
        } 

        else if ((instruction_code & REG_CODE) && (instruction_code & RAM_CODE)) {
            Stack_pop(stack);
            (proc->rand_access_memory)[*(proc->reg_array + *(proc->code_array + proc->IP + 1))] = stack->last_pop_element;
            proc->IP++;
            proc->IP++;
        }        

        else if ((instruction_code & IMO_CODE) && (instruction_code & RAM_CODE)) {
            Stack_pop(stack);

            (proc->rand_access_memory)[*(proc->code_array + proc->IP + 1)] = stack->last_pop_element;

            proc->IP++;
            proc->IP++;
        }
    }  
    else if(n_param == 3) {
        printf("on this stage pop funk don`t analyse 3 param!\n");
        return -1;
    } else {
        return -1;
    }

    return 0;

}

int Push(processor * proc,  bad_stack * stack, int n_param) {

    int instruction_code = *(proc->code_array + proc->IP);

    if (n_param  == 1) { 

        if (instruction_code & IMO_CODE) {
            Stack_push(stack, *(proc->code_array + proc->IP + 1));
            (proc->IP)++;
            (proc->IP)++;
        }
        else if(instruction_code & REG_CODE) {
            Stack_push(stack, *(proc->reg_array + *(proc->code_array + proc->IP + 1)));
            proc->IP++;
            proc->IP++;

        } else {

            return -1;
        }
    } 
    else if (n_param == 2) {

        if ((instruction_code & IMO_CODE) && (instruction_code & REG_CODE)) {
            Stack_push(stack, *(proc->reg_array + *(proc->code_array + proc->IP + 1)) + *(proc->code_array + proc->IP + 2));
            proc->IP++;
            proc->IP++;
            proc->IP++;
        } 
        else if ((instruction_code & IMO_CODE) && (instruction_code & RAM_CODE)) {
            Stack_push(stack, proc->rand_access_memory[*(proc->code_array + proc->IP + 1)]);
            proc->IP++;
            proc->IP++;
        }
        else if ((instruction_code & REG_CODE) && (instruction_code & RAM_CODE)) {
            Stack_push(stack, proc->rand_access_memory[*(proc->reg_array + *(proc->code_array + proc->IP + 1))]);
            proc->IP++;
            proc->IP++;
        } else {
            
            return -1;
        }
    }   

    else if (n_param == 3) {

        Stack_push(stack, proc->rand_access_memory[*(proc->reg_array 
                                                 + *(proc->code_array 
                                                 +   proc->IP + 1))] 
                                                 + *(proc->code_array 
                                                 +   proc->IP + 2));

        proc->IP++;
        proc->IP++;
        proc->IP++;
    } 

    return 0;
    
}

int Add(processor * proc,  bad_stack * stack, int n_param) {

    int elem_1;
    int elem_2;

    Stack_pop(stack);
    elem_1 = stack->last_pop_element;

    Stack_pop(stack);
    elem_2 = stack->last_pop_element;

    int res_1_2 = 0;

    if (*(proc->code_array +  proc->IP) == ADD_CODE) {

        res_1_2 = elem_1 + elem_2;

    } else {

        res_1_2 = elem_2 - elem_1;
    }

    Stack_push(stack, res_1_2);
    proc->IP++;

    return 0;

}

int Div(processor * proc,  bad_stack * stack, int n_param) {

    int elem_1;
    int elem_2;

    Stack_pop(stack);
    elem_1 = stack->last_pop_element;

    Stack_pop(stack);
    elem_2 = stack->last_pop_element;

    int res_1_2 = 0;

    if (elem_1 == 0) {

        printf("ERROR: Division by zero!\n");
        return -1;
    }

    res_1_2 = elem_2 / elem_1;

    Stack_push(stack, res_1_2);
    proc->IP++;

    return 0;
}

int Mul(processor * proc,  bad_stack * stack, int n_param) {

    int elem_1;
    int elem_2;

    Stack_pop(stack);
    elem_1 = stack->last_pop_element;

    Stack_pop(stack);
    elem_2 = stack->last_pop_element;

    int res_1_2 = 0;
    res_1_2 = elem_2 * elem_1;
    
    Stack_push(stack, res_1_2);
    proc->IP++;

    return 0;
}

int Sqrt(processor * proc,  bad_stack * stack, int n_param) {

    int elem_1;

    Stack_pop(stack);
    elem_1 = stack->last_pop_element;

    int res = sqrt((float)elem_1);
    Stack_push(stack, res);
    proc->IP++;

    return 0;
}


int Out(processor * proc,  bad_stack * stack, int n_param) {
    Stack_pop(stack);
    int elem = stack->last_pop_element;
    printf("\n%d\n", elem);
    proc->IP++;
    return 0;
}

int Ja(processor * proc,  bad_stack * stack, int n_param) {

    if (n_param != 1) {
        return -1;
    }
    int elem_1;
    int elem_2;

    Stack_pop(stack);
    elem_1 = stack->last_pop_element;

    Stack_pop(stack);
    elem_2 = stack->last_pop_element;

    if (elem_1 > elem_2) {

         proc->IP = *(proc->code_array + proc->IP + 1);
         
    } else {
        proc->IP++;
        proc->IP++;
    }

    return 0;
}

int Jmp(processor * proc,  bad_stack * stack, int n_param) {

    if (n_param != 1) {
        return -1;
    }

    proc->IP = *(proc->code_array + proc->IP + 1);

    return 0;
}

int Call(processor * proc,  bad_stack * stack, int n_param) {

    if (n_param != 1) {
        return -1;
    }
    Stack_push(&proc->stack_function, proc->IP + 1);

    proc->IP = *(proc->code_array + proc->IP + 1);

    return 0;
}

int Ret(processor * proc,  bad_stack * stack, int n_param) {

    if (n_param != 0) {
        return -1;
    }
    Stack_pop(&proc->stack_function);

    proc->IP = proc->stack_function.last_pop_element+1;

    return 0;
}

void Run_code(processor * proc,  bad_stack * stack) {

    while(proc->IP < proc->code_size + 1) {

        int code_funk = *(proc->code_array + proc->IP) & 31; //00011111

        int n_param = Count_parameter(*(proc->code_array + proc->IP));

        switch (code_funk)
        {
        case START_CODE:
            if (proc->IP < 0){
                printf("START not detected!\n");
                return;
            }

        case PUSH_CODE:
            if(Push(proc, stack, n_param) == -1 ) {
                printf("Error in command number %d\n", proc->IP);
                return;
            }
            break;

        case POP_CODE:
            if(Pop(proc, stack, n_param) == -1 ) {
                printf("Error in command number %d\n", proc->IP);
                return;
            }
            
            break;

        case SUB_CODE:
            if(Add(proc, stack, n_param) == -1 ) {
                printf("Error in command number %d\n", proc->IP);
                return;
            }
            
            break;

        case DIV_CODE:
            if(Div(proc, stack, n_param) == -1 ) {   //TODO: define 
                printf("Error in command number %d\n", proc->IP);
                return;
            }
            
            break;

        case MUL_CODE:
            if(Mul(proc, stack, n_param) == -1 ) {
                printf("Error in command number %d\n", proc->IP);
                return;
            }
            
            break;

        case ADD_CODE:
            if(Add(proc, stack, n_param) == -1 ) {
                printf("Error in command number %d\n", proc->IP);
                return;
            }
            
            break;

        case SQRT_CODE:
            if(Sqrt(proc, stack, n_param) == -1 ) {
                printf("Error in command number %d\n", proc->IP);
                return;
            }
            
            break;

        case OUT_CODE:                               //TODO: arrray whith pointers on struct 
            if(Out(proc, stack, n_param) == -1 ) {
                printf("Error in command number %d\n", proc->IP);
                return;
            }

            break;

        case HLT_CODE:
            return;

        case JMP_CODE:
            if(Jmp(proc, stack, n_param) == -1 ) {
                printf("Error in command number %d\n", proc->IP);
                return;
            }
            
            break;

        case JA_CODE:
            if(Ja(proc, stack, n_param) == -1 ) {
                printf("Error in command number %d", proc->IP);
                return;
            }
            
            break;
        case CALL_CODE:
            if(Call(proc, stack, n_param) == -1 ) {
                printf("Error in command number %d", proc->IP);
                return;
            }
            
            break;

        case RET_CODE:
            if(Ret(proc, stack, n_param) == -1 ) {
                printf("Error in command number %d", proc->IP);
                return;
            }
            
            break;
        
        default:
            printf("funck not detectid, error on command: %d, this command: %d\n", proc->IP, *(proc->code_array + proc->IP));
            return;
        }
        Dumper(stack, "stack", "Run_code");
    }
}
