#ifndef __UCA2_UART_H
#define __UCA2_UART_H

#include <ctl.h>
#include <UART_queue.h>

  //queue a byte to be transfered over the UART
  int UCA2_TxChar(unsigned char c);
  //Get a byte from the receive queue
  int UCA2_Getc(void);
  //initialize UCA0 for UART Usage
  void UCA2_init_UART(unsigned int port,unsigned int tx,unsigned int rx);
  //get character from port without blocking
  int UCA2_CheckKey(void);
  //check if UART is transmitting or receiving
  #define UCA2_CheckBusy()  (UCA2STATW&UCBUSY)

  //baud rate functions
  void UCA2_BR9600(void);
  void UCA2_BR38400(void);
  void UCA2_BR57600(void);

  //buffer structures
  extern struct UART_Tx UCA2_TxBuf;
  extern struct UART_Rx UCA2_RxBuf;

  //setup events to watch for data
  void UCA2_setup_events(CTL_EVENT_SET_t *e,CTL_EVENT_SET_t txnotfull,CTL_EVENT_SET_t rxnotempty);

#endif
  