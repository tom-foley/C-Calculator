#include <stdio.h>
#include "../src/evaluator.h"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RESET   "\x1b[0m"

int sucess_count = 0;
int test_count = 0;

void check(char* expression, long expected) {
    test_count++;
    
    printf("Expression:\t%s\n", expression);
    long result = eval(expression);
    
    printf("Expected:\t%ld\n", expected);
    printf("Result:\t\t%ld\n", result);

    if (expected == result) {
        sucess_count++;
        printf(ANSI_COLOR_GREEN "Success\n\n" ANSI_COLOR_RESET);
    } else {
        printf(ANSI_COLOR_RED "Failure\n\n" ANSI_COLOR_RESET);
    }
}

int main () {
    //  8
    char* exp = "1 + 3 * 2 + 1";
    check(exp, 8);

    //  9
    char* exp1 = "1 + 3 * 2 + 1 * 2";
    check(exp1, 9);
    
    //  14
    char* exp2 = "1 + 3 * 2 + 1 * 2 + 1 + 2 * 2";     
    check(exp2, 14);

    //  10
    char* exp3 = "(1 + 3 + 1 + 2) + 1 * 2 + 1";
    check(exp3, 10);
    
    //  11
    char* exp4 = "(1 + 3 + 1 + 2) + 1 * 2 + 1 * 2";
    check(exp4, 11);

    //  11
    char* exp5 = "(1 + (3 + 1) + 2) + 1 * 2 + 1 * 2";
    check(exp5, 11);    

    //  16
    char* exp6 = "(1 + 3 + 1 + 2) + 1 * 2 + 1 * 2 + (1 + 2 * 2)";
    check(exp6, 16);

    //  16
    char* exp7 = "(1 + (3 + 1) + 2) + 1 * 2 + 1 * 2 + (1 + 2 * 2)";
    check(exp7, 16);

    //  29
    char* exp8 = "(1 + 3 + 1 + 2) * 2 * 2 + 1";
    check(exp8, 29);

    //  37
    char* exp9 = "(1 + 3 * 2 + 2) * 2 * 2 + 1";
    check(exp9, 37);

    //  85
    char* exp10 = "(1 + 3 * (2 * (2 + 1)) + 2) * 2 * 2 + 1";
    check(exp10, 85);

    //  84
    char* exp11 = "21 * 2 ^ 2";
    check(exp11, 84);

    //  28
    char* exp12 = "1 + 3 ^ (2 + 1)";
    check(exp12, 28);

    //  27
    char* exp13 = "(2 + 1) ^ 3";
    check(exp13, 27);

    //  26
    char* exp14 = "3 * (2 * (1 + 1) ^ 2) + 2";
    check(exp14, 26);

    //  109
    char* exp16 = "(1 + 3 * (2 * (1 + 1) ^ 2) + 2) * 2 * 2 + 1";
    check(exp16, 109);

    //  54
    char* exp15 = "2 * (2 + 1) ^ 3";
    check(exp15, 54);

    printf("Tests Passed: %d / %d\n", sucess_count, test_count);
}