#include <cuda.h>
#include <iostream>
#include "FloatMatrix.h"
#include "Regression_cuda.h"
#include<bits/stdc++.h>


#define THREAD_SIZE 512
#define FSIZE sizeof(float)

using namespace std;

/*device functions */
__global__ void d_transpose(float *d_arr_tr,float *d_arr, int rs, int cs);

/*multiplies matrix srt and dest to dest */

__global__ void d_mat_mult_t(float *dest, float *arr_t, float *arr, int rs, int cs);
__global__ void d_swap_row(float* row1,float* row2,int rs);
__global__ void d_copy_row(float* dst, float* src, int rs);
__global__ void d_det_mult(float *mat, float *temp, float num1, float num2,int rs);
__global__ void d_mat_mult(float *dest, float *x_arr, float* y_arr, int x_rs, int x_cs, int y_rs, int y_cs);


/*first transposes a matrix and then multiplies it with itself*/
FloatMatrix* transpose_x_mult_x(FloatMatrix *fm){ 
        float *d_arr_tr, *d_arr;

        int rs = fm->row_size;
        int cs = fm->col_size;
        cudaMalloc((void**) &d_arr_tr, FSIZE*rs*cs);
        cudaMalloc((void**) &d_arr, FSIZE*rs*cs);

        cudaMemcpy(d_arr,fm->mat,FSIZE*rs*cs,cudaMemcpyHostToDevice);

        int blocks = (rs*cs/THREAD_SIZE) + (rs*cs%THREAD_SIZE != 0);
        d_transpose<<<blocks,THREAD_SIZE>>>(d_arr_tr,d_arr,rs,cs);

        FloatMatrix *mult_result = new FloatMatrix(cs,cs);
        int  mult_size = cs*cs;
        float *d_mult_arr;
        cudaMalloc((void**) &d_mult_arr,FSIZE*mult_size);

        blocks = mult_size/THREAD_SIZE + (mult_size%THREAD_SIZE != 0);

        d_mat_mult_t<<<blocks,THREAD_SIZE>>>(d_mult_arr,d_arr_tr,d_arr,rs,cs);
        cudaMemcpy(mult_result->mat,d_mult_arr,FSIZE*mult_size,cudaMemcpyDeviceToHost);

        cudaFree(d_arr_tr);
        cudaFree(d_arr);
	cudaFree(d_mult_arr);
        return mult_result; 

}

/* Transpose a matrix and then multiply it with Y */
FloatMatrix* transpose_x_mult_y(FloatMatrix *x, FloatMatrix *y){
        float *d_arr_tr, *d_arr_x, *d_arr_y;
	int x_rs = x->row_size;
	int x_cs = x->col_size;
	int y_rs = y->row_size;
	int y_cs = y->col_size;
	int xt_rs = x_cs;
	int xt_cs = x_rs;
	
	/* cuda memory for matrix/array */
        cudaMalloc((void**) &d_arr_x, FSIZE*x_rs*x_cs);
	cudaMalloc((void**) &d_arr_y, FSIZE*y_rs*y_cs);
        cudaMalloc((void**) &d_arr_tr, FSIZE*xt_rs*xt_cs);

	int blocks = (x_rs*x_cs/THREAD_SIZE) + ((x_rs*x_cs)%THREAD_SIZE != 0);


	/*Transposing matrix x */	
	cudaMemcpy(d_arr_x,x->mat,FSIZE*x_rs*x_cs,cudaMemcpyHostToDevice);
	d_transpose<<<blocks,THREAD_SIZE>>>(d_arr_tr,d_arr_x,x_rs,x_cs);	

	/*For debugging */
	/*	
	FloatMatrix *trans = new FloatMatrix(xt_rs,xt_cs);
	cudaMemcpy(trans->mat,d_arr_tr,FSIZE*xt_rs*xt_cs,cudaMemcpyDeviceToHost);
	trans->print();
	printf("\n");
	delete(trans);
	*/
	/*               */

	
	FloatMatrix *mult_result = new FloatMatrix(xt_rs,y_cs);	
	int mult_size = xt_rs*y_cs;
	float *d_mult_arr;
	cudaMalloc((void**) &d_mult_arr,FSIZE*mult_size); 

	
	/* copies the matrix y into the cuda memory */	
	cudaMemcpy(d_arr_y,y->mat,FSIZE*y_rs*y_cs,cudaMemcpyHostToDevice);

	blocks = mult_size/THREAD_SIZE + (mult_size%THREAD_SIZE != 0);
	d_mat_mult<<<blocks,THREAD_SIZE>>>(d_mult_arr,d_arr_tr,d_arr_y,xt_rs,xt_cs,y_rs,y_cs);
        cudaMemcpy(mult_result->mat,d_mult_arr,FSIZE*mult_size,cudaMemcpyDeviceToHost);

	/*Free allocated cuda memory*/
        cudaFree(d_arr_tr);
        cudaFree(d_arr_x);
	cudaFree(d_arr_y);
	cudaFree(d_mult_arr);
	return mult_result;
}


