#include <iostream>
#include <string.h>
#include "FloatMatrix.h"


/* public functions */

FloatMatrix::FloatMatrix(int row_size, int col_size){
    this->row_size = row_size; 
    this->col_size = col_size;
    mat = new float [row_size*col_size];
}

FloatMatrix::FloatMatrix(const char* filename){

    FILE *fp = fopen(filename,"r");

    fseek(fp,0L,SEEK_END);
    size_t file_size = ftell(fp);
    fseek(fp,0L,SEEK_SET); 

    char *buffer = new char[file_size+1]; 

    fread(buffer, 1, file_size, fp);
    buffer[file_size] = '\0';
    fclose(fp);

    row_size = get_row_size(buffer);
    col_size = get_col_size(buffer);

    mat = new float [row_size*col_size];
    
    char *saveptr1, *saveptr2; 
    char *c1 = strtok_r(buffer,"\n",&saveptr1);
    int i = 0; 
    while(c1 != NULL){
        char *c2 = strtok_r(c1, ",",&saveptr2);
        int k = 0; 
        while(c2 != NULL){
          mat[(i*col_size) + k] = atof(c2);  
          c2 = strtok_r(NULL,",",&saveptr2);
          k++;  
        }
        c1 = strtok_r(NULL,"\n",&saveptr1);
        i++;
    }

}


FloatMatrix::~FloatMatrix(){
 
    delete mat;

}


void FloatMatrix::print(void){
    for(int i = 0 ; i < row_size ; i++){
            printf("%i [ ",i);
            for(int k = 0; k < col_size ; k++){
               printf("%f ",mat[(i*col_size) + k]);
            }
            printf("]\n");

        }

}

/* private functions */


int FloatMatrix::get_row_size(const char *str_array){
    
    int count = 0; 
    for(int i = 0 ; str_array[i] != '\0'; i++){
    
        if(str_array[i] == '\n' )
            count++;
    }

    return count; 
}


int FloatMatrix::get_col_size(const char *str_array){
    
    int count = 1; 

    for(int i = 0 ; str_array[i] != '\0' && str_array[i] != '\n' ; i++){   
       if(str_array[i] == ',') 
        count++;
    }

    return count;

}



