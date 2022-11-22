#define QSIZE 120		/* tamano de la cola */
#define NOROOM 0		/* no queda lugar */
#define QUEUEEMPTY 1    /* cola vacia */
#define NOERROR	2		/* no hubo error */

typedef struct 	qu{   
                 char items_left;
			  	 char *inptr;
			  	 char *outptr;
			  	 char incnt;
			  	 char outcnt;
			  	 char qerror;	} QUEUE;

/*	prototipos */

QUEUE * init_queue(void);
char push_item( char c );
char pop_item(void);