/*modified code from geeks for geeks */
float determinant_of_matrix(float *mat, int n){
	float* mat_cpy = new float[n*n];
	memcpy(mat_cpy,mat,FSIZE*n*n);
	float num1,num2, det = 1,total = 1; 
	int index;
	float *d_arr;// array for swapping rows
	float *d_temp;
	cudaMalloc((void**) &d_arr, FSIZE*n*n);
	cudaMalloc((void**) &d_temp,FSIZE*n +1);	
	int blocks ;
	
	float temp[n+1]; 

	for(int i = 0 ; i < n ; i++){
		index = i; 
		while(mat_cpy[index*n+i] == 0 && index < n)
			index++;
		if(index == n)
			continue;
		if(index != i){
			blocks = n/THREAD_SIZE + (n%THREAD_SIZE != 0);
			cudaMemcpy(d_arr,mat_cpy,FSIZE*n*n,cudaMemcpyHostToDevice);
			d_swap_row<<<blocks,THREAD_SIZE>>>(&d_arr[index*n],&d_arr[i*n],n);
			cudaMemcpy(mat_cpy,d_arr,FSIZE*n*n,cudaMemcpyDeviceToHost);
			det = det*pow(-1,index-i);	
		}
		
		

		memcpy(temp,&mat_cpy[i*n],FSIZE*n);
		cudaMemcpy(d_temp,temp,FSIZE*n+1,cudaMemcpyHostToDevice);
					
		for(int j = i+1; j < n; j++){
			
			num1 = temp[i];
			num2 = mat_cpy[j*n + i]; 
			blocks = n/THREAD_SIZE + (n%THREAD_SIZE != 0);
			cudaMemcpy(&d_arr[j*n],&mat_cpy[j*n],FSIZE*n,cudaMemcpyHostToDevice);
			d_det_mult<<<blocks,THREAD_SIZE>>>(&d_arr[j*n],d_temp,num1,num2,n);
			cudaMemcpy(&mat_cpy[j*n],&d_arr[j*n],FSIZE*n,cudaMemcpyDeviceToHost);
			
			total = total * num1;
			
		}
	}

	for(int i = 0 ; i < n; i++)
		det = det*mat_cpy[i*n +i];

	cudaFree(d_arr);
	cudaFree(d_temp);
	free(mat_cpy);
	return det/total;

}


/* Modified code from geeks for geeks, URL: https://www.geeksforgeeks.org/adjoint-inverse-matrix/ */
void getCofactor(float *mat, float *temp, int p, int q, int n){ 
    int i = 0, j = 0; 
  
    // Looping for each element of the matrix 
    for (int row = 0; row < n; row++) 
    { 
        for (int col = 0; col < n; col++) 
        { 
            //  Copying into temporary matrix only those element 
            //  which are not in given row and column 
            if (row != p && col != q) 
            { 
                temp[i*(n-1)+j] = mat[row*n+col]; 
		j++;
  
                // Row is filled, so increase row index and 
                // reset col index 
                if (j == n - 1) 
                { 
                    j = 0; 
                    i++; 
                } 
            } 
        } 
    } 
} 


