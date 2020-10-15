#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int validateUserInput(char* arguments[], int numOfArguments);
int allStringsAreNumbers(char* str1, char* str2, char* str3);
void printFibonacciSequence(int sequenceLength, int F1, int F2);
int* getFibonacciSequence(int sequenceLength, int F1, int F2);

/*
    The following program prints to the console the fibonacci sequence.
    program agruments:
    ./fibo F1 F2 N

    F1 = first number in series
    F2 = second number in series
    N = series length

    All numbers must be positive and F2 > F1.
*/
int main(int argc, char* argv[]) {
    // get result of input data validation
    int validatedUserInput = validateUserInput(argv, argc);
    
    if(validatedUserInput == -1) {
        // Incorrect number of arguments
        printf("usage: %s F1 F2 n, with F2>F1>0 and n>0.\n", argv[0]);
        return 0;
    }

    if(validatedUserInput == -2) {
        // F2 =< F1
        printf("error: The second argument must be strictly larger than the first\n");
        return 0;
    }

    // If we have reached this point, then we know our input is valid and safe to parse
    // parse string arguments into integers
    int F1 = atoi(argv[1]);
    int F2 = atoi(argv[2]);
    int sequenceLength = atoi(argv[3]);

    // print the fibonacci sequence
    printFibonacciSequence(sequenceLength, F1, F2);
    printf("\n");
    return 1;
}

/*
    prints the fibonacci sequence in the following format:
    example: n = 6, F1=2, F2=5. 

    6 terms of the Fibonacci series with F1=2 and F2=5:
    F1=2, F2=5, F3=7, F4=12, F5=19, F6=31
*/
void printFibonacciSequence(int sequenceLength, int F1, int F2) {
    printf("%d terms of the Fibonacci series with F1=%d and F2=%d:\n", sequenceLength, F1, F2);
    
    // get array of fibnacci sequence
    int* fibonacciSequence = getFibonacciSequence(sequenceLength, F1, F2);
    int i;

    // loop through sequence array, printing each value with it's Fn label
    for(i = 0; i < sequenceLength; i++) {
        printf("F%d=%d", i + 1, fibonacciSequence[i]);
        if(i != sequenceLength - 1) {
            printf(",  ");
        }
    }
    // free dynamic memory allocated for fibonacci sequence
    free(fibonacciSequence);
}

// returns integer array of the fibonacci sequence up to sequenceLength, with the given F1 and F2
// example: n = 3, F1=2, F2=5.
// returns [2,5,7]
int* getFibonacciSequence(int sequenceLength, int F1, int F2) {
    int* results = (int*)calloc(sequenceLength, sizeof(int));

    // check for base cases

    // if sequence is of length 1, return [F1]
    if(sequenceLength <= 1) {
        results[0] = F1;
        return results;
    }

    // if sequence is of length 2, return [F1,F2]
    if(sequenceLength == 2) {
         results[0] = F1;
         results[1] = F2;
         return results;
    }
    
    // Using Dynamic Programming's linear runtime "Iterative Top-Down Approach" to the fibonacci sequence
    // in this approach we are using memoization to remember the previous two numbers in the sequence, when calculating the current number in the sequence
    // we initially have a variable first = F1, second = F2, and current = NULL. 
    // then we set current = first + second , f^n-2 + f^n-1 = F^n, as is the nature of the fibonacci sequence
    // then we overwrite first with second, and second with current, and append the value (current) that we calculated into the results array
    // keep doing this until we reach i == n
    results[0] = F1;   
    results[1] = F2;
    int current;
    int first = F1;
    int second = F2;
    int i;
    
    for(i = 3; i <= sequenceLength; i++) {
        current = first + second;
        first = second;
        second = current;
        results[i-1] = current;
    }

    return results;
}

/*
    Validates user input
    Rules:
        - Program muse have recieved 3 positive integers
        - The second argument must be larger then the first
*/
int validateUserInput(char* arguments[], int numOfArguments) {
    if(numOfArguments != 4) {
        // invalid number of arguments
        return -1;
    }

    int returnValue = allStringsAreNumbers(arguments[1], arguments[2], arguments[3]);

    return returnValue;
}

/*
    Verifies that the three strings given are all positive integers, and that str2 > str1
    Return values:
        -True: (1)
        -False: (-1), not all strings passed were valid positive integers
        -False: (-2), string2 is not greater than string1
*/
int allStringsAreNumbers(char* str1, char* str2, char*str3) {
    int str1AsNumber = atoi(str1);
    int str2AsNumber = atoi(str2);
    int str3AsNumber = atoi(str3);
    
    // validate all strings are valid positive integers
    if(str1AsNumber <= 0 || str2AsNumber <= 0 || str3AsNumber <= 0) {
        return -1;
    }
    
    // validate second string is greater than first string
    if(str1AsNumber >= str2AsNumber) {
        return -2;
    }
  
    return 1;
}