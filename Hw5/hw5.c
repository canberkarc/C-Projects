/*
 *  
 *
 *  Algorithm: 
 *      1) Get the item name from user.
 *      2) Call the recursive function with making it equals to "cost" value.
 *      3) get_cost function takes item as parameter and takes input about item then realize operations according to the characters,
 *         finds total cost of item with adding entered costs due to calculation.
 *      4) Finally get_cost returns total cost and total cost will be printed.
 *
 *
 * 
 */


#include <stdio.h>
#include <stdlib.h>

#define MAX_BUF_SIZE   128

int str_len(char *str);
double get_cost(char *item);


int main()
{
    char item_name[MAX_BUF_SIZE];
    double cost = 0;
    
    printf("> Define the object:\n");
    printf("> ");
    
    fgets(item_name, sizeof(item_name), stdin);
    item_name[str_len(item_name)-1] = '\0';
    
    cost = get_cost(item_name);

    printf("> Total cost of %s is %.6g\n", item_name, cost);
    
    return 0;
}



/* Returns the length of the string */
int str_len(char *str)
{
    int len = 0;
    char *p = str;
    
    while (*p != '\0') {
        len++;
        p++;
    }
    
    return len;
}

/* Recursive function which returns the cost of the item */
double get_cost(char *item)
{
    int c;
    
    char *p;
    double num = 0;                     /* Double value of the string number */
    
    int has_number = 0;                 /* Has a number read in a set */
    
    char buffer[MAX_BUF_SIZE];          /* Buffer holds a line */
    
    int len_num = 0;                    /* Number length */
    char number[MAX_BUF_SIZE];          /* Buffer which holds the number */
    
    int item_name_len = 0;              /* Item name length */
    char item_name[MAX_BUF_SIZE];       /* Buffer which holds the item name */
    
    int situation = 0;                  /* 0: idle case, 1: number is taking, 2: item name is taking */
    double cost = 0;                    /* Total cost of the current item */
    
    printf("> What is %s?:\n", item);
    printf("> ");
    
    fgets(buffer, sizeof(buffer), stdin);
    buffer[str_len(buffer)-1] = '\0';
    
    p = buffer;
    
    while (*p != '\0') { 
        
        c = *p;
        
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ) {    /* Alphabetical character condition */
           
            if (situation == 0)
                situation = 2;
            
            item_name[item_name_len] = c;
            item_name_len++;
        }
        else if ((c >= '0' && c <= '9') || (c == '.')) {    /* Numerical character condition*/
           
            if (situation == 0) {
                situation = 1;
                has_number = 1;
            }
            
            number[len_num] = c;
            len_num++;
        }
        else if (c == '*') {    /* Multiplication character condition */
           
            number[len_num] = '\0';
            num = atof(number);
            
            situation = 0;
        }
        else if (c == '+') {      /* Plus character condition */
          
            if (situation == 2) {
                item_name[item_name_len] = '\0';
                
                if (has_number)
                    cost += num * get_cost(item_name);
                else
                    cost += get_cost(item_name);
                
                item_name_len = 0;
                len_num = 0;
                situation = 0;
                has_number = 0;
            }
        } 
        else {  /* Other characters condition */
            
        }
        p++;
    }
    
    if (situation == 1) {
        number[len_num] = '\0';
        num = atof(number);
        cost += num;
    }
    else if (situation == 2) {
        item_name[item_name_len] = '\0';
                
        if (has_number)
            cost += num * get_cost(item_name);
        else
            cost += get_cost(item_name);
    }
    
    return cost;
}


