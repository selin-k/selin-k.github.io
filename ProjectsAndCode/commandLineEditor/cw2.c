/** INCLUDES **/
#include <stdio.h>

/** DATA **/
unsigned long finalsum=0; /** Store the output, f(n) **/

/** SUB FUNCTIONS **/
void findDigits(char result[], int index, int n, int sum);
int prompt();


int main()
{
    int n = 1;           /** Number of digits **/
    char result[n+1];    /** Combination strings **/
    
    printf("\nExercise 1 Calculator for f(n)\n");

    /** Prompt user: to get a given sum value
     *  in between 1 and 19 **/
    int sum = prompt();

    /** For n digits that range from 1 to the
     * value of the given sum, call findDigits **/
    while (n<=sum){
        findDigits(result, 0, n, sum);
        n++;
    }


    printf("f(n)=%ld\n", finalsum);
}


/**
 * Given a sum and the required number of digits, finds
 * combinations of digits such that their sum adds up to
 * the given sum. These combinations are produced by looping
 * through a decision space of 1 to 9 for every n.
 * 
 * Useless iterations, where the sum of the combination of digits
 * exceed the given sum, are skipped and the algorithm
 * backtracks to another digit from the decision space that meets 
 * the goal.
 * 
 * @param result is where combination strings are stored
 * @param index is the index of the combonation string
 * @param n is the seeked number of digits
 * @param sum is the total given sum
 */ 
void findDigits(char result[], int index, int n, int sum)
{
    /** Case 1 **/
    if (index < n && sum >= 0)
    {
        /** If the combination's length doesn't exceed
         * n, and the given sum isn't yet reached **/
        char digit = '1';

        /** Loop through the decision space 1 to 9 **/
        while (digit <= '9')
        {
            /** Append a digit **/
            result[index] = digit;

            /** Get the int value of the digit **/
            int intdigit = (digit - '0'); 

            /** Skip useless iterations, backtrack to last 
             * stack frame **/
            if ( (sum-intdigit) < 0) break;

            findDigits(result, index + 1, n, sum - intdigit);
            digit++;
        }
    }
 
    /** Case 2 **/
    else if (index == n && sum == 0) {
        /** If the given sum is reached and it is
         * of expected number of digits, a solution
         * is found **/

        /** Get the integer value of the result
         * string **/
        long ans;
        sscanf(result, "%ld", &ans);

        /** Add it to the finalsum, f(n) **/
        finalsum+=ans;
    }
}


/**
 * Prompts the user to get a given sum value.
 * This is the target sum for all combination 
 * strings.
 * 
 * @return sum is the given sum
 */ 
int prompt()
{
    int sum;
    do {
        printf("Enter n (1 to 19): \n");

        /** Get user input, if not of expected type
         * consume standard output with getchar **/
        int errorCode = scanf("%d", &sum);
        if (errorCode!=1) {
            printf("Invalid input. Please enter an integer from 1 to 19.\n");
            while(getchar() !='\n') {
                continue;
            }
        }

    } while (sum<1 || sum>19);

    return sum;
}
