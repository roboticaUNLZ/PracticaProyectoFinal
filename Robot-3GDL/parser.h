extern void init_parser(void);
extern void parse( char c );
extern char get_excitation(void);


typedef struct pr {	
                      char exc;
                      char next_st;
                      void(* func)(void);		
} ST;


/* definicion de nombres de estados */
						
enum 	{ IDLE , IDLE_MOVE };




#define DUMMY 0


/* 	
*	Categorizacion de las excitaciones:
*	Las excitaciones son numeros de 16 bits.
*	Segun la categoria, tienen un campo de dato
*	y un campo de categoria. En el momento de generar
*	una excitacíon, se empaquetan (suma) el valor del
*	dato y la categoria para componer un int
*	A continuacion se definen las categorias
*/

#define NOEXC		 0x00	/* excitacion nula */
#define	TIEMPO	     0x40	/* de temporizadores del sistema */
#define CNTL		 0x50
#define ADCONV	  	 0x60
#define LED          0x61
#define USART_READ   0x62
#define BEEPER		 0x63
#define LOGUEO		 0x64
#define MOTORA       0x65
#define MOTORB       0x66
#define MOTORC       0x67
#define FIN_PROG     0x68
#define REFRESH      0x69
#define HOME         0x70



//Excitaciones de tiempo

#define	TIMO	TIEMPO
