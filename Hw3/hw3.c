/*
 * 
 *  Efficiency of the algorithm is O(n^2)
 *  
 *  Algorithm: 
 *      1) Open evaluations.txt on write mode
 *      2) Read polynomial from polynomial.txt file to the buffer
 *      3) Read values.txt and return the number of values
 *      4) Then calculation is done
 *      5) After each calculation,evaluation is written to evaluations.txt
 * 
 */


#include <stdio.h>
#include <math.h>

#define MAX_NUMBER_OF_VALUES    100
#define MAX_POLY_BUF_SIZE       1001

#define FILE_NAME_VALUES        "values.txt"
#define FILE_NAME_POLYNOMIAL    "polynomial.txt"
#define FILE_NAME_EVALUATIONS   "evaluations.txt"

// Read values.txt and return the number of values 
int read_values_from_file(double *values)
{
    FILE *fp;
    
    int count = 0;
    
    if ((fp = fopen(FILE_NAME_VALUES, "r")) == NULL)
        return -1;
    
    while (fscanf(fp, "%lf", &values[count]) > 0)
        count++;
    
    fclose(fp);
    
    return count;
}

// Read polynomial from polynomial.txt file to the buffer
void read_polynomial_from_file(char *polybuf)
{
    FILE *fp;
    
    if ((fp = fopen(FILE_NAME_POLYNOMIAL, "r")) == NULL)
        return;
    
    fgets(polybuf, MAX_POLY_BUF_SIZE, fp);
    
    fclose(fp);
    
    return;
}

// Write a single result to the file
void write_result_to_file(FILE *fp, double result)
{
    fprintf(fp, "%.2lf\n", result);
}

// Calculate polynomial result by giving x as value
double calculate(char *polybuf, double value)
{
    char *p;
    char last_op = '+';         // last operator seen
    int state = 0;              // state=0 means idle case, take coefficient. state=1 means x has been seen. state=2 means ^ has been seen
    double result = 0;          // the result of the polynomial
    
    int numlen = 0;             // holds length of coefficient of x as string 
    int powlen = 0;             // holds length of power of x as string      
    
    char number[100] = { 0 };   // holds coefficient of x as string
    char power[100] = { 0 };    // holds power of x as string
    
    double coeff = 0;           // holds coefficient of x as converted to double
    double pownum = 0;          // holds power of x as converted to double
    
    p = polybuf;
    
    while (*p != '\0') {
        switch (*p) {
            case 'x':
                state = 1;
                number[numlen] = '\0';
                break;
            case '^':
                state = 2;
                break;
            case '+':
            case '-':
                // Calculate current result when we reach a '+' or '-' operator
                // And add or subtract to the final result according to the last_op seen
                power[powlen] = '\0';
                if (powlen > 0) {
                    sscanf(power, "%lf", &pownum);
                } else {
                    pownum = 1.0;
                }
                
                number[numlen] = '\0';
                if (numlen > 0) {
                    sscanf(number, "%lf", &coeff);
                } else {
                    coeff = 1.0;
                }
                
                if (last_op == '+') {
                    result += coeff * pow(value, pownum);
                } else if (last_op == '-') {
                    result -= coeff * pow(value, pownum);
                }
                
                // Clear powers, coefficients and state
                coeff = 0;
                pownum = 0;
                
                numlen = 0;
                powlen = 0;
                
                state = 0;
                
                last_op = *p;
                break;
            case '.':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                if (state == 0) {
                    number[numlen++] = *p;
                } else if (state == 2) {
                    power[powlen++] = *p;
                }
                
                break;
            default:
                break;
        }
        
        p++;
    }
    
    if (state != 0) {
        // If the state is not 0 there is a calculation which is not calculated (last case)
        // Calculate current result when we reach a '+' or '-' operator
        // And add or subtract to the final result according to the last_op seen
        power[powlen] = '\0';
        if (powlen > 0) {
            sscanf(power, "%lf", &pownum);
        } else {
            pownum = 1.0;
        }
        
        number[numlen] = '\0';
        if (numlen > 0) {
            sscanf(number, "%lf", &coeff);
        } else {
            coeff = 1.0;
        }
        
        if (last_op == '+') {
            result += coeff * pow(value, pownum);
        } else if (last_op == '-') {
            result -= coeff * pow(value, pownum);
        }
    }
    
    return result;
}

int main()
{
    int i;
    int count = 0;
    double values[MAX_NUMBER_OF_VALUES];
    char polybuf[MAX_POLY_BUF_SIZE] = { 0 };
    double result;
    
    FILE *fp;
    
    if ((fp = fopen(FILE_NAME_EVALUATIONS, "w")) == NULL) {
        printf("File cannot be opened\n");
        return -1;
    }
    
    // Read polynomial
    read_polynomial_from_file(polybuf);
    
    // Read Values
    count = read_values_from_file(values);
    
    for (i = 0; i < count; i++) {
        // For each value calculate the result
        result = calculate(polybuf, values[i]);
        // printf("%.2lf : %.2lf\n", values[i], result);
        
        // Write the result to the evaluations.txt file
        write_result_to_file(fp, result);
    }
    
    fclose(fp);
    
    return 0;
}
