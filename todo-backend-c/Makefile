CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
TARGET = todo-server
SOURCES = main.c todo.c http_server.c
OBJECTS = $(SOURCES:.c=.o)

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

# Compile source files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -f $(OBJECTS) $(TARGET)

# Install dependencies (if needed)
install:
	@echo "No external dependencies required for this project."

# Run the server
run: $(TARGET)
	./$(TARGET)

# Run with custom port
run-port: $(TARGET)
	./$(TARGET) $(PORT)

# Test the server (requires curl)
test:
	@echo "Testing the server..."
	@echo "Creating a todo..."
	curl -X POST http://localhost:8080/todos -H "Content-Type: application/json" -d '{"text": "Test todo"}'
	@echo "\nGetting all todos..."
	curl -X GET http://localhost:8080/todos
	@echo "\nGetting todo with ID 1..."
	curl -X GET http://localhost:8080/todos/1
	@echo "\nUpdating todo with ID 1..."
	curl -X PUT http://localhost:8080/todos/1 -H "Content-Type: application/json" -d '{"text": "Updated todo", "completed": true}'
	@echo "\nDeleting todo with ID 1..."
	curl -X DELETE http://localhost:8080/todos/1

# Show help
help:
	@echo "Available targets:"
	@echo "  all       - Build the project"
	@echo "  clean     - Remove build files"
	@echo "  run       - Run the server on default port (8080)"
	@echo "  run-port  - Run the server on custom port (use PORT=xxxx)"
	@echo "  test      - Run basic API tests (requires curl)"
	@echo "  help      - Show this help message"

.PHONY: all clean install run run-port test help