
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUF_SIZE        256

#define FILES_FILE          "files.txt"
#define COMMAND_FILE        "commands.txt"
#define OUTPUT_FILE         "output.txt"

// Node of linked list
typedef struct node {
    void *data;
    struct node *next;
} node_t;

typedef struct linkedlist {
    node_t *head;
    node_t *tail;
    int size;
} linkedlist_t;

// Directory info, parent is the parent of the directory, if -1 it is root
typedef struct dirinfo {
    int parent;
    char name[256];
    linkedlist_t *files;
} dirinfo_t;

void *pop_front_linkedlist(linkedlist_t *linkedlist);
int is_empty_linkedlist(linkedlist_t *linkedlist);

// Create a node with the given data
node_t *create_node(void *data)
{
    node_t *node = (node_t *) malloc(sizeof(node_t));
    
    node->data = data;
    node->next = NULL;
    
    return node;
}

// Destroy node
void destroy_node(node_t *node)
{
    if (node != NULL)
        free(node);
}

// Create linked list
linkedlist_t *create_linkedlist()
{
    linkedlist_t *linkedlist = (linkedlist_t *) calloc(1, sizeof(linkedlist_t));
    
    return linkedlist;
}

// Destroy linked list
void destroy_linkedlist(linkedlist_t *linkedlist)
{
    if (linkedlist != NULL) {
        while (!is_empty_linkedlist(linkedlist)) {
            pop_front_linkedlist(linkedlist);
        }
        
        free(linkedlist);
    }
}

// Push back new node to the linked list
void push_back_linkedlist(linkedlist_t *linkedlist, void *data)
{
    node_t *new_node = create_node(data);
    
    if (linkedlist->head == NULL) {
        linkedlist->head = new_node;
        linkedlist->tail = new_node;
    } else {
        linkedlist->tail->next = new_node;
        linkedlist->tail = new_node;
    }
    
    linkedlist->size++;
}

// Push front new node to the linked list
void push_front_linkedlist(linkedlist_t *linkedlist, void *data)
{
    node_t *new_node = create_node(data);
    
    if (linkedlist->head == NULL) {
        linkedlist->head = new_node;
        linkedlist->tail = new_node;
    } else {
        new_node->next = linkedlist->head;
        linkedlist->head = new_node;
    }
    
    linkedlist->size++;
}

// Insert new node to the linked list at the specified index
void insert_linkedlist(linkedlist_t *linkedlist, int index, void *data)
{
    int i;
    node_t *node, *new_node;
    
    if (index < 0)
        index = 0;
    
    if (index > linkedlist->size)
        index = linkedlist->size;
    
    if (index == 0) {
        push_front_linkedlist(linkedlist, data);
    } else if (index == linkedlist->size) {
        push_back_linkedlist(linkedlist, data);
    } else {
        new_node = create_node(data);
        
        node = linkedlist->head;
        
        for (i = 1; i < index; i++)
            node = node->next;
        
        new_node->next = node->next;
        node->next = new_node;
        
        linkedlist->size++;
    }
}

// Pop back node from the linked list
void *pop_back_linkedlist(linkedlist_t *linkedlist)
{
    void *data = NULL;
    node_t *node = NULL;
    
    if (linkedlist->size == 0)
        return NULL;
    
    node = linkedlist->head;
    
    if (linkedlist->size == 1) {
        linkedlist->head = NULL;
        linkedlist->tail = NULL;
    } else {
        while (node->next != linkedlist->tail)
            node = node->next;
        
        linkedlist->tail = node;
        node = node->next;
        linkedlist->tail->next = NULL;
        
        data = node->data;
    }
    
    free(node);
    
    linkedlist->size--;
    
    return data;
}

// Pop front node from the linked list
void *pop_front_linkedlist(linkedlist_t *linkedlist)
{
    void *data = NULL;
    node_t *node = NULL;
    
    if (linkedlist->size == 0)
        return NULL;
    
    node = linkedlist->head;
    data = node->data;
    
    if (linkedlist->size == 1) {
        linkedlist->head = NULL;
        linkedlist->tail = NULL;
    } else {
        linkedlist->head = node->next;
    }
    
    free(node);
    
    linkedlist->size--;
    
    return data;
}

