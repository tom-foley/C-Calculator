CC = gcc
CFLAGS = -Wall
DEPS = src/evaluator.h
OBJ = src/evaluator.o test/test_evaluator.o

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

bin/test_evaluator: $(OBJ)
	mkdir -p bin
	$(CC) $(CFLAGS) -o $@ $^