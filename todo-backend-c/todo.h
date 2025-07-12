#ifndef TODO_H
#define TODO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TODOS 1000
#define MAX_TEXT_LENGTH 256
#define MAX_ID_LENGTH 16

typedef struct {
    char id[MAX_ID_LENGTH];
    char text[MAX_TEXT_LENGTH];
    int completed;
    time_t created_at;
    time_t updated_at;
} Todo;

typedef struct {
    Todo todos[MAX_TODOS];
    int count;
} TodoList;

// Global todo list
extern TodoList todo_list;

// Function declarations
void init_todo_list();
char* create_todo(const char* text);
Todo* get_todo(const char* id);
int update_todo(const char* id, const char* text, int completed);
int delete_todo(const char* id);
char* get_all_todos_json();
char* get_todo_json(const Todo* todo);
void generate_id(char* id);

#endif // TODO_H