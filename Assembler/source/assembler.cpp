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

static Errors_of_ASM constructor(struct ASM *Asm);
static Errors_of_ASM destructor(struct ASM *Asm);
static Errors_of_ASM get_count_of_rows(struct ASM *Asm);
static Errors_of_ASM parse_word(struct ASM *Asm, int *step, size_t len, size_t index, struct Labels *all_labels, size_t size_of_all_labels);
static Errors_of_ASM parse_label(struct ASM *Asm, int *step, size_t index, struct Labels *all_labels, size_t size_of_all_labels);
static Errors_of_ASM parse_push_pop_cmd(struct ASM *Asm, size_t index);
static Errors_of_ASM parse_jump_cmds(struct ASM *Asm, size_t index);


static Errors_of_ASM parse_word(struct ASM *Asm, int *step, size_t len, size_t index, struct Labels *all_labels, size_t size_of_all_labels)
{
    Errors_of_ASM error = NO_ASM_ERRORS;
    if (Asm == NULL)
    {
        return ERROR_OF_PARSE_WORD;
    }
    if ((Asm->commands)[index].command[len - 1] == ':')
    {
        *step += 1;
        error = parse_label(Asm, step, index, all_labels, size_of_all_labels);
        if (error != NO_ASM_ERRORS)
        {
            return error;
        }
    }

    if (strcasecmp((Asm->commands)[index].command, "push") == 0
        || strcasecmp((Asm->commands)[index].command, "pop")  == 0)
    {
        *step += 1;
        error = parse_push_pop_cmd(Asm, index);
        if (error != NO_ASM_ERRORS)
        {
            return error;
        }
    }
    else if (strcasecmp((Asm->commands)[index].command, "jmp")    == 0
            || strcasecmp((Asm->commands)[index].command, "ja")   == 0
            || strcasecmp((Asm->commands)[index].command, "jae")  == 0
            || strcasecmp((Asm->commands)[index].command, "jb")   == 0
            || strcasecmp((Asm->commands)[index].command, "jbe")  == 0
            || strcasecmp((Asm->commands)[index].command, "je")   == 0
            || strcasecmp((Asm->commands)[index].command, "jne")  == 0)
    {
        *step += 1;
        error = parse_jump_cmds(Asm, index);
        if (error != NO_ASM_ERRORS)
        {
            return error;
        }
    }
    return error;
}

static Errors_of_ASM parse_label(struct ASM *Asm, int *step, size_t index, struct Labels *all_labels, size_t size_of_all_labels)
{
    if (Asm == NULL)
    {
        return ERROR_OF_PARSE_WORD;
    }
    for (size_t j = 0; j < (Asm->table)->size_of_labels; j++)
    {
        if (strcasecmp(((Asm->table)->labels)[j].name, "") == 0)
        {
            for (size_t k = 0; k < size_of_all_labels; k++)
            {
                if (strcasecmp((Asm->commands)[index].command, (all_labels[k]).name) == 0)
                {
                    memcpy(((Asm->table)->labels)[j].name, (all_labels[k]).name, strlen((all_labels[k]).name));
                    ((Asm->table)->labels)[j].address = *step;
                    break;
                }
            }
        }
        break;
    }
    char str[6] = "label";
    memcpy((Asm->commands)[index].command, str, 6);
    return NO_ASM_ERRORS;
}

static Errors_of_ASM parse_push_pop_cmd(struct ASM *Asm, size_t index)
{
    if (Asm == NULL || (Asm->file_pointer) == NULL)
    {
        return ERROR_OF_PARSE_WORD;
    }
    char s[50] = {0};
    fscanf(Asm->file_pointer, "%s", s);
    if (isalpha(s[0]))
    {
        (Asm->commands)[index].element = TOXIC;
        if (strcasecmp(s, "ax") == 0)
        {
            (Asm->commands)[index].reg = AX;
        }
        else if (strcasecmp(s, "bx") == 0)
        {
            (Asm->commands)[index].reg = BX;
        }
        else if (strcasecmp(s, "cx") == 0)
        {
            (Asm->commands)[index].reg = CX;
        }
        else if (strcasecmp(s, "dx") == 0)
        {
            (Asm->commands)[index].reg = DX;
        }
        else
        {
            return ERROR_OF_UNKNOWN_REGISTER;
        }
    }
    else
    {
        (Asm->commands)[index].element = atoi(s);
        (Asm->commands)[index].reg = NOT_A_REGISTER;
    }
    return NO_ASM_ERRORS;
}

