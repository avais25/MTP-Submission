#include "giotto_input.h"

float s1;
float a=0;
float i1;
float i2;
float o1=0;
float o2=0;
float p1=0;
float p2=0;

void t1(){
f1(i1,&o1);
}

void t2(){
f2(i2,&o2);
}

void input_t1(){
h1(o2,&i1);
}

void input_t2(){
h2(&s1,&i2);
}

void actuation(){
h4(o1,&a);
}
