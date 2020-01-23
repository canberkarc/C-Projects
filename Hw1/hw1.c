
/*
 * 
 *  Efficiency of the algorithm is O(n^2) where n is the number of characters in the file
 *  
 *  Algorithm: 
 *      1) Find the first number (target number) in the file from the current position, if there is no number found, finish the algorithm
 *      2) Find next numbers after the target number, if they are equals, replace them with space, else do nothing.
 *      3) When it comes at the end of the file, assign the end of the current target number the current position and jump to step 1
 * 
 */

#include <stdio.h>
#include <stdlib.h>

// Find the first number in the file after the current position (curr_pos)
// Return the number (number) with begin position (begin_pos) and end position (end_pos) of the number which is found 
int get_next_number(FILE *fp, int curr_pos, int* begin_pos, int* end_pos)
{
    int ch;
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
        // Check if it is a negative number, if yes then set is_negative variable
        if (ch == '-') {
            is_negative = 1;
            is_number = 1;
            *begin_pos = curr_pos;
        } else if (ch >= '0' && ch <= '9') {
            if (is_number == 0) {
                is_number = 1;
                *begin_pos = curr_pos;
            }
            
            // When a new digit comes, multiply current number with 10 and add new digit to it
            number = number * 10 + (ch - '0');
        } else {
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

void remove_repeated_number(FILE *fp, int curr_pos)
{
    int i;
    int begin_pos, end_pos, next_curr_pos;
    int target_number, next_number;
    
    while(fp != NULL){

	    // 1) Find the first number (target_number) in the file from the current position (curr_pos)
	    target_number = get_next_number(fp, curr_pos, &begin_pos, &end_pos);
	    
	    // 1) if there is no number found, finish the algorithm
	    if (begin_pos == -1)
	        return;
	    
	    // Next current position for the function is the end position of the target number
	    next_curr_pos = end_pos;
	    
	    curr_pos = end_pos;
	    
	    while (begin_pos != -1) {
	        // 2) Find next number after the current position (curr_pos) in the file
	        next_number = get_next_number(fp, curr_pos, &begin_pos, &end_pos);
	        
	        // 2) if there is no number remained, break
	        if (begin_pos == -1)
	            break;
	        
	        // 2) if they are equals, replace them with space, else do nothing
	        if (target_number == next_number) {
	            fseek(fp, begin_pos, SEEK_SET);
	            for (i = 0; i < end_pos - begin_pos; i++)
	                fputc(' ', fp);
	        }
	        
	        // 2) update the current position as the end of the number found
	        curr_pos = end_pos;
	    }
	    //3)To make the function continue,set current position as next current position
	    curr_pos = next_curr_pos;
    }
}

int main()
{
    FILE *fp;
    const char *filename = "file.txt";
    
    if ((fp = fopen(filename, "r+")) == NULL) {
        fprintf(stderr, "File cannot be opened\n");
        exit(-1);
    }
    
    remove_repeated_number(fp, 0);
    
    fclose(fp);
    
    return 0;
}
