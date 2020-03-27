#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "csvparser.h"


/*private functions*/
int _get_row_num(const char *str_array);
int _get_col_num(const char *str_array);

FloatMatrix *csv_to_matrix(const char *filename){
    FILE *fp = fopen(filename,"r");

    fseek(fp,0L,SEEK_END);
    size_t file_size = ftell(fp);
    fseek(fp,0L,SEEK_SET); 

    char *buffer = (char*) malloc(sizeof(char)*(file_size+1));

    fread(buffer, 1, file_size, fp);
    buffer[file_size] = '\0';
    fclose(fp);

    int num_row = _get_row_num(buffer);
    int num_col = _get_col_num(buffer);

    float **mat = (float**) malloc(sizeof(float*)*num_row);
    
    for(int i = 0 ; i < num_row ; i++)
        mat[i] = (float*) malloc(sizeof(float)*num_col);
    
    char *saveptr1, *saveptr2; 
    char *c1 = strtok_r(buffer,"\n",&saveptr1);
    int i = 0; 
    while(c1 != NULL){
        char *c2 = strtok_r(c1, ",",&saveptr2);
        int k = 0; 
        while(c2 != NULL){
          mat[i][k] = atof(c2);  
          c2 = strtok_r(NULL,",",&saveptr2);
          k++;  
        }
        c1 = strtok_r(NULL,"\n",&saveptr1);
        i++;
    }


    FloatMatrix *float_mat = (FloatMatrix*) malloc(sizeof(FloatMatrix));

    float_mat->mat = mat; 
    float_mat->num_rows = num_row;
    float_mat->num_cols = num_col;

    return float_mat;
    
}

void free_FloatMatrix(FloatMatrix *float_mat){


    for(int i = 0; i < float_mat->num_rows ; i++)
        free(float_mat->mat[i]); 

    free(float_mat);


}


void print_FloatMatrix(FloatMatrix *float_mat){
    
    for(int i = 0 ; i < float_mat->num_rows ; i++){
        printf("%i [ ",i);
        for(int k = 0; k < float_mat->num_cols; k++){
           printf("%f ",float_mat->mat[i][k]);
        }
        printf("]\n");

    }

}

/* private functions */

//gets the number of rows in the csv file
int _get_row_num(const char *str_array){
    
    int count = 0; 
    for(int i = 0 ; str_array[i] != '\0'; i++){
    
        if(str_array[i] == '\n' )
            count++;
    }

    return count; 
}


int _get_col_num(const char *str_array){
    
    int count = 1; 

    for(int i = 0 ; str_array[i] != '\0' && str_array[i] != '\n' ; i++){   
       if(str_array[i] == ',') 
        count++;
    }

    return count;

}