// Remove node from the linked list at the specified index
void *remove_linkedlist(linkedlist_t *linkedlist, int index)
{
    int i;
    void *data = NULL;
    node_t *node, *rem_node = NULL;
    
    if (linkedlist->size == 0)
        return NULL;
    
    if (index < 0)
        index = 0;
    
    if (index >= linkedlist->size)
        index = linkedlist->size - 1;
    
    if (index == 0) {
        data = pop_front_linkedlist(linkedlist);
    } else if (index == (linkedlist->size - 1)) {
        data = pop_back_linkedlist(linkedlist);
    } else {
        node = linkedlist->head;
        
        for (i = 1; i < index; i++)
            node = node->next;
        
        rem_node = node->next;
        data = rem_node->data;
        
        node->next = rem_node->next;
        destroy_node(rem_node);
        
        linkedlist->size--;
    }
    
    return data;
}

// Get node at the front of the linked list 
void *front_linkedlist(linkedlist_t *linkedlist)
{
    if (linkedlist->size == 0)
        return NULL;
    
    return linkedlist->head->data;
}

// Get node at the back of the node linked list 
void *back_linkedlist(linkedlist_t *linkedlist)
{
    if (linkedlist->size == 0)
        return NULL;
    
    return linkedlist->tail->data;
}

// Check if linked list is empty
int is_empty_linkedlist(linkedlist_t *linkedlist)
{
    return linkedlist->size == 0;
}

// Get next node in the linked list
node_t *next_linkedlist(linkedlist_t *linkedlist, node_t *node)
{
    if (node == NULL)
        return linkedlist->head;
    
    return node->next;
}

// Get indexed node in the linked list
node_t *at_linkedlist(linkedlist_t *linkedlist, int index)
{
    int i = 0;
    node_t *node;
    
    node = linkedlist->head;
    
    if (index < 0 || index >= linkedlist->size)
        return NULL;
    
    for (i = 1; i <= index; i++)
        node = node->next;
    
    return node;
}

// Find directory with the given name and return index
int find_dirinfo(dirinfo_t *dirinfo_list, int dirinfo_count, char *name)
{
    int i;
    int result = -1;
    
    for (i = 0; i < dirinfo_count; i++) {
        if (strcmp(dirinfo_list[i].name, name) == 0) {
            result = i;
            break;
        }
    }
    
    return result;
}

// Find directory with its parent
int find_dirinfo_with_parent(dirinfo_t *dirinfo_list, int dirinfo_count, int parent)
{
    int i;
    int result = -1;
    
    for (i = 0; i < dirinfo_count; i++) {
        if (dirinfo_list[i].parent == parent) {
            result = i;
            break;
        }
    }
    
    return result;
}

// Swap two directories
void swap_dirinfo(dirinfo_t *dirinfo_list, int index1, int index2)
{
    dirinfo_t temp = dirinfo_list[index1];
    dirinfo_list[index1] = dirinfo_list[index2];
    dirinfo_list[index2] = temp;
}

// Add directory to the directory structure
int add_dirinfo(dirinfo_t **pdirinfo_list, int *pdirinfo_count, char *name, int parent, linkedlist_t *files)
{
    int i;
    int result = -1;
    
    for (i = 0; i < *pdirinfo_count; i++) {
        if (strcmp((*pdirinfo_list)[i].name, name) == 0) {
            if ((*pdirinfo_list)[i].parent == -1)
                (*pdirinfo_list)[i].parent = parent;
            
            result = i;
            break;
        }
    }
    
    if (result == -1) {
        *pdirinfo_list = (dirinfo_t *) realloc(*pdirinfo_list, sizeof(dirinfo_t) * (*pdirinfo_count + 1));
        
        snprintf((*pdirinfo_list)[*pdirinfo_count].name, sizeof ((*pdirinfo_list)[*pdirinfo_count].name), "%s", name);
        (*pdirinfo_list)[*pdirinfo_count].parent = parent;
        result = *pdirinfo_count;
        
        if (files != NULL)
            (*pdirinfo_list)[*pdirinfo_count].files = files;
        else
            (*pdirinfo_list)[*pdirinfo_count].files = create_linkedlist();
        
        (*pdirinfo_count)++;
    }
    
    return result;
}

/*
* 	This function is for visualization of program on terminal.It is not necessary to use this function to see 
* that program works fine.If visualization is wanted,comment lines should be made normal lines.
*/
void print_dirinfo(dirinfo_t *dirinfo_list, int dirinfo_count, int index)
{
    int i;
    node_t *node = NULL;
    
    if (dirinfo_count == index)
        return;
    
    //for (i = 0; i < index; i++)
      //  printf("\t");
    
    //printf("%s\n", dirinfo_list[index].name);
    
    print_dirinfo(dirinfo_list, dirinfo_count, index + 1);
    
    while((node = next_linkedlist(dirinfo_list[index].files, node)) != NULL) {
      //  for (i = 0; i < index + 1; i++)
        //    printf("\t");
        //printf("%s\n", (char *) node->data);
    }
    
    return;
}

