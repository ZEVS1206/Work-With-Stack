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
    FILE *fp = fopen("source/cpu_commands.txt", "r");
    struct MySPU spu = {0};
    struct stat statistics = {0};
    int res = stat("source/cpu_commands.txt", &statistics);
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
    struct MyStack stack = {0};
    STACK_CTOR(&stack, 10);
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
                int elem = (spu->commands)[i];
                error = stack_push(&stack, elem);
                //special_dump(&stack);
                break;
            }
            case CMD_ADD:
            {
                if (stack.size < 1)
                {
                    return ERROR_OF_NOT_ENOUGH_ELEMENTS_IN_STACK;
                }
                int first_operand = 0;
                int second_operand = 0;
                error = stack_pop(&stack, &second_operand);
                error = stack_pop(&stack, &first_operand);
                error = stack_push(&stack, (first_operand + second_operand));
                //special_dump(&stack);
                break;
            }
            case CMD_SUB:
            {
                if (stack.size < 1)
                {
                    return ERROR_OF_NOT_ENOUGH_ELEMENTS_IN_STACK;
                }
                int first_operand = 0;
                int second_operand = 0;
                error = stack_pop(&stack, &second_operand);
                error = stack_pop(&stack, &first_operand);
                error = stack_push(&stack, (first_operand - second_operand));
                //special_dump(&stack);
                break;
            }
            case CMD_MUL:
            {
                if (stack.size < 1)
                {
                    return ERROR_OF_NOT_ENOUGH_ELEMENTS_IN_STACK;
                }
                int first_operand = 0;
                int second_operand = 0;
                error = stack_pop(&stack, &second_operand);
                error = stack_pop(&stack, &first_operand);
                error = stack_push(&stack, (first_operand * second_operand));
                //special_dump(&stack);
                break;
            }
            case CMD_DIV:
            {
                if (stack.size < 1)
                {
                    return ERROR_OF_NOT_ENOUGH_ELEMENTS_IN_STACK;
                }
                int first_operand = 0;
                int second_operand = 0;
                error = stack_pop(&stack, &second_operand);
                error = stack_pop(&stack, &first_operand);
                error = stack_push(&stack, (first_operand / second_operand));
                //special_dump(&stack);
                break;
            }
            case CMD_OUT:
            {
                if (stack.size - 1 < 0)
                {
                    return ERROR_OF_NOT_ENOUGH_ELEMENTS_IN_STACK;
                }
                error = stack_element(&stack);
                break;
            }
            case CMD_DUMP:
            {
                special_dump(&stack);
                break;
            }
            case CMD_HLT:
                break;
            default:
                return ERROR_OF_UNKNOWN_CMD;
                break;
        }
        i++;
    }
    free(spu->commands);
    error = stack_destructor(&stack);
    return NO_CPU_ERRORS;
}