/* Modified code from geeks for geeks, URL : https://www.geeksforgeeks.org/adjoint-inverse-matrix/ */
float* adjoint(float* mat, int n){ 
    float *adj = new float[n*n];
    if (n == 1) 
    { 
        adj[0] = 1; 
        return adj; 
    } 
  
    // temp is used to store cofactors of A[][] 
    int sign = 1;
    float *temp = new float[n*n];
  
    for (int i=0; i<n; i++) 
    { 
        for (int j=0; j<n; j++) 
        { 
            // Get cofactor of A[i][j] 
            getCofactor(mat, temp, i, j, n); 
  
            // sign of adj[j][i] positive if sum of row 
            // and column indexes is even. 
            sign = ((i+j)%2==0)? 1: -1; 
  
            // Interchanging rows and columns to get the 
            // transpose of the cofactor matrix 
            adj[j*n+i] = (sign)*(determinant_of_matrix(temp, n-1)); 
        } 
    } 
    free(temp);
    return adj;
} 

/* code modified from geeks for geeks URL: // Function to calculate and store inverse, returns false if */
// matrix is singular 
float* inverse_of_matrix(float* mat, int n) 
{ 

    float *inv = new float[n*n];
    float det = determinant_of_matrix(mat, n); 
    if (det == 0) 
    { 
        cout << "Singular matrix, can't find its inverse"; 
        return NULL; 
    } 
  
    // Find adjoint 
    float *adj = adjoint(mat, n); 
  
    // Find Inverse using formula "inverse(A) = adj(A)/det(A)" 
    for (int i=0; i<n; i++) 
        for (int j=0; j<n; j++) 
            inv[i*n +j] = adj[i*n+j]/det; 
    free(adj);
    return inv;
  
} 

__global__ void d_transpose(float *d_arr_tr,float *d_arr, int rs, int cs){

        int i = blockIdx.x*blockDim.x + threadIdx.x;

	if( i < rs*cs){
            int row = (i%cs);
            int col = (i/cs);

             int trans_index = row*(rs) + col;

             d_arr_tr[trans_index] = d_arr[i];
        }

}


__global__ void d_mat_mult_t(float *dest, float *arr_t, float *arr, int rs, int cs){


    int i = blockIdx.x*blockDim.x + threadIdx.x;


    int cs_t = rs;
    int rs_t = cs;

    __shared__ float shared[THREAD_SIZE];

    int d_row; 
    int d_col;
    if( i < rs_t*cs){
            shared[i] = 0;
            d_row = i/cs;
            d_col = i%cs;
    

        for(int k = 0 ; k < rs ; k++){
           shared[i] += arr_t[(d_row*cs_t)+k]*arr[(k*cs) + d_col];
            __syncthreads();
        }

        dest[i] = shared[i];


    }
 
}

__global__ void d_mat_mult(float *dest, float *x_arr, float* y_arr, int x_rs, int x_cs, int y_rs, int y_cs){

	int  i = blockIdx.x*blockDim.x + threadIdx.x;

	__shared__ float shared[THREAD_SIZE];
	int d_row,d_col;
	if( i < x_rs*y_cs){
		shared[i] = 0;
		d_row = i/x_cs;
		d_col = i%y_cs;
	
	
		for(int k = 0 ; k < y_rs ;k++){
			shared[i] += x_arr[(d_row*x_cs)+k] * y_arr[(k*y_cs) + d_col];
			__syncthreads();
		}
		
		dest[i] = shared[i];

	}
	
}

__global__ void d_swap_row(float *row1,float *row2,int rs){

	__shared__ float shared[THREAD_SIZE];

    	int i = blockIdx.x*blockDim.x + threadIdx.x;
	if( i < rs)
		shared[i] = row1[i];

	if( i < rs){
		row1[i] = row2[i];
		row2[i] = shared[i];
	}

}

__global__ void d_copy_row(float* dst, float* src, int rs){

	
    	int i = blockIdx.x*blockDim.x + threadIdx.x;
	
	if( i < rs)
		dst[i] = src[i];

}

__global__ void d_det_mult(float *mat, float *temp, float num1, float num2,int rs){
	
    	int i = blockIdx.x*blockDim.x + threadIdx.x;
	
	if( i < rs)
		mat[i] = (num1*mat[i]) - (num2*temp[i]);

}