// Add file to the directory
void add_file_to_directory(dirinfo_t *dirinfo, char *name)
{    
    push_back_linkedlist(dirinfo->files, strdup(name));
}

// Add all directory path as directories and file at the end
void add_directories(dirinfo_t **pdirinfo_list, int *pdirinfo_count, char **line_tokens, int token_count)
{
    int i;
    int parent = -1;
    
    for (i = 0; i < token_count - 1; i++) {
        parent = add_dirinfo(pdirinfo_list, pdirinfo_count, line_tokens[i], parent, NULL);
    }
    
    add_file_to_directory(&((*pdirinfo_list)[parent]), line_tokens[token_count-1]);
}

// Add sorted directories
void add_sorted_dirinfo(dirinfo_t *dirinfo_list, int dirinfo_count, dirinfo_t **pdirinfo_slist, int *pdirinfo_scount)
{
    int i;
    int parent = -1;
    
    for (i = 0; i < dirinfo_count; i++) {
        parent = find_dirinfo_with_parent(dirinfo_list, dirinfo_count, parent);

        add_dirinfo(pdirinfo_slist, pdirinfo_scount, dirinfo_list[parent].name, i-1, dirinfo_list[parent].files);
        dirinfo_list[parent].files = NULL;
    }
}

// Copy all files in from directory to to directory
void copy_all(dirinfo_t *dirinfo_from, dirinfo_t *dirinfo_to)
{
    node_t *node = NULL;
    
    while((node = next_linkedlist(dirinfo_from->files, node)) != NULL) {
        push_back_linkedlist(dirinfo_to->files, strdup(node->data));
    }
}

// Copy file given to to directory
void copy_file(char *filename, dirinfo_t *dirinfo_to)
{
    push_back_linkedlist(dirinfo_to->files, strdup(filename));
}

// Move all files in from directory to to directory
void move_all(dirinfo_t *dirinfo_from, dirinfo_t *dirinfo_to)
{
    node_t *node = NULL;
    
    while((node = next_linkedlist(dirinfo_from->files, node)) != NULL) {
        push_back_linkedlist(dirinfo_to->files, strdup((char *) node->data));
    }
    
    while (!is_empty_linkedlist(dirinfo_from->files)) {
        void *data = pop_front_linkedlist(dirinfo_from->files);
        free(data);
    }
}

// Move file given in from directory to to directory
void move_file(dirinfo_t *dirinfo_from, char *filename, dirinfo_t *dirinfo_to)
{
    int i = 0;
    node_t *node = NULL;
    
    while((node = next_linkedlist(dirinfo_from->files, node)) != NULL) {
        if (strcmp((char *) node->data, filename) == 0) {
            break;
        }
        
        i++;
    }
    
    if (i < dirinfo_from->files->size) {
        remove_linkedlist(dirinfo_from->files, i);
        push_back_linkedlist(dirinfo_to->files, strdup(filename));
    }
}

// Delete all files in a directory
void delete_all(dirinfo_t *dirinfo_from)
{
    while (!is_empty_linkedlist(dirinfo_from->files)) {
        void *data = pop_front_linkedlist(dirinfo_from->files);
        free(data);
    }
}

// Delete file in a directory
void delete_file(dirinfo_t *dirinfo_from, char *filename)
{
    int i = 0;
    node_t *node = NULL;
    
    while((node = next_linkedlist(dirinfo_from->files, node)) != NULL) {
        if (strcmp((char *) node->data, filename) == 0) {
            break;
        }
        
        i++;
    }
    
    if (i < dirinfo_from->files->size) {
        remove_linkedlist(dirinfo_from->files, i);
    }
}

// Tokenize the line
char** parse_line(char *line, int *token_count)
{
    char **tokens = NULL;
    char *token = NULL;
    
    int len;
    char *p;
    const char delim[] = " /\t\r\n";
    
    *token_count = 0;
    
    tokens = (char **) malloc(sizeof(char *));
    
    p = line;
    p = strtok(p, delim);
        
    while (p != NULL) {
        len = strlen(p);
        token = (char *) malloc((len + 1) * sizeof(char));
        
        strcpy(token ,p);
        
        tokens = (char **) realloc(tokens, (*token_count + 1) * sizeof(char *));
        tokens[*token_count] = token;
        (*token_count)++;
        
        p = strtok (NULL, delim); 
    }
    
    return tokens;
}

