#ifndef HTTP_CLIENT
#define HTTP_CLIENT

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <unistd.h> /* read, write, close */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h> /* struct hostent, gethostbyname */

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

char* capitalize(char* string, int size);
char* capitalize(char* string, int size) {
    char* capitalizedString = (char*)malloc(size * sizeof(char) + 1);
    for(int i = 0; i < size; i = i + 1) {
        char charAti[1] = {toupper(string[i])};
        memcpy(capitalizedString + i, charAti, 1);
    }

    return capitalizedString;
}

char* concat(char* s1, const char* s2);
char* concat(char* s1, const char* s2)
{
    // Re-allocate memory for s1 to accomodate s2.
    if(s1 == NULL && s2 == NULL) {
        return "";
    } else if(s1 == NULL) {
        s1 = (char*)realloc(s1, strlen(s2) + 1);
    } else if (s2 == NULL) {
        return s1;
    } else {
        s1 = (char*)realloc(s1, strlen(s1) + strlen(s2) + 1);
    }

    if (s1 == NULL) {
        fprintf(stderr, "ERROR: httpClientLibrary::concat() - Failed to allocate memory for string concatenation.\n");
        return "\0";
    }

    // Copy the character values from s2 into the remaining memory slots of result.
    memcpy(s1 + strlen(s1), s2, strlen(s2) + 1);

    return s1;
}

char* extractProtocolFromURI(char* URI);
char* extractProtocolFromURI(char* URI) {
    int URISize = strlen(URI) + 1;
    char URIcpy[URISize];
    memcpy(URIcpy, URI, URISize);

    char* token = strtok(URIcpy, ":");
    if(strcmp(token, "http") == 0 || strcmp(token, "HTTP") == 0) {
        return token;
    } else {
        return "\0";
    }
}

char* extractHostFromURI(char* URI);
char* extractHostFromURI(char* URI) {
    int URISize = strlen(URI) + 1;
    char URIcpy[URISize];
    memcpy(URIcpy, URI, URISize);
    char* result;

    char* token = strtok(URIcpy, "www");
    if(token != NULL) {
        token = strtok(NULL, "/");
        
        if(token != NULL) {
            return concat(result, token);
        } else {
            return NULL;
        }
    } else {
        return NULL;
    }
}

int getSlashCountOnPathFromURI(char* URI);
int getSlashCountOnPathFromURI(char* URI) {
    int slashCount = 0;
    int URISize = strlen(URI) + 1;
    char URIcpy[URISize];
    memcpy(URIcpy, URI, URISize);
    char* result = NULL;

    char* token = strtok(URIcpy, "/");
    if(token != NULL) {
        token = strtok(NULL, "/");
    }

    while(token != NULL) {
        char* www = strstr(token, "www");
        if(www != NULL) {
            token = strtok(NULL, ".");
        }

        token = strtok(NULL, "/");
        char* tmpToken = NULL;
        while(token != NULL) {
            tmpToken = (char*)realloc(tmpToken, strlen(token) + 1);
            strcpy(tmpToken, token);

            result = concat(result, tmpToken);
            token = strtok(NULL, "/");

            if(token != NULL) {
                result = concat(result, "/");
                slashCount = slashCount + 1;
            }
        }

        tmpToken = strtok(tmpToken, "?");
        if(tmpToken != NULL) {
            slashCount = slashCount + 1;
        }

        free(tmpToken);
    }

    return slashCount;
}

char* extractPathFromURI(char* URI);
char* extractPathFromURI(char* URI) {
    int slashCount = getSlashCountOnPathFromURI(URI);
    int URISize = strlen(URI) + 1;
    char URIcpy[URISize];
    memcpy(URIcpy, URI, URISize);
    char* result = NULL;

    char* token = strtok(URIcpy, "/");
    if(token != NULL) {
        token = strtok(NULL, "/");
    }

    while(token != NULL) {
        char* www = strstr(token, "www");
        if(www != NULL) {
            token = strtok(NULL, ".");
        }

        token = strtok(NULL, "/");
        char* tmpToken = NULL;
        while(token != NULL) {
            tmpToken = (char*)realloc(tmpToken, strlen(token) + 1);
            strcpy(tmpToken, token);

            result = concat(result, tmpToken);
            token = strtok(NULL, "/");

            if(token != NULL) {
                result = concat(result, "/");
            }
        }

        tmpToken = strtok(tmpToken, "?");
        if(tmpToken != NULL) {
            result = strtok(result, "?");
        }

        free(tmpToken);
    }

    return result;
}

int getQueryStringParamSize(char* URI);
int getQueryStringParamSize(char* URI) {
    int URISize = strlen(URI) + 1;
    char URIcpy[URISize];
    memcpy(URIcpy, URI, URISize);

    int paramSize = 0;

    char* token = strtok(URIcpy, "?");
    char* tmpToken = NULL;
    while(token != NULL) {
        token = strtok(NULL, "&");
        
        if(token != NULL) {
            paramSize = paramSize + 1;
        }
    }
    
    free(tmpToken);
    return paramSize;
}

char** extractQueryStringFromURI(char* URI);
char** extractQueryStringFromURI(char* URI) {
    int URISize = strlen(URI) + 1;
    char URIcpy[URISize];
    memcpy(URIcpy, URI, URISize);

    int paramSize = getQueryStringParamSize(URI);
    char** params = (char**)malloc(sizeof(char**));
    *params = (char*)malloc(paramSize * sizeof(char*));

    char* token = strtok(URIcpy, "?");
    char* tmpToken = NULL;
    for(int i = 0; i < paramSize && token != NULL; i = i + 1) {
        token = strtok(NULL, "&");
        
        if(token != NULL) {
            tmpToken = (char*)realloc(tmpToken, strlen(token) + 1);
            strcpy(tmpToken, token);
            memcpy(params[i], tmpToken, strlen(tmpToken));
        }
    }

    free(tmpToken);
    return params;
}

size_t getTotalHeadersStringSize(char** headers, const int headersSize);
size_t getTotalHeadersStringSize(char** headers, const int headersSize) {
    size_t totalHeadersStringSize = 0;

    for(int i = 0; i < headersSize; i = i + 1) {
        totalHeadersStringSize = totalHeadersStringSize + strlen(headers[i]) + strlen("\r\n");
    }

    return totalHeadersStringSize;
}

char* constructHeadersString(char** headers, const int headersSize);
char* constructHeadersString(char** headers, const int headersSize) {
    // Allocate the total size for the headers string.
    char* headersString = NULL;

    for(int i = 0; i < headersSize; i = i + 1) {
        headersString = concat(headersString, headers[i]);
    }

    if(headersString == NULL) {
        return concat(NULL, "\r\n");
    }

    return headersString;
}

#endif
