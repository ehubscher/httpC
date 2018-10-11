#ifndef HTTP_REQUEST
#define HTTP_REQUEST

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

    void (*constructRequestLine)(HttpRequest*);

    unsigned int headersSize;
    // i.e. `field-name:field-value`. "field-name"s should be case-insensitive
    char** headers;
};

void constructRequestLine(HttpRequest* request);
void constructRequestLine(HttpRequest* request) {
    request->requestLine = NULL;

    int isMethodValid = 0;
    for(int i = 0; i < HTTP_METHODS_SIZE; i = i + 1) {
        if(strcmp(request->method, HTTP_METHODS[i]) == 0) {
            isMethodValid = 1;
            break;
        }
    }

    if(!isMethodValid) {
        fprintf(stderr, "EXCEPTION: httpRequest::constructRequestLine() - Invalid HTTP method. Defaulting to GET.\n");
        request->method = "GET";
    } 

    request->requestLine = concat(request->requestLine, request->method);
    request->requestLine = concat(request->requestLine, " ");

    request->requestLine = concat(request->requestLine, request->requestURI);
    request->requestLine = concat(request->requestLine, " ");

    if(strcmp(request->protocolVersion, "1.0") != 0 || strcmp(request->protocolVersion, "1.1") != 0) {
        fprintf(stderr, "EXCEPTION: httpRequest::constructRequestLine() - Invalid HTTP protocol version. Defaulting to 1.0.\n");
        request->protocolVersion = "1.0";
    }

    request->requestLine = concat(request->requestLine, "HTTP/");
    request->requestLine = concat(request->requestLine, request->protocolVersion);
    request->requestLine = concat(request->requestLine, " ");

    request->requestLine = concat(request->requestLine, "\r\n");
}

HttpMessage* constructHttpMessageFromRequest(HttpRequest* request);
HttpMessage* constructHttpMessageFromRequest(HttpRequest* request) {
    HttpMessage message;
    HttpMessage* messagePtr = &message;

    message.messageType = REQUEST;
    message.host = request->requestURI;
    message.port = 80;

    // Construct message start line.
    request->constructRequestLine(request);
    message.startLine = request->requestLine;

    // Construct message header string.
    message.headers = constructHeadersString(request->headers, request->headersSize);
    message.headers = concat(message.headers, "\r\n");

    // Construct mesage body.
    message.messageBody = concat(message.messageBody, request->requestBody);

    return messagePtr;
}

#endif
