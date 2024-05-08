/**
 * list data structure containing the tasks in the system
 */

typedef struct
{
    void (*function)(void *p);
    void *data;
} Task;

struct node {
    Task *task;
    struct node *next;
};

// insert and delete operations.
void insert(struct node **head, Task *task);
void delete(struct node **head, Task *task);
void traverse(struct node *head);
