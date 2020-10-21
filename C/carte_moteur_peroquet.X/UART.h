/* 
 * File:   UART.h
 * Author: Table2
 *
 * Created on 14 septembre 2020, 10:23
 */

#ifndef UART_H
#define	UART_H



void InitUART (void);
void SendMessageDirect (unsigned char* message , int length);
void DecodeMessage(unsigned char c);

#endif	/* UART_H */

