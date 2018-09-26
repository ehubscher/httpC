#ifndef HTTP_CLIENT
#define HTTP_CLIENT

struct http_message {
    char* message_header;
    char* message_body;
};
typedef struct http_message HttpMessage;

struct http_request {
    /*
    {
        {"GET"},
        {"POST"},
        {"PUT"},
        {"DELETE"},
        {"HEAD"},
        {"OPTIONS"},
        {"TRACE"},
        {"CONNECT"}
    };
    */
    const char methods[8][8];

    // i.e. Request|Response 
    char* startLine;
    // i.e. `field-name:field-value`. "field-name"s should be case-insensitive
    char* headers;
    /*
     * Begins with request method (i.e. GET|POST), followed by the request URI (i.e. domain + query string)
     * and the protocol version(i.e. HTTP/1.0|HTTP/1.1), and ending with CRLF (i.e. \r\n)
    */
    char* requestLine;
};
typedef struct http_request HttpRequest;
void constructHttpRequest(void); // Constructor

struct http_response {

};
typedef struct http_response HttpResponse;
void constructHttpResponse(void); // Constructor

struct http_server {
    HttpRequest request;
    HttpResponse response;
};
typedef struct http_server HttpServer;
void constructHttpServer(void); // Constructor

#endif