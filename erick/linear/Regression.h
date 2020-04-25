#ifndef REGRESSION_H
#define REGRESSION_H


#include "FloatMatrix.h"

//#define DEBUG // for debugging purposes


class Regression{

    
    public:

    Regression();

    /*static func*/
    static FloatMatrix *fit(FloatMatrix *x_train, FloatMatrix *y_train);
    static FloatMatrix *predict(FloatMatrix *data,FloatMatrix *weights);


};






#endif
