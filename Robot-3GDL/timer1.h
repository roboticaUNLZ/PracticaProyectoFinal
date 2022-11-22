void init_timer1(void);

//void timer1(void);

void set_beeptim_seconds(unsigned char secs);

unsigned int get_temp_data(void);
unsigned int get_tamb_data(void);

void delay_ms(int ms);
void delay( char t);
void set_timer_seconds(unsigned int secs);
void adconvert(void);
void do_control(void);
void do_logueo(void);
void go_idle(void);
//void stop_run(void);
void init_delta_pt100(unsigned int cor);
void set_time_alarm(void);

#define	MILISEC		1000
#define TIMEOUT		0


enum { BEEPOFF , BEEPON };









