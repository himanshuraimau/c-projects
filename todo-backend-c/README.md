# Todo Backend API (C Implementation)

A lightweight HTTP-based todo application backend written in C. This server provides a RESTful API for managing todo items with full CRUD operations.

## Features

- **Pure C Implementation**: No external dependencies required
- **RESTful API**: Standard HTTP methods for CRUD operations
- **JSON Support**: Request and response bodies in JSON format
- **CORS Enabled**: Cross-origin resource sharing for web frontend integration
- **Memory Efficient**: In-memory storage with configurable limits
- **Portable**: Runs on any Unix-like system with GCC

## API Endpoints

### Get All Todos
```
GET /todos
```

**Response:**
```json
[
  {
    "id": "1",
    "text": "Buy groceries",
    "completed": false,
    "created_at": 1642678800,
    "updated_at": 1642678800
  }
]
```

### Get Single Todo
```
GET /todos/{id}
```

**Response:**
```json
{
  "id": "1",
  "text": "Buy groceries",
  "completed": false,
  "created_at": 1642678800,
  "updated_at": 1642678800
}
```

### Create Todo
```
POST /todos
Content-Type: application/json

{
  "text": "Buy groceries"
}
```

**Response:**
```json
{
  "id": "1",
  "text": "Buy groceries",
  "completed": false,
  "created_at": 1642678800,
  "updated_at": 1642678800
}
```

### Update Todo
```
PUT /todos/{id}
Content-Type: application/json

{
  "text": "Buy groceries and cook dinner",
  "completed": true
}
```

**Response:**
```json
{
  "id": "1",
  "text": "Buy groceries and cook dinner",
  "completed": true,
  "created_at": 1642678800,
  "updated_at": 1642682400
}
```

### Delete Todo
```
DELETE /todos/{id}
```

**Response:** `204 No Content`

## Building and Running

### Prerequisites
- GCC compiler
- Make
- Unix-like operating system (Linux, macOS, etc.)

### Build
```bash
make
```

### Run
```bash
# Run on default port (8080)
make run

# Run on custom port
make run-port PORT=3000
```

### Test
```bash
# Run basic API tests (requires curl)
make test
```

### Clean
```bash
# Remove build artifacts
make clean
```

## Project Structure

```
todo-backend-c/
├── main.c           # Entry point and server startup
├── todo.c           # Todo data structures and operations
├── todo.h           # Todo header file
├── http_server.c    # HTTP server implementation
├── http_server.h    # HTTP server header file
├── Makefile         # Build configuration
└── README.md        # This file
```

## Architecture

### Core Components

1. **Todo Management (`todo.c/h`)**
   - Data structures for todo items
   - CRUD operations
   - JSON serialization
   - In-memory storage

2. **HTTP Server (`http_server.c/h`)**
   - Socket-based HTTP server
   - Request parsing
   - Response generation
   - Routing logic

3. **Main Application (`main.c`)**
   - Server initialization
   - Command-line argument parsing
   - Signal handling for graceful shutdown

### Data Structures

#### Todo
```c
typedef struct {
    char id[MAX_ID_LENGTH];
    char text[MAX_TEXT_LENGTH];
    int completed;
    time_t created_at;
    time_t updated_at;
} Todo;
```

#### TodoList
```c
typedef struct {
    Todo todos[MAX_TODOS];
    int count;
} TodoList;
```

### Configuration

The following constants can be modified in the header files:

- `MAX_TODOS`: Maximum number of todos (default: 1000)
- `MAX_TEXT_LENGTH`: Maximum length of todo text (default: 256)
- `MAX_ID_LENGTH`: Maximum length of todo ID (default: 16)
- `DEFAULT_PORT`: Default server port (default: 8080)

## Usage Examples

### Using curl

```bash
# Start the server
./todo-server

# Create a todo
curl -X POST http://localhost:8080/todos \
  -H "Content-Type: application/json" \
  -d '{"text": "Learn C programming"}'

# Get all todos
curl -X GET http://localhost:8080/todos

# Get specific todo
curl -X GET http://localhost:8080/todos/1

# Update todo
curl -X PUT http://localhost:8080/todos/1 \
  -H "Content-Type: application/json" \
  -d '{"text": "Master C programming", "completed": true}'

# Delete todo
curl -X DELETE http://localhost:8080/todos/1
```

### Using HTTPie

```bash
# Create a todo
http POST localhost:8080/todos text="Learn C programming"

# Get all todos
http GET localhost:8080/todos

# Update todo
http PUT localhost:8080/todos/1 text="Master C programming" completed:=true

# Delete todo
http DELETE localhost:8080/todos/1
```

## Error Handling

The server returns appropriate HTTP status codes:

- `200 OK`: Successful GET/PUT requests
- `201 Created`: Successful POST requests
- `204 No Content`: Successful DELETE requests
- `400 Bad Request`: Invalid request body
- `404 Not Found`: Todo not found or invalid endpoint
- `405 Method Not Allowed`: Unsupported HTTP method
- `500 Internal Server Error`: Server-side errors

Error responses include JSON error messages:
```json
{
  "error": "Not found"
}
```

## Limitations

- **In-memory storage**: Data is lost when the server stops
- **Single-threaded**: Handles one request at a time
- **Basic JSON parsing**: Simple string-based JSON parsing
- **No authentication**: No user authentication or authorization
- **No persistence**: No database or file storage

## Future Enhancements

- [ ] Multi-threading support
- [ ] File-based persistence
- [ ] Database integration
- [ ] Authentication/authorization
- [ ] Request validation
- [ ] Logging system
- [ ] Configuration file support
- [ ] Docker containerization

## License

This project is released under the MIT License.

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly
5. Submit a pull request

## Support

For questions or issues, please open an issue in the repository.