static Errors_of_ASM parse_jump_cmds(struct ASM *Asm, size_t index)
{
    if (Asm == NULL || (Asm->file_pointer) == NULL)
    {
        return ERROR_OF_PARSE_WORD;
    }
    char str[50] = {0};
    fscanf(Asm->file_pointer, "%s", str);
    (Asm->commands)[index].reg = NOT_A_REGISTER;
    for (size_t k = 0; k < (Asm->table)->size_of_labels; k++)
    {
        if (strcasecmp(str, ((Asm->table)->labels)[k].name) == 0)
        {
            (Asm->commands)[index].element = ((Asm->table)->labels)[k].address;
            break;
        }
    }
    return NO_ASM_ERRORS;
}

Errors_of_ASM get_commands(struct ASM *Asm, struct Labels *all_labels, size_t size_of_all_labels)
{
    if (Asm == NULL)
    {
        return ERROR_OF_READING_FROM_FILE;
    }
    size_t i = 0;
    int step = 0;
    Errors_of_ASM error = NO_ASM_ERRORS;
    while (i < (Asm->count_of_rows) && fscanf(Asm->file_pointer, "%s", (Asm->commands)[i].command))
    {
        size_t len = strlen((Asm->commands)[i].command);
        error = parse_word(Asm, &step, len, i, all_labels, size_of_all_labels);
        i++;
    }
    return error;
}


Errors_of_ASM transform_commands(struct ASM *Asm, struct CMD *all_commands, size_t size_of_all_commands)
{
    if (Asm == NULL || all_commands == NULL)
    {
        return ERROR_OF_NO_COMMANDS;
    }
    for (size_t i = 0; i < (Asm->count_of_rows); i++)
    {
        bool flag = false;
        for (size_t j = 0; j < size_of_all_commands; j++)
        {
            if (strcasecmp((Asm->commands)[i].command, (all_commands[j]).name) == 0)
            {
                (Asm->commands)[i].transformed_command = (all_commands[j]).transformed_name;
                flag = true;
                break;
            }
        }
        if (!flag)
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
    for (size_t i = 0; i < (Asm->table)->size_of_labels; i++)
    {
        ((Asm->table)->labels)[i].address = -1;
    }
    return NO_ASM_ERRORS;
}


static Errors_of_ASM get_count_of_rows(struct ASM *Asm)
{
    if (Asm == NULL)
    {
        return ERROR_OF_READING_FROM_FILE;
    }
    int c = 0;
    while ((c = getc(Asm->file_pointer)) != EOF)
    {
        if (c == '\n')
        {
            (Asm->count_of_rows)++;
        }
    }
    rewind(Asm->file_pointer);
    return NO_ASM_ERRORS;
}


int main()
{
    printf("Assembler started!\n");

    struct CMD all_commands[] = {{"push", CMD_PUSH},
                                 {"pop",  CMD_POP},
                                 {"add",  CMD_ADD},
                                 {"sub",  CMD_SUB},
                                 {"mul",  CMD_MUL},
                                 {"div",  CMD_DIV},
                                 {"out",  CMD_OUT},
                                 {"in",   CMD_IN},
                                 {"sqrt", CMD_SQRT},
                                 {"sin",  CMD_SIN},
                                 {"cos",  CMD_COS},
                                 {"dump", CMD_DUMP},
                                 {"jmp",  CMD_JMP},
                                 {"ja",   CMD_JA},
                                 {"jae",  CMD_JAE},
                                 {"jb",   CMD_JB},
                                 {"jbe",  CMD_JBE},
                                 {"je",   CMD_JE},
                                 {"jne",  CMD_JNE},
                                 {"label", CMD_LABEL},
                                 {"hlt", CMD_HLT}};
    size_t size_of_all_commands = sizeof(all_commands) / sizeof(CMD);

    struct Labels all_labels[] = {{"NEXT:"},
                                  {"SKIP:"}};
    size_t size_of_all_labels = sizeof(all_labels) / sizeof(Labels);


    struct ASM Asm = {0};
    (Asm.file_pointer) = fopen("Assembler/source/text_cpu_commands.txt", "rb");
    Errors_of_ASM error = get_count_of_rows(&Asm);
    if (error != NO_ASM_ERRORS)
    {
        fprintf(stderr, "error=%d\n", error);
        return 0;
    }

    error = constructor(&Asm);
    if (error != NO_ASM_ERRORS)
    {
        fprintf(stderr, "error=%d\n", error);
        return 0;
    }

    error = get_commands(&Asm, all_labels, size_of_all_labels);
    if (error != NO_ASM_ERRORS)
    {
        fprintf(stderr, "error=%d\n", error);
        return 0;
    }

    error = transform_commands(&Asm, all_commands, size_of_all_commands);
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