// Release memory of line tokens
void free_line_tokens(char **line_tokens, int token_count)
{
    int i;
    
    for (i = 0; i < token_count; i++) {
        if (line_tokens[i] != NULL)
            free(line_tokens[i]);
    }
    
    if (line_tokens != NULL)
        free(line_tokens);
}

// Read files.txt and create directory structure (linked list)
void read_files(dirinfo_t **pdirinfo_slist, int *pdirinfo_scount)
{
    int len;
    char buffer[4096];
    
    dirinfo_t *dirinfo_glist = NULL;
    int dirinfo_gcount = 0;

    FILE *fp = fopen(FILES_FILE, "r");
    
    if(fp == NULL)
        return;
    
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        len = strlen(buffer);
        
        if (len > 1) {
            char** tokens;
            int token_count;
            
            buffer[len-1] = '\0';
            
            tokens = parse_line(buffer, &token_count);
            
            add_directories(&dirinfo_glist, &dirinfo_gcount, tokens, token_count);
        }
    }
    
    add_sorted_dirinfo(dirinfo_glist, dirinfo_gcount, pdirinfo_slist, pdirinfo_scount);
    print_dirinfo(*pdirinfo_slist, *pdirinfo_scount, 0);
    
    fclose(fp);
}

// Recursive method which reads command, line by line (Each line is a command in commands.txt)
void read_command_file_line(FILE *fp, char ***pcommands, int *command_count)
{
    char *command = NULL;
    
    int len;
    char buffer[MAX_BUF_SIZE];
    
    if (fgets(buffer, sizeof(buffer), fp) != NULL) {
        len = strlen(buffer);
        command = (char *) malloc((len + 1) * sizeof(char));
        
        strcpy(command, buffer);
        
        *pcommands = (char **) realloc(*pcommands, (*command_count + 1) * sizeof(char *));
        (*pcommands)[*command_count] = command;
        (*command_count)++;
        
        read_command_file_line(fp, pcommands, command_count);
    }
    
    return;
}

// Read command file and parse it line by line to a 2D char array
char** read_command_file(char *filename, int *command_count)
{
    char **commands = NULL;
    
    *command_count = 0;
    
    FILE *fp = fopen(filename, "r");
    
    if(fp == NULL)
        return NULL;
    
    commands = (char **) malloc(sizeof(char *));
    
    read_command_file_line(fp, &commands, command_count);
    
    fclose(fp);
    
    return commands;
    
}

// Tokenize the command
char** parse_command(char *command, int *token_count)
{
    char **tokens = NULL;
    char *token = NULL;
    
    int len;
    char *p;
    const char delim[] = " \t\r\n";
    
    *token_count = 0;
    
    tokens = (char **) malloc(sizeof(char *));
    
    p = command;
    p = strtok(p, delim);
        
    while (p != NULL) {
        len = strlen(p);
        token = (char *) malloc((len + 1) * sizeof(char));
        
        strcpy(token ,p);
        
        tokens = (char **) realloc(tokens, (*token_count + 1) * sizeof(char *));
        tokens[*token_count] = token;
        (*token_count)++;
        
        p = strtok (NULL, delim); 
    }
    
    return tokens;
}

// Release memory of commands
void free_commands(char **commands, int command_count)
{
    int i;
    
    for (i = 0; i < command_count; i++) {
        if (commands[i] != NULL)
            free(commands[i]);
    }
    
    if (commands != NULL)
        free(commands);
}

// Release memory of command tokens
void free_command_tokens(char **command_tokens, int token_count)
{
    int i;
    
    for (i = 0; i < token_count; i++) {
        if (command_tokens[i] != NULL)
            free(command_tokens[i]);
    }
    
    if (command_tokens != NULL)
        free(command_tokens);
}

void print_output(dirinfo_t *dirinfo_list, int dirinfo_count)
{
    int i, j;
    node_t *node = NULL;
    char buf[4096];
    
    FILE *fp;
    
    fp = fopen(OUTPUT_FILE, "w");
    
    if(fp == NULL)
        return;
    
    for (i = 0; i < dirinfo_count; i++) {
        int len = 0;
        
        for (j = 0; j <= i; j++) {
            len += snprintf(buf + len, sizeof(buf) - len, "/%s", dirinfo_list[j].name);
        }
        
        fprintf(fp, "%s\n", buf);
        
        while((node = next_linkedlist(dirinfo_list[i].files, node)) != NULL) {
            fprintf(fp, "%s/%s\n", buf, (char *) node->data);
        }
    }
    
    fclose(fp);
    
    return;
}

