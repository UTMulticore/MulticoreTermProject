#ifndef REGRESSION_H
#define REGRESSION_H


#include "FloatMatrix.h"


class Regression{

    
    public:

    Regression();

    /*static func*/
    static FloatMatrix *fit(FloatMatrix *x_train, FloatMatrix *y_train);
    static FloatMatrix *predict(FloatMatrix *betas,FloatMatrix *data);

    /*obj func*/
    void print_fitted(char* file_name);    
    void print_prediction(char* file_name);

    private:
    /* fields */
    FloatMatrix *fitted_mat; 
    FloatMatrix * predicted_mat;


    /*functions*/

    /*Transposes a matrix*/
    //input: float matrix to be transposed
    //output: pointer to allocated float matrix
    FloatMatrix *transpose(FloatMatrix fm);


};






#endif
