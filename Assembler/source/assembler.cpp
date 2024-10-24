#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>

#include "../../Processor/include/processor.h"
#include "assembler.h"

#define SIZE 10

static const int num_of_labels = 10;

//array of structures
static Errors_of_ASM destructor(struct Command *commands);

Errors_of_ASM get_commands(struct Command *commands, size_t count_of_rows, FILE *file_pointer, struct Table_labels *table)
{
    if (commands == NULL || file_pointer == NULL)
    {
        return ERROR_OF_READING_FROM_FILE;
    }
    size_t i = 0;
    int step = 0;
    while (i < count_of_rows && fscanf(file_pointer, "%s", commands[i].command))
    {
        size_t len = strlen(commands[i].command);
        //parse_word()
        //parse_label()
        if (commands[i].command[len - 1] == ':')
        {
            step += 1;
            for (size_t j = 0; j < table->size_of_labels; j++)
            {
                if ((table->labels)[j].name == EMPTY)
                {
                    if (strcasecmp(commands[i].command, "NEXT:") == 0)
                    {
                        (table->labels)[j].name = NEXT;
                        (table->labels)[j].address = step;
                    }
                }
                break;
            }
            char str[6] = "label";
            memcpy(commands[i].command, str, 6);
        }

        if (strcasecmp(commands[i].command, "push") == 0
         || strcasecmp(commands[i].command, "pop")  == 0)
        {
            //parse_push_pop_cmd()
            step += 1;
            char s[50] = {0};
            fscanf(file_pointer, "%s", s);
            if (isalpha(s[0]))
            {
                commands[i].element = TOXIC;
                if (strcasecmp(s, "ax") == 0)
                {
                    commands[i].reg = AX;
                }
                else if (strcasecmp(s, "bx") == 0)
                {
                    commands[i].reg = BX;
                }
                else if (strcasecmp(s, "cx") == 0)
                {
                    commands[i].reg = CX;
                }
                else if (strcasecmp(s, "dx") == 0)
                {
                    commands[i].reg = DX;
                }
                else
                {
                    return ERROR_OF_UNKNOWN_REGISTER;
                }
            }
            else
            {
                commands[i].element = atoi(s);
                commands[i].reg = NOT_A_REGISTER;
            }
        }
        else if (strcasecmp(commands[i].command, "jmp")  == 0
                || strcasecmp(commands[i].command, "ja")   == 0
                || strcasecmp(commands[i].command, "jae")  == 0
                || strcasecmp(commands[i].command, "jb")   == 0
                || strcasecmp(commands[i].command, "jbe")  == 0
                || strcasecmp(commands[i].command, "je")   == 0
                || strcasecmp(commands[i].command, "jne")  == 0)
        {
            //parse_jump_cmd()
            step += 1;
            char str[50] = {0};
            fscanf(file_pointer, "%s", str);
            commands[i].reg = NOT_A_REGISTER;
            Label_name label = EMPTY;
            if (strcasecmp(str, "NEXT:") == 0)
            {
                label = NEXT;
            }
            else if (strcasecmp(str, "SKIP:") == 0)
            {
                label = SKIP;
            }
            for (size_t k = 0; k < table->size_of_labels; k++)
            {
                if ((table->labels)[k].name == label)
                {
                    commands[i].element = (table->labels)[k].address;
                    break;
                }
            }
        }
        //step += 1;
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
        else if (strcasecmp(commands[i].command, "pop") == 0)
        {
            commands[i].transformed_command = CMD_POP;
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
        else if (strcasecmp(commands[i].command, "label") == 0)
        {
            commands[i].transformed_command = CMD_LABEL;
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
    fprintf(fp, "ZEVS_GROM\n");
    fprintf(fp, "Vesrion %d\n", VERSION);
    time_t my_time = time(NULL);
    struct tm *now = localtime(&my_time);
    char current_time[20] = {};
    char date[20] = {};
    char day[20] = {};
    char month[20] = {};
    strftime(month, sizeof(month), "%B", now);
    strftime(day, sizeof(day), "%A", now);
    strftime(current_time, sizeof(current_time), "%T", now);
    strftime(date, sizeof(date), "%D", now);
    fprintf(fp, "Date: %s\n", date);
    fprintf(fp, "Day: %s\n", day);
    fprintf(fp, "Month: %s\n", month);
    fprintf(fp, "Time: %s\n", current_time);
    fprintf(fp, "________________________________________________________________\n");
    for (size_t i = 0; i < count_of_rows; i++)
    {
        if (commands[i].transformed_command == CMD_LABEL)
        {
            continue;
        }
        if (commands[i].transformed_command == CMD_PUSH
         || commands[i].transformed_command == CMD_POP)
        {
            fprintf(fp, "%d %d %d\n", commands[i].transformed_command, commands[i].element, commands[i].reg);
        }
        else if (commands[i].transformed_command == CMD_JMP
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
    // get_count_of_rows()
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

    // c...tor
    struct Command *commands = (Command *) calloc(count_of_rows, sizeof(Command));
    struct Table_labels table = {0};
    table.size_of_labels = (size_t)num_of_labels;
    table.labels = (Label *) calloc(table.size_of_labels, sizeof(Label));

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
    error = get_commands(commands, count_of_rows, fp, &table);
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
