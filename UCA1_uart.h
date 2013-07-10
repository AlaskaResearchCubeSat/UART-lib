#ifndef __UCA1_UART_H
#define __UCA1_UART_H

#include <ctl.h>
#include <UART_queue.h>

  //queue a byte to be transfered over the UART
  int UCA1_TxChar(unsigned char c);
  //Get a byte from the receive queue
  int UCA1_Getc(void);
  //initialize UCA0 for UART Usage
  void UCA1_init_UART(void);
  //get character from port without blocking
  int UCA1_CheckKey(void);
  //check if UART is transmitting or receiving
  #define UCA1_CheckBusy()  (UCA1STAT&UCBUSY)

  //baud rate functions
  void UCA1_BR9600(void);
  void UCA1_BR38400(void);
  void UCA1_BR57600(void);

  //buffer structures
  extern struct UART_Tx UCA1_TxBuf;
  extern struct UART_Rx UCA1_RxBuf;

  //setup events to watch for data
  void UCA1_setup_events(CTL_EVENT_SET_t *e,CTL_EVENT_SET_t txnotfull,CTL_EVENT_SET_t rxnotempty);

#endif
  