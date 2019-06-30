#include "fblib/firebird.h"
#include "cor_c/cor.h"
#include "rightO_c/rightO.h"


Cor__acc_out *_out;

RightO__rightO_out *_out2;

void Cor__acc_step(float blocked, float leftOut, float rightOut, float centreOut,int rightBlock,
                   float *lw,float *rw) {
  
  int v_29;
  int v_28;
  int v_27;
  int v_26;
  int v_25;
  int v_24;
  int v_23;
  int v_22;
  int v_21;
  int v_20;
  int v_19;
  int v_18;
  int v_17;
  int v_16;
  int v_15;
  int v_14;
  int v_13;
  int v_12;
  int v_11;
  int v_10;
  int v_9;
  int v_8;
  int v_7;
  int v_6;
  int v_5;
  int v_4;
  int v_3;
  int v_2;
  int v_1;
  int v;
  v_25 = (leftOut==true);
  if (v_25) {
    v_26 = 1;
  } else {
    v_26 = 0;
  };
  v_24 = (rightOut==true);
  if (v_24) {
    v_27 = 2;
  } else {
    v_27 = v_26;
  };
  v_22 = (rightOut==true);
  v_20 = (leftOut==true);
  v_19 = (centreOut==true);
  v_21 = (v_19&&v_20);
  v_23 = (v_21&&v_22);
  if (v_23) {
    v_28 = 0;
  } else {
    v_28 = v_27;
  };
  v_18 = (centreOut==false);
  if (v_18) {
    v_29 = 3;
  } else {
    v_29 = v_28;
  };
  v_16 = (rightBlock==0);
  v_15 = (blocked==true);
  v_17 = (v_15||v_16);
  if (v_17) {
    _out->wheelRight = 0;
  } else {
    _out->wheelRight = v_29;
  };
  v_10 = (leftOut==true);
  if (v_10) {
    v_11 = 2;
  } else {
    v_11 = 0;
  };
  v_9 = (rightOut==true);
  if (v_9) {
    v_12 = 1;
  } else {
    v_12 = v_11;
  };
  v_7 = (rightOut==true);
  v_5 = (leftOut==true);
  v_4 = (centreOut==true);
  v_6 = (v_4&&v_5);
  v_8 = (v_6&&v_7);
  if (v_8) {
    v_13 = 0;
  } else {
    v_13 = v_12;
  };
  v_3 = (centreOut==false);
  if (v_3) {
    v_14 = 3;
  } else {
    v_14 = v_13;
  };
  v_1 = (rightBlock==0);
  v = (blocked==true);
  v_2 = (v||v_1);
  if (v_2) {
    _out->wheelLeft = 0;
  } else {
    _out->wheelLeft = v_14;
  };;

  *lw = _out->wheelLeft;
  *rw = _out->wheelRight;
}

void RightO__rightO_step(float rf, float rb, float *blocked) {
  
  int v_2;
  int v_1;
  int v;
  v_1 = (rb>100);
  v = (rf>150);
  v_2 = (v&&v_1);
  if (v_2) {
    _out2->rightBlocked = 0;
  } else {
    _out2->rightBlocked = 1;
  };;

  *blocked = _out2->rightBlocked;
}

void inputDriver2(float *rightF,float *rightB,float *rfi,float *rbi) {

  *rightF = ADC_Conversion(7);
  *rfi=*rightF;
// print_sensor(1,8,7);
  
  *rightB = ADC_Conversion(8);
  *rbi=*rightB;
// print_sensor(2,8,8);
}


void actDriver2(float rBlk, float *bzr) {
    if(rBlk == 0)
    {
    buzzer_on();
    }
    else
    {
      buzzer_off();
    }
    
    //  print_sensor(2,1,rBlk);
}




int getSpeed(int s)
{
  switch (s) {
    case 0:
    return 0;
    case 1:
    return 50;
    case 2:
    return 130;
    case 3:
    return 150;
    default:
    return 0;
  }
}


void actDriver(float lw,float rw,float *acc) {
    int left=getSpeed(lw);
    int right=getSpeed(rw);

    

    velocity(left, right);

    forward();

     lcd_print(2, 1, left, 3);

    lcd_print(2, 5, right, 3);
}

//read the sensor then execut the driver
void inputDriver(float *Front_Sharp_Sensor,float *Left_white_line,float *Right_white_line,float *Center_white_line,float blkin,float *blocked,float *leftOut,float *rightOut,float *centreOut,float *rblk) {
  *Left_white_line = ADC_Conversion(3);	//Getting data of Left WL Sensor
  *Center_white_line = ADC_Conversion(2);	//Getting data of Center WL Sensor
  *Right_white_line = ADC_Conversion(1);	//Getting data of Right WL Sensor

  *Front_Sharp_Sensor = ADC_Conversion(11);

  // print_sensor(2,4,11);

  *rblk = blkin;

  if (*Front_Sharp_Sensor > 150)
  *blocked=1;
  else
  *blocked=0;


  if(*Left_white_line>0x40)
  *leftOut=1;
  else
  *leftOut=0;

  if(*Right_white_line>0x40)
  *rightOut=1;
  else
  *rightOut=0;

  if(*Center_white_line>0x40)
  *centreOut=1;
  else
  *centreOut=0;


}