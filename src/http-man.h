#ifndef HTTP_MAN
#define HTTP_MAN
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void help(int c);
void help(int c) {
    switch (c) {
        case 1:
            printf("usage: httpc get [-v] [-h key:value] URL\n \
                Get executes a HTTP GET request for a given URL.\n \
                \t-v            Prints the detail of the response such as protocol, status, and headers.\n \
                \t-h key:value  Associates headers to HTTP Request with the format 'key:value'.");
            break;
        case 2:
            printf("usage: httpc post [-v] [-h key:value] [-d inline-data] [-f file] URL\n \
                Post executes a HTTP POST request for a given URL with inline data or from\n \
                file.\n \
                \t-v            Prints the detail of the response such as protocol, status, and headers.\n \
                \t-h key:value  Associates headers to HTTP Request with the format 'key:value'.\n \
                \t-d string     Associates an inline data to the body HTTP POST request.\n \
                \t-f file       Associates the content of a file to the body HTTP POST request.\n \
                Either [-d] or [-f] can be used but not both.");
            break;
        default:
            printf("httpc is a curl-like application but supports HTTP protocol only.\n \
                Usage:\n \
                \thttpc command [arguments]\n \
                The commands are:\n \
                \tget     executes a HTTP GET request and prints the response.\n \
                \tpost    executes a HTTP POST request and prints the response.\n \
                \thelp    prints this screen.\n \
                Use \"httpc help [command]\" for more information about a command.\n");
    }
}

#endif