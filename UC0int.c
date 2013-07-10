#include <msp430.h>
#include <ctl.h>
#include <UCA0_uart.h>

//buffers for UCA0 UART
struct UART_Tx UCA0_TxBuf;
struct UART_Rx UCA0_RxBuf;

//UART TX ISR called to transmit UART data
void UC1_TX(void) __ctl_interrupt[USCIAB0TX_VECTOR]{
  unsigned char flags=IFG2&(IE2);
//=============[UART Transmit Handler]===============
  if(flags&UCA1TXIFG){
    unsigned char c;
    if (ctl_byte_queue_receive_nb(&UCA0_TxBuf.queue,&c)==0){
      //buffer empty disable TX
      UCA0_TxBuf.done=1;
      IFG2&=~UCA0TXIFG;
    }else{
      //send char to UART
      UCA0TXBUF=c;
    }
  }
//==============================================================
}

// receive UART ISR
void UC1_rx(void) __ctl_interrupt[USCIAB0RX_VECTOR]{
  unsigned char flags=IFG2&(IE2);
//==============[UART Receive Handler]==========================
  if(flags&UCA0RXIFG){
    //read a byte from UART
    unsigned char c=UCA0RXBUF;
    //put byte in queue, if no room too darn bad
    ctl_byte_queue_post_nb(&UCA0_RxBuf.queue,c);
    //TODO: raise error if no room
  }
//==============================================================
}
