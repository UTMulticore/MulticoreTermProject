#include <stdlib.h>
#include <iostream>
#include "FloatMatrix.h"
#include "Regression.h"
#include "Regression_cuda.h"

#define THREAD_SIZE 512
#define FSIZE sizeof(float)



FloatMatrix* Regression::fit(FloatMatrix *x_train, FloatMatrix *y_train){

      int n_x = x_train->row_size; 
      FloatMatrix *trans_mult_x =  transpose_x_mult_x(x_train);
      FloatMatrix *inv = new FloatMatrix(n_x,n_x,inverse_of_matrix(trans_mult_x->mat,n_x));


}







        
