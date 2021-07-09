/* 
 * File:   EE200_LCD.h
 * Author: Schiano
 *
 * Created on March 25, 2016, 6:37 AM
 */

#ifndef EE200_LCD_H
#define	EE200_LCD_H

/* declare visible functions */
void Init_LCD_Module(void);
void Position_LCD_Cursor(int cell_num);
void Write_LCD_String(char char_Array[16]);

#endif	/* EE200_LCD_H */

