#include <stdio.h>
#include <stdlib.h>

#include "stack.h"
#include "processor.h"

int main(){
    Errors_of_CPU error = do_cmd();
    printf("%d\n", error);
    return 0;
}


