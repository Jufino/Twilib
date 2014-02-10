#define TWINT 7
#define TWEA 6
#define TWSTA 5
#define TWSTO 4
#define TWWC 3
#define TWEN 2
#define TWIE 0
#define ack 1
//master
void twi_initM(void);
void twi_stop(void);
char twi_start(void);
unsigned char twi_writeAdd(unsigned char data);
unsigned char twi_writeData(unsigned char data);
unsigned char twi_readData(unsigned char acknak);
//slave
void twi_initS(void);
void twi_function(void);