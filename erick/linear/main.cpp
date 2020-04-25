#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "FloatMatrix.h"
#include "Regression.h"

#define TIME  // used for timing purposes

#ifdef TIME
#include <time.h>
clock_t start_t, end_t;


#endif

int main(int argc,char *argv[]){

    using namespace std;     
    if(argc < 2){
        cout << "Wrong number of arguments" << endl; 
        exit(-1);
    }

    if(strcmp("-f",argv[1])==0){
        if(argc == 4){
            FloatMatrix *x = new FloatMatrix(argv[2]); 
            FloatMatrix *y = new FloatMatrix(argv[3]);
            #ifdef DEBUG
            x->print();
            printf("\n");
            y->print();
            printf("\n");
            #endif
        
            #ifdef TIME
            start_t = clock(); 
            #endif
            FloatMatrix *r = Regression::fit(x,y);
            #ifdef TIME
            end_t = clock(); 
            int msec = ((start_t-end_t)*1000)/CLOCKS_PER_SEC;
            cout << "Time taken for Regression fit is " << msec << " miliseconds" << endl;
            #endif
            if(r == NULL){
                delete(x);
                delete(y);
                cout << "ERROR regression fit returned null" << endl;
                exit(-1);

            }

            r->printcsv();
            delete(x);
            delete(y);
            delete(r);
            
            exit(0);

            
        }else{

            cout << "Wrong number of arguments, follow the format of \"./program.o -f x_train.csv y_train.csv" << endl;
        }

    }
    else if(strcmp("-p",argv[1])==0){

        if(argc == 4){
            FloatMatrix *x = new FloatMatrix(argv[2]); 
            FloatMatrix *y = new FloatMatrix(argv[3]);
            
            FloatMatrix *r = Regression::predict(x,y);

            if(r == NULL){
                delete(x);
                delete(y);
                cout << "ERROR regression fit returned null" << endl;
                exit(-1);
            }

            r->printcsv();
            delete(x);
            delete(y);
            delete(r);
            
            exit(0);

        }else{
            cout << "Wrong number of arguments, follow the format of \" ./program.o -p data.csv weights.csv" << endl;
        }
    }
    else{

        cout << "Please use flags -f for fitting and flag -p for prediction" << endl;
        exit(-1);

    }
    

}



