#include <stdio.h>
#include "evaluator.h"

int is_number(char c) {
    char x = 0x30;
    while (x <= 0x39) {
        if (c == x) {
            return 1;
        }
        x += 0x01;
    }

    return 0;
}


int is_op(char c) {
    return c == ADDER || c == SUBTRACTER
        || c == MULTIPLIER || c == DIVIDER
        || c == EXPONENT;
}


enum TOKEN_TYPES get_token_type(char token) {
    if (token == OPEN_BRACE) {
        return START_EXP;
    } else if (token == CLOSE_BRACE) {
        return END_EXP;
    } else if (is_op(token)) {
        return OP;
    } else if (is_number(token)) {
        return NUMBER;
    } else {
        return INVALID;
    }
}


enum OP_TYPES get_op_type(char op) {
    switch (op) {
        case ADDER:
            return ADD;
        case SUBTRACTER:
            return SUBTRACT;
        case MULTIPLIER:
            return MULTIPLY;
        case DIVIDER:
            return DIVIDE; 
        case EXPONENT:
            return POWER;
        case OPEN_BRACE:
            return NEW_EXPRESSION;
        default:
            return NO_OP;
    }    
}


int order_operations_greater(enum OP_TYPES cur_op, enum OP_TYPES next_op) {
    return next_op > cur_op;
}


long power(long num, long exp) {
    if (exp > 1) {
        return num * power(num, exp - 1);
    }    
    return num;
}


void set_token_index(char* expression, long* counter) {
    while (expression[*counter] == WHITESPACE) {
        *counter += 1;
    }
}


long get_next_number(char* expression, long* counter) {
    long number = 0;
    char num_token = expression[*counter];
    do {
        number *= 10;
        number += (num_token - '0');
        *counter += 1;
        set_token_index(expression, counter);
        num_token = expression[*counter];
    } while (is_number(num_token));

    return number;
}


long perform_op(long left_exp, long right_exp, char op) {
    long op_result;

    if (op != NUL) {
        printf("Performing Op:\t\%ld %c %ld = ", left_exp, op, right_exp);
    }

    switch (op) {
        case ADDER:
            op_result = left_exp + right_exp;
            break;
        case SUBTRACTER:
            op_result = left_exp - right_exp;
            break; 
        case MULTIPLIER:
            op_result = left_exp * right_exp; 
            break; 
        case DIVIDER:
            op_result = left_exp / right_exp;
            break; 
        case EXPONENT:
            op_result = power(left_exp, right_exp);
            break; 
        default:
            op_result = right_exp;
            break;
    }

    if (op != NUL) {
        printf("%ld\n", op_result);
    }

    return op_result;
}


long parse(char* expression, long sum, long* counter) {
    char op = NUL, 
         token = NUL, 
         next_token = NUL; 
    
    int negative_flag = 0,
        consecutive_ops = 0;

    int paren_flag = (*counter > 1) 
        ? expression[*counter - 1] == OPEN_BRACE 
        : 0;
    
    long number, 
         counter_next;

    enum OP_TYPES op_type = NUL;
    enum TOKEN_TYPES token_type = NUL, 
                     last_token_type = NUL;

    while (expression[*counter] != NUL) {
        set_token_index(expression, counter);
        token = expression[*counter];
        last_token_type = token_type;
        token_type = get_token_type(token);

        switch (token_type) {
            case (START_EXP):
                //  Move past OPEN_BRACE token
                *counter += 1;

                //  Get result of () expression
                number = parse(expression, 0, counter);

                paren_flag = 0;
                //  Process next token
                *counter += 1;
                counter_next = *counter;
                set_token_index(expression, &counter_next);
                next_token = expression[counter_next];

                //  Check if
                //      (next_token is an OP) AND
                //      (next_token has higher order operations than current op) 
                if (order_operations_greater(op_type, get_op_type(next_token))) {
                    number = parse(expression, number, counter);
                }

                sum = perform_op(sum, number, op);
                break;
            case (END_EXP):
                return sum;
            case (OP):
                op_type = get_op_type(token);

                if (last_token_type == OP) {
                    consecutive_ops += 1;
                    if (consecutive_ops > 2) {
                        //  TODO: return error up recursion stack
                        printf("Too many consecutive OPS...Exiting.\n");
                    }
                    if (token == ADDER) {
                        negative_flag = NUL;
                    } else if (token == SUBTRACTER) {
                        negative_flag = 1;
                    } else {
                        //  TODO: return error up recursion stack
                        printf("Illegal order of operations...Exiting.\n");
                    }
                } else {
                    consecutive_ops = 1;
                    op = token;
                }
                
                *counter += 1;
                break;
            case (NUMBER):
                //  Process current number
                number = get_next_number(expression, counter);
                if (negative_flag) {
                    number *= -1;
                }
                
                //  Process next token
                counter_next = *counter;
                set_token_index(expression, &counter_next);
                next_token = expression[counter_next];
                
                //  Check if
                //      (next_token is an OP) AND
                //      (next_token has higher order operations than current op) 
                if (order_operations_greater(op_type, get_op_type(next_token))) {
                    number = parse(expression, number, counter);
                }

                sum = perform_op(sum, number, op);
                break;
            default:
                *counter += 1;
                break;
        }
    }

    if (paren_flag) {
        //  TODO: return error up recursion stack
        printf("Missing closing parentheses...Exiting.\n");
    }

    return sum;
}


long eval(char* expression) {
    long counter = 0;
    return parse(expression, 0, &counter);
}