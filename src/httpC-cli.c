#include <stdlib.h>
#include <stdio.h>
#include "http.h"

int main(/* int argc, char *argv[] */) {
    char* hello = (char*)malloc(strlen("HELLO" + 1));

    strcpy(hello, "HELLO");
    hello = concat(hello, " WORLD!");
    hello = concat(hello, " WORLD!");

    printf("\n\n%s\n\n", hello);

    free(hello);
    
    hello = NULL;
    printf("\n\n%s\n\n", hello);

    return 0;
}
