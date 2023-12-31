#define F_CPU 20000000UL
 
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


// PortA pin 0 = horizontal visible region
//       pin 1 = Vsync pulse 
//       pin 2 = vertical visible region
//       pin 3 = Vsync pulse 

int line=0;
char fbuffer[600][1];
int i,j,x;
//unsigned int v=0;
//int x,y=0;
//int i=0;

ISR(TIMER1_COMPA_vect){
    PORTA = 1<<0;                  //start sending pixel data
    line++;
}

ISR(TIMER1_COMPB_vect){
    PORTA = 1<<7;
}

int main(void){
    DDRA=0xff;
    DDRC=0xff;
    DDRD=0xff;
    PORTA=0x00;

    sei();
    TIMSK|= 1<<OCIE1A | 1<<OCIE1B;
    TCCR1A|= 0<<WGM10 | 0<<WGM11;
    TCCR1B|= 1<<WGM12 | 0<<WGM13 | 1<<CS10 | 0<<CS11 | 0<<CS12;   //10Mhz with no prescaler;
 
    OCR1A = 528;
    OCR1B = 420;

    TCNT1 = 0;  		/* Load TCNT1 */
    
    PORTA = 1<<1;
    
    for(i=0; i<600; i++){
        for(j=0; j<1; j++){
            fbuffer[i][j]=(0);
            _delay_ms(1);
        }
    }

    //fbuffer[300][0]=1;
    //fbuffer[150][0]=1;
    
    PORTA = 0<<1;
    
    

    while(1){

        switch(TCNT1){
            
            case 400:
                PORTA = 0<<0; 
                break;
            
            case 484:
                PORTA = 0<<7; 
                break;
        }

        switch(line){    
            case 600:
                PORTC = 0<<1;          //last line 
                break;
            
            case 601:
                PORTC = 1<<7;          //Vsync pulse
                break;
            
            case 605:
                PORTC = 0<<7;
                break;
            
            case 628:
                line=0;                //end of frame
                PORTC = 1<<1;          //first line 
                break;
        }

        i = TCNT1;
        j = line;

        if((i<30) & (j<40)){
            PORTD = (fbuffer[j][i]<<7);
            //PORTD = 1<<7;
        }else{
            PORTD = 0<<7;
        }
      
    }
}