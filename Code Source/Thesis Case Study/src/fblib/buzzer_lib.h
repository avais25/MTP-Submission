
//Function to initialize Buzzer 
void buzzer_pin_config (void)
{
 DDRC = DDRC | 0x08;		//Setting PORTC 3 as output 
 PORTC = PORTC & 0xF7;		//Setting PORTC 3 logic low to turnoff buzzer (1111 0111)
}

void buzzer_init_devices (void)
{
 // cli(); //Clears the global interrupts
 buzzer_pin_config();
 // sei(); //Enables the global interrupts
}


//Function to turn the Buzzer on
void buzzer_on (void)
{
 unsigned char port_restore = 0;
 port_restore = PINC;
 port_restore = port_restore | 0x08;  
 PORTC = port_restore;
}

//Function to turn the Buzzer off
void buzzer_off (void)
{
 unsigned char port_restore = 0;
 port_restore = PINC;
 port_restore = port_restore & 0xF7;
 PORTC = port_restore;
}








