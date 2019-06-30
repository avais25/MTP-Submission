#include "fblib/firebird.h"

//task function definition
//driver will be associated with these tasks which 
//will provide value to the arguments
void f1(float i1,float *temp1)
{
    _delay_us(100);
    i1++;
    *temp1=40;
}


void f2(float i2,float *temp1)
{
    _delay_us(500);
    i2++;
    *temp1=50;
}


//driver function definition

void h1(float o2,float *temp1)
{
    //do some computation 
    *temp1=o2;
}

//read the sensor then execut the driver
void h2(float *s1,float *temp1)
{
    // some sensor read needs to be performed|| read(s1)
    *s1=1;
    *temp1=10;
}

void h4(float o1,float *temp1)
{
    temp1++;
    //do some actuation
}