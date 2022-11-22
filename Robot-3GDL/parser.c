#include "queue.h"
#include "parser.h"
#include "timer1.h"
#include "main.h"
#include "sci.h"
#include "Motor.h"
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>



//variables del Parser//
unsigned char state;
extern QUEUE *qptr;

//variables generales// 

char RxBuffer[30];
unsigned char RxContador=0;
static float q1_act,q2_act,q3_act,q1_f,q2_f,q3_f,q1_rel,q2_rel,q3_rel;//coordenadas articulares
double q1_rad,q2_rad,q3_rad;//coord articulares en radianes
double px,py,pz;//coordenadas cartesiandas


///FUNCIONES///
void do_nothing(void);
void led(void);
void SerialRx(void);
void cinematica_directa(void);
void cinematica_inversa(void);
void imprimo_var(char *s,float val);

//Funciones de beep


// 	Tablas de los estados del automata

//	exc , next_st , func()
ST const st_idle[] = {  LED     			, IDLE 	   , led,
                        USART_READ			, IDLE     , SerialRx,
                        HOME                , IDLE_MOVE, home,
                        NOEXC				, DUMMY    , do_nothing	};

ST const st_idle_move[] = { LED     			, IDLE_MOVE , led,
                            MOTORA              , IDLE_MOVE	, motorA,	
                            MOTORB              , IDLE_MOVE	, motorB,
                            MOTORC              , IDLE_MOVE	, motorC,
                            NOEXC	  			, DUMMY	    , do_nothing	};				
						

//Tabla de punteros a tablas de estado//
const ST * const stats[] = 	

	{ 	st_idle , st_idle_move };


//Funciones y primitivas del parser//
void init_parser(void){//inicialización del parser.
    state = IDLE;
}

void parse( char c ){							//Ejecuta el próximo elemento de la cola. 
  const ST *pp;
  
	pp = stats[state];					//
	while( pp->exc != NOEXC )			//se mueve hasta encontrar el estado correcto.
	{
		if( pp->exc == c )
		{
			state = pp->next_st ;
			pp->func();					//lo encuentra y ejecuta la función.
			return;
		}
		++pp;
	}
}

//static
void do_nothing(void){
}

char get_excitation(void){					//mueve los estados de la máquina, busca una acción en la cola y la devuelve a la función parse. 
	char c;
	for(;;){ 
		c = pop_item(); 
		if( qptr->qerror == QUEUEEMPTY )
			c = push_item( NOEXC );
			//continue;
		return c;						//retorna al main la siguiente acción.
	}
}

void led(void){
    static unsigned char led;
    
    led = ~led;
    if(led) PORTB |= (1<<PB5);
    else PORTB &= ~(1<<PB5);
}

