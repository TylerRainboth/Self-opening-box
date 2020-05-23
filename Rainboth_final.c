#include <avr/io.h>             /* Defines pins, ports, etc */
#include <util/delay.h>         /* Functions to waste time */
#include <avr/interrupt.h>		/* includes stuff for the interrupts*/	

typedef enum {open=0,waiting=1,close=2}State;//creates a type for states
volatile State current_state;
volatile State previous_state;

void interrupt_init(){
	EIMSK |= (1<<INT0);					//initilize interrupt 0
	EICRA |= (1<<ISC01) | (0<<ISC00);	//set type of interupt for INT0
	sei();								//enables all interrupts
}

ISR(INT0_vect){
	if(previous_state==close){//checks previous state and set the next state based off it
		current_state=open;
	}
	else if (previous_state==open){
		current_state=close;
	}	
}

int main(void)
{
	current_state=waiting;
	previous_state=close;
	interrupt_init();
	DDRD=0<<PD2;							//sets DDRD at PD2 to input
	DDRB=0b11111111;						//enables ports for the Leds
	DDRC=0b11111111;						//enables ports for the DC motor

    while(1)
    {
		switch(current_state)
		{
			case waiting:				//waits
				break;
			case open: 
				PORTB=0b11111111;		//turn on leds
				PORTC=0b00000001;		//start motor clockwise
				_delay_ms(100);
				PORTC=0x00;				//Turn off motor
				_delay_ms(50);      
				previous_state=open;	//sets states for the next change
				current_state=waiting;
				break;
			case close: 
				PORTB=0b00000000;		//turn off Leds
				PORTC=0b00000010;  		//motor move counterclockwise
				_delay_ms(100);
				PORTC=0x00;				//turns off motor
				_delay_ms(50);     
				previous_state=close;	//sets states for the next change
				current_state=waiting;
				break;
		}
	}
	return 0;
}