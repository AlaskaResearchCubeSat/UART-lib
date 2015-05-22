#ifndef __UART_H
#define __UART_H

#include <ctl.h>
#include "UART_queue.h"

#ifdef USE_UCA0
  //definition for function names
  #define UART_NAME(name)  UCA0##_##name
  //definition for ports
  #define UART_REG(name)  UCA0##name
  //vector definition
  #define UART_VECTOR     USCI_A0_VECTOR
  //portmaping definitions
  #define PM_UART_TXD   PM_UCA0TXD
  #define PM_UART_RXD   PM_UCA0RXD

  #ifdef USE_UCA1
    #error "Can\'t use UCA1 and UCA0 at the same time"
  #endif
#elif USE_UCA1
  //definition for function names
  #define UART_NAME(name)  UCA1##_##name
  //definition for ports
  #define UART_REG(name)  UCA1##name
  //vector definition
  #define UART_VECTOR     USCI_A1_VECTOR
  //portmaping definitions
  #define PM_UART_TXD   PM_UCA1TXD
  #define PM_UART_RXD   PM_UCA1RXD
#elif USE_UCA2
  //definition for function names
  #define UART_NAME(name)  UCA2##_##name
  //definition for ports
  #define UART_REG(name)  UCA2##name
  //vector definition
  #define UART_VECTOR     USCI_A2_VECTOR
  //portmaping definitions
  #define PM_UART_TXD   PM_UCA2TXD
  #define PM_UART_RXD   PM_UCA2RXD
#elif USE_UCA3
  //definition for function names
  #define UART_NAME(name)  UCA3##_##name
  //definition for ports
  #define UART_REG(name)  UCA3##name
  //vector definition
  #define UART_VECTOR     USCI_A3_VECTOR
  //portmaping definitions
  #define PM_UART_TXD   PM_UCA3TXD
  #define PM_UART_RXD   PM_UCA3RXD
#else
  #error Please spesify USE_UCA0, USE_UCA2 or USE_UCA3
#endif


#endif
  
