#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef HTTP_CLIENT
#define HTTP_CLIENT

typedef enum HTTP_MESSAGE_TYPES HTTP_MESSAGE_TYPES;
enum HTTP_MESSAGE_TYPES {
    REQUEST,
    RESPONSE
};

static const char* HTTP_METHODS[8] = {
    "GET",
    "POST",
    "PUT",
    "DELETE",
    "HEAD",
    "OPTIONS",
    "TRACE",
    "CONNECT"
};

static const char* HTTP_GENERAL_HEADERS[9] = {
    "Cache-Control",
    "Connection",
    "Date",
    "Pragma",
    "Trailer",
    "Transfer-Encoding",
    "Upgrade",
    "Via",
    "Warning"
};

static const char* HTTP_REQUEST_HEADERS[19] = {
    "Accept",
    "Accept-Charset",
    "Accept-Encoding",
    "Accept-Language",
    "Authorization",
    "Expect",
    "From",
    "Host",
    "If-Match",
    "If-Modified-Since",
    "If-None-Match",
    "If-Range",
    "If-Unmodified-Since",
    "Max-Forwards",
    "Proxy-Authorization",
    "Range",
    "Referer",
    "TE",
    "User-Agent"
};

static const char* HTTP_ENTITY_HEADERS[10] = {
    "Allow",
    "Content-Encoding",
    "Content-Language",
    "Content-Length",
    "Content-Location",
    "Content-MD5",
    "Content-Range",
    "Content-Type",
    "Expires",
    "Last-Modified"
};

char* concat(const char* s1, const char* s2)
{
    // +1 for the null-terminator
    int resultSize = strlen(s1) + strlen(s2) + 1;
    char* result = (char*)malloc(resultSize);

    if (result == NULL) {
        fprintf(stderr, "ERROR: httpClientLibrary::concat() - Failed to allocate memory for string concatenation.\n");
        exit(-1);
    }

    strcpy(result, s1);
    strcat(result, s2);

    return result;
}

typedef struct http_message HttpMessage;
struct http_message {
    HTTP_MESSAGE_TYPES messageType;

    char* startLine;
    char* messageBody;
    char* message;

    char* headers[];
};

int sendMessage(HttpMessage* message);
HttpMessage* receiveMessage();

typedef struct http_request HttpRequest;
struct http_request {
    // i.e. GET|POST
    char* method;
    // i.e. domain + query string
    char* requestURI;
    // i.e. HTTP/${VERSION}
    char* protocolVersion;

    char* requestLine;
    // Will be replaced by entity body given a "Transfer-Encoding" header.
    char* requestBody;

    char* request;

    void (*constructRequestLine)(HttpRequest*);

    // i.e. `field-name:field-value`. "field-name"s should be case-insensitive
    char* headers[];
};

void constructRequestLine(HttpRequest* request);
void constructRequestLine(HttpRequest* request) {
    request->requestLine = '\0';

    request->requestLine = concat(request->requestLine, request->method);
    request->requestLine = concat(request->requestLine, " ");

    request->requestLine = concat(request->requestLine, request->requestURI);
    request->requestLine = concat(request->requestLine, " ");

    request->requestLine = concat(request->requestLine, request->protocolVersion);
    request->requestLine = concat(request->requestLine, " ");

    request->requestLine = concat(request->requestLine, "\r\n\r\n");
}

HttpMessage* constructHttpMessage(HttpRequest* request);
HttpMessage* constructHttpMessageFromRequest(HttpRequest* request) {
    HttpMessage message;
    HttpMessage* messagePtr = &message;
    messagePtr->messageType = REQUEST;
    
    request->constructRequestLine(request);
    messagePtr->startLine = request->requestLine;
}

typedef struct http_response HttpResponse;
struct http_response {
    char* statusLine;
    // Will be replaced by entity body given a "Transfer-Encoding" header.
    // All responses to the HEAD request method MUST NOT include a message-body,
    // even though the presence of entity- header fields might lead one to believe they do.
    // All 1xx (informational), 204 (no content), and 304 (not modified) responses MUST NOT include a message-body.
    // All other responses do include a message-body, although it MAY be of zero length. 
    char* responseBody;

    // i.e. `field-name:field-value`. "field-name"s should be case-insensitive
    char* headers[];
};

HttpMessage* constructHttpMessageFromResponse(HttpResponse* response);
HttpMessage* constructHttpMessageFromResponse(HttpResponse* response) {
    HttpMessage* message;
    message->messageType = RESPONSE;
}

#endif