#ifndef CPU_H
#define CPU_H

#include "stack.h"

enum Commands
{
    CMD_PUSH    = 1,
    CMD_ADD     = 2,
    CMD_SUB     = 3,
    CMD_MUL     = 4,
    CMD_DIV     = 5,
    CMD_OUT     = 6,
    CMD_IN      = 7,
    CMD_SQRT    = 8,
    CMD_SIN     = 9,
    CMD_COS     = 10,
    CMD_DUMP    = 11,
    CMD_HLT     = 12,
    CMD_UNKNOWN = 13
};

enum Errors_of_CPU
{
    ERROR_OF_GETTING_INFORMATION_ABOUT_CMD = 3,
    ERROR_OF_UNKNOWN_CMD                   = 2,
    ERROR_OF_NOT_ENOUGH_ELEMENTS_IN_STACK  = 1,
    NO_CPU_ERRORS                          = 0
};

struct MySPU
{
    int *commands;
    size_t size_of_commands;
    size_t ip;
    MyStack *stack;
    int *registers;
    size_t size_of_registers;
};

Errors_of_CPU create_commands();
Errors_of_CPU do_cmd(struct MySPU *spu);


#endif
