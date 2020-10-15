#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifdef TRUE
	#undefine TRUE
	#undefine FALSE
	#undefine Bool
#endif

typedef enum Bool {
    FALSE,	//	0
    TRUE	//	1
} Bool;

Bool validateUserInput(char* userInput[], int length);
Bool validateTriplet(char* F1, char* F2, char*n);
int* getFibonacciSequence(int sequenceLength, int F1, int F2);
void printFibonacciSequences(int* triplets, int length);
int* printFibonacciSequence(int sequenceLength, int F1, int F2);
int filterFunction (const void * a, const void * b);
int sumOfN(int* triplets, int length);

/*
    The following program prints to the console multiple fibonacci sequences and disctinct values from all sequences.
    It also prints the distinct values sorted in increasing order
    program agruments:
    ./fibo [F1 F2 n]+

    F1 = first number in series
    F2 = second number in series
    N = series length

    All numbers must be positive and F2 > F1.
*/
int main(int argc, char* argv[]) {
    // get result of input data validation
    Bool isUserInputValid = validateUserInput(argv, argc);

    if(!isUserInputValid) {
        // invalid input
        printf("usage: %s [F1 F2 n]+, with F2>F1>0 and n>0\n", argv[0]);
        return 0;
    }

    // If we have reached this point, then we know our input is valid and safe to parse
    // Parsing all arguments into integers and storing them in an array
    int parsedUserInput[argc -1];
    int i;

    for(i = 1; i < argc; i++) {
        parsedUserInput[i-1] = atoi(argv[i]);
    }  

    printFibonacciSequences(parsedUserInput, argc - 1);
    return 1;
}


/*
    Example: Program Usage
    
    ./fibo 2 5 6 1 4 8 4 5 4 7 12 3

    Rules:
    - Program should be passed 1 To N triplets
    - Where a triplet is defined as [F1 F2 n]
    - Where F2 > F1, and F1, F2, and n are all positive integers 

    - Example input from above can conceptual be looked at as three valid triplets
    
    (2 5 6) (1 4 8) (4 5 4) (7 12 3)
*/
Bool validateUserInput(char *userInput[], int length) {

    Bool output = FALSE;
    // valdiate correct number of elements
    // first element in userInput is exec name
    // so size - 1 is the count of other arguments
    // this count should should divisable by 3 for any number of triplets
    if((length - 1) % 3 != 0) {
        // invalid number of arugments
        return output;
    }

    // valdiate that each value in the triplet is a valid positive integer, and that F2 > F1
    Bool isTripletValid = TRUE;
    int i;

    // loop through user input in segements of 3
    for(i = 1; (i < length) && isTripletValid; i+=3) {
        char* F1 = userInput[i];
        char* F2 = userInput[i+1];
        char* n = userInput[i+2];
        isTripletValid = validateTriplet(F1, F2, n);
    }
    output = isTripletValid;

    return output;
}

// validates a triplet
// F1, F2, n must all be postiive integers
// F2 must be greater than F1
Bool validateTriplet(char* F1, char* F2, char*n) {
    Bool output = TRUE;

    // convert triplet to integer
    int F1AsNum = atoi(F1);
    int F2AsNum = atoi(F2);
    int nAsNum = atoi(n);
    
    // validate all strings are valid positive integers
    if(F1AsNum <= 0 || F2AsNum <= 0 || nAsNum <= 0) {
        output = FALSE;
    }
    
    // validate second string is greater than first string
    if(F1AsNum >= F2AsNum) {
        output = FALSE;
    }
  
    return output;
}

// calculate the sum of all the n (sequence lengths) in each triplet
int sumOfN(int *triplets, int length) {
    int sum = 0;
    int i;
    for(i = 2; i < length; i+=3) {
        sum += triplets[i];
    }
    return sum;
}

void printFibonacciSequences(int* triplets, int length) {
    int maxLength = sumOfN(triplets, length);

    // maxLength is the sum of all sequence lengths
    // the reason to make it that size is because in the worst case, every value in every sequence
    // will be distinct
    int oversize[maxLength];
    unsigned int counter = 0;
    int i;

    // print fibonacci sequence for each triplet
    for(i = 0; i < length; i+=3) {
        int F1 = triplets[i];
        int F2 = triplets[i+1];
        int n = triplets[i+2];
        int* fibSeq = printFibonacciSequence(n, F1, F2);

        int j;
        int k;
        
        // oversize holds the distinct values
        // so, one we computer a fibonacci sequence, we check if each value occurs already
        // in oversize, if it doesn't, that means it is distinct, and we can add it to oversize
        for(j = 0; j < n; j++) {
            Bool found = FALSE;

            for(k = 0; (k < counter) && !found; k++) {
                if(fibSeq[j] == oversize[k]) {
                    // value is not distinct
                    found = TRUE;
                }
            }

            if(!found) {
                // value is distinct
                oversize[counter++] = fibSeq[j];
            }
        }
        // free memory allocated for fibonacci sequence
        free(fibSeq);
    }

    // print all distinct values from sequences
    printf("The values encountered in the sequences are\n\t");
    printf("{");
    for(i = 0; i < counter; i++) {
        printf("%d", oversize[i]);
        if(i != counter - 1) {
            printf(", ");
        }
    }
    printf("}\n");

    // print all distinct values from sequences sorted in increasing order
    qsort(oversize, counter, sizeof(int), filterFunction);
        printf("{");
      for(i = 0; i < counter; i++) {
        printf("%d", oversize[i]);
        if(i != counter - 1) {
            printf(", ");
        }
    }
    printf("}\n");
}

/*
    prints the fibonacci sequence in the following format:
    example: n = 6, F1=2, F2=5. 

    6 terms of the Fibonacci series with F1=2 and F2=5:
    F1=2, F2=5, F3=7, F4=12, F5=19, F6=31
*/
int* printFibonacciSequence(int sequenceLength, int F1, int F2) {
    printf("%d terms of the Fibonacci sequence with F1=%d and F2=%d:\n", sequenceLength, F1, F2);
    printf("\t");
    int* fibonacciSequence = getFibonacciSequence(sequenceLength, F1, F2);
    int i;
    for(i = 0; i < sequenceLength; i++) {
        printf("F%d=%d", i + 1, fibonacciSequence[i]);
        if(i != sequenceLength - 1) {
            printf(",  ");
        }
    }
    printf("\n");
    return fibonacciSequence;
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
    // in this approach we are using memoization to remember the previous two number in the sequence, when calculating the current number in the sequence
    // we have initially have a variable first = F1, second = F2, and current = NULL. 
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

// filter function that specifies to sort integers in increasing order
int filterFunction (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}
