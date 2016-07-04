#ifndef _EVALUATOR_H
#define _EVALUATOR_H
#endif

#define NUL                 0x00
#define WHITESPACE          0x20
#define OPEN_BRACE          0x28
#define CLOSE_BRACE         0x29
#define EXPONENT            0x5E
#define MULTIPLIER          0x2A
#define DIVIDER             0x2F
#define ADDER               0x2B
#define SUBTRACTER          0x2D

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