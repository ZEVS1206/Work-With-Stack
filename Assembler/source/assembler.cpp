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
static Errors_of_ASM constructor(struct ASM *Asm);
static Errors_of_ASM destructor(struct ASM *Asm);

Errors_of_ASM get_commands(struct ASM *Asm, FILE *file_pointer)
{
    if (Asm == NULL || file_pointer == NULL)
    {
        return ERROR_OF_READING_FROM_FILE;
    }
    size_t i = 0;
    int step = 0;
    while (i < (Asm->count_of_rows) && fscanf(file_pointer, "%s", (Asm->commands)[i].command))
    {
        size_t len = strlen((Asm->commands)[i].command);
        //parse_word()
        //parse_label()
        if ((Asm->commands)[i].command[len - 1] == ':')
        {
            step += 1;
            for (size_t j = 0; j < (Asm->table)->size_of_labels; j++)
            {
                if (((Asm->table)->labels)[j].name == EMPTY)
                {
                    if (strcasecmp((Asm->commands)[i].command, "NEXT:") == 0)
                    {
                        ((Asm->table)->labels)[j].name = NEXT;
                        ((Asm->table)->labels)[j].address = step;
                    }
                }
                break;
            }
            char str[6] = "label";
            memcpy((Asm->commands)[i].command, str, 6);
        }

        if (strcasecmp((Asm->commands)[i].command, "push") == 0
         || strcasecmp((Asm->commands)[i].command, "pop")  == 0)
        {
            //parse_push_pop_cmd()
            step += 1;
            char s[50] = {0};
            fscanf(file_pointer, "%s", s);
            if (isalpha(s[0]))
            {
                (Asm->commands)[i].element = TOXIC;
                if (strcasecmp(s, "ax") == 0)
                {
                    (Asm->commands)[i].reg = AX;
                }
                else if (strcasecmp(s, "bx") == 0)
                {
                    (Asm->commands)[i].reg = BX;
                }
                else if (strcasecmp(s, "cx") == 0)
                {
                    (Asm->commands)[i].reg = CX;
                }
                else if (strcasecmp(s, "dx") == 0)
                {
                    (Asm->commands)[i].reg = DX;
                }
                else
                {
                    return ERROR_OF_UNKNOWN_REGISTER;
                }
            }
            else
            {
                (Asm->commands)[i].element = atoi(s);
                (Asm->commands)[i].reg = NOT_A_REGISTER;
            }
        }
        else if (strcasecmp((Asm->commands)[i].command, "jmp")    == 0
                || strcasecmp((Asm->commands)[i].command, "ja")   == 0
                || strcasecmp((Asm->commands)[i].command, "jae")  == 0
                || strcasecmp((Asm->commands)[i].command, "jb")   == 0
                || strcasecmp((Asm->commands)[i].command, "jbe")  == 0
                || strcasecmp((Asm->commands)[i].command, "je")   == 0
                || strcasecmp((Asm->commands)[i].command, "jne")  == 0)
        {
            //parse_jump_cmd()
            step += 1;
            char str[50] = {0};
            fscanf(file_pointer, "%s", str);
            (Asm->commands)[i].reg = NOT_A_REGISTER;
            Label_name label = EMPTY;
            if (strcasecmp(str, "NEXT:") == 0)
            {
                label = NEXT;
            }
            else if (strcasecmp(str, "SKIP:") == 0)
            {
                label = SKIP;
            }
            for (size_t k = 0; k < (Asm->table)->size_of_labels; k++)
            {
                if (((Asm->table)->labels)[k].name == label)
                {
                    (Asm->commands)[i].element = ((Asm->table)->labels)[k].address;
                    break;
                }
            }
        }
        //step += 1;
        i++;
    }
    return NO_ASM_ERRORS;
}


