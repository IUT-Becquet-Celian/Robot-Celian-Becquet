/* 
 * File:   CB_RX1.h
 * Author: table4
 *
 * Created on 6 octobre 2020, 10:07
 */

#ifndef CB_RX1_H
#define	CB_RX1_H

void CB_RX1_Add(unsigned char);
unsigned char CB_RX1_Get(void);
unsigned char CB_RX1_IsDataAvailable (void);
int CB_RX1_GetRemainingSize(void);
int CB_RX1_GetDataSize(void);





#ifdef	__cplusplus
extern "C" {
#endif

#endif	/* CB_RX1_H */

