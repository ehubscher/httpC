#ifndef HTTP_MESSAGE
#define HTTP_MESSAGE

#include "http.h"

typedef struct http_message HttpMessage;
struct http_message {
    HTTP_MESSAGE_TYPES messageType;

    char* startLine;
    char* messageBody;
    char* message;

    void (*constructHttpMessage)(HttpMessage*);
    int (*sendMessage)(HttpMessage*);
    HttpMessage* (*receiveMessage)();


    size_t headersSize;
    char* headers[][2];
};

void constructHttpMessage(HttpMessage* message);
void constructHttpMessage(HttpMessage* message) {
    message->message = concat(message->message, message->startLine);

    for(int i = 0; i < )
    message->message = concat(message->message, message->headers[i]);
}

int sendMessage(HttpMessage* message);
int sendMessage(HttpMessage* message) {

}

HttpMessage* receiveMessage();
HttpMessage* receiveMessage() {

}

#endif
