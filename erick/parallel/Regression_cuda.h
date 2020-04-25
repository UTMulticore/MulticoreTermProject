#ifndef REGRESSION_CUDA_H
#define REGRESSION_CUDA_H



/*Transposes x and multiplies it with itself*/
FloatMatrix* transpose_x_mult_x(FloatMatrix *fm);

/*Tranposes x and multiplies it with Y */
FloatMatrix* transpose_x_mult_y(FloatMatrix *x, FloatMatrix *y);

/*Multiplies matrix x with matrix y*/
FloatMatrix* matrix_x_mult_y(FloatMatrix *x, FloatMatrix *y);

/* Findx iverse of matrix (must be invertible or else returns null) */
FloatMatrix *inverse_of_matrix(FloatMatrix *fm);
#endif
