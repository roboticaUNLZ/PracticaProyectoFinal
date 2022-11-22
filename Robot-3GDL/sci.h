void USART_Transmit(unsigned char data);
unsigned char USART_Receive(void);
void USART_Transmit_txt(char *cadena);
//char * USART_Receive_txt(void);
//extern char rxdata(void );
//extern void txstr( const char *s );
void USART_Init(unsigned int ubrr);

#define BAUD 103
#define MYUBRR FOSC/16/BAUD-1