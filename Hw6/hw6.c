#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUF_SIZE        256
#define FILE_INPUT          "input.txt"
#define FILE_CIRCUIT        "circuit.txt"

/*  Struct which holds inputs and gates with their names and values  */
typedef struct container_s {
    char *name;
    int value;
} container_t;

/*  Function to create a gate or input container with the given name and value  */
container_t *create_container(char *name, int value)
{
    container_t *container = (container_t *) malloc(sizeof(container_t));
    
    container->name = strdup(name);
    container->value = value;
    
    return container;
}

/*  Free gate or input container  */
void free_container(container_t *container)
{
    if (container != NULL) {
        free(container->name);
        free(container);
    }
}

/*  Get line input as an integer array from input.txt  */
int get_file_line_input(FILE *fp, int **pinput, int *psize)
{
    int c;
    
    int size = 0;
    int *input = NULL;
    
    while ((c = fgetc(fp)) != EOF) {
        if (c == '\n') {
            break;
        }
        
       	if (c == '0') {
            input = realloc(input, (size + 1) * sizeof(int));
            input[size++] = 0;
        } else if (c == '1') {
            input = realloc(input, (size + 1) * sizeof(int));
            input[size++] = 1;
        }
    }
    
    *pinput = input;
    *psize = size;
    
    return size;
}

/*  Function to add gate or input container to the container list  */
void add_to_container_list(container_t ***pcontainer_list, int *pcontainer_list_size, container_t *container)
{
    int container_list_size = *pcontainer_list_size;
    container_t **container_list = *pcontainer_list;

    if (container_list_size == 0) {
        container_list = (container_t **) malloc(sizeof(container_t *));

        container_list[0] = container;
        container_list_size++;
    } else {
        container_list = (container_t **) realloc(container_list, (container_list_size + 1) * sizeof(container_t *));

        container_list[container_list_size] = container;
        container_list_size++;
    }

    *pcontainer_list = container_list;
    *pcontainer_list_size = container_list_size;
}

/*  Function to find gate or input container in the container list  */
container_t *find_in_container_list(container_t **container_list, int container_size, char *name)
{
    int i;
    
    for (i = 0; i < container_size; i++) {
        if (strcmp(container_list[i]->name, name) == 0)
            return container_list[i];
    }
    
    return NULL;
}

/*  Freeing container list  */
void free_container_list(container_t **container_list, int container_list_size)
{
    if (container_list_size > 0) {
        int i;
        
        for (i = 0; i < container_list_size; i++) {
            free_container(container_list[i]);
        }
        
        free(container_list);
    }
}

/*  Recursive method to read commands, line by line (Each line is a command in circuit.txt)  */
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

/*  Read command file and parse it line by line to a 2D char array  */
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

/*  Tokenize the command  */
char** parse_command(char *command, int *token_count)
{
    char **tokens = NULL;
    char *token = NULL;
    int len;
    char *p;
    char arr[] = " \t\r\n";
    *token_count = 0;
    tokens = (char **) malloc(sizeof(char *));
    
    p = command;
    p = strtok(p, arr);
        
    while (p != NULL) {
        len = strlen(p);
        token = (char *) malloc((len + 1) * sizeof(char));
        
        strcpy(token ,p);
        
        tokens = (char **) realloc(tokens, (*token_count + 1) * sizeof(char *));
        tokens[*token_count] = token;
        (*token_count)++;
        
        p = strtok (NULL, arr); 
    }
    
    return tokens;
}

/*  Free memory of commands  */
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

/*  Free memory of command tokens  */
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


