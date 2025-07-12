#!/bin/bash

# Example client script for Todo Backend API
# This script demonstrates how to use the todo API endpoints

BASE_URL="http://localhost:8080"

echo "Todo Backend API Example Client"
echo "==============================="
echo ""

# Function to make HTTP requests with better formatting
make_request() {
    local method=$1
    local endpoint=$2
    local data=$3
    local description=$4
    
    echo ">>> $description"
    echo "Request: $method $endpoint"
    
    if [ -n "$data" ]; then
        echo "Data: $data"
        response=$(curl -s -w "\nHTTP Status: %{http_code}" -X "$method" "$BASE_URL$endpoint" \
                      -H "Content-Type: application/json" \
                      -d "$data")
    else
        response=$(curl -s -w "\nHTTP Status: %{http_code}" -X "$method" "$BASE_URL$endpoint")
    fi
    
    echo "Response:"
    echo "$response"
    echo ""
    echo "---"
    echo ""
}

# Check if server is running
echo "Checking if server is running..."
if ! curl -s "$BASE_URL/todos" > /dev/null 2>&1; then
    echo "Error: Server is not running on $BASE_URL"
    echo "Please start the server first with: ./todo-server"
    exit 1
fi

echo "Server is running. Starting API demonstration..."
echo ""

# 1. Get all todos (should be empty initially)
make_request "GET" "/todos" "" "Get all todos (initially empty)"

# 2. Create first todo
make_request "POST" "/todos" '{"text": "Learn C programming"}' "Create first todo"

# 3. Create second todo
make_request "POST" "/todos" '{"text": "Build a web server"}' "Create second todo"

# 4. Create third todo
make_request "POST" "/todos" '{"text": "Write documentation"}' "Create third todo"

# 5. Get all todos (should now have 3 items)
make_request "GET" "/todos" "" "Get all todos (should have 3 items)"

# 6. Get specific todo
make_request "GET" "/todos/2" "" "Get specific todo with ID 2"

# 7. Update todo (mark as completed)
make_request "PUT" "/todos/1" '{"text": "Learn C programming", "completed": true}' "Update todo 1 (mark as completed)"

# 8. Update todo (change text and mark as completed)
make_request "PUT" "/todos/2" '{"text": "Build an awesome web server in C", "completed": true}' "Update todo 2 (change text and mark as completed)"

# 9. Get all todos (should show updates)
make_request "GET" "/todos" "" "Get all todos (should show updates)"

# 10. Delete a todo
make_request "DELETE" "/todos/2" "" "Delete todo with ID 2"

# 11. Get all todos (should have 2 items)
make_request "GET" "/todos" "" "Get all todos (should have 2 items after deletion)"

# 12. Try to get deleted todo (should return 404)
make_request "GET" "/todos/2" "" "Try to get deleted todo (should return 404)"

# 13. Try invalid endpoint (should return 404)
make_request "GET" "/invalid" "" "Try invalid endpoint (should return 404)"

# 14. Try unsupported method (should return 405)
make_request "PATCH" "/todos/1" '{"completed": false}' "Try unsupported method (should return 405)"

echo "API demonstration completed!"
echo ""
echo "Summary of operations performed:"
echo "1. Retrieved empty todo list"
echo "2. Created 3 todos"
echo "3. Retrieved all todos"
echo "4. Retrieved specific todo"
echo "5. Updated todos (text and completion status)"
echo "6. Deleted a todo"
echo "7. Tested error conditions (404, 405)"
echo ""
echo "You can now experiment with the API using curl or any HTTP client."