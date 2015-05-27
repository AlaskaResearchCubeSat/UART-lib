#ifndef __UCA3_UART_H
#define __UCA3_UART_H

#include <ctl.h>
#include <UART_queue.h>

  //queue a byte to be transfered over the UART
  int UCA3_TxChar(unsigned char c);
  //Get a byte from the receive queue
  int UCA3_Getc(void);
  //initialize UCA0 for UART Usage
  void UCA3_init_UART(unsigned int port,unsigned int tx,unsigned int rx);
  //get character from port without blocking
  int UCA3_CheckKey(void);
  //check if UART is transmitting or receiving
  #define UCA3_CheckBusy()  (UCA3STATW&UCBUSY)

  //baud rate functions
  void UCA3_BR9600(void);
  void UCA3_BR38400(void);
  void UCA3_BR57600(void);

  //buffer structures
  extern struct UART_Tx UCA3_TxBuf;
  extern struct UART_Rx UCA3_RxBuf;

  //setup events to watch for data
  void UCA3_setup_events(CTL_EVENT_SET_t *e,CTL_EVENT_SET_t txnotfull,CTL_EVENT_SET_t rxnotempty);

#endif
  