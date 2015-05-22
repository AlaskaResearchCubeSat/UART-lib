#include <msp430.h>
#include <ctl.h>
#include <uart.h>


//buffers for UCA0 UART
struct UART_Tx UART_NAME(TxBuf);
struct UART_Rx UART_NAME(RxBuf);

//UART TX ISR called to transmit UART data
void UART_NAME(ISR)(void) __ctl_interrupt[UART_VECTOR]{
  unsigned char c;
  //check interrupt source
  switch(UART_REG(IV)){
    case 0x02:        //RXIFG
      //read a byte from UART
      c=UART_REG(RXBUF);
      //put byte in queue, if no room too darn bad
      ctl_byte_queue_post_nb(&UART_NAME(RxBuf).queue,c);
      //TODO: raise error if no room?
    break;
    case 0x04:        //TXIFG
      if (ctl_byte_queue_receive_nb(&UART_NAME(TxBuf).queue,&c)==0){
        //buffer empty disable TX
        UART_NAME(TxBuf).done=1;
      }else{
        //send char to UART
        UART_REG(TXBUF)=c;
      }
    break;
    case 0x06:        //start bit
    break;
    case 0x08:        //Transmit complete
    break;
  }
}
