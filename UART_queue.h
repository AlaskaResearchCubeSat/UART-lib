#ifndef __UART_QUEUE
#define __UART_QUEUE

#include <ctl.h>
//define size for RX and TX buffers
#define UART_RX_SIZE   (150)
#define UART_TX_SIZE   (150)

  //TX buffer type
  struct UART_Tx{
    CTL_BYTE_QUEUE_t queue;
    char done;
    unsigned char buf[UART_TX_SIZE];
  };
  
  //RX buffer type
  struct UART_Rx{
    CTL_BYTE_QUEUE_t queue;
    unsigned char buf[UART_RX_SIZE];
  };

#endif
