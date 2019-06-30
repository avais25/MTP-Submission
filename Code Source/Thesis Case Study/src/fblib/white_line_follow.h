unsigned char white_line_follow()
{
	    unsigned char Left_white_line = ADC_Conversion(3);	//Getting data of Left WL Sensor
	    unsigned char Center_white_line = ADC_Conversion(2);	//Getting data of Center WL Sensor
	    unsigned char Right_white_line = ADC_Conversion(1);	//Getting data of Right WL Sensor

	    unsigned char flag=0;

	    print_sensor(1,1,3);	//Prints value of White Line Sensor1
	    print_sensor(1,5,2);	//Prints Value of White Line Sensor2
	    print_sensor(1,9,1);	//Prints Value of White Line Sensor3
	    
		
		forward();
		velocity(150,150);
	    

	    if(Center_white_line<0x28)
	    {
		    flag=1;
		    forward();
		    velocity(150,150);
	    }

	    if((Left_white_line>0x28) && (flag==0))
	    {
		    flag=1;
		    forward();
		    velocity(130,50);
	    }

	    if((Right_white_line>0x28) && (flag==0))
	    {
		    flag=1;
		    forward();
		    velocity(50,130);
	    }

	    if(Center_white_line>0x28 && Left_white_line>0x28 && Right_white_line>0x28)
	    {
		    forward();
		    velocity(0,0);
	    }
		return flag;

}