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


void get_next_number(char* expression, long* counter, long* number) {
    *number = 0;
    char num_token = expression[*counter];
    do {
        *number *= 10;
        *number += (num_token - '0');
        *counter += 1;
        set_token_index(expression, counter);
        num_token = expression[*counter];
    } while (is_number(num_token));
}


void perform_op(long* left_exp, long right_exp, char op) {
    if (op != NUL) {
        printf("Performing Op:\t\%ld %c %ld = ", *left_exp, op, right_exp);
    }

    switch (op) {
        case ADDER:
            *left_exp += right_exp;
            break;
        case SUBTRACTER:
            *left_exp -= right_exp;
            break; 
        case MULTIPLIER:
            *left_exp *= right_exp; 
            break; 
        case DIVIDER:
            *left_exp /= right_exp;
            break; 
        case EXPONENT:
            *left_exp = power(*left_exp, right_exp);
            break; 
        default:
            *left_exp = right_exp;
            break;
    }

    if (op != NUL) {
        printf("%ld\n", *left_exp);
    }
}


void set_error(eval_result* result, char* msg) {
    int i = 0;
    while (msg[i] != NUL) {
        result->error_msg[i] = msg[i];
        i++;
    }

    result->error_msg[i] = NUL;
    result->error = 1;
}


eval_result parse(char* expression, eval_result result, long* counter) {
    char op = NUL, 
         token = NUL, 
         next_token = NUL; 
    
    int negative_flag = 0,
        consecutive_ops = 0;

    //  set paren_flag to 1 
    //  if (expression[0] is OPEN_BRACE) OR
    //  we are in recursive call and 
    //  (expression[counter - 1] is OPEN_BRACE)  
    int paren_flag = 0;
    if (!*counter) {
        paren_flag = expression[*counter] == OPEN_BRACE;
    } else if (*counter > 1) {
        paren_flag = expression[*counter - 1] == OPEN_BRACE; 
    }

    long counter_next;

    eval_result next_result;
    next_result.error = 0;
    next_result.result = 0;

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
                next_result.result = 0;
                next_result = parse(expression, next_result, counter);
                if (next_result.error) {
                    return next_result;
                }

                //  Process next token
                *counter += 1;
                counter_next = *counter;
                set_token_index(expression, &counter_next);
                next_token = expression[counter_next];

                //  Check if
                //      (next_token is an OP) AND
                //      (next_token has higher order operations than current op) 
                if (order_operations_greater(op_type, get_op_type(next_token))) {
                    next_result = parse(expression, next_result, counter);
                    if (next_result.error) {
                        return next_result;
                    }
                }

                perform_op(&result.result, next_result.result, op);
                break;
            case (END_EXP):
                return result;
            case (OP):
                op_type = get_op_type(token);

                if (last_token_type == OP) {
                    consecutive_ops += 1;
                    if (consecutive_ops > 2) {
                        set_error(&result, "Too many consecutive OPS");
                        return result;
                    }

                    if (token == ADDER) {
                        negative_flag = NUL;
                    } else if (token == SUBTRACTER) {
                        negative_flag = 1;
                    } else if (token != OPEN_BRACE) {
                        set_error(&result, "Illegal order of operations");
                        return result;                 
                    }
                } else {
                    consecutive_ops = 1;
                    op = token;
                }
                
                *counter += 1;
                break;
            case (NUMBER):
                //  Process current number
                get_next_number(expression, counter, &next_result.result);
                if (negative_flag) {
                    next_result.result *= -1;
                }
                
                //  Process next token
                counter_next = *counter;
                set_token_index(expression, &counter_next);
                next_token = expression[counter_next];
                
                //  Check if
                //      (next_token is an OP) AND
                //      (next_token has higher order operations than current op) 
                if (order_operations_greater(op_type, get_op_type(next_token))) {
                    next_result = parse(expression, next_result, counter);
                    if (next_result.error) {
                        return next_result;
                    }
                }

                perform_op(&result.result, next_result.result, op);
                break;
            default:
                if (paren_flag) {
                    set_error(&result, "Missing closing parentheses");
                }
                return result;
        }
    }

    if (paren_flag) {
        set_error(&result, "Missing closing parentheses");
    }    

    return result;
}


long eval(char* expression) {
    long counter = 0;

    eval_result result;
    result.error = 0;
    result.result = 0;
    
    result = parse(expression, result, &counter);

    if (result.error) {
        printf("%s...Exiting\n", result.error_msg);
        return 0;
    } else {
        return result.result;
    }
}