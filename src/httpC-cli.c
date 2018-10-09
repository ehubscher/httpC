#include "http.h"

#include <getopt.h>

void print_usage() {
    printf("Usage: httpc get [-v] [-h key:value] URL | httpc post [-v] [-h key:value] [-d inline-data] [-f file] URL\n");
    exit(2);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        print_usage();
    }

    if (strncmp(argv[1], "get", 3) == 0) {

    }

    int option;

    while ((option = getopt(argc, argv, "vh:df")) != -1) {
        switch (option) {
            case 'v':
                printf("you want verbose");
                break;
            case 'h':
                printf("you want to pass headers");
                break;
            case 'd':
                printf("you want inline data");
                break;
            case 'f':
                printf("you want file");
                break;
            default:
                printf("error");
        }
    }

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

    return 0;
}
