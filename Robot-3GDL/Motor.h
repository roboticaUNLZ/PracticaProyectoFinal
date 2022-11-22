/* 
 * File:   Motor.h
 * Author: Mauro
 *
 * Created on 6 de noviembre de 2022, 23:17
 */

#ifndef MOTOR_H
#define	MOTOR_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* MOTOR_H */

void home(void);
void motorA(void);
void motorB(void);
void motorC(void);
void obtengo_tiempo_mov(void);
void actualizo_cant_pasosA(float var);
void actualizo_cant_pasosB(float var);
void actualizo_cant_pasosC(float var);

#define PASOSxSEC 8 //PASOS POR SECUENCIA
#define TPASO     25
#define PASO      0.9

//Parámetros mecánicos
#define L1  1
#define L2  1
#define L3  1