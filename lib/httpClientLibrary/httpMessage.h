#ifndef HTTP_MESSAGE
#define HTTP_MESSAGE

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "http.h"

typedef struct http_message HttpMessage;
struct http_message {
    HTTP_MESSAGE_TYPES messageType;
    char* host;
    int port;

    char* startLine;
    char* headers;
    char* messageBody;

    char* message;

    void (*constructHttpMessage)(HttpMessage*);
    int (*sendMessage)(HttpMessage*);
    HttpMessage* (*receiveMessage)();
};

void constructHttpMessage(HttpMessage* message);
void constructHttpMessage(HttpMessage* message) {
    // Start line.
    message->message = concat(message->message, message->startLine);

    message->message = concat(message->message, message->headers);
    message->message = concat(message->message, "\r\n");

    // Message body.
    message->message = concat(message->message, message->messageBody);
}

int sendMessage(HttpMessage* message);
int sendMessage(HttpMessage* message) {
    int status;
    struct addrinfo hints;
    struct addrinfo* serverInfo;

    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_UNSPEC; // IPv4 | IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP
    hints.ai_flags = AI_PASSIVE; // Fill in IP
    
    if ((status = getaddrinfo(message->host, "80", &hints, &serverInfo)) != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(1);
    }
    
    // serverInfo now points to a linked list of 1 or more struct addrinfos.
    // Do everything until you don't need serverInfo anymore.
    freeaddrinfo(serverInfo); 
}

HttpMessage* receiveMessage();
HttpMessage* receiveMessage() {

}

#endif
