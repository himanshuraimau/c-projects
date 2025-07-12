#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "todo.h"
#include "http_server.h"

#define DEFAULT_PORT 8080

void signal_handler(int signum) {
    printf("\nShutting down server...\n");
    exit(0);
}

int main(int argc, char* argv[]) {
    int port = DEFAULT_PORT;
    
    // Parse command line arguments
    if (argc > 1) {
        port = atoi(argv[1]);
        if (port <= 0 || port > 65535) {
            fprintf(stderr, "Invalid port number: %s\n", argv[1]);
            fprintf(stderr, "Port must be between 1 and 65535\n");
            return 1;
        }
    }
    
    // Set up signal handler for graceful shutdown
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    
    // Initialize the todo list
    init_todo_list();
    
    printf("Todo Backend Server\n");
    printf("==================\n");
    printf("Starting server on port %d\n", port);
    printf("Press Ctrl+C to stop the server\n\n");
    
    // Print API endpoints
    printf("Available endpoints:\n");
    printf("  GET    /todos      - Get all todos\n");
    printf("  GET    /todos/{id} - Get a specific todo\n");
    printf("  POST   /todos      - Create a new todo\n");
    printf("  PUT    /todos/{id} - Update a todo\n");
    printf("  DELETE /todos/{id} - Delete a todo\n\n");
    
    // Start the HTTP server
    start_server(port);
    
    return 0;
}