Errors_of_ASM transform_commands(struct ASM *Asm)
{
    if (Asm == NULL)
    {
        return ERROR_OF_NO_COMMANDS;
    }
    for (size_t i = 0; i < (Asm->count_of_rows); i++)
    {
        if (strcasecmp((Asm->commands)[i].command, "push") == 0)
        {
            (Asm->commands)[i].transformed_command = CMD_PUSH;
        }
        else if (strcasecmp((Asm->commands)[i].command, "pop") == 0)
        {
            (Asm->commands)[i].transformed_command = CMD_POP;
        }
        else if (strcasecmp((Asm->commands)[i].command, "add") == 0)
        {
            (Asm->commands)[i].transformed_command = CMD_ADD;
        }
        else if (strcasecmp((Asm->commands)[i].command, "sub") == 0)
        {
            (Asm->commands)[i].transformed_command = CMD_SUB;
        }
        else if (strcasecmp((Asm->commands)[i].command, "mul") == 0)
        {
            (Asm->commands)[i].transformed_command = CMD_MUL;
        }
        else if (strcasecmp((Asm->commands)[i].command, "div") == 0)
        {
            (Asm->commands)[i].transformed_command = CMD_DIV;
        }
        else if (strcasecmp((Asm->commands)[i].command, "out") == 0)
        {
            (Asm->commands)[i].transformed_command = CMD_OUT;
        }
        else if (strcasecmp((Asm->commands)[i].command, "dump") == 0)
        {
            (Asm->commands)[i].transformed_command = CMD_DUMP;
        }
        else if (strcasecmp((Asm->commands)[i].command, "in") == 0)
        {
            (Asm->commands)[i].transformed_command = CMD_IN;
        }
        else if (strcasecmp((Asm->commands)[i].command, "sqrt") == 0)
        {
            (Asm->commands)[i].transformed_command = CMD_SQRT;
        }
        else if (strcasecmp((Asm->commands)[i].command, "sin") == 0)
        {
            (Asm->commands)[i].transformed_command = CMD_SIN;
        }
        else if (strcasecmp((Asm->commands)[i].command, "cos") == 0)
        {
            (Asm->commands)[i].transformed_command = CMD_COS;
        }
        else if (strcasecmp((Asm->commands)[i].command, "jmp") == 0)
        {
            (Asm->commands)[i].transformed_command = CMD_JMP;
        }
        else if (strcasecmp((Asm->commands)[i].command, "ja") == 0)
        {
            (Asm->commands)[i].transformed_command = CMD_JA;
        }
        else if (strcasecmp((Asm->commands)[i].command, "jae") == 0)
        {
            (Asm->commands)[i].transformed_command = CMD_JAE;
        }
        else if (strcasecmp((Asm->commands)[i].command, "jb") == 0)
        {
            (Asm->commands)[i].transformed_command = CMD_JB;
        }
        else if (strcasecmp((Asm->commands)[i].command, "jbe") == 0)
        {
            (Asm->commands)[i].transformed_command = CMD_JBE;
        }
        else if (strcasecmp((Asm->commands)[i].command, "je") == 0)
        {
            (Asm->commands)[i].transformed_command = CMD_JE;
        }
        else if (strcasecmp((Asm->commands)[i].command, "jne") == 0)
        {
            (Asm->commands)[i].transformed_command = CMD_JNE;
        }
        else if (strcasecmp((Asm->commands)[i].command, "hlt") == 0)
        {
            (Asm->commands)[i].transformed_command = CMD_HLT;
        }
        else if (strcasecmp((Asm->commands)[i].command, "label") == 0)
        {
            (Asm->commands)[i].transformed_command = CMD_LABEL;
        }
        else
        {
            (Asm->commands)[i].transformed_command = CMD_UNKNOWN;
        }
    }
    return NO_ASM_ERRORS;
}

Errors_of_ASM create_file_with_commands(struct ASM *Asm)
{
    FILE *fp = fopen("Assembler/source/cpu_commands.txt", "w");
    if (Asm == NULL || fp == NULL)
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
    for (size_t i = 0; i < (Asm->count_of_rows); i++)
    {
        if ((Asm->commands)[i].transformed_command == CMD_LABEL)
        {
            continue;
        }

        if ((Asm->commands)[i].transformed_command == CMD_PUSH
         || (Asm->commands)[i].transformed_command == CMD_POP)
        {
            fprintf(fp, "%d %d %d\n", (Asm->commands)[i].transformed_command, (Asm->commands)[i].element, (Asm->commands)[i].reg);
        }
        else if ((Asm->commands)[i].transformed_command == CMD_JMP
              || (Asm->commands)[i].transformed_command == CMD_JA
              || (Asm->commands)[i].transformed_command == CMD_JAE
              || (Asm->commands)[i].transformed_command == CMD_JB
              || (Asm->commands)[i].transformed_command == CMD_JBE
              || (Asm->commands)[i].transformed_command == CMD_JE
              || (Asm->commands)[i].transformed_command == CMD_JNE)
        {
            fprintf(fp, "%d %d\n", (Asm->commands)[i].transformed_command, (Asm->commands)[i].element);
        }
        else
        {
            fprintf(fp, "%d\n", (Asm->commands)[i].transformed_command);
        }
    }
    return NO_ASM_ERRORS;
}


static Errors_of_ASM constructor(struct ASM *Asm)
{
    if (Asm == NULL)
    {
        return ERROR_OF_CONSTRUCTOR;
    }
    Asm->commands = (Command *) calloc(Asm->count_of_rows, sizeof(Command));
    if (Asm->commands == NULL)
    {
        return ERROR_OF_CONSTRUCTOR;
    }
    Asm->table = (Table_labels *) calloc(1, sizeof(Table_labels));
    if (Asm->table == NULL)
    {
        return ERROR_OF_CONSTRUCTOR;
    }
    (Asm->table)->size_of_labels = (size_t)num_of_labels;
    (Asm->table)->labels = (Label *) calloc((Asm->table)->size_of_labels, sizeof(Label));
    if ((Asm->table)->labels == NULL)
    {
        return ERROR_OF_CONSTRUCTOR;
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
    struct ASM Asm = {0};
    Asm.count_of_rows = count_of_rows;
    Errors_of_ASM error = constructor(&Asm);
    if (error != NO_ASM_ERRORS)
    {
        fprintf(stderr, "error=%d\n", error);
        return 0;
    }
    error = get_commands(&Asm, fp);
    if (error != NO_ASM_ERRORS)
    {
        fprintf(stderr, "error=%d\n", error);
        return 0;
    }
    error = transform_commands(&Asm);
    if (error != NO_ASM_ERRORS)
    {
        fprintf(stderr, "error=%d\n", error);
        return 0;
    }
    error = create_file_with_commands(&Asm);
    if (error != NO_ASM_ERRORS)
    {
        fprintf(stderr, "error=%d\n", error);
        return 0;
    }
    error = destructor(&Asm);
    if (error != NO_ASM_ERRORS)
    {
        fprintf(stderr, "error=%d\n", error);
        return 0;
    }
    printf("Assembler finished!\n");
    return 0;
}



static Errors_of_ASM destructor(struct ASM *Asm)
{
    if (Asm == NULL)
    {
        return ERROR_OF_DESTRUCTOR;
    }
    Asm->count_of_rows = 0;
    free(Asm->commands);
    (Asm->table)->size_of_labels = 0;
    free((Asm->table)->labels);
    free(Asm->table);
    return NO_ASM_ERRORS;
}
