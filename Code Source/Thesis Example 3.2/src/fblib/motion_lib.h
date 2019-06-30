volatile unsigned long int ShaftCountLeft = 0;  //to keep track of left position encoder
volatile unsigned long int ShaftCountRight = 0; //to keep track of right position encoder


//Function to initialize pins for Motion Control
void motion_pin_config (void)
{
 DDRA = DDRA | 0x0F; 	//PA0-PA3 for direction control
 PORTA = PORTA & 0xF0;  
 DDRL = DDRL | 0x18;    //PL3, PL4 for enabling motor driving IC  
 PORTL = PORTL | 0x18;  
}


//initializes timer5 for PWM and velocity control
void timer5_init()
{
	TCCR5B = 0x00;	//Stop

	TCNT5H = 0xFF;	//setting up the counter
	TCNT5L = 0x01;	

	OCR5AH = 0x00;	//Output compare register high value for Left Motor
	OCR5BH = 0x00;	//Output compare register high value for Right Motor
	OCR5CH = 0x00;	//Higher 8 bits are not required for velocity control
	
  OCR5AL = 0xFF;	//Output compare register low value for Left Motor
	OCR5BL = 0xFF;	//Output compare register low value for Right Motor
	OCR5CL = 0xFF;	//Output compare register low value for channel C

	
	TCCR5A = 0xA9;	//Configures Timer 5
	TCCR5B = 0x0B;	//Configures Timer 5
}


//Function to configure INT4 (PORTE 4) pin as input for the left position encoder
void left_encoder_pin_config (void)
{
	DDRE  = DDRE & 0xEF;  //Set the direction of the PORTE 4 pin as input
	PORTE = PORTE | 0x10; //Enable internal pull-up for PORTE 4 pin
}


//Function to configure INT5 (PORTE 5) pin as input for the right position encoder
void right_encoder_pin_config (void)
{
	DDRE  = DDRE & 0xDF;  //Set the direction of the PORTE 4 pin as input
	PORTE = PORTE | 0x20; //Enable internal pull-up for PORTE 4 pin
}


void left_position_encoder_interrupt_init (void) //Interrupt 4 enable
{
//	cli(); //Clears the global interrupt
	EICRB = EICRB | 0x02; // INT4 is set to trigger with falling edge
	EIMSK = EIMSK | 0x10; // Enable Interrupt INT4 for left position encoder
//	sei();   // Enables the global interrupt
}


void right_position_encoder_interrupt_init (void) //Interrupt 5 enable
{
//	cli(); //Clears the global interrupt
	EICRB = EICRB | 0x08; // INT5 is set to trigger with falling edge
	EIMSK = EIMSK | 0x20; // Enable Interrupt INT5 for right position encoder
//	sei();   // Enables the global interrupt
}


void motion_init_devices (void)
{
// cli(); 								//Clears the global interrupts
 motion_pin_config();
 timer5_init();
 left_encoder_pin_config(); 			//left encoder pin config
 right_encoder_pin_config();			//right encoder pin config
 left_position_encoder_interrupt_init();
 right_position_encoder_interrupt_init();
// sei(); 								//Enables the global interrupts
}


// Function for robot velocity control
void velocity (unsigned char left_motor, unsigned char right_motor)
{
	OCR5AL = (unsigned char)left_motor;
	OCR5BL = (unsigned char)right_motor;
}


//Function used for setting motor's direction
void motion_set (unsigned char Direction)
{
 unsigned char PortARestore = 0;

 Direction &= 0x0F; 			// storing only last four bits needed for direction control
 PortARestore = PORTA; 			// reading the PORTA's original status
 PortARestore &= 0xF0; 			// storing only first four bits
 PortARestore |= Direction; 	// restoring upper nibble and adding direction
 PORTA = PortARestore; 			
}


void forward (void) //both wheels forward
{
  motion_set(0x06);
}

void back (void) //both wheels backward
{
  motion_set(0x09);
}

void left (void) //Left wheel backward, Right wheel forward
{
  motion_set(0x05);
}

void right (void) //Left wheel forward, Right wheel backward
{
  motion_set(0x0A);
}

void soft_left (void) //Left wheel stationary, Right wheel forward
{
 motion_set(0x04);
}

void soft_right(void) //Left wheel forward, Right wheel is stationary
{
 motion_set(0x02);
}

void stop (void) //hard stop
{
  motion_set(0x00);
}


//ISR for right position encoder
ISR(INT5_vect)
{
	ShaftCountRight++;  //increment right shaft position count
}

//ISR for left position encoder
ISR(INT4_vect)
{
	ShaftCountLeft++;  //increment left shaft position count
}


//Function used for turning robot by specified degrees
void angle_rotate(unsigned int Degrees)
{
	float ReqdShaftCount = 0;
	unsigned long int ReqdShaftCountInt = 0;

	ReqdShaftCount = (float) Degrees/ 4.09; // division by resolution to get shaft count
	ReqdShaftCountInt = (unsigned int) ReqdShaftCount;
	ShaftCountRight = 0;
	ShaftCountLeft = 0;
//	velocity(150,150);

	while (1)
	{
		if((ShaftCountRight >= ReqdShaftCountInt) || (ShaftCountLeft >= ReqdShaftCountInt))
		break;
	}
	stop(); //Stop robot
}

//Function used for moving robot forward by specified distance
void linear_distance_mm(unsigned int DistanceInMM)
{
	float ReqdShaftCount = 0;
	unsigned long int ReqdShaftCountInt = 0;

	ReqdShaftCount = DistanceInMM / 5.6; // division by resolution to get shaft count
	ReqdShaftCountInt = (unsigned long int) ReqdShaftCount;
	
	ShaftCountRight = 0;
	ShaftCountLeft = 0;
	while(1)
	{
		if ((ShaftCountRight > ReqdShaftCountInt) || (ShaftCountLeft > ReqdShaftCountInt))
		{
			break;
		}
	}
	stop(); //Stop robot
}


void forward_mm(unsigned int DistanceInMM)
{
	forward();
	linear_distance_mm(DistanceInMM);
}

void back_mm(unsigned int DistanceInMM)
{
	back();
	linear_distance_mm(DistanceInMM);
}

void left_degrees(unsigned int Degrees)
{
	// 88 pulses for 360 degrees rotation --> 4.090 degrees per count
	left(); //Turn left
	angle_rotate(Degrees);
}



void right_degrees(unsigned int Degrees)
{
	// 88 pulses for 360 degrees rotation --> 4.090 degrees per count
	right(); //Turn right
	angle_rotate(Degrees);
}

void soft_left_degrees(unsigned int Degrees)
{
	// 176 pulses for 360 degrees rotation --> 2.045 degrees per count
	soft_left(); //Turn soft left
	Degrees=Degrees*2;
	angle_rotate(Degrees);
}

void soft_right_degrees(unsigned int Degrees)
{
	// 176 pulses for 360 degrees rotation --> 2.045 degrees per count
	soft_right();  //Turn soft right
	Degrees=Degrees*2;
	angle_rotate(Degrees);
}