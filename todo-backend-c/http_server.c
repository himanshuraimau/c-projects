#include "http_server.h"
#include "todo.h"

int start_server(int port) {
    int server_fd, client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    
    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }
    
    // Set socket options
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("Setsockopt failed");
        exit(EXIT_FAILURE);
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    
    // Bind the socket
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
    
    // Listen for connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
    
    printf("Server listening on port %d\n", port);
    
    while (1) {
        if ((client_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
            perror("Accept failed");
            continue;
        }
        
        handle_request(client_socket);
        close(client_socket);
    }
    
    return 0;
}

void handle_request(int client_socket) {
    char buffer[MAX_REQUEST_SIZE] = {0};
    HttpRequest request;
    HttpResponse response;
    
    // Read request
    ssize_t bytes_read = read(client_socket, buffer, MAX_REQUEST_SIZE - 1);
    if (bytes_read <= 0) {
        return;
    }
    
    // Parse request
    parse_request(buffer, &request);
    
    // Initialize response
    memset(&response, 0, sizeof(response));
    
    printf("Request: %s %s\n", request.method, request.path);
    
    // Route the request
    if (strcmp(request.method, "GET") == 0) {
        if (strcmp(request.path, "/todos") == 0) {
            handle_get_todos(&response);
        } else if (strncmp(request.path, "/todos/", 7) == 0) {
            char id[MAX_ID_LENGTH];
            extract_path_id(request.path, id);
            handle_get_todo(&response, id);
        } else {
            handle_not_found(&response);
        }
    } else if (strcmp(request.method, "POST") == 0) {
        if (strcmp(request.path, "/todos") == 0) {
            handle_create_todo(&response, request.body);
        } else {
            handle_not_found(&response);
        }
    } else if (strcmp(request.method, "PUT") == 0) {
        if (strncmp(request.path, "/todos/", 7) == 0) {
            char id[MAX_ID_LENGTH];
            extract_path_id(request.path, id);
            handle_update_todo(&response, id, request.body);
        } else {
            handle_not_found(&response);
        }
    } else if (strcmp(request.method, "DELETE") == 0) {
        if (strncmp(request.path, "/todos/", 7) == 0) {
            char id[MAX_ID_LENGTH];
            extract_path_id(request.path, id);
            handle_delete_todo(&response, id);
        } else {
            handle_not_found(&response);
        }
    } else {
        handle_method_not_allowed(&response);
    }
    
    send_response(client_socket, &response);
}

void parse_request(const char* request_str, HttpRequest* request) {
    memset(request, 0, sizeof(HttpRequest));
    
    // Parse first line (method and path)
    char* line = strtok((char*)request_str, "\r\n");
    if (line) {
        sscanf(line, "%s %s", request->method, request->path);
    }
    
    // Parse headers and find content length
    while ((line = strtok(NULL, "\r\n")) != NULL) {
        if (strlen(line) == 0) break; // Empty line indicates end of headers
        
        if (strncmp(line, "Content-Length:", 15) == 0) {
            request->content_length = atoi(line + 16);
        }
    }
    
    // Parse body
    if (request->content_length > 0) {
        line = strtok(NULL, "");
        if (line) {
            strncpy(request->body, line, MAX_BODY_SIZE - 1);
        }
    }
}

void send_response(int client_socket, HttpResponse* response) {
    char response_str[MAX_RESPONSE_SIZE];
    
    // Build status line
    const char* status_text;
    switch (response->status_code) {
        case 200: status_text = "OK"; break;
        case 201: status_text = "Created"; break;
        case 204: status_text = "No Content"; break;
        case 404: status_text = "Not Found"; break;
        case 405: status_text = "Method Not Allowed"; break;
        case 500: status_text = "Internal Server Error"; break;
        default: status_text = "Unknown";
    }
    
    snprintf(response_str, MAX_RESPONSE_SIZE,
        "HTTP/1.1 %d %s\r\n"
        "%s"
        "Content-Length: %zu\r\n"
        "\r\n"
        "%s",
        response->status_code, status_text,
        response->headers,
        strlen(response->body),
        response->body);
    
    write(client_socket, response_str, strlen(response_str));
}

void set_response_headers(HttpResponse* response, const char* content_type) {
    snprintf(response->headers, MAX_HEADER_SIZE,
        "Content-Type: %s\r\n"
        "Access-Control-Allow-Origin: *\r\n"
        "Access-Control-Allow-Methods: GET, POST, PUT, DELETE, OPTIONS\r\n"
        "Access-Control-Allow-Headers: Content-Type\r\n",
        content_type);
}

