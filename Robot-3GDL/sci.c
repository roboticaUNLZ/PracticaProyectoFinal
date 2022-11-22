#include <avr/io.h>
#include <avr/interrupt.h>
#include "sci.h"
#include "parser.h"
#include "queue.h"
#include "timer1.h"



void USART_Init(unsigned int ubrr)
{
    /*Set baud rate */
    UBRR0H = (unsigned char)(ubrr>>8);
    UBRR0L = (unsigned char)ubrr;
    /*Enable receiver and transmitter */
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);
    /* Set frame format: 8data, 2stop bit */
    UCSR0C = (1<<USBS0)|(3<<UCSZ00);
    UCSR0B |= (1<<RXCIE0);//habilito interrupción por recepción
}
void USART_Transmit(unsigned char data)
{
    /* Wait for empty transmit buffer */
    while (!(UCSR0A & (1<<UDRE0)));
    /* Put data into buffer, sends the data */
    UDR0 = data;
}

unsigned char USART_Receive(void)
{
    /* Wait for data to be received */
    while (!(UCSR0A & (1<<RXC0)));
    /* Get and return received data from buffer */
    return UDR0;
}

/*char * USART_Receive_txt(void){
    
    
    
}*/

void USART_Transmit_txt(char* cadena){			//cadena de caracteres de tipo char
	while(*cadena !=0x00){				//mientras el último valor de la cadena sea diferente a el caracter nulo
		USART_Transmit(*cadena);			//transmite los caracteres de cadena
		cadena++;						//incrementa la ubicación de los caracteres en cadena
										//para enviar el siguiente caracter de cadena
	}
}

ISR(USART_RX_vect){
    push_item( USART_READ );
}
