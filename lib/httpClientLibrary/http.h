#ifndef HTTP_CLIENT
#define HTTP_CLIENT

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define HTTP_METHODS_SIZE 8
#define HTTP_GENERAL_HEADERS_SIZE 9
#define HTTP_REQUEST_HEADERS_SIZE 19
#define HTTP_RESPONSE_HEADERS_SIZE 9
#define HTTP_ENTITY_HEADERS_SIZE 10
#define HTTP_STATUS_CODES_SIZE 40
#define HTTP_REASON_PHRASES_SIZE 40

typedef enum HTTP_MESSAGE_TYPES HTTP_MESSAGE_TYPES;
enum HTTP_MESSAGE_TYPES {
    REQUEST,
    RESPONSE
};

static const char* HTTP_METHODS[HTTP_METHODS_SIZE] = {
    "GET",
    "POST",
    "PUT",
    "DELETE",
    "HEAD",
    "OPTIONS",
    "TRACE",
    "CONNECT"
};

static const char* HTTP_GENERAL_HEADERS[HTTP_GENERAL_HEADERS_SIZE] = {
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

static const char* HTTP_REQUEST_HEADERS[HTTP_REQUEST_HEADERS_SIZE] = {
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

static const char* HTTP_RESPONSE_HEADERS[HTTP_RESPONSE_HEADERS_SIZE] = {
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

static const char* HTTP_ENTITY_HEADERS[HTTP_ENTITY_HEADERS_SIZE] = {
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

static const char* HTTP_STATUS_CODES[HTTP_STATUS_CODES_SIZE] = {
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

static const char* HTTP_REASON_PHRASES[HTTP_REASON_PHRASES_SIZE] = {
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

char* concat(char* s1, const char* s2);
char* concat(char* s1, const char* s2)
{
    // Allocate enough memory for s1.
    char* tempS1 = (char*)malloc(strlen(s1) + 1);
    // Copy s1 character values into tempS1.
    strcpy(tempS1, s1);

    // Compute the new amount of memory needed for s1.
    size_t newS1Size = strlen(s1) + strlen(s2) + 1;

    // Free up the dynamically allocated memory for s1's previous value.
    free(s1);
    // Allocate s1 it's new amount of memory.
    s1 = (char*)malloc(newS1Size);

    if (s1 == NULL) {
        fprintf(stderr, "ERROR: httpClientLibrary::concat() - Failed to allocate memory for string concatenation.\n");
        return s1;
    }

    // Copy s1's original value back into the larger s1.
    strcpy(s1, tempS1);
    // Free up tempS1 now that it's no longer needed.
    free(tempS1);

    // Concatenate the character values from s2 into the larger s1.
    strcat(s1, s2);

    return s1;
}

size_t getTotalHeadersStringSize(char* headers[][2], int headersSize);
size_t getTotalHeadersStringSize(char* headers[][2], int headersSize) {
    size_t totalHeadersStringSize = 0;

    for(int i = 0; i < headersSize; i = i + 1) {
        totalHeadersStringSize = totalHeadersStringSize + strlen(headers[i][0]) + strlen(":") + strlen(headers[i][1]) + strlen("\r\n");
    }

    return totalHeadersStringSize;
}

void constructHeadersString(char* headersString, char* headers[][2], int headersSize);
void constructHeadersString(char* headersString, char* headers[][2], int headersSize) {
    // Free up the memory allocated to the original headersString.
    free(headersString);

    // Compute the total size needed based off the headers array provided.
    size_t totalHeadersStringSize = getTotalHeadersStringSize(headers, headersSize);
    // Allocate the total size for the headers string.
    headersString = (char*)malloc(totalHeadersStringSize + 1);

    for(int i = 0; i < headersSize; i = i + 1) {
        headersString = concat(headersString, headers[i][0]);
        headersString = concat(headersString, ":");
        headersString = concat(headersString, headers[i][1]);
        headersString = concat(headersString, "\r\n");
    }
}

#endif
