#include "timer1.h"
#include "parser.h"
#include "Motor.h"
#include "sci.h"
#include <stdio.h>
#include <avr/io.h>

const unsigned char b[]={0x05,0x04,0x06,0x02,0x0A,0x08,0x09,0x01};
static volatile int aA=0,aB=0,aC=0;
volatile int cant_pasosA,cant_pasosB,cant_pasosC;
static volatile int cant_pasosA_aux,cant_pasosB_aux,cant_pasosC_aux;
volatile unsigned int tpasoA=TPASO,tpasoB=TPASO,tpasoC=TPASO;
static volatile unsigned int tiempo_mov;
//float q1_act, q2_act,q1_f,q2_f,q1_rel,q2_rel;//coordenadas articulares
extern unsigned char state;
char s[128];
static volatile char flag_camb_sign_MA=0,flag_camb_sign_MB=0,flag_camb_sign_MC=0;

void home(void){

    cant_pasosA = -1;
    cant_pasosB = -1;
    cant_pasosC = -1;
    //state=IDLE_MOVE;
/*
   if(cant_pasosA > cant_pasosB){
        tiempo_mov = TPASO * cant_pasosA;
        tpasoA = TPASO;
        tpasoB = tiempo_mov/cant_pasosB;
    }
    else {
        tiempo_mov = TPASO * cant_pasosB;
        tpasoB = TPASO;
        tpasoA = tiempo_mov/cant_pasosA;
    }
    tpasoC = TPASO;*/
}

void motorA(void){
    if(cant_pasosA == 0) {
        //state = IDLE;
        return;
    }
    if(cant_pasosA>0) {
        aA++;
        if(aA>=PASOSxSEC){
            aA=0;
        }
        PORTC=b[aA];
        cant_pasosA--;
        
    }
    else {
        aA--;
        if(aA<0){
            aA=PASOSxSEC-1;
        }
        PORTC=b[aA];
        cant_pasosA++; 
    }
    /*sprintf(s,"\n\rPasos Motor A: %d",cant_pasosA);
    USART_Transmit_txt(s);*/
}
void motorB(void){
    if(cant_pasosB == 0) {
        return;
    }
    if(cant_pasosB>0){
        aB++;
        if(aB>=PASOSxSEC){
            aB=0;
        }
        PORTB = (PORTB & 0xF0) | b[aB];
        cant_pasosB--;
    }
    else {
        aB--;
        if(aB<0){
            aB=(PASOSxSEC-1);
        }
        PORTB = (PORTB & 0xF0) | b[aB];
        cant_pasosB++;
    }
    /*sprintf(s,"\n\rPasos Motor B: %d,%d",cant_pasosB,aB);
    USART_Transmit_txt(s);*/
}
void motorC(void){
    if(cant_pasosC == 0) {
        return;
    }
    if(cant_pasosC>0){
        aC++;
        if(aC>=PASOSxSEC){
            aC=0;
        }
        PORTD = (PORTD & 0x0F) | (b[aC]<<4);
        cant_pasosC--;
        
    }
    else {
        aC--;
        if(aC<0){
            aC=PASOSxSEC-1;
        }
        PORTD = (PORTD & 0x0F) | (b[aC]<<4);
        cant_pasosC++;
    }
    /*sprintf(s,"\n\rPasos Motor C: %d",cant_pasosC);
    USART_Transmit_txt(s);*/
}

void obtengo_tiempo_mov(void){
    
   //Primero verificamos signos de las variables cant_pasosX y sacamos el módulo
    if(cant_pasosA > 0 ){
        cant_pasosA_aux=cant_pasosA;
    }
    else{
        cant_pasosA_aux=cant_pasosA*(-1);
    }
    if(cant_pasosB > 0 ){
        cant_pasosB_aux=cant_pasosB;
    }
    else{
        cant_pasosB_aux=cant_pasosB*(-1);
    }
    if(cant_pasosC > 0 ){
        cant_pasosC_aux=cant_pasosC;
    }
    else{
        cant_pasosC_aux=cant_pasosC*(-1);
    }
    //calculamos el tiempo de movimiento
    if(cant_pasosA_aux >= cant_pasosB_aux && cant_pasosA_aux >= cant_pasosC_aux){//cant_pasosA es mayor
        tiempo_mov = TPASO * cant_pasosA_aux;
        tpasoA = TPASO;
        tpasoB = tiempo_mov/cant_pasosB_aux;
        tpasoC = tiempo_mov/cant_pasosC_aux;
    }
    else{
        if(cant_pasosB_aux >= cant_pasosA_aux && cant_pasosB_aux >= cant_pasosC_aux){//cant_pasosB es mayor
            tiempo_mov = TPASO * cant_pasosB_aux;
            tpasoB = TPASO;
            tpasoA = tiempo_mov/cant_pasosA_aux;
            tpasoC = tiempo_mov/cant_pasosC_aux;
        }
        else{//cant_pasosC es mayor o igual por descarte
            tiempo_mov = TPASO * cant_pasosC_aux;
            tpasoC = TPASO;
            tpasoA = tiempo_mov/cant_pasosA_aux;
            tpasoB = tiempo_mov/cant_pasosB_aux;
        }
    }
}

void actualizo_cant_pasosA(float var){
    cant_pasosA=(int)var;
}
void actualizo_cant_pasosB(float var){
    cant_pasosB=(int)var;
}
void actualizo_cant_pasosC(float var){
    cant_pasosC=(int)var;
}