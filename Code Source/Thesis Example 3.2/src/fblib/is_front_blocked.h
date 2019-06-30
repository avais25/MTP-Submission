unsigned char is_front_blocked()
{

		unsigned char Front_Sharp_Sensor = ADC_Conversion(11);
		unsigned char Front_IR_Sensor = ADC_Conversion(6);

		print_sensor(2,4,11);	//Prints Value of Front Sharp Sensor
		print_sensor(2,8,6);	//Prints Value of Front IR Sensor
				

		//if(Front_Sharp_Sensor>0x82 || Front_IR_Sensor<0xF0)
		if(Front_Sharp_Sensor>150 || Front_IR_Sensor<50)
		{
			return 1;
		}

		return 0;


}