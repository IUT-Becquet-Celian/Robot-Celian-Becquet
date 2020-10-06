/* 
 * File:   CB_TX1.h
 * Author: Table2
 *
 * Created on 25 septembre 2020, 16:14
 */

#ifndef CB_TX1_H
#define	CB_TX1_H

int CB_TX1_RemainingSize(void);
unsigned char CB_TX1_IsTranmitting(void);

void CB_TX1_Add(unsigned char);
unsigned char CB_TX1_Get(void);

void SendOne(void);
void SendMessage(unsigned char*, int);

#ifdef	__cplusplus
}
#endif

#endif	/* CB_TX1_H */

