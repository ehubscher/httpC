#include "http.h"
#include "http-man.h"
#include "httpc-helper.h"

#include <curl/curl.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>

void print_usage() {
    printf("Usage: httpc get [-v] [-h key:value] URL | httpc post [-v] [-h key:value] [-d inline-data] [-f file] URL\n");
    exit(2);
}

int main(int argc, char *argv[]) {
    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    char* url = argv[argc - 1];

    int option;
    char* optstring;
    int dflag = 0;
    int fflag = 0;

    char* http_message = (char*)malloc(sizeof(char));
    char* request_line = (char*)malloc(sizeof(char));

    struct curl_slist headers;
    struct curl_slist* headersPtr = NULL;
    
    char* message_body = NULL;//(char*)malloc(sizeof(char));

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

    curl_easy_setopt(curl, CURLOPT_URL, url);
    if (strncmp(argv[1], "get", 3) == 0) {
        optstring = "vh:";
    } else if (strncmp(argv[1], "post", 4) == 0) {
        if(headersPtr == NULL) {
            headersPtr = &headers;
        }

        optstring = "vh:d:f:";
    } else {
        print_usage();
    }

    // starts evaluating options after argv[1]
    // argv[1] is help | get | post
    optind = 2;
    while ((option = getopt(argc, argv, optstring)) != -1) {
        switch (option) {
            case 'v':
                curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
                break;

            case 'h':
                if(headersPtr == NULL) {
                    headersPtr = &headers;
                }

                curl_slist_append(headersPtr, optarg);

                break;

            case 'd':
                if (dflag) {
                    printf("Either [-d] or [-f] can be used but not both.\n");
                    help(2);
                } else {
                    int contentLength = strlen(optarg);
                    char* contentLengthCharArr = (char*)malloc(sizeof(char) * 100);
                    sprintf(contentLengthCharArr, "Content-Length: %i", contentLength);

                    curl_slist_append(headersPtr, contentLengthCharArr);
                    curl_easy_setopt(curl, CURLOPT_POST, optarg);

                    dflag++;
                    fflag++;

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

                    int contentLength = strlen(optarg);
                    char* contentLengthCharArr = NULL;
                    concat(contentLengthCharArr, "Content-Length: ");
                    concat(contentLengthCharArr, string);

                    curl_slist_append(headersPtr, contentLengthCharArr);
                   curl_easy_setopt(curl, CURLOPT_POST, string);

                   //free(string);
                }

                break;

            default:
                print_usage();
        }
    }

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers); 
    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);
    /* Check for errors */ 
    if(res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }
    
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return 0;
}
