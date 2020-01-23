/*
 *	
 *	Time Complexity is O(n)+O(n) = O(n)
 *	n is count of numbers
 * 	write_to_file function turns at most n times so its complexity is O(n)
 * 	operation function turns at most n times so its complexity is O(n) as well 				
 *
 *
 *  Algorithm: 
 *      1) Open input.txt on read mode and output.txt on write mode
 *      2) Call operation function with input file pointer, output file pointer and level which is 0
 *      3) Read input character by character until the end of file with while
 *      4) Realize needed operations according to the character 
 *      
 */


#include <stdio.h>

#define INPUT_FILE_NAME     "input.txt"
#define OUTPUT_FILE_NAME    "output.txt"

// Recursive function, write visualization to the output file
// Function puts character if its level is 0 otherwise puts - and reduces level 1.It continues like this until level is 0.
void write_to_file(FILE *ofp, char c, int level)
{
    if (level == 0) {
        fprintf(ofp, "%c\n", c);
    } else {
        fprintf(ofp, "-");
        write_to_file(ofp, c, level - 1);
    }
}

// Recursive function, read characters from input file and write visualization to the output file
// ifp: input file pointer, ofp: output file pointer, level: open bracket makes a level plus 1
void operation(FILE *ifp, FILE *ofp, int level)
{
    char c;
    
    while ((c = fgetc(ifp)) != EOF) {
        switch (c) {
            case '(':
                operation(ifp, ofp, level + 1);
                break;
            case ')':
                return;
            case ',':
            case ' ':
            case '\t':
            case '\r':
            case '\n':
                break;
            default:  //If character is different from above characters,do visualization operation for this character.
                write_to_file(ofp, c, level);
                break;
        }
    }
    
    return;
}


int main()
{
    FILE *ifp, *ofp;
    
    // Open input file
    if ((ifp = fopen(INPUT_FILE_NAME, "r")) == NULL) {
        printf("Input file cannot be opened\n");
        return -1;
    }
    
    // Open output file
    if ((ofp = fopen(OUTPUT_FILE_NAME, "w")) == NULL) {
        printf("Output file cannot be opened\n");
        return -1;
    }
    
    // Run recursive operation function
    operation(ifp, ofp, 0);
    
    fclose(ifp);
    fclose(ofp);
    
    return 0;
}