int main()
{
    int container_list_size = 0; /*  Size of container list  */
    int *input; /*  Holds one line of binary values which is read from input.txt  */
    int input_size;  /*  Holds one line of binary values count which is read from input.txt  */
    int i;
    char **commands;  /*  Holds lines which is read from circuit.txt  */
    int command_count;  /*  Holds line count which is read from circuit.txt  */
    container_t **container_list = NULL;  /* List which holds containers (variables), it is implemented as "array", not "linkedlist" */
    FILE *fp;
    
    if ((fp = fopen(FILE_INPUT, "r")) == NULL) {
        printf("Cannot open input file\n");
        return -1;
    }
    
    while(get_file_line_input(fp, &input, &input_size) > 0) { /*  Get a line of binary input to input array from input.txt  */
       
        int last_result = 0; /*  last_result holds the last operation result, it is printed at the end of all commands  */
        
        commands = read_command_file(FILE_CIRCUIT, &command_count);  /*  Read all commands(lines) from circuit.txt  */
        
        for (i = 0; i < command_count; i++) {
            int token_count;
            
            char **tokens = parse_command(commands[i], &token_count);  /*  Tokenize a command to tokens  */
            
            if (token_count > 0) {  /*  Do the required action according to the command  */
                if (strcasecmp(tokens[0], "INPUT") == 0) {/* I used "strcasecmp" for making comparison without lower-upper case condition  */
                    int k = 0;	
                    
                    /*  Create/assign containers as an input  */
                    /*  INPUT a b c d (circuit.txt) <-> 1 1 0 1 (input.txt)  */
                    for (k = 1; k < token_count; k++) {
                        container_t *container = find_in_container_list(container_list, container_list_size, tokens[k]);
                        
                        if (container != NULL) {
                            container->value = input[k-1];
                        } else {
                            container = create_container(tokens[k], input[k-1]);
                            add_to_container_list(&container_list, &container_list_size, container);
                        }
                    }
                } 
                else if (strcasecmp(tokens[0], "AND") == 0) {
                    char *out_name = tokens[1];
                    char *in1_name = tokens[2];
                    char *in2_name = tokens[3];
                    
                    /*  AND res x y (circuit.txt), I have found x and y in the container list and AND the values  */
                    /*  Then I create/assign the result to the variable res  */
                    
                    container_t *container_out = NULL;
                    container_t *container_in1 = find_in_container_list(container_list, container_list_size, in1_name);
                    container_t *container_in2 = find_in_container_list(container_list, container_list_size, in2_name);
                    
                    if (container_in1 == NULL || container_in2 == NULL) {
                        printf("Error: AND\n");
                        return -1;
                    }
                    
                    last_result = container_in1->value && container_in2->value;
                    
                    container_out = find_in_container_list(container_list, container_list_size, out_name);
                        
                    if (container_out != NULL) {
                        container_out->value = last_result;
                    } else {
                        container_out = create_container(out_name, last_result);
                        add_to_container_list(&container_list, &container_list_size, container_out);
                    }
                }
                else if (strcasecmp(tokens[0], "OR") == 0) {
                    char *out_name = tokens[1];
                    char *in1_name = tokens[2];
                    char *in2_name = tokens[3];
                    
                    /*  OR res x y (circuit.txt), I have found x and y in the container list and OR the values  */
                    /*  Then I create/assign the result to the variable res  */
                    
                    container_t *container_out = NULL;
                    container_t *container_in1 = find_in_container_list(container_list, container_list_size, in1_name);
                    container_t *container_in2 = find_in_container_list(container_list, container_list_size, in2_name);
                    
                    if (container_in1 == NULL || container_in2 == NULL) {
                        printf("Error: OR\n");
                        return -1;
                    }
                    
                    last_result = container_in1->value || container_in2->value;
                    
                    container_out = find_in_container_list(container_list, container_list_size, out_name);
                        
                    if (container_out != NULL) {
                        container_out->value = last_result;
                    } else {
                        container_out = create_container(out_name, last_result);
                        add_to_container_list(&container_list, &container_list_size, container_out);
                    }
                }
                else if (strcasecmp(tokens[0], "NOT") == 0) {
                    char *out_name = tokens[1];
                    char *in_name = tokens[2];
                    
                    /*  NOT res x (circuit.txt), I have found x in the container list and NOT the value  */
                    /*  Then I create / assign the result to the variable res  */
                    
                    container_t *container_out = NULL;
                    container_t *container_in = find_in_container_list(container_list, container_list_size, in_name);
                    
                    if (container_in == NULL) {
                        printf("Error: NOT\n");
                        return -1;
                    }
                    
                    last_result = !(container_in->value);
                    
                    container_out = find_in_container_list(container_list, container_list_size, out_name);
                        
                    if (container_out != NULL) {
                        container_out->value = last_result;
                    } else {
                        container_out = create_container(out_name, last_result);
                        add_to_container_list(&container_list, &container_list_size, container_out);
                    }
                }
                else if (strcasecmp(tokens[0], "FLIPFLOP") == 0) {
                    char *out_name = tokens[1];
                    char *in_name = tokens[2];
                    int former_out = 0;
                    
                    /*  FLIPFLOP res x (circuit.txt), I have found x in the container list and NOT the value  */
                    /*  Then I create/assign the result to the variable res  */
                    
                    container_t *container_out = find_in_container_list(container_list, container_list_size, out_name);
                    container_t *container_in = find_in_container_list(container_list, container_list_size, in_name);
                    
                    if (container_in == NULL) {
                        printf("Error: FLIPFLOP\n");
                        return -1;
                    }
                    
                    /*  If the value is already in the container list, I take past "former_out" value else default is 0  */
                    if (container_out != NULL)
                        former_out = container_out->value;
                    
                    /*  As I can see, flipflop result is just an XOR operation of former_out and input  */
                    last_result = container_in->value ^ former_out;
                        
                    if (container_out != NULL) {
                        container_out->value = last_result;
                    } else {
                        container_out = create_container(out_name, last_result);
                        add_to_container_list(&container_list, &container_list_size, container_out);
                    }
                }
            }
            
            /*  Free all command tokens  */
            free_command_tokens(tokens, token_count);
        }
        
        printf("%d\n", last_result);
        
        /*  Free all command lines  */
        free_commands(commands, command_count);
        
        /*  Free all input array which holds binary input numbers  */
        free(input);
    }
    
    /*  Close input file  */
    fclose(fp);
    
    /*  Free container list which has been createds  */
    free_container_list(container_list, container_list_size);
    
    return 0;
}
