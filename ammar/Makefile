CC = g++
CFLAGS =  -g -Wall -std=c++17

all: csv_matrix.h stopwatch.h debug.h fp_growth.h fp_tree.h
	$(CC) $(CFLAGS) -o test.out testing.cc fp_growth.cc fp_tree.cc -fopenmp

# all: testing

# testing : testing.o csv_matrix.h stopwatch.h
# 	$(CC) $(CFLAGS) -o test.out

# clean: 
# 	$(RM) testing *.o *~