#ifndef HTTP_REQUEST
#define HTTP_REQUEST

#include "http.h"
#include "httpMessage.h"

typedef struct http_request HttpRequest;
struct http_request {
    // i.e. GET|POST
    char* method;
    // i.e. host + path + query string
    char* requestURI;
    // i.e. HTTP/${VERSION}
    char* protocolVersion;

    char* requestLine;
    // Will be replaced by entity body given a "Transfer-Encoding" header.
    char* requestBody;

    char* request;

    void (*constructRequestLine)(HttpRequest*);

    // i.e. `field-name:field-value`. "field-name"s should be case-insensitive
    char* headers[][2];
};

void constructRequestLine(HttpRequest* request);
void constructRequestLine(HttpRequest* request) {
    request->requestLine = '\0';

    int isMethodValid = 0;
    for(int i = 0; i < HTTP_METHODS_SIZE; i = i + 1) {
        if(strcmp(request->method, HTTP_METHODS[i]) == 0) {
            isMethodValid = 1;
            break;
        }
    }

    if(!isMethodValid) {
        fprintf(stderr, "EXCEPTION: httpRequest::constructRequestLine() - Invalid HTTP method. Defaulting to GET.\n");
        request->method = (char*)malloc(strlen("GET") + 1);

        if(request->method == NULL) {
            fprintf(stderr, "MEMORY ERROR: httpRequest::constructRequestLine() - Could not allocate memory for request method.");
            exit(-1);
        }

        request->method = "GET";
    } 

    request->requestLine = concat(request->requestLine, request->method);
    request->requestLine = concat(request->requestLine, " ");

    request->requestLine = concat(request->requestLine, request->requestURI);
    request->requestLine = concat(request->requestLine, " ");

    if(strcmp(request->protocolVersion, "1.0") != 0 || strcmp(request->protocolVersion, "1.1") != 0) {
        fprintf(stderr, "EXCEPTION: httpRequest::constructRequestLine() - Invalid HTTP protocol version. Defaulting to 1.0.\n");
        request->protocolVersion = (char*)malloc(strlen("1.0") + 1);

        if(request->method == NULL) {
            fprintf(stderr, "MEMORY ERROR: httpRequest::constructRequestLine() - Could not allocate memory for request protocol version.");
            exit(-1);
        }

        request->protocolVersion = "1.0";
    }

    request->requestLine = concat(request->requestLine, request->protocolVersion);
    request->requestLine = concat(request->requestLine, " ");

    request->requestLine = concat(request->requestLine, "\r\n");
}

HttpMessage* constructHttpMessageFromRequest(HttpRequest* request);
HttpMessage* constructHttpMessageFromRequest(HttpRequest* request) {
    HttpMessage message;
    HttpMessage* messagePtr = &message;
    messagePtr->messageType = REQUEST;

    request->constructRequestLine(request);
    messagePtr->startLine = request->requestLine;

    return messagePtr;
}

#endif