void SerialRx(void){
    unsigned char dato;
    char s[128];
    static char flag;
    float aux1,aux2,aux3;    
    dato=USART_Receive();
    if(dato==';' && flag==0){
        px=strtod(RxBuffer,NULL);
        
        for(int i=0;i<=RxContador;i++) RxBuffer[i]=0;
        RxContador=0;
        flag=1;
    } 
    else if(dato==';' && flag==1){
        py=strtod(RxBuffer,NULL);
        
        for(int i=0;i<=RxContador;i++) RxBuffer[i]=0;
        RxContador=0;
        flag=0;
    } 
    else if(dato=='*'){
        pz=strtod(RxBuffer,NULL);
        cinematica_inversa();
        //calculo variables articulares
        q1_rel=q1_f-q1_act;
        aux1=q1_rel/PASO;
        actualizo_cant_pasosA(aux1);//actualizo cant de pasos por motor
        q1_act = q1_act+(int)aux1*PASO;
        
        q2_rel=q2_f-q2_act;        
        aux2=q2_rel/PASO;
        actualizo_cant_pasosB(aux2);//actualizo cant de pasos por motor
        q2_act = q2_act+(int)aux2*PASO;
        
        q3_rel=q3_f-q3_act;
        aux3=q3_rel/PASO;
        actualizo_cant_pasosC(aux3);//actualizo cant de pasos por motor
        q3_act = q3_act+(int)aux3*PASO;
        //Borro buffer
        for(int i=0;i<=RxContador;i++) RxBuffer[i]=0;
        RxContador=0;
        //Imprimo Coordenadas articulares
        sprintf(s,"\n\rCoordenadas articulares calculadas ");
        USART_Transmit_txt(s);
        sprintf(s,"\n\rPasos Motor A %d - ",(int)aux1);
        USART_Transmit_txt(s);
        dtostrf(q1_act,3,2,s);
        USART_Transmit_txt(s);
        sprintf(s,"\n\rPasos Motor B %d - ",(int)aux2);
        USART_Transmit_txt(s);
        dtostrf(q2_act,3,2,s);
        USART_Transmit_txt(s);
        sprintf(s,"\n\rPasos Motor C %d - ",(int)aux3);
        USART_Transmit_txt(s);
        dtostrf(q3_act,3,2,s);
        USART_Transmit_txt(s);
        sprintf(s,"\n\r");
        USART_Transmit_txt(s);
        
        //obtengo tiempo de movimiento
        obtengo_tiempo_mov();
        //obtengo coor artic en radianes
        q1_rad=(q1_act*2*M_PI/360);
        q2_rad=(q2_act*2*M_PI/360);
        q3_rad=(q3_act*2*M_PI/360);
        cinematica_directa();
        state=IDLE_MOVE;
    }
    else{
       RxBuffer[RxContador++]= dato;
    }
    
    
}

void cinematica_directa(void){
    char s[20];
    double r;

    r=L2*cos(q2_rad)+L3*cos(q2_rad+q3_rad);
    
    px=r*cos(q1_rad);
    py=r*sin(q1_rad);
    pz=L1+L2*sin(q2_rad)+L3*sin(q2_rad+q3_rad);

    sprintf(s,"\n\rPX = ");
    USART_Transmit_txt(s);
    dtostrf(px,2,2,s);//convierte una variable double a string (var, cant de cifras parte entera, cant de cifras parte decimal, puntero a cadena)
    USART_Transmit_txt(s);

    sprintf(s,"\n\rPY = ");
    USART_Transmit_txt(s);
    dtostrf(py,2,2,s);//convierte una variable double a string (var, cant de cifras parte entera, cant de cifras parte decimal, puntero a cadena)
    USART_Transmit_txt(s); 
    
    sprintf(s,"\n\rPZ = ");
    USART_Transmit_txt(s);
    dtostrf(pz,2,2,s);//convierte una variable double a string (var, cant de cifras parte entera, cant de cifras parte decimal, puntero a cadena)
    USART_Transmit_txt(s);
    
    sprintf(s,"\n\r");
    USART_Transmit_txt(s);
}
void cinematica_inversa(void){
    double r,C3,alfa,beta;
    char s[16];
    
    r=sqrt(square(px)+square(py));
    q1_rad=atan(py/px);
    q1_f=(q1_rad/(2*M_PI))*360;
    C3=square(px)+square(py)+square(pz-L1)-square(L2)-square(L3);
    C3=C3/(2*L2*L3);
    /*sprintf(s,"\n\r C3 = ");
    imprimo_var(s,C3);*/
    q3_rad=1-square(C3);
    q3_rad=sqrt(q3_rad);
    q3_rad=q3_rad/C3;
    q3_rad=atan(q3_rad);
    /*sprintf(s,"\n\r q3rad = ");
    imprimo_var(s,q3_rad);*/
    q3_f=(q3_rad/(2*M_PI))*360;
    /*sprintf(s,"\n\r q3gr = ");
    imprimo_var(s,q3_f);*/
    beta=atan((pz-L1)/r);
    alfa=atan((L3*sin(q3_rad))/(L2+L3*cos(q3_rad)));
    q2_rad=beta-alfa;
     
    q2_f=(q2_rad/(2*M_PI))*360;
    
    
}
void imprimo_var(char *s,float val){
    USART_Transmit_txt(s);
    dtostrf(val,2,2,s);//convierte una variable double a string (var, cant de cifras parte entera, cant de cifras parte decimal, puntero a cadena)
    USART_Transmit_txt(s); 
}