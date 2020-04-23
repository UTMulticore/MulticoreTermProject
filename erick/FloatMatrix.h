#ifndef __CSVPARSER__H
#define __CSVPARSER__H

class FloatMatrix{
    
    public:
    float *mat; 
    int row_size; 
    int col_size; 

    //Constructor for user generated matrix
    //input: row size and column size
    //output: none
    FloatMatrix(int row_size, int col_size);

    //Constructor for matrix with a pointer to data
    //input: row size and column size along with already generated data (must be of size row_size*col_size)
    //output: none
    FloatMatrix(int row_size,int col_size,float *mat);
    
    //Constructor for matrix generated from file
    //input: csv/matrix file name
    //output: none
    FloatMatrix(const char* filename);

    //Destructor for matrix
    //input: none
    //output: none
    ~FloatMatrix();

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
