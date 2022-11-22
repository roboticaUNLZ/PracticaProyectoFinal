#include "timer1.h"
#include "queue.h"
#include "parser.h"
#include "Motor.h"
#include <avr/interrupt.h>

static volatile unsigned int timer_motorA, timer_motorB,timer_motorC;
static volatile unsigned int timer, beeptim = 0 ,seconds ;
static volatile int milisec;
extern int cant_pasosA,cant_pasosB,cant_pasosC;
extern unsigned int tpasoA,tpasoB,tpasoC;
extern unsigned char state;

void init_timer1(void){
  timer = MILISEC;
  timer_motorA = tpasoA;
  timer_motorB = tpasoB;
  timer_motorC = tpasoC;
  TCCR0B = (1<<CS01)|(1<<CS00);
  TCNT0 = 6;
  TIMSK0 |= (1<<TOIE0);
  
  
}

/*
	Interrupcion de 1000 Hz - 1ms por tick
	Atiende el teclado, el display y servicios como beep 
*/
ISR(TIMER0_OVF_vect){ 
//
 	TCNT0 = 6;//recargo timer
 	if( milisec )
 		--milisec;

 	
    		
	if(timer_motorA!=0)
        --timer_motorA;
    else{/* timer cada tpasoA milisegundos */ 
        timer_motorA = tpasoA;
        if((cant_pasosA!=0) && (state == IDLE_MOVE)) push_item( MOTORA );
        if(cant_pasosA == 0 && cant_pasosB==0 && cant_pasosC==0) go_idle();
    }
    if(timer_motorB!=0)
        --timer_motorB;
    else{/* timer cada tpasoB milisegundos */ 
        timer_motorB = tpasoB;
        if((cant_pasosB!=0) && (state == IDLE_MOVE)) push_item( MOTORB );
        if(cant_pasosA == 0 && cant_pasosB==0 && cant_pasosC==0) go_idle();
    }
    if(timer_motorC!=0)
        --timer_motorC;
    else{/* timer cada tpasoC milisegundos */ 
        timer_motorC = tpasoC;
        if((cant_pasosC!=0) && (state == IDLE_MOVE)) push_item( MOTORC );
        if(cant_pasosA == 0 && cant_pasosB==0 && cant_pasosC==0) go_idle();
    }
    
	if( timer )
 		--timer;
 	else
 	{	//cada 1 segundo
 		timer = MILISEC;
 		push_item( LED );  	//exitacion parpadeo de LED
        
        if(seconds)
            --seconds;
 	}
}

 
      	//EnableInterrupts;  //Habilito interrupciones

 
/////////////////////////////////////Fin timer 0/////////////////////////////////////////
void set_beeptim_seconds(unsigned char secs){
	beeptim = secs;
}

void set_timer_seconds(unsigned int secs){
	seconds = secs;
}

void delay( char t){
	seconds = t;
	while(seconds){
		//wait();
    }
}

void delay_ms(int ms){
	while( milisec ){
		//wait();
	
	milisec = ms;
	}
	while( milisec ){
		//wait();	
    }
}

void go_idle(){
    cant_pasosA=0;
    cant_pasosB=0;
    cant_pasosC=0;
    state = IDLE;
    
}
void actualizo_tpasoABC(void){
    
    
}