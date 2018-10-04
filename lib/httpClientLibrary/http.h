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

static const char* HTTP_RESPONSE_HEADERS[9] = {
    "Accept-Ranges",
    "Age",
    "ETag",
    "Location",
    "Proxy-Authenticate",
    "Retry-After",
    "Server",
    "Vary",
    "WWW-Authenticate"
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

static const char* HTTP_STATUS_CODES[40] = {
    // 1xx: Informational - Request received, continuing process.
    "100",
    "101",

    // 2xx: Success - The action was successfully received, understood, and accepted.
    "200",
    "201",
    "202",
    "203",
    "204",
    "205",
    "206",

    // 3xx: Redirection - Further action must be taken in order to complete the request.
    "300",
    "301",
    "302",
    "303",
    "304",
    "305",
    "307",

    // 4xx: Client Error - The request contains bad syntax or cannot be fulfilled.
    "400",
    "401",
    "402",
    "403",
    "404",
    "405",
    "406",
    "407",
    "408",
    "409",
    "410",
    "411",
    "412",
    "413",
    "414",
    "415",
    "416",
    "417",

    // 5xx: Server Error - The server failed to fulfill an apparently valid request.
    "500",
    "501",
    "502",
    "503",
    "504",
    "505"
};

static const char* HTTP_REASON_PHRASES[40] = {
    // 1xx: Informational - Request received, continuing process.
    "Continue",
    "Switching Protocols",

    // 2xx: Success - The action was successfully received, understood, and accepted
    "OK",
    "Created",
    "Accepted",
    "Non-Authoritative Information",
    "No Content",
    "Reset Content",
    "Partial Content",

    // 3xx: Redirection - Further action must be taken in order to complete the request.
    "Multiple Choices",
    "Moved Permanently",
    "Found",
    "See Other",
    "Not Modified",
    "Use Proxy",
    "Temporary Redirect",

    // 4xx: Client Error - The request contains bad syntax or cannot be fulfilled.
    "Bad Request",
    "Unauthorized",
    "Payment Required",
    "Forbidden",
    "Not Found",
    "Method Not Allowed",
    "Not Acceptable",
    "Proxy Authentication Required",
    "Request Time-out",
    "Conflict",
    "Gone",
    "Length Required",
    "Precondition Failed",
    "Request Entity Too Large",
    "Request-URI Too Large",
    "Unsupported Media Type",
    "Requested range not satisfiable",
    "Expectation Failed",

    // 5xx: Server Error - The server failed to fulfill an apparently valid request.
    "Internal Server Error",
    "Not Implemented",
    "Bad Gateway",
    "Service Unavailable",
    "Gateway Time-out",
    "HTTP Version not supported"
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

    request->requestLine = concat(request->requestLine, "\r\n");
}

HttpMessage* constructHttpMessageFromRequest(HttpRequest* request);
HttpMessage* constructHttpMessageFromRequest(HttpRequest* request) {
    HttpMessage message;
    HttpMessage* messagePtr = &message;
    messagePtr->messageType = REQUEST;
    
    request->constructRequestLine(request);
    messagePtr->startLine = request->requestLine;
}

typedef struct http_response HttpResponse;
struct http_response {
    // i.e. HTTP/${VERSION}
    char* protocolVersion;
    // i.e. 200
    char* statusCode;
    // i.e. OK
    char* reasonPhrase;

    char* statusLine;
    // Will be replaced by entity body given a "Transfer-Encoding" header.
    // All responses to the HEAD request method MUST NOT include a message-body,
    // even though the presence of entity- header fields might lead one to believe they do.
    // All 1xx (informational), 204 (no content), and 304 (not modified) responses MUST NOT include a message-body.
    // All other responses do include a message-body, although it MAY be of zero length. 
    char* responseBody;

    void (*constructStatusLine)(HttpResponse*);

    // i.e. `field-name:field-value`. "field-name"s should be case-insensitive
    char* headers[];
};

void constructStatusLine(HttpResponse* response);
void constructStatusLine(HttpResponse* response) {
    response->statusLine = '\0';

    response->statusLine = concat(response->statusLine, response->protocolVersion);
    response->statusLine = concat(response->statusLine, " ");

    response->statusLine = concat(response->statusLine, response->statusCode);
    response->statusLine = concat(response->statusLine, " ");

    response->statusLine = concat(response->statusLine, response->reasonPhrase);
    response->statusLine = concat(response->statusLine, " ");

    response->statusLine = concat(response->statusLine, "\r\n");
}

HttpMessage* constructHttpMessageFromResponse(HttpResponse* response);
HttpMessage* constructHttpMessageFromResponse(HttpResponse* response) {
    HttpMessage message;
    HttpMessage* messagePtr = &message;
    messagePtr->messageType = RESPONSE;

    response->constructStatusLine(response);
    messagePtr->startLine = response->statusLine;
}

#endif