#ifndef __UCA0_UART_H
#define __UCA0_UART_H

#include <ctl.h>
#include <UART_queue.h>

  //queue a byte to be transfered over the UART
  int UCA0_TxChar(unsigned char c);
  //Get a byte from the receive queue
  int UCA0_Getc(void);
  //initialize UCA0 for UART Usage
  void UCA0_init_UART(unsigned int port,unsigned int tx,unsigned int rx);
  //get character from port without blocking
  int UCA0_CheckKey(void);
  //check if UART is transmitting or receiving
  #define UCA0_CheckBusy()  (UCA0STAT&UCBUSY)

  //baud rate functions
  void UCA0_BR9600(void);
  void UCA0_BR38400(void);
  void UCA0_BR57600(void);

  //buffer structures
  extern struct UART_Tx UCA0_TxBuf;
  extern struct UART_Rx UCA0_RxBuf;

  //setup events to watch for data
  void UCA0_setup_events(CTL_EVENT_SET_t *e,CTL_EVENT_SET_t txnotfull,CTL_EVENT_SET_t rxnotempty);

#endif
  