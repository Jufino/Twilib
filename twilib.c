#define TWINT 7
#define TWEA 6
#define TWSTA 5
#define TWSTO 4
#define TWWC 3
#define TWEN 2
#define TWIE 0
#define ack 1
//master
void twi_initM(void)
{
 TWCR= 0x00;    //disable twi
 TWBR= 12;      //set bit rate
 TWSR= 0x00;    //set prescale
 TWAR= 0x00;    //set slave address
 TWCR= 0x04;    //enable twi
}
void twi_stop(void)
{
  TWCR = (1<<TWINT)|(1<<TWEN) | (1<<TWSTO);
}
void twi_reset(void)
{
  TWCR = (1<<TWINT)|(1<<TWEN) | (1<<TWSTO);
}
unsigned char twi_start(void)
{
 TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
 while (!(TWCR & (1<<TWINT)));
 if (((TWSR & 0xF8) != 0x08) && ((TWSR & 0xF8) != 0x10)){
//        printf("Start i2c error,%x\n",(TWSR & 0xF8));
        twi_reset();
        return 0;
 }
 else   return 1;
}
unsigned char twi_writeAdd(unsigned char data)
{
        TWDR = data;
        if(ack != 1){
                TWCR = (1<<TWINT)|(1<<TWEN|(1<<TWEA));
                while (!(TWCR & (1<<TWINT)));
                if (((TWSR & 0xF8) != 0x48)&&((TWSR & 0xF8) != 0x20)){
//                        printf("Address i2c write error,%x\n",(TWSR & 0xF8));
                        twi_reset();
                        return 0;                
                }
                else return 1;

        }       
        else{
                TWCR = (1<<TWINT)|(1<<TWEN);
                while (!(TWCR & (1<<TWINT)));
                if (((TWSR & 0xF8) != 0x40)&&((TWSR & 0xF8) != 0x18)){
//                        printf("Address i2c write error,%x\n",(TWSR & 0xF8));
                        twi_reset();
                        return 0;
                }
                else return 1;
        }          
}
unsigned char twi_writeData(unsigned char data){
        TWDR = data;
        if(ack != 1){
                TWCR = (1<<TWINT)|(1<<TWEN|(1<<TWEA));
                while (!(TWCR & (1<<TWINT)));
                if ((TWSR & 0xF8) != 0x30){
//                        printf("Data i2c write error,%x\n",(TWSR & 0xF8));
                        twi_reset();
                        return 0;
                }
                else return 1;
        }        
        else{            
                TWCR = (1<<TWINT)|(1<<TWEN);
                while (!(TWCR & (1<<TWINT)));
                if ((TWSR & 0xF8) != 0x28){
//                        printf("Data i2c write error,%x\n",(TWSR & 0xF8));
                        twi_reset();
                        return 0;
                }
                else return 1;
        } 
}
unsigned char twi_readData(unsigned char acknak)
{
 if(acknak == 1)
 {
  TWCR = 0xC4;
  while (!(TWCR & (1<<TWINT)));
  if ((TWSR & 0xF8) != 0x50){
//        printf("Data i2c read error,%x\n",(TWSR & 0xF8));
        twi_reset();
        return 0;
  } 
 }        
 else
 {
  TWCR = 0x84;
  while (!(TWCR & (1<<TWINT)));
  if ((TWSR & 0xF8) != 0x58){
//        printf("Data i2c read error,%x\n",(TWSR & 0xF8));
        twi_reset();
        return 0;
  }        
 }        
 return(TWDR);
} 
//slave
void twi_initS(void)
{
 TWAR= 0x02; //set slave address
 TWCR= (1<<TWEN)|(1<<TWEA);
}
void twi_function(void){
        switch((TWSR & 0xF8)){
        //prijem 
                case 0x60:
                case 0x68:
                case 0x70:
                case 0x78:
                case 0xA0:      if(ACK == 1)    TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWEA);//send ACK 
                                else            TWCR=(1<<TWINT)|(1<<TWEN)|(0<<TWEA);//send NACK  
                break;
                case 0x90:
                case 0x88:
                case 0x98:
                case 0x80:      printf("%d",TWDR);//receive data                   
                                if(ACK == 1)    TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWEA);//send ACK 
                                else            TWCR=(1<<TWINT)|(1<<TWEN)|(0<<TWEA);//send NACK  
                break;//data receive    
        //-----------------------
        //odosielanie Slave to Master
                case 0xA8:
                        TWDR = 30;       //send first data 
                        if(ACK == 1)    TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWEA);//send ACK 
                        else            TWCR=(1<<TWINT)|(1<<TWEN)|(0<<TWEA);//send NACK 
                break;
                case 0xB8:
                        TWDR = 20;       //send next data
                        if(ACK == 1)    TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWEA);//send ACK 
                        else            TWCR=(1<<TWINT)|(1<<TWEN)|(0<<TWEA);//send NACK 
                break;
                case 0xC0:
                case 0xC8:
                        if(ACK == 1)    TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWEA);//send ACK 
                        else            TWCR=(1<<TWINT)|(1<<TWEN)|(0<<TWEA);//send NACK  
                break;
                case 0x00:TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWSTO)|(1<<TWEA);//reset i2c
        }
}