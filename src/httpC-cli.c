#include "http.h"
#include "http-man.h"
#include "httpc-helper.h"

#include <unistd.h>
#include <getopt.h>

void print_usage() {
    printf("Usage: httpc get [-v] [-h key:value] URL | httpc post [-v] [-h key:value] [-d inline-data] [-f file] URL\n");
    exit(2);
}

int main(int argc, char *argv[]) {
    char* url = argv[argc - 1];
    char* newUrl = NULL;
    char method[8];
    int sockfd;

    int option;
    char* optstring;
    int dflag = 0;
    int fflag = 0;
    int vflag = 0;

    char* http_message = NULL;
    char* request_line = NULL;
    char* headers = NULL;
    char* message_body = NULL;

    // returns the appropriate help man
    if (strncmp(argv[1], "help", 4) == 0) {
        if (argc < 3) {
            help(0);
        } else if (strncmp(argv[2], "get", 3) == 0) {
            help(1);
        } else if (strncmp(argv[2], "post", 4) == 0) {
            help(2);
        } else {
            help(0);
        }

        return 0;
    }

    if(strcmp(extractProtocolFromURI(url), "http") == 0) {
        newUrl = (char*)malloc(strlen(url) - 6);
        memcpy(newUrl, url + 7, strlen(url) - 6);
    } else {
        newUrl = url;
    }
    
    if (strncmp(argv[1], "get", 3) == 0) {
        memcpy(method, "GET", 4);
        optstring = "vh:";
    } else if (strncmp(argv[1], "post", 4) == 0) {
        memcpy(method, "POST", 5);
        optstring = "vh:d:f:";
    } else {
        print_usage();
    }

    request_line = concat(request_line, method);
    request_line = concat(request_line, " ");
    request_line = concat(request_line, newUrl);
    request_line = concat(request_line, " ");
    request_line = concat(request_line, "HTTP/1.0 \r\n");

    // starts evaluating options after argv[1]
    // argv[1] is help | get | post
    optind = 2;
    while ((option = getopt(argc, argv, optstring)) != -1) {
        switch (option) {
            case 'v':
                vflag = 1;
                break;

            case 'h':
                headers = concat(headers, optarg);
                headers = concat(headers, "\r\n");
                break;

            case 'd':
                if (dflag) {
                    printf("Either [-d] or [-f] can be used but not both.\n");
                    help(2);
                } else {
                    dflag++;
                    fflag++;

                    if (message_body != NULL) {
                        message_body = concat(message_body, "&");
                    }
                    
                    message_body = concat(message_body, optarg);

                    printf("Inline data has been associated to the request body: %s\n", message_body);
                }

                break;

            case 'f':
                if (fflag) {
                    printf("Either [-d] or [-f] can be used but not both.\n");
                    help(2);
                } else {
                    dflag++;
                    fflag++;

                    char* string = readFile(optarg);
                    int size = readFileSize(optarg);
                    
                    char tmpString[size];
                    memcpy(tmpString, string, size);
                    free(string);

                    message_body = concat(message_body, tmpString);

                    if (message_body) {
                        printf("File data stored in request body: %s\n", message_body);
                    }
                }
                
                break;

            default:
                print_usage();
        }
    }

    http_message = concat(http_message, request_line);
    http_message = concat(http_message, headers);
    http_message = concat(http_message, "\r\n");
    http_message = concat(http_message, message_body);

    sockfd = sendMessage(http_message, newUrl);
    receiveMessage(sockfd, 100);

    // get response, format output

    close(sockfd);

    free(http_message);
    free(request_line);
    free(headers);
    free(message_body);

    return 0;
}
