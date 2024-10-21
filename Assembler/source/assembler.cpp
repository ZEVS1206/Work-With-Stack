#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "../../Processor/include/processor.h"
#include "assembler.h"

static Errors_of_ASM destructor(struct Command *commands);

Errors_of_ASM get_commands(struct Command *commands, size_t count_of_rows, FILE *file_pointer)
{
    if (commands == NULL || file_pointer == NULL)
    {
        return ERROR_OF_READING_FROM_FILE;
    }
    size_t i = 0;
    while (i < count_of_rows && fscanf(file_pointer, "%s", commands[i].command))
    {
        if (strcasecmp(commands[i].command, "push") == 0
         || strcasecmp(commands[i].command, "jmp")  == 0
         || strcasecmp(commands[i].command, "ja")   == 0
         || strcasecmp(commands[i].command, "jae")  == 0
         || strcasecmp(commands[i].command, "jb")   == 0
         || strcasecmp(commands[i].command, "jbe")  == 0
         || strcasecmp(commands[i].command, "je")   == 0
         || strcasecmp(commands[i].command, "jne")  == 0)
        {
            fscanf(file_pointer, "%d", &(commands[i].element));
        }
        i++;
    }
    return NO_ASM_ERRORS;
}


Errors_of_ASM transform_commands(struct Command *commands, size_t count_of_rows)
{
    if (commands == NULL)
    {
        return ERROR_OF_NO_COMMANDS;
    }
    for (size_t i = 0; i < count_of_rows; i++)
    {
        if (strcasecmp(commands[i].command, "push") == 0)
        {
            commands[i].transformed_command = CMD_PUSH;
        }
        else if (strcasecmp(commands[i].command, "add") == 0)
        {
            commands[i].transformed_command = CMD_ADD;
        }
        else if (strcasecmp(commands[i].command, "sub") == 0)
        {
            commands[i].transformed_command = CMD_SUB;
        }
        else if (strcasecmp(commands[i].command, "mul") == 0)
        {
            commands[i].transformed_command = CMD_MUL;
        }
        else if (strcasecmp(commands[i].command, "div") == 0)
        {
            commands[i].transformed_command = CMD_DIV;
        }
        else if (strcasecmp(commands[i].command, "out") == 0)
        {
            commands[i].transformed_command = CMD_OUT;
        }
        else if (strcasecmp(commands[i].command, "dump") == 0)
        {
            commands[i].transformed_command = CMD_DUMP;
        }
        else if (strcasecmp(commands[i].command, "in") == 0)
        {
            commands[i].transformed_command = CMD_IN;
        }
        else if (strcasecmp(commands[i].command, "sqrt") == 0)
        {
            commands[i].transformed_command = CMD_SQRT;
        }
        else if (strcasecmp(commands[i].command, "sin") == 0)
        {
            commands[i].transformed_command = CMD_SIN;
        }
        else if (strcasecmp(commands[i].command, "cos") == 0)
        {
            commands[i].transformed_command = CMD_COS;
        }
        else if (strcasecmp(commands[i].command, "jmp") == 0)
        {
            commands[i].transformed_command = CMD_JMP;
        }
        else if (strcasecmp(commands[i].command, "ja") == 0)
        {
            commands[i].transformed_command = CMD_JA;
        }
        else if (strcasecmp(commands[i].command, "jae") == 0)
        {
            commands[i].transformed_command = CMD_JAE;
        }
        else if (strcasecmp(commands[i].command, "jb") == 0)
        {
            commands[i].transformed_command = CMD_JB;
        }
        else if (strcasecmp(commands[i].command, "jbe") == 0)
        {
            commands[i].transformed_command = CMD_JBE;
        }
        else if (strcasecmp(commands[i].command, "je") == 0)
        {
            commands[i].transformed_command = CMD_JE;
        }
        else if (strcasecmp(commands[i].command, "jne") == 0)
        {
            commands[i].transformed_command = CMD_JNE;
        }
        else if (strcasecmp(commands[i].command, "hlt") == 0)
        {
            commands[i].transformed_command = CMD_HLT;
        }
        else
        {
            commands[i].transformed_command = CMD_UNKNOWN;
        }
    }
    return NO_ASM_ERRORS;
}

Errors_of_ASM create_file_with_commands(struct Command *commands, size_t count_of_rows)
{
    FILE *fp = fopen("Assembler/source/cpu_commands.txt", "w");
    if (commands == NULL || fp == NULL)
    {
        return ERROR_OF_CREATING_OUT_FILE;
    }
    for (size_t i = 0; i < count_of_rows; i++)
    {
        if (commands[i].transformed_command == CMD_PUSH
         || commands[i].transformed_command == CMD_JMP
         || commands[i].transformed_command == CMD_JA
         || commands[i].transformed_command == CMD_JAE
         || commands[i].transformed_command == CMD_JB
         || commands[i].transformed_command == CMD_JBE
         || commands[i].transformed_command == CMD_JE
         || commands[i].transformed_command == CMD_JNE)
        {
            fprintf(fp, "%d %d\n", commands[i].transformed_command, commands[i].element);
        }
        else
        {
            fprintf(fp, "%d\n", commands[i].transformed_command);
        }
    }
    return NO_ASM_ERRORS;
}



int main()
{
    printf("Assembler started!\n");
    FILE *fp = fopen("Assembler/source/text_cpu_commands.txt", "rb");
    int c = 0;
    size_t count_of_rows = 0;
    while ((c = getc(fp)) != EOF)
    {
        if (c == '\n')
        {
            count_of_rows++;
        }
    }
    rewind(fp);
    struct Command *commands = (Command *) calloc(count_of_rows, sizeof(Command));
    Errors_of_ASM error = NO_ASM_ERRORS;
    if (commands == NULL)
    {
        error = ERROR_OF_CREATE_ARRAY;
    }
    if (error != NO_ASM_ERRORS)
    {
        fprintf(stderr, "error=%d\n", error);
        return 0;
    }
    error = get_commands(commands, count_of_rows, fp);
    if (error != NO_ASM_ERRORS)
    {
        fprintf(stderr, "error=%d\n", error);
        return 0;
    }
    error = transform_commands(commands, count_of_rows);
    if (error != NO_ASM_ERRORS)
    {
        fprintf(stderr, "error=%d\n", error);
        return 0;
    }
    error = create_file_with_commands(commands, count_of_rows);
    if (error != NO_ASM_ERRORS)
    {
        fprintf(stderr, "error=%d\n", error);
        return 0;
    }
    error = destructor(commands);
    if (error != NO_ASM_ERRORS)
    {
        fprintf(stderr, "error=%d\n", error);
        return 0;
    }
    printf("Assembler finished!\n");
    return 0;
}



static Errors_of_ASM destructor(struct Command *commands)
{
    if (commands == NULL)
    {
        return ERROR_OF_DESTRUCTOR;
    }
    free(commands);
    return NO_ASM_ERRORS;
}
