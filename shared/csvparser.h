#ifndef __CSVPARSER__H
#define __CSVPARSER__H

class FloatMatrix{
    
    public:
    float **mat; 
    int row_size; 
    int col_size; 

    //Constructor for user generated matrix
    //input: row size and column size
    //output: none
    FloatMatrix(int row_size, int col_size);
    
    //Constructor for matrix generated from file
    //input: csv/matrix file name
    //output: noen
    FloatMatrix(const char* filename);

    //prints matrix
    //input:none
    //output: matrix to standard out 
    void print(void);

   
    //private/helper functions
    private:
    int get_row_size(const char* str_array);
    int get_col_size(const char* str_array);

}; 

#endif
