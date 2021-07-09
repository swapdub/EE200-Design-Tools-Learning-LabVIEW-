/* 
 * File:   EE200_LCD.c
 * Author: SCHIANO
 *
 * Created on March 25, 2016, 6:18 AM
 */

/* define FCY before including libpic30.h */
#define FCY 3685000UL

#include <p33EP64MC502.h>
#include <libpic30.h>
#include <string.h>

/* define constants for programming the LCD module */
#define pgm_delay_ms 2
#define INSTRUCTION 0
#define DATA 1

/* declare local functions */
void Init_LCD_DIO_Port(void);
void Toggle_Enable_line(void);
void Write_LCD_Nibble(int data, int cmd);
void Write_LCD_Byte(int data, int cmd);

/* declare global functions */
void Init_LCD_Module(void);
void Position_Cursor(int cell_num);
void Write_LCD_String(char char_Array[16]);

void Init_LCD_DIO_Port (void) {
    ANSELB = 0;     // use peripheral pins associted with PORTB for digital I/O
    TRISB = 0xC0FF; // set RB8 to RB13 for output, the rest for input
    PORTB =  0;     // set all LCD inputs low
  }

void Toggle_Enable_line (void) {
    __delay_ms(pgm_delay_ms);   // delay
    PORTBbits.RB13 = 1;         // set E high
    __delay_ms(pgm_delay_ms);   // delay
    PORTBbits.RB13 = 0;         // set E low
    __delay_ms(pgm_delay_ms);   // delay
}

void Write_LCD_Nibble(int data, int cmd) {
 PORTB =  data << 8;     // place nibble at LCD inputs DB4 through DB7
 PORTBbits.RB12 = cmd;   // set RS; cmd = 0 for instruction, 1 for data
 Toggle_Enable_line ();  // strobe data into LCD module
 PORTB =  0;             // set all LCD inputs low
}

void Write_LCD_Byte(int data, int cmd) {
    Write_LCD_Nibble((data & 0x00F0) >> 4, cmd); // write upper nibble
    Write_LCD_Nibble( data & 0x000F, cmd);       // write lower nibble
}

void Init_LCD_Module(void) {
    Init_LCD_DIO_Port(); // Configure RB8 through RB13 for digital output
    Write_LCD_Nibble(0b0011, INSTRUCTION);  // Initialize the LCD Module
    Write_LCD_Nibble(0b0011, INSTRUCTION);
    Write_LCD_Nibble(0b0011, INSTRUCTION);
    Write_LCD_Nibble(0b0010, INSTRUCTION);  // invoke 4-bit mode
    Write_LCD_Byte(0b00101000, INSTRUCTION);// 4-bit mode, two-line,5X7 dot
    Write_LCD_Byte(0b00000001, INSTRUCTION);// clear display, cursor at 0x00
    Write_LCD_Byte(0b00001100, INSTRUCTION);// display on,cursor off
}

void Position_LCD_Cursor(int cell_num) {
    Write_LCD_Byte(0x80 + cell_num, INSTRUCTION);
}

void Write_LCD_String(char char_Array[16]) {
    int idx;

    for(idx = 0; idx < strlen(char_Array); idx++)
        Write_LCD_Byte(char_Array[idx], DATA);  
}