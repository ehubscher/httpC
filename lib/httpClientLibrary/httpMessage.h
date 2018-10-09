#ifndef HTTP_MESSAGE
#define HTTP_MESSAGE

#include "http.h"

typedef struct http_message HttpMessage;
struct http_message {
    HTTP_MESSAGE_TYPES messageType;

    char* startLine;
    char* headers;
    char* messageBody;

    char* message;

    void (*constructHttpMessage)(HttpMessage*);
    int (*sendMessage)(HttpMessage*);
    HttpMessage* (*receiveMessage)();

    unsigned int headersSize;
    char* headers[][2];
};

void constructHttpMessage(HttpMessage* message);
void constructHttpMessage(HttpMessage* message) {
    // Start line.
    message->message = concat(message->message, message->startLine);

    // Construct CRLF terminated headers.
    char* headers = NULL;
    constructHeadersString(headers, message->headers, message->headersSize);
    message->message = concat(message->message, headers);
    message->message = concat(message->message, "\r\n");

    // Message body.
    message->message = concat(message->message, message->messageBody);
}

int sendMessage(HttpMessage* message);
int sendMessage(HttpMessage* message) {

}

HttpMessage* receiveMessage();
HttpMessage* receiveMessage() {

}

#endif
