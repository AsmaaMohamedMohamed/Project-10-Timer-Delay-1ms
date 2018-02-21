/*
 * main.c
 *
 *  Created on: ??þ/??þ/????
 *      Author: Asmaa
 */

#include<avr/io.h>
#include<avr/interrupt.h>
#include"Std_Types.h"
#include"Macros.h"
#include"DIO_interface.h"

void Timer_Delay_ms(u32 Time);

#define LED_PIN PIN0

u16 OV_Count;
u32 Desired_Time=0;
u8 End_Flag=0;
u32 ms_Count=0;

int main(void)
{
	Dio_vidSetPinDirection(DIO_PORTB,LED_PIN,STD_HIGH);
	while(1)
	{
		Dio_vidSetPinValue(DIO_PORTB,LED_PIN,STD_HIGH);

		Timer_Delay_ms(1000);

		Dio_vidSetPinValue(DIO_PORTB,LED_PIN,STD_LOW);

		Timer_Delay_ms(1000);
	}
	return 0;
}
void Timer_Delay_ms(u32 Time)
{
	/*Timer register configuration*/

		/*clear force output compare bit*/
		CLEAR_BIT(TCCR0,FOC0);

		/*set normal mode */
		CLEAR_BIT(TCCR0,WGM00);
		CLEAR_BIT(TCCR0,WGM01);

		/*No output compare value*/
		CLEAR_BIT(TCCR0,COM01);
		CLEAR_BIT(TCCR0,COM00);

		/*set no prescaling mode  CS02 CS01 CS00 >> 001*/
		CLEAR_BIT(TCCR0,CS02);
		CLEAR_BIT(TCCR0,CS01);
		SET_BIT(TCCR0,CS00);

		/*initialize timer counter by 0 */
		TCNT0=192;

		/*timer overflow int enable*/
		SET_BIT(TIMSK,TOIE0);

		/*Global int enable*/
		SET_BIT(SREG,7);

		Desired_Time=0;
		End_Flag=0;
		Desired_Time=Time;

		while(End_Flag==0)
		{

		}
		End_Flag=0;
		/*timer overflow int disable*/
		CLEAR_BIT(TIMSK,TOIE0);
}
ISR(TIMER0_OVF_vect)
{
	OV_Count++;
	if(OV_Count==32)
	{
		TCNT0=192;
		OV_Count=0;
		ms_Count++;
		if(ms_Count==Desired_Time)
		{
			ms_Count=0;
			End_Flag=1;
		}
	}
}
