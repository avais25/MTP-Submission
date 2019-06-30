
unsigned char ADC_Value;
void lcd_port_config (void)
{
 DDRC = DDRC | 0xF7;   //all the LCD pin's direction set as output
 PORTC = PORTC & 0x80; 
}

//ADC pin configuration
void adc_pin_config (void)
{
 DDRF = 0x00; //set PORTF direction as input
 PORTF = 0x00; //de-activate pull-up
 DDRK = 0x00; //set PORTK direction as input
 PORTK = 0x00; //de-activate pull-up
}

//Function to Initialize ADC
void adc_init()
{
	ADCSRA = 0x00;		//Control ADC Operation 
	ADCSRB = 0x00;		
	ADMUX = 0x20;		//Select channel
	ACSR = 0x80;		//Disable analog comparator
	ADCSRA = 0x86;		
}


void adc_lcd_init_devices (void)
{
// cli(); //Clears the global interrupts
 lcd_port_config();
 adc_pin_config();
 adc_init();
 lcd_set_4bit();
 lcd_init();
// sei(); //Enables the global interrupts
}

//This Function accepts the Channel Number and returns the corresponding Digital Value 
unsigned char ADC_Conversion(unsigned char Ch)
{
	unsigned char a;
	if(Ch>7)
	{
		ADCSRB = 0x08;
	}
	Ch = Ch & 0x07;  			
	ADMUX= 0x20| Ch;	   		
	ADCSRA = ADCSRA | 0x40;		//Set start conversion bit
	while((ADCSRA&0x10)==0);	//Wait for ADC conversion to complete
	a=ADCH;
	ADCSRA = ADCSRA|0x10; 		//clear ADIF (ADC Interrupt Flag) by writing 1 to it
	ADCSRB = 0x00;
	return a;
}

void print_sensor(char row, char coloumn,unsigned char channel)
{
	ADC_Value = ADC_Conversion(channel);
	lcd_print(row, coloumn, ADC_Value, 3);
}