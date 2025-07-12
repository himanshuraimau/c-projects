#include "todo.h"

// Global todo list
TodoList todo_list;

void init_todo_list() {
    todo_list.count = 0;
    memset(todo_list.todos, 0, sizeof(todo_list.todos));
}

void generate_id(char* id) {
    static int counter = 0;
    snprintf(id, MAX_ID_LENGTH, "%d", ++counter);
}

char* create_todo(const char* text) {
    if (todo_list.count >= MAX_TODOS) {
        return NULL; // List is full
    }
    
    Todo* todo = &todo_list.todos[todo_list.count];
    generate_id(todo->id);
    strncpy(todo->text, text, MAX_TEXT_LENGTH - 1);
    todo->text[MAX_TEXT_LENGTH - 1] = '\0';
    todo->completed = 0;
    todo->created_at = time(NULL);
    todo->updated_at = todo->created_at;
    
    todo_list.count++;
    
    // Return the ID of the created todo
    char* id = malloc(MAX_ID_LENGTH);
    strcpy(id, todo->id);
    return id;
}

Todo* get_todo(const char* id) {
    for (int i = 0; i < todo_list.count; i++) {
        if (strcmp(todo_list.todos[i].id, id) == 0) {
            return &todo_list.todos[i];
        }
    }
    return NULL;
}

int update_todo(const char* id, const char* text, int completed) {
    Todo* todo = get_todo(id);
    if (!todo) {
        return 0; // Todo not found
    }
    
    if (text) {
        strncpy(todo->text, text, MAX_TEXT_LENGTH - 1);
        todo->text[MAX_TEXT_LENGTH - 1] = '\0';
    }
    
    todo->completed = completed;
    todo->updated_at = time(NULL);
    
    return 1; // Success
}

int delete_todo(const char* id) {
    for (int i = 0; i < todo_list.count; i++) {
        if (strcmp(todo_list.todos[i].id, id) == 0) {
            // Shift all todos after this one to the left
            for (int j = i; j < todo_list.count - 1; j++) {
                todo_list.todos[j] = todo_list.todos[j + 1];
            }
            todo_list.count--;
            return 1; // Success
        }
    }
    return 0; // Todo not found
}

char* get_todo_json(const Todo* todo) {
    char* json = malloc(512);
    snprintf(json, 512, 
        "{\n"
        "  \"id\": \"%s\",\n"
        "  \"text\": \"%s\",\n"
        "  \"completed\": %s,\n"
        "  \"created_at\": %ld,\n"
        "  \"updated_at\": %ld\n"
        "}",
        todo->id, todo->text, todo->completed ? "true" : "false",
        todo->created_at, todo->updated_at);
    return json;
}

char* get_all_todos_json() {
    // Calculate required size
    int required_size = 100; // Base size for array brackets and formatting
    for (int i = 0; i < todo_list.count; i++) {
        required_size += 512; // Approximate size per todo
    }
    
    char* json = malloc(required_size);
    strcpy(json, "[\n");
    
    for (int i = 0; i < todo_list.count; i++) {
        char* todo_json = get_todo_json(&todo_list.todos[i]);
        strcat(json, "  ");
        strcat(json, todo_json);
        if (i < todo_list.count - 1) {
            strcat(json, ",");
        }
        strcat(json, "\n");
        free(todo_json);
    }
    
    strcat(json, "]");
    return json;
}