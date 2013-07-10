#ifndef __UART_H
#define __UART_H

#include <ctl.h>
#include "UART_queue.h"

#ifdef USE_UCA0
  //definition for function names
  #define UART_NAME(name)  UCA0##_##name
  //definition for ports
  #define UART_REG(name)  UCA0##name

  #ifdef USE_UCA1
    #error "Can\'t use UCA1 and UCA0 at the same time"
  #endif
#elif USE_UCA1
  //definition for function names
  #define UART_NAME(name)  UCA1##_##name
  //definition for ports
  #define UART_REG(name)  UCA1##name
#else
  #error Please spesify USE_UCA0 or USE_UCA1
#endif

#endif
  
