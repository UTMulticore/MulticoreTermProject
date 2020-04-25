#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "FloatMatrix.h"
#include "Regression.h"



#define THREAD_SIZE 512
#define FSIZE sizeof(float)

using namespace std;

/*private helper methods for regression*/
FloatMatrix *inverse_of_matrix(FloatMatrix *fm);
FloatMatrix *transpose_x_mult_y(FloatMatrix *x, FloatMatrix *y);
FloatMatrix *matrix_x_mult_y(FloatMatrix *x, FloatMatrix *y);
FloatMatrix *transpose_matrix(FloatMatrix *fm);
float determinant_of_matrix(float *omat,int n);
float* adjoint(float* mat, int n);
void getCofactor(float *mat, float *temp, int p, int q, int n);


/* REGRESSION CLASS METHODS */
FloatMatrix* Regression::fit(FloatMatrix *x_train, FloatMatrix *y_train){

    int n_x = x_train->row_size; 

    /* (X^T X)^-1  = A*/
    FloatMatrix *trans_mult_x = transpose_x_mult_y(x_train,x_train);

    #ifdef DEBUG
    cout << "The transpose of matrix x*xis: " << endl;
    trans_mult_x->print();
    #endif

    FloatMatrix *inv = inverse_of_matrix(trans_mult_x);

    if(inv == NULL){
        delete(trans_mult_x);
        return NULL;
    }


    #ifdef DEBUG
    cout << "The inverse of the matrix (X^TX)^-1 is: " << endl;
    inv->print();
    #endif

    /*  X^T Y * = B*/ 
    FloatMatrix *trans_x_mult_y = transpose_x_mult_y(x_train,y_train);

    /* A*B = ans */
    FloatMatrix *final_result = matrix_x_mult_y(inv,trans_x_mult_y);

    delete(trans_mult_x);
    delete(inv);
    delete(trans_x_mult_y);

    return final_result;
}


FloatMatrix* Regression::predict(FloatMatrix *data, FloatMatrix *weights){

	return transpose_x_mult_y(data,weights);
}


/*HELPER FUNCTIONS (not part of regression class directly)*/


/* code modified from geeks for geeks URL : https://www.geeksforgeeks.org/adjoint-inverse-matrix/ */
/* Function to calculate and store inverse, returns null if matrix is singular */
FloatMatrix* inverse_of_matrix(FloatMatrix *fm){ 
    float *mat = fm->mat;
    int n = fm->row_size;
    float *inv = new float[n*n];
    float det = determinant_of_matrix(mat, n); 
    if (det == 0) 
    { 
        printf("Singular matrix, can't find its inverse"); 
        return NULL; 
    } 
  
    // Find adjoint 
    float *adj = adjoint(mat, n); 
  
    // Find Inverse using formula "inverse(A) = adj(A)/det(A)" 
    for (int i=0; i<n; i++) 
        for (int j=0; j<n; j++) 
            inv[i*n +j] = adj[i*n+j]/det; 
    free(adj);

    FloatMatrix *result = new FloatMatrix(n,n,inv);
    return result;
  
} 

/*modified code from geeks for geeks  URL : https://www.geeksforgeeks.org/adjoint-inverse-matrix/ */
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



/*Modified code from geeks for geeks URL : https://www.geeksforgeeks.org/adjoint-inverse-matrix/ */
float determinant_of_matrix(float *omat, int n){
    
    float *mat = new float[n*n];
    for(int i = 0 ; i < n ; i++)
        for(int j = 0 ; j < n; j++)
            mat[i*n+j] = omat[i*n+j];
    float num1,num2;
    int index;
    double det = 1,total = 1; // Initialize result   
        
    // temporary array for storing row   
    float temp[n + 1];   
        
    //loop for traversing the diagonal elements  
    for(int i = 0; i < n; i++)    
    {  
        index = i; // initialize the index   
            
        //finding the index which has non zero value   
        while(mat[index*n +i] == 0 && index < n) {   
            index++;       
                
        }   
        if(index == n) // if there is non zero element   
        {   
            // the determinat of matrix as zero   
            continue;   
                
        }   
        if(index != i)   
        {   
            //loop for swaping the diagonal element row and index row   
            for(int j = 0; j < n; j++)   
            {  
               float temp = mat[index*n+j];
               mat[index*n+j] = mat[i*n+j];
               mat[i*n+j] = temp; 
            }   
                //determinant sign changes when we shift rows   
                //go through determinant properties   
                float sign = (index-i)%2==0 ? 1:-1;
                det = det*sign;
       }   
           
       //storing the values of diagonal row elements   
       for(int j = 0; j < n; j++)   
       {   
           temp[j] = mat[i*n+j];   
               
       }   
       //traversing every row below the diagonal element   
       for(int j = i+1; j < n; j++)   
       {   
           num1 = temp[i]; //value of diagonal element   
           num2 = mat[j*n+i]; //value of next row element   
               
           //traversing every column of row   
           // and multiplying to every row   
           for(int k = 0; k < n; k++)   
           {   
               //multiplying to make the diagonal   
               // element and next row element equal   
               mat[j*n+k] = (num1 * mat[j*n+k]) - (num2 * temp[k]);   
                   
           }   
           total = total * num1; // Det(kA)=kDet(A);   
           }   
            
    }   
    
    //mulitplying the diagonal elements to get determinant   
    for(int i = 0; i < n; i++)   
    {   
        det = det * mat[i*n+i];   
            
    }  

    free(mat);
    return (det/total); //Det(kA)/k=Det(A);   
    
}

/* my code written */

/*transposes matrix x and then multiplies it with matrix y */
FloatMatrix *transpose_x_mult_y(FloatMatrix *x, FloatMatrix *y){
    FloatMatrix *x_tr = transpose_matrix(x);

    FloatMatrix *r = matrix_x_mult_y(x_tr,y);
    delete(x_tr);
    return r;

}


/* transposes matrix given */
FloatMatrix *transpose_matrix(FloatMatrix *fm){ 
    
    FloatMatrix *trans = new FloatMatrix(fm->col_size,fm->row_size);

    for(int i = 0 ; i < trans->row_size; i++){
        for(int j = 0 ; j < trans->col_size; j++){
            trans->mat[(trans->col_size)*i + j]  = fm->mat[(fm->col_size)*j + i];
        }

    }


    return trans;
}

/* multiplies matrix x with matrix y */
FloatMatrix *matrix_x_mult_y(FloatMatrix *x, FloatMatrix *y){
    
    int mult_rs = x->row_size;
    int mult_cs = y->col_size;
    int k_size = y->row_size;

    FloatMatrix *mult = new FloatMatrix(mult_rs,mult_cs);

   for(int i = 0 ; i < mult_rs ; i++)
      for(int j = 0 ; j < mult_cs ; j++)
        for(int k = 0 ; k < k_size; k++)
            mult->mat[i*mult_cs + j] += x->mat[i*x->row_size + k] * y->mat[k*y->col_size + j];

   return mult;
}
