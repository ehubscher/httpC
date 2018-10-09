#include "http.h"

int main(/* int argc, char *argv[] */)
{
    char*** test = extractQueryStringFromURI("http://google.com/controller/action?x1=root");
    free(test);

    return 0;
}
