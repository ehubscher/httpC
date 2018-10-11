#ifndef HTTPC_HELPER
#define HTTPC_HELPER
#define CHUNK_SIZE 1024

#include <httpRequest.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

/*
    Receive data in multiple chunks by checking a non-blocking socket
    Timeout in seconds
*/
int recv_timeout(int sockfd, int timeout);
int recv_timeout(int sockfd, int timeout)
{
    int size_recv, total_size = 0;
    struct timeval begin, now;
    char chunk[CHUNK_SIZE];
    double timediff;
     
    //beginning time
    gettimeofday(&begin, NULL);
     
    while(1)
    {
        gettimeofday(&now, NULL);
         
        //time elapsed in seconds
        timediff = (now.tv_sec - begin.tv_sec) + 1e-6 * (now.tv_usec - begin.tv_usec);
         
        //if you got some data, then break after timeout
        if (total_size > 0 && timediff > timeout)
        {
            break;
        }
         
        //if you got no data at all, wait a little longer, twice the timeout
        else if (timediff > timeout*2)
        {
            break;
        }
         
        memset(chunk, 0, CHUNK_SIZE);  //clear the variable
        if((size_recv =  recv(sockfd, chunk, CHUNK_SIZE , 0) ) < 0)
        {
            //if nothing was received then we want to wait a little before trying again, 0.1 seconds
            usleep(100000);
        }
        else
        {
            total_size += size_recv;
            printf("%s", chunk);
            //reset beginning time
            gettimeofday(&begin , NULL);
        }
    }
     
    return total_size;
}

/*
int fetch_response(int sockfd, Buffer **response, int recv_size);
int fetch_response(int sockfd, Buffer **response, int recv_size)
{
    size_t bytes_received;
    int status = 0;
    char data[recv_size];

    debug("Receiving data ...");

    while (1) {
        bytes_received = recv(sockfd, data, RECV_SIZE, 0);

        if (bytes_received == -1) {
            return -1;
        } else if (bytes_received == 0) {
            return 0;
        }

        if (bytes_received > 0) {
            status = buffer_append(*response, data, bytes_received);
            if (status != 0) {
                fprintf(stderr, "Failed to append to buffer.\n");
                return -1;
            }
        }
    }

    debug("Finished receiving data.");

    return status;
}*/

/*HttpRequest build_request(HttpRequest request, char* method, char* url, int headersSize, char* headers[]);
HttpRequest build_request(HttpRequest request, char* method, char* url, int headersSize, char* headers[]) {
    request.method = method;
    request.requestURI = url;
    request.headersSize = headersSize;
    request.headers = headers;

    return request;
}
*/

char* readFile(char *filename);
char* readFile(char *filename)
{
   char *buffer = NULL;
   int string_size, read_size;
   FILE *handler = fopen(filename, "r");

   if (handler)
   {
       // Seek the last byte of the file
       fseek(handler, 0, SEEK_END);
       // Offset from the first to the last byte, or in other words, filesize
       string_size = ftell(handler);
       // go back to the start of the file
       rewind(handler);

       // Allocate a string that can hold it all
       buffer = (char*) malloc(sizeof(char) * (string_size + 1) );

       // Read it all in one operation
       read_size = fread(buffer, sizeof(char), string_size, handler);

       // fread doesn't set it so put a \0 in the last position
       // and buffer is now officially a string
       buffer[string_size] = '\0';

       if (string_size != read_size)
       {
           // Something went wrong, throw away the memory and set
           // the buffer to NULL
           free(buffer);
           buffer = NULL;
       }

       // Always remember to close the file.
       fclose(handler);
    }

    return buffer;
}

int readFileSize(char *filename);
int readFileSize(char *filename) {
    int string_size, buffer_size;
    FILE *handler = fopen(filename, "r");

    if (handler)
    {
       // Seek the last byte of the file
       fseek(handler, 0, SEEK_END);
       // Offset from the first to the last byte, or in other words, filesize
       string_size = ftell(handler);
       // go back to the start of the file
       rewind(handler);

        // get the size of the buffer
       buffer_size = sizeof(char) * (string_size + 1);

       // Always remember to close the file.
       fclose(handler);
    }
    return buffer_size;
}

#endif