#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_REQUEST_SIZE 4096
#define MAX_RESPONSE_SIZE 8192
#define MAX_HEADER_SIZE 1024
#define MAX_BODY_SIZE 2048
#define MAX_PATH_SIZE 256
#define MAX_METHOD_SIZE 16

typedef struct {
    char method[MAX_METHOD_SIZE];
    char path[MAX_PATH_SIZE];
    char headers[MAX_HEADER_SIZE];
    char body[MAX_BODY_SIZE];
    int content_length;
} HttpRequest;

typedef struct {
    int status_code;
    char headers[MAX_HEADER_SIZE];
    char body[MAX_BODY_SIZE];
} HttpResponse;

// Function declarations
int start_server(int port);
void handle_request(int client_socket);
void parse_request(const char* request_str, HttpRequest* request);
void send_response(int client_socket, HttpResponse* response);
void set_response_headers(HttpResponse* response, const char* content_type);
void handle_get_todos(HttpResponse* response);
void handle_get_todo(HttpResponse* response, const char* id);
void handle_create_todo(HttpResponse* response, const char* body);
void handle_update_todo(HttpResponse* response, const char* id, const char* body);
void handle_delete_todo(HttpResponse* response, const char* id);
void handle_not_found(HttpResponse* response);
void handle_method_not_allowed(HttpResponse* response);
void extract_path_id(const char* path, char* id);
void url_decode(char* dst, const char* src);

#endif // HTTP_SERVER_H