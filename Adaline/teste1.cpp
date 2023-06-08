#include "adaline.h"
#include <iostream>

int main(){

    Matriz<double> p(3,3);

    p[0][0] = 1;    
    p[1][0] = 1;  
    p[2][0] = 1;       

    p[0][1] = -1;   
    p[1][1] = -1;
    p[2][1] = 1;
    
    p[0][2] = 2;    
    p[1][2] = 2;
    p[2][2] = 1;

    Matriz<double> t(1,3);
    t[0][0] = 1; 
    t[0][1] = 1; 
    t[0][2] = -1;

    Adaline ada;
    ada.initAdaline(1,2);
    ada.printLayers();
    ada.train(&p,&t,100,0.05);
    ada.printLayers();

    return 0;
}