#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>

#include "cpruss.h"

int main(void){
    printf("Started executing!\n");

    char command[200];
    strcpy(command, "cd code && make");
    system(command);
    
    int x = check_module();
    printf("Value: %i\n", x);
    rmmod(0);

    printf("Completed execution!\n");

    return 0;
}
