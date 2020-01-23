/*
*
*
* !!!!! IMPORTANT !!!!!
* There was an uncertainty about homework which is that whether single points which are above water and paths should be assumed as island or not.
*  So I have done both of possibilities.
*  If single points which are above water and paths should be assumed as island,please run first code.
*  Otherwise,if only at least 3 connected points(above water) whose at least 2 points' x and y coordinates must be different should be assumed as island,
*  please run second code.
*  
*
*/

/*****************************FIRST CODE*********************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define INPUT_FILE_NAME     "input.txt"
#define OUTPUT_FILE_NAME    "output.txt"

#define MAX_ARRAY_SIZE      262144

//  Convert 2d coordinate to 1d coordinate  
int convert_2d_to_1d(int row, int col, int dim)
{
    return row * dim + col;
}

//  Read input file, return value is level value L  
//  Fill input array with numbers read, psize holds total number of integers read except level value L  
int read_input_file(int *input, int *psize)
{
    int value;
    int level;
    int result;
    int size = 0;
    
    FILE *fp;
    
    fp = fopen(INPUT_FILE_NAME, "r");
    
    if(fp == NULL)
        return -1;
    
    fscanf(fp, "%d", &level);
    
    while (!feof(fp)) {
        result = fscanf(fp, "%d", &value);
        
        if (result > 0) {
            input[size++] = value;
        }
    }

    fclose(fp);

    *psize = size;
    
    return level;
}

//  Is current position (row, col) is in the map, where dim is the dimension of map  
int is_valid(int row, int col, int dim)
{
    return row >= 0 && col >= 0 && row < dim && col < dim;
}

//  Assign -1 if terrain is under water else assign its current index  
void arrange_input(int *input, int size, int level)
{
    int row, col, l;
    int dim = (int) sqrt(size);
    
    for (row = 0; row < dim; row++) {
        for (col = 0; col < dim; col++) {
            int pos = convert_2d_to_1d(row, col, dim);
            
            l = input[pos];
            
            if (l > level) {
                input[pos] = pos;
            } else {
                input[pos] = -1;
            }
        }
    }
}

//  Unite two coordinates (I connect neighbours)  
void unite(int *input, int p, int q)
{
    int i;
    int pid = input[p];
    int qid = input[q];
    
    int max = p > q ? p : q;
    
    for(i = 0; i <= max; i++) {
        if (input[i] == pid) {
            input[i] = qid;
        }
    }
}

//  Change all integer values from_index to another integer value in the map  
void change_value(int *input, int size, int from_index, int to_index)
{
    int i;
    
    for(i = 0; i < size; i++) {
        if (input[i] == from_index) {
            input[i] = to_index;
        }
    }
}

//  Unite all neigbours (make same index value for the same island)  
//  Then assign indexes to each island beginning from 0  
int unite_parts(int *input, int size)
{
    int row, col, l;
    int curr_index = 0;
    int dim = (int) sqrt(size);
    
    for (row = 0; row < dim; row++) {
        for (col = 0; col < dim; col++) {
            int p = convert_2d_to_1d(row, col, dim);
            
            l = input[p];
            
            if (l >= 0) {
                if (is_valid(row - 1, col, dim)) {
                    int q = convert_2d_to_1d(row - 1, col, dim);
                    
                    if (input[q] >= 0) {
                        unite(input, p, q);
                    }
                }
                
                if (is_valid(row, col - 1, dim)) {
                    int q = convert_2d_to_1d(row, col - 1, dim);
                    
                    if (input[q] >= 0) {
                        unite(input, p, q);
                    }
                }
            }
        }
    }
    
    for (row = 0; row < dim; row++) {
        for (col = 0; col < dim; col++) {
            int p = convert_2d_to_1d(row, col, dim);
            
            if (input[p] > 0)
                input[p] += 1000000;
        }
    }
    
    for (row = 0; row < dim; row++) {
        for (col = 0; col < dim; col++) {
            int p = convert_2d_to_1d(row, col, dim);
            
            l = input[p];
            
            if (l > 0 && l > curr_index) {
                change_value(input, size, l, curr_index);
                curr_index++;
            }
        }
    }
    
    return curr_index;
}

//  Print result to the output file  
void print_result_to_file(int *input, int size, int number_of_islands)
{
    int row, col, l;
    int dim = (int) sqrt(size);
    int curr_index = 0;
    
    FILE *fp;
    
    fp = fopen(OUTPUT_FILE_NAME, "w");
    
    if(fp == NULL)
        return;
    
    fprintf(fp, "%d\n", number_of_islands);
    
    for (row = 0; row < dim; row++) {
        for (col = 0; col < dim; col++) {
            int p = convert_2d_to_1d(row, col, dim);
            
            l = input[p];
            
            if (l == curr_index) {
                fprintf(fp, "x:%d   y:%d\n", col, row);
                curr_index++;
            }
        }
    }
    
    fclose(fp);
}

int main()
{
    //   The only array used to hold the map  
    int input[MAX_ARRAY_SIZE] = { 0 };
    //   Total numbers read from file except level value L  
    int size = 0;
    //  Level value L  
    int level = 0;
    //  Number of islands  
    int number_of_islands = 0;
  
    //  Read file and assign level, input array values and size  
    level = read_input_file(input, &size);
    
    //  Arrange input according to x > L  
    arrange_input(input, size, level);
    
    //  Unite connected terrains and index them as island  
    number_of_islands = unite_parts(input, size);
    
    //  Print the read to the output file  
    print_result_to_file(input, size, number_of_islands);
    
    return 0;  
}




/********************************************FIRST CODE END*********************************************************/



