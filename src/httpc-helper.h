#ifndef HTTPC_HELPER
#define HTTPC_HELPER

#include <httpRequest.h>

HttpRequest build_request(HttpRequest request, char* method, char* url, int headersSize, char* headers[]);
HttpRequest build_request(HttpRequest request, char* method, char* url, int headersSize, char* headers[]) {
    request.method = method;
    request.requestURI = url;
    request.headersSize = headersSize;
    request.headers = headers;

    return request;
}


#endif HTTPC_HELPER
