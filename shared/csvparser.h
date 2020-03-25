#ifndef __CSVPARSER__H
#define __CSVPARSER__H

struct IntMatrix{

    int **mat; 
    int num_rows; 
    int num_cols; 

}; 


typedef struct IntMatrix IntMatrix; 

//Turns a csv file into a matrix
//input: name of csv file
//output: matrix
IntMatrix *csv_to_matrix(const char *filename);


//Frees allocated space for IntMatrix
//input: IntMatrix pointer
//output: none
void free_IntMatrix(IntMatrix *mat);

//prints IntMatrix
//input: IntMatrix reference
//ouotput: contents of matrix and size (to stdout)
void print_IntMatrix(IntMatrix *int_mat);
#endif
