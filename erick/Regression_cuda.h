#ifndef REGRESSION_CUDA_H
#define REGRESSION_CUDA_H




FloatMatrix* transpose_x_mult_x(FloatMatrix *fm);
FloatMatrix* transpose_x_mult_y(FloatMatrix *x, FloatMatrix *y);
FloatMatrix* matrix_x_mult_y(FloatMatrix *x, FloatMatrix *y);


float determinant_of_matrix(float *mat, int n);


float determinant(float *A, int n);

void getCofactor(float *mat, float *temp, int p, int q, int n);

float* adjoint(float* mat, int n);


FloatMatrix *inverse_of_matrix(FloatMatrix *fm);
#endif
