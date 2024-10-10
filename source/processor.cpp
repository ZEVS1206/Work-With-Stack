#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#include "stack.h"
#include "processor.h"

Errors_of_CPU do_cmd()
{
    struct MyStack stack = {0};
    STACK_CTOR(&stack, 10);
    Errors error = NO_ERRORS;
    bool fl = true;
    while (fl)
    {
        char cmd[50] = "";
        scanf("%s", cmd);
        if (strcasecmp(cmd, "push") == 0)
        {
            int elem = 0;
            scanf("%d", &elem);
            error = stack_push(&stack, elem);
        } else if (strcasecmp(cmd, "pop") == 0)
        {
            int last_element = 0;
            error = stack_pop(&stack, &last_element);
        } else if (strcasecmp(cmd, "add") == 0)
        {
            if (stack.size < 1)
            {
                return ERROR_OF_NOT_ENOUGH_ELEMENTS_IN_STACK;
            }
            int first_operand = 0;
            int second_operand = 0;
            error = stack_pop(&stack, &second_operand);
            error = stack_pop(&stack, &first_operand);
            printf("first_operand=%d\nsecond_operand=%d\n", first_operand, second_operand);
            error = stack_push(&stack, (first_operand + second_operand));
        } else if (strcasecmp(cmd, "sub") == 0)
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
        } else if (strcasecmp(cmd, "mul") == 0)
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
        } else if (strcasecmp(cmd, "div") == 0)
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
        } else if (strcasecmp(cmd, "hlt") == 0)
        {
            fl = false;
        } else
        {
            return ERROR_OF_UNKNOWN_CMD;
        }
    }
    error = stack_destructor(&stack);
    return NO_CPU_ERRORS;
}

