#include "http.h"
#include "httpRequest.h"
#include "http-man.h"
#include "httpc-helper.h"

#include <unistd.h>
#include <getopt.h>

void print_usage() {
    printf("Usage: httpc get [-v] [-h key:value] URL | httpc post [-v] [-h key:value] [-d inline-data] [-f file] URL\n");
    exit(2);
}

int main(int argc, char *argv[]) {

    // returns the appropriate help man
    if (strncmp(argv[1], "help", 4) == 0) {
        if (argc < 3)
        {
            help(0);
        }
        else if (strncmp(argv[2], "get", 3) == 0)
        {
            help(1);
        }
        else if (strncmp(argv[2], "post", 4) == 0)
        {
            help(2);
        }
        else
        {
            help(0);
        }
        return 0;
    }

    char* method;
    int option;
    char* optstring;
    HttpRequest request;
    int dflag = 0;
    int fflag = 0;
    int vflag = 0;
    char* url = argv[argc - 1];
    FILE *file;

    if (strncmp(argv[1], "get", 3) == 0) {
        method = "get";
        optstring = "vh:";
    }

    else if (strncmp(argv[1], "post", 4) == 0) {
        method = "post";
        optstring = "vh:df:";
    }

    else {
        print_usage();
    }

    request.method = method;
    request.requestURI = url;

    // starts evaluating options after argv[1]
    // argv[1] is help | get | post
    optind = 2;
    while ((option = getopt(argc, argv, optstring)) != -1) {
        switch (option) {
            case 'v':
                vflag = 1;
                break;
            case 'h':
                request.headersSize = request.headersSize + 1;
                request.headers = (char**)realloc(request.headers, request.headersSize * sizeof(char*));
                concat(request.headers[request.headersSize - 1], optarg); //atoi()
                break;
            case 'd':
                if (dflag) {
                    help(2);
                }
                else {
                    dflag++;
                    fflag++;
                    printf("you want inline data\n");
                }
                break;
            case 'f':
                if (fflag) {
                    help(2);
                }
                else {
                    dflag++;
                    fflag++;
                    printf("you want file");
                    request.requestBody = readFile(optarg);
                }
                break;
            default:
                printf("error");
        }
    }

    HttpMessage* message;
    message = constructHttpMessageFromRequest(&request);

    sendMessage(message);

    /*
    // Parse the url to get the individual parts
    parse_url();

    // open connection to server
    make_connection();

    // build a request object and send it
    make_request();

    // get a response and make a response object
    fetch_response();

    // close the connection
    close(socket);

    // if -v
    make_verbose();

    // return the response
    return_response();
*/
    free(request.requestBody);
    return 0;
}
