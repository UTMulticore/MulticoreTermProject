#include <stdlib.h>
#include <iostream>
#include "FloatMatrix.h"
#include "Regression.h"
#include "Regression_cuda.h"

#define THREAD_SIZE 512
#define FSIZE sizeof(float)



FloatMatrix* Regression::fit(FloatMatrix *x_train, FloatMatrix *y_train){

      int n_x = x_train->row_size; 

      /* (X^T X)^-1  = A*/
      FloatMatrix *trans_mult_x =  transpose_x_mult_x(x_train);

      printf("Trans mult is:\n");
      trans_mult_x->print();
      printf("\n");
      FloatMatrix *inv = inverse_of_matrix(trans_mult_x);
      printf("inverse is:\n");
      inv->print();
      printf("\n");

     
      /*  X^T Y * = B*/ 
      FloatMatrix *trans_x_mult_y = transpose_x_mult_y(x_train,y_train);
      printf("trans_x_mult_y is :\n");
      trans_x_mult_y->print();
      printf("\n");
      
      /* A*B = ans */
      FloatMatrix *final_result = matrix_x_mult_y(inv,trans_x_mult_y);

      delete(trans_mult_x);
      delete(inv);
      delete(trans_x_mult_y);

      return final_result;
}


FloatMatrix* Regression::predict(FloatMatrix *betas, FloatMatrix *data){

	return transpose_x_mult_y(betas,data);
}








        
