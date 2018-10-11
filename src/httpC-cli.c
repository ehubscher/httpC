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
    HttpRequest* requestPtr = &request;
    HttpMessage* message;
    int dflag = 0;
    int fflag = 0;
    int vflag = 0;
    char* url = argv[argc - 1];

    if (strncmp(argv[1], "get", 3) == 0) {
        method = "GET";
        optstring = "vh:";
    }

    else if (strncmp(argv[1], "post", 4) == 0) {
        method = "POST";
        optstring = "vh:d:f:";
    }

    else {
        print_usage();
    }

    request.method = method;
<<<<<<< HEAD

    char* newUrl = NULL;
    if(strcmp(extractProtocolFromURI(url), "http") == 0) {
        newUrl = (char*)malloc(strlen(url) - 6);
        memcpy(newUrl, url + 7, strlen(url) - 6);
    }
    
    request.requestURI = newUrl;
=======
    request.requestBody = NULL;
    request.requestURI = url;
>>>>>>> f362106813aee898d8fafd1cb4d8e38a25f9f549
    request.protocolVersion = "1.0";
    request.headers = NULL;
    request.headersSize = 0;

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
                    printf("Either [-d] or [-f] can be used but not both.\n");
                    help(2);
                }
                else {
                    dflag++;
                    fflag++;
                    request.requestBody = optarg;
                    printf("Inline data has been associated to the request body: %s\n", request.requestBody);
                }
                break;

            case 'f':
                if (fflag) {
                    printf("Either [-d] or [-f] can be used but not both.\n");
                    help(2);
                }
                else {
                    dflag++;
                    fflag++;

                    char* string = readFile(optarg);
                    int size = readFileSize(optarg);
                    
                    char tmpString[size];
                    memcpy(tmpString, string, size);
                    free(string);

                    request.requestBody = tmpString;
                    if (request.requestBody) {
                        printf("File data stored in request body: %s\n", request.requestBody);
                    }
                }
                break;

            default:
                printf("error");
        }
    }

    message = constructHttpMessageFromRequest(requestPtr);
    constructHttpMessage(message);

    sendMessage(message);
    receiveMessage(message, 100);
<<<<<<< HEAD
=======

>>>>>>> f362106813aee898d8fafd1cb4d8e38a25f9f549
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
    return 0;
}
