#ifndef _EVALUATOR_H
#define _EVALUATOR_H
#endif

#define NUL	                0x00
#define WHITESPACE	        0x20
#define OPEN_BRACE	        0x28
#define CLOSE_BRACE	        0x29
#define EXPONENT	        0x5E
#define MULTIPLIER          0x2A
#define DIVIDER	            0x2F
#define ADDER               0x2B
#define SUBTRACTER          0x2D

#define IS_NULL(x)	        (x == NUL)
#define IS_WHITESPACE(x)    (x == WHITESPACE)
#define IS_OPEN_BRACE(x)    (x == OPEN_BRACE)
#define IS_CLOSE_BRACE(x)   (x == CLOSE_BRACE)

#define IS_NUMBER(x)\
    (x == 0x30) ^ (x == 0x31) ^\
    (x == 0x32) ^ (x == 0x33) ^\
    (x == 0x34) ^ (x == 0x35) ^\
    (x == 0x36) ^ (x == 0x37) ^\
    (x == 0x38) ^ (x == 0x39)

#define IS_OP(x)\
    (x == ADDER) ^ (x == SUBTRACTER) ^\
    (x == MULTIPLIER) ^ (x == DIVIDER) ^\
    (x == EXPONENT)

enum TOKEN_TYPES {
    START_EXP = 0,
    END_EXP = 1,
    OP = 2,
    NUMBER = 3,
    INVALID = 4
};

enum OP_TYPES {
    NO_OP = 0,
    ADD = 1,
    SUBTRACT = 1,
    MULTIPLY = 2,
    DIVIDE = 2,
    POWER = 3,
    NEW_EXPRESSION = 4,
};

long eval(char* expression);