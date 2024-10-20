#ifndef ASM_H
#define ASM_H

#include "../../Processor/include/processor.h"

enum Errors_of_ASM
{
    NO_ASM_ERRORS              = 0,
    ERROR_OF_UNKNOWN_TEXT_CMD  = 1,
    ERROR_OF_READING_FROM_FILE = 2,
    ERROR_OF_NO_COMMANDS       = 3,
    ERROR_OF_CREATING_OUT_FILE = 4,
    ERROR_OF_DESTRUCTOR        = 5,
    ERROR_OF_CREATE_ARRAY      = 6
};

struct Command
{
    char command[50];
    Commands transformed_command;
    int element;
};


Errors_of_ASM get_commands(struct Command *commands, size_t count_of_rows, FILE *file_pointer);
Errors_of_ASM transform_commands(struct Command *commands, size_t count_of_rows);
Errors_of_ASM create_file_with_commands(struct Command *commands, size_t count_of_rows);

#endif