void handle_get_todos(HttpResponse* response) {
    char* todos_json = get_all_todos_json();
    
    response->status_code = 200;
    set_response_headers(response, "application/json");
    strncpy(response->body, todos_json, MAX_BODY_SIZE - 1);
    
    free(todos_json);
}

void handle_get_todo(HttpResponse* response, const char* id) {
    Todo* todo = get_todo(id);
    
    if (todo) {
        char* todo_json = get_todo_json(todo);
        response->status_code = 200;
        set_response_headers(response, "application/json");
        strncpy(response->body, todo_json, MAX_BODY_SIZE - 1);
        free(todo_json);
    } else {
        handle_not_found(response);
    }
}

void handle_create_todo(HttpResponse* response, const char* body) {
    // Simple JSON parsing for text field
    char text[MAX_TEXT_LENGTH] = {0};
    char* start = strstr(body, "\"text\":");
    if (start) {
        start = strchr(start, '"');
        if (start) {
            start++; // Skip opening quote
            start = strchr(start, '"');
            if (start) {
                start++; // Skip opening quote
                char* end = strchr(start, '"');
                if (end) {
                    int len = end - start;
                    if (len < MAX_TEXT_LENGTH) {
                        strncpy(text, start, len);
                    }
                }
            }
        }
    }
    
    if (strlen(text) > 0) {
        char* id = create_todo(text);
        if (id) {
            Todo* todo = get_todo(id);
            char* todo_json = get_todo_json(todo);
            
            response->status_code = 201;
            set_response_headers(response, "application/json");
            strncpy(response->body, todo_json, MAX_BODY_SIZE - 1);
            
            free(todo_json);
            free(id);
        } else {
            response->status_code = 500;
            set_response_headers(response, "application/json");
            strcpy(response->body, "{\"error\": \"Failed to create todo\"}");
        }
    } else {
        response->status_code = 400;
        set_response_headers(response, "application/json");
        strcpy(response->body, "{\"error\": \"Invalid request body\"}");
    }
}

void handle_update_todo(HttpResponse* response, const char* id, const char* body) {
    // Simple JSON parsing
    char text[MAX_TEXT_LENGTH] = {0};
    int completed = 0;
    
    // Parse text field
    char* start = strstr(body, "\"text\":");
    if (start) {
        start = strchr(start, '"');
        if (start) {
            start++; // Skip opening quote
            start = strchr(start, '"');
            if (start) {
                start++; // Skip opening quote
                char* end = strchr(start, '"');
                if (end) {
                    int len = end - start;
                    if (len < MAX_TEXT_LENGTH) {
                        strncpy(text, start, len);
                    }
                }
            }
        }
    }
    
    // Parse completed field
    start = strstr(body, "\"completed\":");
    if (start) {
        start = strchr(start, ':');
        if (start) {
            start++; // Skip colon
            while (*start == ' ') start++; // Skip spaces
            completed = (strncmp(start, "true", 4) == 0) ? 1 : 0;
        }
    }
    
    if (update_todo(id, strlen(text) > 0 ? text : NULL, completed)) {
        Todo* todo = get_todo(id);
        char* todo_json = get_todo_json(todo);
        
        response->status_code = 200;
        set_response_headers(response, "application/json");
        strncpy(response->body, todo_json, MAX_BODY_SIZE - 1);
        
        free(todo_json);
    } else {
        handle_not_found(response);
    }
}

void handle_delete_todo(HttpResponse* response, const char* id) {
    if (delete_todo(id)) {
        response->status_code = 204;
        set_response_headers(response, "application/json");
        strcpy(response->body, "");
    } else {
        handle_not_found(response);
    }
}

void handle_not_found(HttpResponse* response) {
    response->status_code = 404;
    set_response_headers(response, "application/json");
    strcpy(response->body, "{\"error\": \"Not found\"}");
}

void handle_method_not_allowed(HttpResponse* response) {
    response->status_code = 405;
    set_response_headers(response, "application/json");
    strcpy(response->body, "{\"error\": \"Method not allowed\"}");
}

void extract_path_id(const char* path, char* id) {
    const char* last_slash = strrchr(path, '/');
    if (last_slash) {
        strncpy(id, last_slash + 1, MAX_ID_LENGTH - 1);
        id[MAX_ID_LENGTH - 1] = '\0';
    }
}

void url_decode(char* dst, const char* src) {
    char* p = dst;
    char code[3];
    
    while (*src) {
        if (*src == '%') {
            memcpy(code, src + 1, 2);
            code[2] = '\0';
            *p++ = (char)strtol(code, NULL, 16);
            src += 3;
        } else {
            *p++ = *src++;
        }
    }
    *p = '\0';
}