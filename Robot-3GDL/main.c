#define F_CPU 16000000
#include <avr/io.h>
//#include <util/delay.h>
#include "main.h"
#include "parser.h"
#include "init.h"
#include "timer1.h"
 
//extern unsigned int count_reset;
		 
int main(void){
//int c;
	
	init_system();   	
    exec();
}

//********************************************************************//
void
exec(void)
{
	char c=0;
	
  	for(;;)
  	{
        //count_reset = 0;
		c = get_excitation();
		parse( c );
	  }
}
