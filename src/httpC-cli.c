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
    int status;
    struct addrinfo hints;
    struct addrinfo* serverInfoResults;
    struct addrinfo* resultsPtr; 
    
    void *address;
    char *IPVersion;
    int socketFileDescriptor;

    int connectResult;

    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_UNSPEC; // IPv4 | IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP
    
    if ((status = getaddrinfo("google.com", "http", &hints, &serverInfoResults)) != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(1);
    }

    // serverInfoResults now points to a linked list of 1 or more struct addrinfo s.
    // Do everything until you don't need serverInfo anymore.
    resultsPtr = serverInfoResults;
    while(resultsPtr != NULL) {
        // get the pointer to the address itself,
        // different fields in IPv4 and IPv6:
        if (resultsPtr->ai_family == AF_INET) { // IPv4
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)resultsPtr->ai_addr;
            address = &(ipv4->sin_addr);
            IPVersion = "IPv4";
        } else { // IPv6
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)resultsPtr->ai_addr;
            address = &(ipv6->sin6_addr);
            IPVersion = "IPv6";
        }

        resultsPtr = resultsPtr->ai_next;
    }
    
    socketFileDescriptor = socket(serverInfoResults->ai_family, serverInfoResults->ai_socktype, serverInfoResults->ai_protocol);
    socketFileDescriptor = socket(serverInfoResults->ai_family, serverInfoResults->ai_socktype, serverInfoResults->ai_protocol);
socketFileDescriptor = socket(serverInfoResults->ai_family, serverInfoResults->ai_socktype, serverInfoResults->ai_protocol);
socketFileDescriptor = socket(serverInfoResults->ai_family, serverInfoResults->ai_socktype, serverInfoResults->ai_protocol);
socketFileDescriptor = socket(serverInfoResults->ai_family, serverInfoResults->ai_socktype, serverInfoResults->ai_protocol);

    connectResult = connect(socketFileDescriptor, serverInfoResults->ai_addr, serverInfoResults->ai_addrlen);
    
    // serverInfoResults now points to a linked list of 1 or more struct addrinfos.
    // Do everything until you don't need serverInfoResults anymore.
    freeaddrinfo(serverInfoResults);
    

// -----------------------------------------------------------------------------

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
                printf("you want verbose\n");
                vflag = 1;
                break;
            case 'h':
                printf("you want to pass headers\n");
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
                }
                break;
            default:
                printf("error");
        }
    }

    HttpMessage* message;
    message = constructHttpMessageFromRequest(&request);

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
