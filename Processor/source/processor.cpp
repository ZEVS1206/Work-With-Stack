#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <ctype.h>

#include "stack.h"
#include "processor.h"


Errors_of_CPU create_commands()
{
    FILE *fp = fopen("Assembler/source/cpu_commands.txt", "r");
    struct MySPU spu = {0};
    struct stat statistics = {0};
    int res = stat("Assembler/source/cpu_commands.txt", &statistics);
    if (res != 0)
    {
        return ERROR_OF_GETTING_INFORMATION_ABOUT_CMD;
    }

    size_t size_of_file = statistics.st_size;
    char *buffer = (char *) calloc(size_of_file, sizeof(char));
    if (buffer == NULL)
    {
        return ERROR_OF_GETTING_INFORMATION_ABOUT_CMD;
    }
    size_t result_of_reading = fread(buffer, sizeof(char), size_of_file, fp);
    size_t size = 0;
    for (size_t i = 0; i < size_of_file; i++)
    {
        if (isspace(buffer[i]))
        {
            size++;
        }
    }
    spu.size_of_commands = size;
    spu.commands = (int *) calloc(size, sizeof(int));
    size_t j = 0;
    size_t i = 0;
    while (i < size_of_file)
    {
        char tmp[10] = {0};
        int k = 0;
        while (!isspace(buffer[i]))
        {
            tmp[k++] = buffer[i];
            i++;
        }
        tmp[k] = '\0';
        (spu.commands)[j++] = atoi(tmp);
        i++;
    }
    Errors_of_CPU error = NO_CPU_ERRORS;
    error = do_cmd(&spu);
    free(buffer);
    return error;
}



Errors_of_CPU do_cmd(struct MySPU *spu)
{
    spu->stack = (struct MyStack *) calloc(1, sizeof(struct MyStack));
    STACK_CTOR(spu->stack, 10);
    Errors error = NO_ERRORS;
    size_t i = 0;
    while (i < spu->size_of_commands)
    {
        Commands cmd = (Commands)(spu->commands)[i];
        switch (cmd)
        {
            case CMD_PUSH:
            {
                i++;
                Stack_Elem_t elem = (spu->commands)[i];
                error = stack_push(spu->stack, elem);
                //special_dump(&stack);
                break;
            }
            case CMD_ADD:
            {
                if ((spu->stack)->size < 1)
                {
                    return ERROR_OF_NOT_ENOUGH_ELEMENTS_IN_STACK;
                }
                Stack_Elem_t first_operand = 0;
                Stack_Elem_t second_operand = 0;
                error = stack_pop(spu->stack, &second_operand);
                error = stack_pop(spu->stack, &first_operand);
                error = stack_push(spu->stack, (first_operand + second_operand));
                //special_dump(&stack);
                break;
            }
            case CMD_SUB:
            {
                if ((spu->stack)->size < 1)
                {
                    return ERROR_OF_NOT_ENOUGH_ELEMENTS_IN_STACK;
                }
                Stack_Elem_t first_operand = 0;
                Stack_Elem_t second_operand = 0;
                error = stack_pop(spu->stack, &second_operand);
                error = stack_pop(spu->stack, &first_operand);
                error = stack_push(spu->stack, (first_operand - second_operand));
                //special_dump(&stack);
                break;
            }
            case CMD_MUL:
            {
                if ((spu->stack)->size < 1)
                {
                    return ERROR_OF_NOT_ENOUGH_ELEMENTS_IN_STACK;
                }
                Stack_Elem_t first_operand = 0;
                Stack_Elem_t second_operand = 0;
                error = stack_pop(spu->stack, &second_operand);
                error = stack_pop(spu->stack, &first_operand);
                error = stack_push(spu->stack, (first_operand * second_operand));
                //special_dump(&stack);
                break;
            }
            case CMD_DIV:
            {
                int first_operand = 0;
                int second_operand = 0;
                error = stack_pop(spu->stack, &second_operand);
                error = stack_pop(spu->stack, &first_operand);
                error = stack_push(spu->stack, (first_operand / second_operand));
                //special_dump(&stack);
                break;
            }
            case CMD_OUT:
            {
                if ((spu->stack)->size - 1 < 0)
                {
                    return ERROR_OF_NOT_ENOUGH_ELEMENTS_IN_STACK;
                }
                Stack_Elem_t element = 0;
                error = stack_element(spu->stack, &element);
                break;
            }
            case CMD_DUMP:
            {
                special_dump(spu->stack);
                break;
            }
            case CMD_IN:
            {
                Stack_Elem_t element = 0;
                printf("Enter the element: ");
                scanf("%d", &element);
                error = stack_push(spu->stack, element);
                break;
            }
            case CMD_SIN:
            {
                if ((spu->stack)->size < 1)
                {
                    return ERROR_OF_NOT_ENOUGH_ELEMENTS_IN_STACK;
                }
                Stack_Elem_t operand = 0;
                error = stack_pop(spu->stack, &operand);
                error = stack_push(spu->stack, ceil(sin(operand)));
                break;
            }
            case CMD_COS:
            {
                if ((spu->stack)->size < 1)
                {
                    return ERROR_OF_NOT_ENOUGH_ELEMENTS_IN_STACK;
                }
                Stack_Elem_t operand = 0;
                error = stack_pop(spu->stack, &operand);
                error = stack_push(spu->stack, ceil(cos(operand)));
                break;
            }
            case CMD_SQRT:
            {
                if ((spu->stack)->size < 1)
                {
                    return ERROR_OF_NOT_ENOUGH_ELEMENTS_IN_STACK;
                }
                Stack_Elem_t operand = 0;
                error = stack_pop(spu->stack, &operand);
                error = stack_push(spu->stack, ceil(sqrt(operand)));
                break;
            }
            case CMD_JMP:
            {
                i++;
                int position = (spu->commands[i]);
                i = position - 1;
                break;
            }
            case CMD_JA:
            {
                i++;
                Stack_Elem_t operand = 0;
                error = stack_element(spu->stack, &operand);
                if (operand > 100)
                {
                    break;
                }
                else
                {
                    int position = (spu->commands[i]);
                    i = position - 1;
                    break;
                }
            }
            case CMD_HLT:
                break;
            case CMD_UNKNOWN:
                return ERROR_OF_UNKNOWN_CMD;
                break;
            default:
                printf("cmd=%d\n", cmd);
                return ERROR_OF_UNKNOWN_TYPE;
                break;
        }
        i++;
    }
    free(spu->commands);
    error = stack_destructor(spu->stack);
    free(spu->stack);
    return NO_CPU_ERRORS;
}

int main()
{
    printf("Processor started!\n");
    Errors_of_CPU error = create_commands();
    fprintf(stderr, "error=%d\n", error);
    printf("Processor finished!\n");
    return 0;
}

