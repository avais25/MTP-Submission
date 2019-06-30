#include "giotto_output2.h" 
volatile unsigned int schedule = 0; 
volatile unsigned int isr_schedule = 0; 
int main(int argc, char *argv[]) { 
init_devices(); 
TCCR4A = 0 ;
TCCR4B = (1 << CS00) | (1 << CS01); 
TIMSK4 = (1 << OCIE4A); 
OCR4A = 330; 
schedule=1; 
isr_schedule = 1; 
while(1) { 
switch (schedule) { 
case 0: 
// schedule = 0;
break;
case 1: 
input_t1();
schedule = 0;
break;
case 2: 
input_t2();
t1();
t2();
schedule = 0;
break;
case 3: 
actuation2();
schedule = 0;
break;
case 4: 
input_t2();
t2();
schedule = 0;
break;
case 5: 
actuation();
schedule = 0;
break;
case 6: 
actuation2();
schedule = 0;
break;
}} 
return 0; 
} 
ISR(TIMER4_COMPA_vect) { 
switch (isr_schedule) { 
case 1: 
OCR4A = 1482; 
schedule = 2;
isr_schedule = 2;
break; 
case 2: 
OCR4A = 1943; 
schedule = 3;
isr_schedule = 3;
break; 
case 3: 
OCR4A = 3325; 
schedule = 4;
isr_schedule = 4;
break; 
case 4: 
OCR4A = 3556; 
schedule = 5;
isr_schedule = 5;
break; 
case 5: 
OCR4A = 3786; 
schedule = 6;
isr_schedule = 6;
break; 
case 6: 
OCR4A = 330; 
schedule = 1; 
isr_schedule = 1; 
TCNT4 = 100;
break; 
}} 
