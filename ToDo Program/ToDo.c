#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    int id;
    char description[256];
    int completed;
    int priority;
} Task;

typedef struct
{
    Task *tasks;
    int size;
    int capacity;
    int nextId;
} ToDoList;

// This is the intial to do list
void initList(ToDoList *list)
{
    list->tasks = malloc(sizeof(Task) * 10); // dynamically allocated array of tasks
    list->size = 0;
    list->capacity = 10;
    list->nextId = 1;
}

// This function adds a task to the list
void addTask(ToDoList *list, const char *desc, int priority)
{
    if (list->size == list->capacity)
    { // if the list is at capacity, allocate new memory for more tasks
        list->capacity *= 2;
        list->tasks = realloc(list->tasks, list->capacity * sizeof(Task));
    }

    Task newTask = {
        list->nextId++, "", 0, priority};

    strncpy(newTask.description, desc, 255);
    list->tasks[list->size++] = newTask;
}

// This function displays the task list
void displayTasks(ToDoList *list)
{
    for (int i = 0; i < list->size; i++)
    {
        Task *t = &list->tasks[i];
        printf("[%d] %s (Priority: %d) - %s\n", t->id, t->completed ? "[X]" : "[ ]", t->priority, t->description);
    }
}

// This function marks a task as complete
void markTaskCompleted(ToDoList *list, int id)
{
    for (int i = 0; i < list->size; i++)
    {
        if (list->tasks[i].id == id)
        {
            list->tasks[i].completed = 1;
            return;
        }
    }
    printf("Task not found.\n");
}

// This function deletes a task
void deleteTask(ToDoList *list, int id)
{
    for (int i = 0; i < list->size; i++)
    {
        if (list->tasks[i].id == id)
        {
            for (int j = i; j < list->size - 1; j++)
            {
                list->tasks[j] = list->tasks[j + 1];
            }
            list->size--;
            return;
        }
    }
    printf("Task not found.\n");
}

// This function saves a task list to a file
void saveToFile(ToDoList *list, const char *filename)
{
    FILE *file = fopen(filename, "w");
    for (int i = 0; i < list->size; i++)
    {
        Task *t = &list->tasks[i];
        fprintf(file, "%d,%d,%d,%s\n", t->id, t->completed, t->priority, t->description);
    }
    fclose(file);
}

// This function loads a task list from a file
void loadFromFile(ToDoList *list, const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Could not open file.\n");
        return;
    }
    list->size = 0;
    while (!feof(file))
    {
        Task newTask;
        fscanf(file, "%d,%d,%d,%[^\n]", &newTask.id, &newTask.completed, &newTask.priority, newTask.description);
        list->tasks[list->size++] = newTask;
        if (newTask.id >= list->nextId)
            list->nextId = newTask.id + 1;
    }
    fclose(file);
}

// The main function
int main()
{
    ToDoList list;
    initList(&list);
    char command[10];

    while (1)
    {
        printf("\nCOmmands: add, complete, delete, view, save, load, exit\n");
        printf("Enter a command: ");
        scanf("%s", command);

        if (strcmp(command, "add") == 0)
        { // add command
            char desc[256];
            int priority;
            getchar(); // Clear newline

            printf("Enter task description: ");
            fgets(desc, 256, stdin);

            printf("Enter task priority: ");
            scanf("%d", &priority);

            desc[strcspn(desc, "\n")] = 0; // Remove newline from input

            addTask(&list, desc, priority);
        }
        else if (strcmp(command, "complete") == 0)
        { // complete command
            int id;

            printf("Enter task ID to mark complete: ");
            scanf("%d", &id);

            markTaskCompleted(&list, id);
        }
        else if (strcmp(command, "delete") == 0)
        { // delete command
            int id;

            printf("Enter task ID to delete: ");
            scanf("%d", &id);

            deleteTask(&list, id);
        }
        else if (strcmp(command, "view") == 0)
        { // view command
            displayTasks(&list);
        }
        else if (strcmp(command, "save") == 0)
        { // save a file
            saveToFile(&list, "tasks.txt");
        }
        else if (strcmp(command, "load") == 0)
        { // load file
            loadFromFile(&list, "tasks.txt");
        }
        else if (strcmp(command, "exit") == 0)
        { // exit command
            break;
        }
        else
        {
            printf("Unkown command.\n");
        }
    }

    free(list.tasks); // Clean up allocated memory
    return 0;
}