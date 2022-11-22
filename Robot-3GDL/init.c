#include "init.h"
#include "queue.h"
#include "timer1.h"
#include "parser.h"
#include "sci.h"
#include "Motor.h"
#include <avr/io.h>
#include<avr/interrupt.h>
/*****************************************/
void init_system(void){
   
// Inicialización del sistema //
   cli();
//habilito el watchdog
                                                            
// Clock FEE //                                            
   
// Puertos de entrada/salida //
  	DDRB = 0xFF;
    PORTB= 0;
    DDRC = 0xFF;
    PORTC= 0;
    DDRD = 0b11110010;
    PORTD= 0;
    

//Inicialización de dispositivos y variables
	(void)init_queue();
	 
// Puerto Serial //
    USART_Init(BAUD);
	

// Interrupción Externa //
     
  
//Inicialización de dispositivos y variables que utilizan el timer
  
 	init_parser();
 	init_timer1(); 
    sei();
    push_item(HOME);
}



/*************************************************/