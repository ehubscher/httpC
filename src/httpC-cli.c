#include "http.h"

int main(/* int argc, char *argv[] */)
{
    char* test = extractPathFromURI("http://google.com/controller/action/method?x1=root");
    free(test);

    return 0;
}
