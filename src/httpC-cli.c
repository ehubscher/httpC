#include "http.h"
#include "http-man.h"

#include <unistd.h>
#include <getopt.h>

void print_usage() {
    printf("Usage: httpc get [-v] [-h key:value] URL | httpc post [-v] [-h key:value] [-d inline-data] [-f file] URL\n");
    exit(2);
}

int main(int argc, char *argv[]) {
    char* test = extractHostFromURI("http://www.google.com/controller/action");
   
    // method - GET/POST
    char* method;
    int option;

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

    if (strncmp(argv[1], "get", 3) == 0) {
        method = "get";
    }

    else if (strncmp(argv[1], "post", 4) == 0) {
        method = "post";
    }

    // starts evaluating options after argv[2]
    // argv[2] is help | get | post
    optind = 2;
    while ((option = getopt(argc, argv, "+vh:df:")) != -1) {
        switch (option) {
            case 'v':
                printf("you want verbose\n");
                break;
            case 'h':
                printf("you want to pass headers\n");
                // optarg - this is what the header var is called
                // store it somewhere and add it to the msg
                break;
            case 'd':
                printf("you want inline data\n");
                break;
            case 'f':
                printf("you want file");
                break;
            default:
                printf("error");
        }
    }
    

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
