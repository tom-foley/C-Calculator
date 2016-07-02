## Expression Evaluator

An expression evaluator written in C with no dependencies. 

### Usage:
In order to use the eval() function, first download and build/link the source by running `make` in the root directory. 
Then, we can use eval() like so:
```
char* expression = "3 * (2 * (1 + 1) ^ 2) + 2";
long result = eval(expression);     //  result will yield 26

char* expression2 = "(1 + 3 * (2 * (1 + 1) ^ 2) + 2) * 2 * 2 + 1";
long result2 = eval(expression2);     //  result will yield 109
```

More examples can be found in the `test/test_evaluator.c` file.

### TODO:
* Check for integer overflows
* Check for invalid expressions (missing parentheses/brackets, invalid operations, etc.)
* Add support for evaluating decimals/floats/doubles