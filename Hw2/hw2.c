/*
 * 
 *  Efficiency of the algorithm is O(n^2) where n is the count of numbers
 *  
 *  Algorithm: 
 *      1) Read the file and put the numbers to the array
 *      2) Sort the array in ascending order (Bubble sort - O(n^2)
 *      3) Store the numbers which repeats more than one on the left of the array with the number of repetitions, store the number that has seen only once on the right
 *      4) Sort the number on the left with their number of repetitions
 *      5) Write to the file, firstly single numbers on the right of the array, from right to left (ascending order)
 *      6) Write to the file, secondly the numbers which repeats more than once with their number of repetitions
 * 
 */

#include <stdio.h>

// Find the first number in the file after the current position (curr_pos)
// Return the number (number) with begin position (begin_pos) and end position (end_pos) of the number which is found 
int get_next_number(FILE *fp, int curr_pos, int* begin_pos, int* end_pos, int* is_error)
{
    int ch;
    int excep = 0;
    int number = 0;         // Holds the number which is found
    int is_number = 0;      // It is set when it comes to the beginning of the first number, when it is set it means there is a number which is found
    int is_negative = 0;    // Holds if the number which is found, is negative (1) or position (0)
    
    // Initialize begin and end positions of the number as -1,
    // if they are still -1 at the end of the function, it means that there is no number found
    *begin_pos = -1;
    *end_pos = -1;
    
    // Seek to the current position in the file
    fseek(fp, curr_pos, SEEK_SET);
    
    // Until it is found a number or it comes to the end of the file do
    while ((ch = fgetc(fp)) != EOF) {
        // Check if it is a negative number, if yes set is_negative variable
        if (ch == '-') {
            is_negative = 1;
            is_number = 1;
            *begin_pos = curr_pos;
        }
        else if (ch >= '0' && ch <= '9') {
            if (is_number == 0) {
                is_number = 1;
                *begin_pos = curr_pos;
            }
            
            // When a new digit comes, multiply current number with 10 and add new digit to it
            number = number * 10 + (ch - '0');
            
            // Control if given number is out of range (integer size)
            if (number < 0) {
                if (is_negative) {
                    if (number != -2147483648) {
                        *is_error = 1;
                    } else {
                        excep = 1;
                    }   
                } 
                else {
                    *is_error = 1;
                }
            }
            else {
                if (is_negative && excep)
                    *is_error = 1;
            }
        }
        else {
            if (is_number == 1) {
                *end_pos = curr_pos;
                break;
            }
        }
        
        curr_pos++;
    }
    
    if (is_number == 1) {
        if (*end_pos == -1)
            *end_pos = curr_pos;
        
        // If the number is negative change the sign of the number as negative
        if (is_negative == 1)
            number = -number;
    }
    
    return number;
}

// Applies the algorithm explained above
void fill_array_with_number(FILE* fp, int curr_pos, int* array, int* pcount)
{
    while (1) {
        int begin_pos, end_pos;
        int target_number;
        int is_error = 0;
            
        // 1) Find the first number (target_number) in the file from the current position (curr_pos)
        target_number = get_next_number(fp, curr_pos, &begin_pos, &end_pos, &is_error);
        
        // 1) if there is no number found, finish the algorithm
        if (begin_pos == -1)
            return;
        
        curr_pos = end_pos;
        
        if (is_error == 0) {
            array[*pcount] = target_number;
            (*pcount)++;
        }
    }
}

// Sort array in ascending order (Bubble Sort)
void sort_array(int* array, int count)
{
    int tmp;
    int i, j;
    
    for (i = 1; i < count; i++) {
        for (j = 0; j < count - i; j++) {
            if (array[j] > array[j+1]) {
                tmp = array[j];
                array[j] = array[j+1];
                array[j+1] = tmp;
            }
        }
    }
}

// Sort the numbers according to their repetitions in ascending order (Bubble Sort)
void sort_multi(int* array, int count)
{
    int tmp;
    int i, j;
    
    for (i = 2; i < count; i += 2) {
        for (j = 1; j < count - i; j += 2) {
            if (array[j] > array[j+2]) {
                // change number of repetitions
                tmp = array[j];
                array[j] = array[j+2];
                array[j+2] = tmp;
                	
                // change value
                tmp = array[j-1];
                array[j-1] = array[j+1];
                array[j+1] = tmp;
            }
        }
    }
}

// This function puts the numbers which repeats more than once on left of the array with their repetitions
// Besides it puts the numbers which occurs only once on the right of the array
void format_array(int *array, int curr_pos, int last_pos, int *pmulti_end, int *psingle_end)
{
    while (curr_pos < last_pos) {
        int number;
        int i, total;
        
        number = array[curr_pos];
        
        total = 1;
        
        for (i = curr_pos + 1; i < last_pos; i++) {
            if (number == array[i])
                total++;
            else
                break;
        }
        
        if (total == 1) {
            for (i = curr_pos; i < last_pos; i++)
                array[i] = array[i+1];
            
            array[*psingle_end] = number;
            (*psingle_end)--;
            last_pos--;
        } 
        else {
            for (i = 0; i < last_pos - curr_pos - total; i++)
                array[i + curr_pos + 2] = array[i + curr_pos + total];
            
            array[curr_pos+1] = total;
            last_pos -= (total - 2);
            curr_pos += 2;
            *pmulti_end = curr_pos;
        }
    }
}

// Main function
int main()
{
    FILE *fp;
    char *input_filename = "input.txt";
    char *output_filename = "output.txt";
    
    int i;
    int array[100];
    int count = 0;
    int multi_end, single_end;
    
    if ((fp = fopen(input_filename, "r+")) == NULL) {
        fprintf(fp, "cannot open input file\n");
        return -1;
    }
    
    // Parse numbers from file to array
    fill_array_with_number(fp, 0, array, &count);
    
    fclose(fp);
    
    // Sort the array in ascending order
    sort_array(array, count);
    
    multi_end = 0;
    single_end = count - 1;
    
    // Format the array
    format_array(array, 0, count, &multi_end, &single_end);
    
    // Sort the numbers which repeats more than once
    sort_multi(array, multi_end);
    
    
    // Write solution to the output file
    if ((fp = fopen(output_filename, "w")) == NULL) {
        fprintf(fp, "cannot open output file\n");
        return -1;
    }
    
    for (i = count - 1; i > single_end; i--)
        fprintf(fp, "%d:1\n", array[i]);
    
    for (i = 0; i < multi_end; i += 2)
        fprintf(fp, "%d:%d\n", array[i], array[i+1]);
    
    fclose(fp);
    
    return 0;
}