int main()
{
    int i;
    dirinfo_t *dirinfo_slist = NULL;
    int dirinfo_scount = 0;
    
    int dir_index = 0;
    
    read_files(&dirinfo_slist, &dirinfo_scount);
    
    print_output(dirinfo_slist, dirinfo_scount);
    
    // Holds lines which is read from commands.txt
    char **commands;
    
    // Holds line count which is read from commands.txt
    int command_count;
    
    // Read all commands
    commands = read_command_file("commands.txt", &command_count);
        
    for (i = 0; i < command_count; i++) {
        int token_count;
        
        dir_index = dir_index;
        
        // Tokenize a command to tokens
        char **tokens = parse_command(commands[i], &token_count);
        
        // Do the appropriate action according to the command
        if (token_count > 0) {
            if (strcasecmp(tokens[0], "copy") == 0) {
                int index1;
                int index2;
                
                int first_count = 0;
                int second_count = 0;
                
                char** first_param = parse_line(tokens[1], &first_count);
                char** second_param = parse_line(tokens[2], &second_count);

                index1 = find_dirinfo(dirinfo_slist, dirinfo_scount, first_param[first_count-1]);
                index2 = find_dirinfo(dirinfo_slist, dirinfo_scount, second_param[second_count-1]);
                
                if (index1 == -1) {
                    index1 = find_dirinfo(dirinfo_slist, dirinfo_scount, first_param[first_count-2]);
                    
                    copy_file(first_param[first_count-1], &dirinfo_slist[index2]);
                } else {
                    copy_all(&dirinfo_slist[index1], &dirinfo_slist[index2]);
                }
                
                print_dirinfo(dirinfo_slist, dirinfo_scount, 0);
            } else if (strcasecmp(tokens[0], "move") == 0) {
                int index1;
                int index2;
                
                int first_count = 0;
                int second_count = 0;
                
                char** first_param = parse_line(tokens[1], &first_count);
                char** second_param = parse_line(tokens[2], &second_count);

                index1 = find_dirinfo(dirinfo_slist, dirinfo_scount, first_param[first_count-1]);
                index2 = find_dirinfo(dirinfo_slist, dirinfo_scount, second_param[second_count-1]);
                
                if (index1 == -1) {
                    index1 = find_dirinfo(dirinfo_slist, dirinfo_scount, first_param[first_count-2]);
                    
                    move_file(&dirinfo_slist[index1], first_param[first_count-1], &dirinfo_slist[index2]);
                } else {
                    move_all(&dirinfo_slist[index1], &dirinfo_slist[index2]);
                }
                
                print_dirinfo(dirinfo_slist, dirinfo_scount, 0);
            } else if (strcasecmp(tokens[0], "delete") == 0) {
                int index1;
                
                int first_count = 0;
                
                char** first_param = parse_line(tokens[1], &first_count);

                index1 = find_dirinfo(dirinfo_slist, dirinfo_scount, first_param[first_count-1]);
                
                if (index1 == -1) {
                    index1 = find_dirinfo(dirinfo_slist, dirinfo_scount, first_param[first_count-2]);
                    
                    delete_file(&dirinfo_slist[index1], first_param[first_count-1]);
                } else {
                    int c;
                    for (c = dirinfo_scount - 1; c >= index1; c--) {
                        delete_all(&dirinfo_slist[c]);
                    }
                    
                    dirinfo_scount = index1;
                }
                
                print_dirinfo(dirinfo_slist, dirinfo_scount, 0);
            } else if (strcasecmp(tokens[0], "cd") == 0) {
                int index1;
                
                int first_count = 0;
                
                char** first_param = parse_line(tokens[1], &first_count);

                if (first_count == 0) {
                    dir_index = 0;
                    continue;
                }
                
                if (strcmp(first_param[first_count-1], "..") == 0) {
                    if (dir_index > 0)
                        dir_index--;
                } else {
                    index1 = find_dirinfo(dirinfo_slist, dirinfo_scount, first_param[first_count-1]);
                    
                    if (index1 >= 0)
                        dir_index = index1;
                }
            }
        }
    }
    
    return 0;
}
