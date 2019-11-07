// CPP program to illustrate (##) operator 
#include <stdio.h> 
#define concat(a, b, c) a##b##c 
#define mkstr(s) #s
int main(void) 
{ 
    int xyz = 30; 
    int abe = 20;
    printf(mkstr(gogo+++gogog"%d"), concat(a, b, e)); 
    return 0; 
} 

