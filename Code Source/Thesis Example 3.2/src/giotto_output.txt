#include "giotto_input.c"

//ports;
float s1,a=0,i1,i2,o1=0,o2=0,p1=0,p2=0;


//temporary ports
float t_o1=0, t_o2=0;

//complete tasks

void t1()
{

    //function
    f1(i1, &t_o1);

}

void t1_update()
{
    o1=t_o1;
}

void t2()
{
    f2(i2, &t_o2);
}

void t2_update()
{
    o2=t_o2;
}

//complete driver

void d1()
{
    h1(o2,&i1);
}

void d2()
{
    h2(&s1,&i2);
}

void d4()
{
    h4(o1,&a);
}
