#include "giotto_input.h"

float bl;
float le;
float ri;
float cn;
float rf;
float rb;
float wheel=0;
float buzzer=0;
float bli;
float lei;
float rii;
float cni;
float rfi;
float rbi;
float rBlki;
float lw=0;
float rw=0;
float rBlk=0;

void t1(){
Cor__acc_step(bli,lei,rii,cni,rBlki,&lw,&rw);
}

void t2(){
RightO__rightO_step(rfi,rbi,&rBlk);
}

void input_t1(){
inputDriver(&bl,&le,&ri,&cn,rBlk,&bli,&lei,&rii,&cni,&rBlki);
}

void input_t2(){
inputDriver2(&rf,&rb,&rfi,&rbi);
}

void actuation(){
actDriver(lw,rw,&wheel);
}

void actuation2(){
actDriver2(rBlk,&buzzer);
}
