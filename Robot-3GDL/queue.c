/***********************************************
	funciones de manejo de una cola FIFO de int
************************************************/

#include "queue.h"


/********************************
	crear e inicializar la cola
*********************************/
static char qmem[QSIZE];	    // espacio para la cola
static QUEUE qinfo;       // definicion del bloque de control
QUEUE *const qptr=&qinfo;	// puntero al bloque de control


QUEUE *
init_queue(void)
{
	unsigned char i;
	//qptr = &qinfo;

	qptr->inptr = qmem ;		// puntero de entrada nuevo item
	qptr->outptr = qmem ;		// puntero de salida item en cola
	qptr->items_left = 0 ;	// elementos en cola
	qptr->incnt = 0; 			  // offset del puntero de entrada
	qptr->outcnt = 0;			  // offset del puntero de salida
	qptr->qerror = NOERROR;
	for(i=0;i<QSIZE;i++) qmem[i]=0;
	return qptr;

}

char
push_item( char c )							//agrega un elemento a la cola.
{
	//qptr = &qinfo;
	if( (qptr->items_left > 0 && ( qptr->incnt == qptr->outcnt)) || qptr->items_left >= QSIZE)
	{
		qptr->qerror = NOROOM;				//cola llena
		return qptr->items_left;
	}

	*(qptr->inptr+(qptr->incnt)) = c;		//carga el elemento en la cola.
	++qptr->items_left;

	if( qptr->incnt == QSIZE-1 )			//mueve el puntero.
		qptr->incnt = 0;
	else
		++qptr->incnt;

	qptr->qerror = NOERROR ;
	return qptr->items_left;				//retorna la cantidad de items en la cola.
}


char
pop_item(void)								//saca un elemento de la cola
{													
	char c;

	//qptr = &qinfo;
	if( qptr->items_left > 0 )				//si la cola no está vacía
	{
		c = *(qptr->outptr+(qptr->outcnt));	//se posiciona en el principio de la cola
		--qptr->items_left;					//decrementa items en la cola 

		if( qptr->outcnt == QSIZE-1 )		//mueve el puntero 
			qptr->outcnt = 0  ;
		else
			++qptr->outcnt;

		qptr->qerror = NOERROR ;
		return c;							//retorna la dirección del puntero.
	}
	else									//cola vacía
	{
		qptr->qerror = QUEUEEMPTY;
		return 0;
	}
}