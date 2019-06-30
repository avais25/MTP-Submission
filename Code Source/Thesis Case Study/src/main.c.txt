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
schedule = 0;
break;
case 1: 
input_t1();
t1();
schedule = 0;
break;
case 2: 
actuation();
schedule = 0;
break;
case 3: 
input_t1();
t1();
schedule = 0;
break;
case 4: 
actuation();
schedule = 0;
break;
}} 
return 0; 
} 
ISR(TIMER4_COMPA_vect) { 
switch (isr_schedule) { 
case 1: 
OCR4A = 560; 
schedule = 2;
isr_schedule = 2;
break; 
case 2: 
OCR4A = 791; 
schedule = 3;
isr_schedule = 3;
break; 
case 3: 
OCR4A = 1021; 
schedule = 4;
isr_schedule = 4;
break; 
case 4: 
OCR4A = 330; 
schedule = 1; 
isr_schedule = 1; 
TCNT4 = 100;
break; 
}} 
