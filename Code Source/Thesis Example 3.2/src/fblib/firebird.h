#define F_CPU 14745600
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "lcd.h"
#include "motion_lib.h"
#include "adc_lib.h"
#include "buzzer_lib.h"
#include "is_front_blocked.h"
#include "white_line_follow.h"


// =============================================================================================================




// ==============================================================================================================
void init_devices()
{
	cli();
	buzzer_init_devices();
	motion_init_devices();
	adc_lcd_init_devices();
	sei();
}
