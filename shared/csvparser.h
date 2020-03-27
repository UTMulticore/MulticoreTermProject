#ifndef __CSVPARSER__H
#define __CSVPARSER__H

struct FloatMatrix{

    float **mat; 
    int num_rows; 
    int num_cols; 

}; 


typedef struct FloatMatrix FloatMatrix; 

//Turns a csv file into a matrix
//input: name of csv file
//output: matrix
FloatMatrix *csv_to_matrix(const char *filename);


//Frees allocated space for FloatMatrix
//input: FloatMatrix pointer
//output: none
void free_FloatMatrix(FloatMatrix *float_mat);

//prints FloatMatrix
//input: FloatMatrix reference
//ouotput: contents of matrix and size (to stdout)
void print_FloatMatrix(FloatMatrix *float_mat);
#endif
