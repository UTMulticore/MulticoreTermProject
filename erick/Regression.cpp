#include <stdlib.h>
#include <iostream>
#include "FloatMatrix.h"
#include "Regression.h"
#include "Regression_cuda.h"

#define THREAD_SIZE 512
#define FSIZE sizeof(float)



FloatMatrix* Regression::fit(FloatMatrix *x_train, FloatMatrix *y_train){

    
       return transpose_mult(x_train);


}







        
