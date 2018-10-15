#ifndef HTTPC_HELPER
#define HTTPC_HELPER
#define CHUNK_SIZE 1024

#include <arpa/inet.h>
#include <ctype.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 

struct addrinfo* getHostInfo(char* host);
struct addrinfo* getHostInfo(char* host) {
    char ipstr[INET6_ADDRSTRLEN];
    int status;
    struct addrinfo hints, *serverInfoResults, *resultsPtr; 

    void *address;
    char *IPVersion;

    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_UNSPEC; // IPv4 | IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP

    if ((status = getaddrinfo(host, "http", &hints, &serverInfoResults)) != 0) {
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

        // convert the IP to a string and print it:
        inet_ntop(resultsPtr->ai_family, address, ipstr, sizeof ipstr);
        printf("  %s: %s\n", IPVersion, ipstr);

        resultsPtr = resultsPtr->ai_next;
    }

    return serverInfoResults;
}

int createSocketFileDescriptor(struct addrinfo* serverInfoResults);
int createSocketFileDescriptor(struct addrinfo* serverInfoResults) {
    return socket(serverInfoResults->ai_family, serverInfoResults->ai_socktype, serverInfoResults->ai_protocol);
}

int canConnect(int sockfd, struct addrinfo* serverInfoResults);
int canConnect(int sockfd, struct addrinfo* serverInfoResults) {
    int connectResult;

    int tryConnecting = 0;
    while(tryConnecting < 100 && (connectResult = connect(sockfd, serverInfoResults->ai_addr, serverInfoResults->ai_addrlen) == -1)) {
        tryConnecting = tryConnecting + 1;
    }

    return connectResult;
}

int sendMessage(char* http_message, char* host);
int sendMessage(char* http_message, char* host) {
    struct addrinfo *serverInfoResults = getHostInfo(host);
    int sockfd = createSocketFileDescriptor(serverInfoResults);
    int isConnected = canConnect(sockfd, serverInfoResults);
    
    if(isConnected == -1) {
        fprintf(stdout, "Currently cannot connect to server: %s\n", serverInfoResults);
        exit(1);
    }

    freeaddrinfo(serverInfoResults);

    int messageLength = strlen(http_message);
    int bytes_left = messageLength;
    int bytes_sent;

    while(bytes_left > 0) {
        bytes_sent = send(sockfd, http_message, bytes_left, 0);
        bytes_left = bytes_left - bytes_sent;
    }
    
    return sockfd;
}

char* getresponsebody(char* response_msg);
char* getresponsebody(char* response_msg) {
    char* token = (char*)malloc(sizeof(char) + 1);
    int resp_msg_len = strlen(response_msg);
    int total_token_size = 0;
    char* resp_msg_copy = (char*)malloc(resp_msg_len * sizeof(char) + 1);
    char* resp_body = (char*)malloc(sizeof(char) + 1);

    memcpy(resp_msg_copy, response_msg, resp_msg_len + 1);

    token = strtok(resp_msg_copy, "\n");
    while (token != NULL) {
        total_token_size = total_token_size + strlen(token) + 1;
        if (strcmp(token, "\r") == 0) {
            total_token_size = total_token_size + strlen(token);

            resp_body = (char*)realloc(resp_body, resp_msg_len + 1);
            if(resp_body == NULL) {
                fprintf(stderr, "ERROR: realloc() returned NULL.");
                exit(1);
            }

            memcpy(resp_body, response_msg + total_token_size - 1, resp_msg_len - total_token_size  + 1);

            token = NULL;
            resp_msg_copy = NULL;
            free(token);
            free(resp_msg_copy);

            return resp_body;
        }

        token = strtok(NULL, "\n");
    }

    free(token);
    free(resp_msg_copy);

    return NULL;
}

int receiveMessage(int sockfd, int timeout, int verbose);
int receiveMessage(int sockfd, int timeout, int verbose) {
    int size_recv, total_size = 0;
    struct timeval begin, now;
    char chunk[CHUNK_SIZE];
    double timediff;
    char* response_msg = (char*)malloc(sizeof(char*));
    int chunk_counter = 0;
    char* response_body = NULL;

    //beginning time
    gettimeofday(&begin, NULL);

    while(1) {
        gettimeofday(&now, NULL);

        //time elapsed in seconds
        timediff = (now.tv_sec - begin.tv_sec) + 1e-6 * (now.tv_usec - begin.tv_usec);
         
        // If you got some data, then break after timeout.
        // Otherwise, wait a little longer, twice the timeout.
        if (total_size > 0 || timediff > timeout) {
            break;
        } else if (timediff > timeout * 2) {
            break;
        }

        memset(chunk, 0, CHUNK_SIZE);
        if((size_recv =  recv(sockfd, chunk, CHUNK_SIZE , 0) ) == -1) {
            // If nothing was received then we want to wait a little before trying again, 0.1 seconds
            usleep(100000);
        } else {
            total_size += size_recv;
            realloc(response_msg, (chunk_counter * CHUNK_SIZE) + size_recv + 1);
            if(response_msg == NULL) {
                fprintf(stderr, "ERROR: realloc() returned NULL.");
                exit(1);
            }

            memcpy(response_msg + (chunk_counter * CHUNK_SIZE), chunk, size_recv + 1);
            chunk_counter = chunk_counter + 1;
            //reset beginning time
            gettimeofday(&begin , NULL);
        }
    }
    if (verbose) {
        printf("%s", response_msg);
    }
    else {
        response_body = getresponsebody(response_msg);
        printf("%s", response_body);
    }

    response_msg = NULL;
    free(response_msg);

    return total_size;
}

char* readFile(char *filename);
char* readFile(char *filename)
{
   char *buffer = NULL;
   int string_size, read_size;
   FILE *handler = fopen(filename, "r");

   if (handler)
   {
       // Seek the last byte of the file
       fseek(handler, 0, SEEK_END);
       // Offset from the first to the last byte, or in other words, filesize
       string_size = ftell(handler);
       // go back to the start of the file
       rewind(handler);

       // Allocate a string that can hold it all
       buffer = (char*) malloc(sizeof(char) * (string_size + 1) );

       // Read it all in one operation
       read_size = fread(buffer, sizeof(char), string_size, handler);

       // fread doesn't set it so put a \0 in the last position
       // and buffer is now officially a string
       buffer[string_size] = '\0';

       if (string_size != read_size)
       {
           // Something went wrong, throw away the memory and set
           // the buffer to NULL
           free(buffer);
           buffer = NULL;
       }

       // Always remember to close the file.
       fclose(handler);
    }

    return buffer;
}

int readFileSize(char *filename);
int readFileSize(char *filename) {
    int string_size, buffer_size;
    FILE *handler = fopen(filename, "r");

    if (handler)
    {
       // Seek the last byte of the file
       fseek(handler, 0, SEEK_END);
       // Offset from the first to the last byte, or in other words, filesize
       string_size = ftell(handler);
       // go back to the start of the file
       rewind(handler);

        // get the size of the buffer
       buffer_size = sizeof(char) * (string_size + 1);

       // Always remember to close the file.
       fclose(handler);
    }
    return buffer_size;
}

#endif