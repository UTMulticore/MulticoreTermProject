#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "FloatMatrix.h"
#include "Regression.h"





int main(int argc,char *argv[]){

    using namespace std;     
    if(argc < 2){
        cout << "Wrong number of arguments" << endl; 
        exit(-1);
    }

    if(strcmp("-f",argv[1])==0){
        if(argc == 4){
            FloatMatrix *x = new FloatMatrix("tempx.csv"); 
            FloatMatrix *y = new FloatMatrix("tempy.csv");
            x->print();
            printf("\n");
            y->print();
            printf("\n");
            FloatMatrix *r = Regression::fit(x,y);
            if(r == NULL){
                delete(x);
                delete(y);
                printf("ERROR regression fit returned null");
                exit(-1);

            }

            r->print();
            delete(x);
            delete(y);
            delete(r);
            
            exit(0);

            
        }else{

            cout << "Wrong number off arguments, follow the format of \"./program -t name_of_csv\"" << endl;
        }

    }
    else if(strcmp("-p",argv[1])==0){
         
    }
    else{

        cout << "Please use flags -t for training and flag -p for prediction" << endl;
        exit(-1);

    }
    

}



