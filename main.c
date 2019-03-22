/*  	BLDC controller using the ATtiny24a microcontroller
	The motor's back-EMF is used to sense the rotor position and trigger the commutation
	A three-resistor star-network generates a virtual star-point for the motor, to measure the voltage across each phase using the analog 		 	 comparator of the ATtiny24A.
	
	This very simple approach works, but doesn't allow autonomous starting of the motor (i.e. you have to spin up the motor by hand for it tos 		start. This is because when the motor is at rest, there is no voltage inducted in the phases, thus we can't deduce the rotor position to trigger 	the commutation.
	
	by Marcel Meyer Garcia, see LICENCE.txt
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay_basic.h>

// 2000 t eq. 1ms
uint16_t t = 1000;
volatile uint8_t step = 1;
uint8_t last_step = 0;


// analog comparator interrupt handler
ISR(ANA_COMP_vect)
{
// display the state of the comparator output on pin PA4 for debugging purposes
//    if( ACSR & (1<<ACO) ){
//        PORTA |= (1<<PA4);
////        _delay_loop_1(255);
////        PORTA &=~(1<<PA4);
//    }
//    else{
//        PORTA &=~(1<<PA4);
//    }
   PORTA ^= (1<<PA4);
   if( step < 6 )
   {
      ++step;
   }
   else
   {
      step = 1;
   }
}


int main(void)
{
   // default state on startup
   PORTA = 0;
   PORTB = 0;
   // set pin PA4 as output for debugging purposes
   DDRA |= (1<<DDA4);
   // set output pins to control the triple H-bridge MOSFETs
   DDRB |= (1<<DDB0)|(1<<DDB1)|(1<<DDB2);
   DDRA |= (1<<DDA5)|(1<<DDA6)|(1<<DDA7);
   // set input pins for measuring the Back-EMF from the motor
   DDRA &=~ ((1<<DDA0)|(1<<DDA1)|(1<<DDA2)|(1<<DDA3));
   // no input pullups
   PORTA &=~ ((1<<PA0)|(1<<PA1)|(1<<PA2)|(1<<PA3));
   // turn ADC off (is off at startup anyway, but...)
   ADCSRA &=~ (1<<ADEN);
   // enable analog comparator multiplexer
   ADCSRB |= (1<<ACME);
   // enable the analog comparator
   ACSR &=~(1<<ACD);
   // connect external input AIN0 to the positive input of the comparator
   ACSR &=~(1<<ACBG);
   // enable analog comparator interrupt
   ACSR |= (1<<ACIE);
   // interrupts occurs on toggle (i.e. on rising AND falling edge)
   ACSR &=~ ((1<<ACIS1)|(1<<ACIS0));
   // globally enable interrupts
   sei();
   // select PA0/U as comparator negative input
   ADMUX = 0;
   // select PA2/V as comparator negative input
   ADMUX = 2;
   // select PA3/W as comparator negative input
   ADMUX = 3;

//   for(uint8_t i = 0; i<50; ++i)
//   {
//      _delay_loop_2(t);
//      //step 1
//      ADMUX = 3;
//      PORTA &= 0b00011111;
//      PORTB &= 0b11111000;
//      PORTA |= 0b01000000;
//      PORTB |= 0b00000011;
//      _delay_loop_2(t);
//      //step 2
//      ADMUX = 0;
//      PORTA &= 0b00011111;
//      PORTB &= 0b11111000;
//      PORTA |= 0b01100000;
//      PORTB |= 0b00000001;
//      _delay_loop_2(t);
//      //step 3
//      ADMUX = 2;
//      PORTA &= 0b00011111;
//      PORTB &= 0b11111000;
//      PORTA |= 0b01100000;
//      PORTB |= 0b00000100;
//      _delay_loop_2(t);
//      //step 4
//      ADMUX = 3;
//      PORTA &= 0b00011111;
//      PORTB &= 0b11111000;
//      PORTA |= 0b11000000;
//      PORTB |= 0b00000100;
//      _delay_loop_2(t);
//      //step 5
//      ADMUX = 0;
//      PORTA &= 0b00011111;
//      PORTB &= 0b11111000;
//      PORTA |= 0b10000000;
//      PORTB |= 0b00000101;
//      _delay_loop_2(t);
//      //step 6
//      ADMUX = 2;
//      PORTA &= 0b00011111;
//      PORTB &= 0b11111000;
//      PORTA |= 0b00000000;
//      PORTB |= 0b00000111;
//   }

   while(1)
   {
      if(last_step != step)
      {
         switch(step)
         {
         case 1:
            //step 1
            ADMUX = 3;
            PORTA &= 0b00011111;
            PORTB &= 0b11111000;
            PORTA |= 0b01000000;
            PORTB |= 0b00000011;
            break;
         case 2:
            //step 2
            ADMUX = 0;
            PORTA &= 0b00011111;
            PORTB &= 0b11111000;
            PORTA |= 0b01100000;
            PORTB |= 0b00000001;
            break;
         case 3:
            //step 3
            ADMUX = 2;
            PORTA &= 0b00011111;
            PORTB &= 0b11111000;
            PORTA |= 0b01100000;
            PORTB |= 0b00000100;
            break;
         case 4:
            //step 4
            ADMUX = 3;
            PORTA &= 0b00011111;
            PORTB &= 0b11111000;
            PORTA |= 0b11000000;
            PORTB |= 0b00000100;
            break;
         case 5:
            //step 5
            ADMUX = 0;
            PORTA &= 0b00011111;
            PORTB &= 0b11111000;
            PORTA |= 0b10000000;
            PORTB |= 0b00000101;
            break;
         case 6:
            //step 6
            ADMUX = 2;
            PORTA &= 0b00011111;
            PORTB &= 0b11111000;
            PORTA |= 0b00000000;
            PORTB |= 0b00000111;
            break;
         default:
            break;
         }
         last_step = step;

      }
   }
   return 0;
}
