/* 
 * File:   Main.c
 * Author: sfd5205
 *
 * Created on April 17, 2019, 2:34 PM
 */
#define FCY 3685000UL

#include <p33EP64MC502.h>
#include <libpic30.h>
#include <xc.h>

#pragma config ICS = PGD2 //

void init_DIO(void);
void init_INT(void);
void __attribute__((__interrupt__,auto_psv)) _INT1Interrupt (void);

typedef enum {
    S0, S1, S2, S3
} STATES;

STATES state = S2;

void init_DIO(void){
    ANSELAbits.ANSA0 = 0;   //initialize port A as Digital input/output
    ANSELAbits.ANSA1 = 0;   //initialize port A as Digital input/output
    ANSELAbits.ANSA4 = 0;   //initialize port A as Digital input/output
    
    TRISAbits.TRISA0 = 1;   //configure pin RA0 as input
    TRISAbits.TRISA1 = 1;   //configure pin RA1 as input
    TRISAbits.TRISA2 = 1;   //configure pin RA2 as input
    
    TRISBbits.TRISB13 = 0;  //configure pin RB13 as output
    TRISBbits.TRISB14 = 0;  //configure pin RB14 as output
    TRISBbits.TRISB15 = 0;  //configure pin RB15 as output
}

void init_INT (void){
    RPINR0bits.INT1R    = 0x21; //set interrupt to pin 4 (RPI32)
    INTCON2bits.GIE     = 1;    //enable global interrupts
    INTCON2bits.INT1EP  = 1;    //Interrupt on positive edge
    IFS1bits.INT1IF     = 0;    //clear interrupt flag
    IEC1bits.INT1IE     = 1;    //enable interrupt
}

void __attribute__((__interrupt__,auto_psv)) _INT1Interrupt (void){
    LATBbits.LATB13  =  0; // set output to 0
    LATBbits.LATB14  =  0; // set output to 0
    LATBbits.LATB15  =  0; // set output to 0
    state            = S0; // set state to S0
    IFS1bits.INT1IF  =  0; // clear interrupt flag
}

int main(void) {
    
    int A, B;
    
    init_DIO();
    init_INT();
    
    while(1){
        
        A = PORTAbits.RA0 ^ 1;
        B = PORTAbits.RA1 ^ 1;
        
        switch (state){
            case S0:
                switch(A){
                    case 0: switch(B){
                            case 0: state = S0; break;
                            case 1: state = S1; break;
                        }; break;
                    case 1:switch(B){
                            case 0: state = S1; break;
                            case 1: state = S1; break;
                        }; break;
                        }; break;
            case S1:
                switch(A){
                    case 0: 
                        switch(B){
                            case 0: state = S2; break;
                            case 1: state = S3; break;
                        }; break;
                    case 1: switch(B){
                            case 0: state = S2; break;
                            case 1: state = S1; break;
                        }; break;
                        }; break;
            case S2:
                switch(A){
                    case 0: 
                        switch(B){
                            case 0: state = S3; break;
                            case 1: state = S1; break;}; break;
                    case 1: switch(B){
                            case 0: state = S3; break;
                            case 1: state = S1; break;
                        }; break;
                };break;
            case S3:
                switch(A){
                    case 0: 
                        switch(B){
                            case 0: state = S1; break;
                            case 1: state = S2; break;
                        }; break;
                    case 1: switch(B){
                            case 0: state = S1; break;
                            case 1: state = S1; break;
                        }; break;
                }; break;
        }
        
        switch (state) {
            case S0: 
                LATBbits.LATB13 = 0;
                LATBbits.LATB14 = 0;
                LATBbits.LATB15 = 0; break;
            case S1: 
                LATBbits.LATB13 = 1;
                LATBbits.LATB14 = 1;
                LATBbits.LATB15 = 1; break;
            case S2: 
                LATBbits.LATB13 = 1;
                LATBbits.LATB14 = 0;
                LATBbits.LATB15 = 1; break;
            case S3: 
                LATBbits.LATB13 = 0;
                LATBbits.LATB14 = 1;
                LATBbits.LATB15 = 0; break;
            }
        __delay_ms(500);
        
        ClrWdt();
    }
        //return 0;

}