/*********************************************SECOND CODE***********************************************/


    /*


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define INPUT_FILE_NAME     "input.txt"
#define OUTPUT_FILE_NAME    "output.txt"

#define MAX_ARRAY_SIZE      262144

//  Convert 2d coordinate to 1d coordinate  
int convert_2d_to_1d(int row, int col, int dim)
{
    return row * dim + col;
}


//  Read input file, return value is level value L  
//  Fill input array with numbers read, psize holds total number of integers read except level value L  
int read_input_file(int *input, int *psize)
{
    int value;
    int level;
    int result;
    int size = 0;
    
    FILE *fp;
    
    fp = fopen(INPUT_FILE_NAME, "r");
    
    if(fp == NULL)
        return -1;
    
    fscanf(fp, "%d", &level);
    
    while (!feof(fp)) {
        result = fscanf(fp, "%d", &value);
        
        if (result > 0) {
            input[size++] = value;
        }
    }

    fclose(fp);

    *psize = size;
    
    return level;
}


//  Is current position (row, col) is in the map, where dim is the dimension of map  
int is_valid(int row, int col, int dim)
{
    return row >= 0 && col >= 0 && row < dim && col < dim;
}


//  Assign -1 if terrain is under water else assign its current index  
void arrange_input(int *input, int size, int level)
{
    int row, col, l, pos;
    int dim = (int) sqrt(size);
    
    for (row = 0; row < dim; row++) {
        for (col = 0; col < dim; col++) {
            pos = convert_2d_to_1d(row, col, dim);
            
            l = input[pos];
            
            if (l > level) {
                input[pos] = pos;
            } else {
                input[pos] = -1;
            }
        }
    }
}


//  Unite two coordinates (I connect neighbours)  
void unite(int *input, int p, int q)
{
    int i;
    int pid = input[p];
    int qid = input[q];
    
    int max = p > q ? p : q;
    
    for(i = 0; i <= max; i++) {
        if (input[i] == pid) {
            input[i] = qid;
        }
    }
}


//  Change all integer values from_index to another integer value in the map 
void change_value(int *input, int size, int from_index, int to_index)
{
    int i;
    
    for(i = 0; i < size; i++) {
        if (input[i] == from_index) {
            input[i] = to_index;
        }
    }
}


//  Is given index value an island? 1 if true, 0 if false  
int is_island(int *input, int size, int value)
{
    int row, col, l, tot, p;
    int dim = (int) sqrt(size);
    
    int vertical = 0, horizontal = 0;
    
    for (row = 0; row < dim; row++) {
        tot = 0;
         for (col = 0; col < dim; col++) {
            p = convert_2d_to_1d(row, col, dim);
            
            l = input[p];
            
            if (l == value) {
                tot++;
            }
        }
        
        if (tot > 0) {
            vertical++;
        }
        
        if (tot > horizontal) {
            horizontal = tot;
        }
    }
    return horizontal > 1 && vertical > 1;
}


//  Unite all neigbours (make same index value for the same island)  
//  Then assign indexes to each island beginning from 0  
int unite_parts(int *input, int size)
{
    int row, col, l, p, q;
    int curr_index = 0;
    int dim = (int) sqrt(size);
    
    //  Uniting neighbours here  
    for (row = 0; row < dim; row++) {
        for (col = 0; col < dim; col++) {
            p = convert_2d_to_1d(row, col, dim);
            
            l = input[p];
            
            if (l >= 0) {
                if (is_valid(row - 1, col, dim)) {
                    q = convert_2d_to_1d(row - 1, col, dim);
                    
                    if (input[q] >= 0) {
                        unite(input, p, q);
                    }
                }
                
                if (is_valid(row, col - 1, dim)) {
                    q = convert_2d_to_1d(row, col - 1, dim);
                    
                    if (input[q] >= 0) {
                        unite(input, p, q);
                    }
                }
            }
        }
    }
    
    //  Incrementing island points' indexes' value 1000000 to avoid from having same value crashes and making other indexes' value -1  
    for (row = 0; row < dim; row++) 
    {
        for (col = 0; col < dim; col++) {
            p = convert_2d_to_1d(row, col, dim);
            
            l = input[p];
            
            if (l >= 0 && l < 1000000) {
                if (is_island(input, size, l)) {
                    change_value(input, size, l, l + 1000000);
                } else {
                    change_value(input, size, l, -1);
                }
            }  
        }
    }
    
    for (row = 0; row < dim; row++) {
        for (col = 0; col < dim; col++) {
            p = convert_2d_to_1d(row, col, dim);
            
            l = input[p];
            
            if (l > 0 && l > curr_index) {
                change_value(input, size, l, curr_index);
                curr_index++;
            }
        }
    }
    
    return curr_index;
}


//  Print result to the output file  
void print_result_to_file(int *input, int size, int number_of_islands)
{
    int row, col, l, p;
    int dim = (int) sqrt(size);
    int curr_index = 0;
    
    FILE *fp;
    
    fp = fopen(OUTPUT_FILE_NAME, "w");
    
    if(fp == NULL)
        return;
    
    fprintf(fp, "%d\n", number_of_islands);
    
    for (row = 0; row < dim; row++) {
        for (col = 0; col < dim; col++) {
            p = convert_2d_to_1d(row, col, dim);
            
            l = input[p];
            
            if (l == curr_index) {
                fprintf(fp, "x:%d y:%d\n", col, row);
                curr_index++;
            }
        }
    }
    
    fclose(fp);
}


int main()
{
    //   The only array used to hold the map  
    int input[MAX_ARRAY_SIZE] = { 0 };
    //   Total numbers read from file except level value L  
    int size = 0;
    //  Level value L  
    int level = 0;
    //  Number of islands 
    int number_of_islands = 0;
    
    //  Read file and assign level, input array values and size  
    level = read_input_file(input, &size);
    
    //  Arrange input according to x > L  
    arrange_input(input, size, level);
    
    //  Unite connected terrains and index them as island  
    number_of_islands = unite_parts(input, size);
    
    //  Print the read to the output file  
    print_result_to_file(input, size, number_of_islands);
    
  
    
    return 0;
}



    */

/*********************************************SECOND CODE END***********************************************/
