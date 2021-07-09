/* 
 * File:   Exercise 1.c
 * Author: SCHIANO
 *
 * Created on April 24, 2016, 2:00 PM
 */

/* define FCY before including libpic30.h */
#define FCY 3685000UL

#include <p33EP64MC502.h>
#include <libpic30.h>
#include <math.h>
#include <stdio.h>
#include <xc.h>
#include "EE200_LCD.h"

/* set Configuration Bits */
#pragma config ICS = PGD2    // communicate on PGED2 (pin 14) and PGEC2 (pin 15)

/* declare functions */
void Init_ADC(void);

void Init_ADC (void) {
    ANSELAbits.ANSA0 = 1;  // set pin 2 (ANO) for analog input
    TRISAbits.TRISA0 = 1;  // configure for input (disable DO)
    AD1CON1bits.ADON = 1;  // turn ADC module on
  }

int main(void) {
    char Line_char_Array[16];
    unsigned int TS;
    double V_PV, TC, TF_PV;

    /* Set Parameters */
    TS = 500; // sample period in ms

    Init_LCD_Module();
    Init_ADC ( );

    while (1) {
       __delay_ms(TS);             // set sample rate to 500 ms

       /* Capture analog input from LM19 on pin 2 (ANO) */
       AD1CON1bits.SAMP = 1;       // start Sample; place inp signal across cap
       __delay_us(10);             // wait for cap voltage to track analog inp
       AD1CON1bits.SAMP = 0;       // start SA converter
       while (!AD1CON1bits.DONE);  // wait for SA conv to complete
       V_PV;                                    // convert to units of Volts

       /* Determine temp in units of degrees F */
       TC;
       TF_PV;
       
       /* Update LCD */
       Position_LCD_Cursor(0x00);  // place cursor at cell 0x00
       sprintf(Line_char_Array,"V PV [V] = %4.3f", V_PV);
       Write_LCD_String(Line_char_Array);

       Position_LCD_Cursor(0x40);  // place cursor at cell 0x40
       sprintf(Line_char_Array,"T PV [F] = %4.1f",TF_PV);
       Write_LCD_String(Line_char_Array);

       ClrWdt(); // restart the watchdog timer
    }
    return 0;
}
