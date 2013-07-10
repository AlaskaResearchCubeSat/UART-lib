#include <msp430.h>
#include <stdio.h>
#include "uart.h"


//buffer structures
extern struct UART_Tx UART_NAME(TxBuf);
extern struct UART_Rx UART_NAME(RxBuf);

#ifdef USE_UCA0
  static void enable_ints(void){
    //enable interrupts
    IE2|=UCA0TXIE|UCA0RXIE;
  }

  static void disable_ints(void){
    //disable interrupts
    IE2&=~(UCA0TXIE|UCA0RXIE);
  }
#else
  static void enable_ints(void){
    //enable interrupts
    UC1IE|=UCA1TXIE|UCA1RXIE;
  }
  
  static void disable_ints(void){
    //disable interrupts
    UC1IE&=~(UCA1TXIE|UCA1RXIE);
  }
#endif

void UART_NAME(init_UART) (void){
 //init queues
  ctl_byte_queue_init(&UART_NAME(TxBuf).queue,UART_NAME(TxBuf).buf,UART_TX_SIZE);
  ctl_byte_queue_init(&UART_NAME(RxBuf).queue,UART_NAME(RxBuf).buf,UART_RX_SIZE);
  UART_NAME(TxBuf).done=0;
   //setup UART operation
  UART_REG(CTL1)=UCSWRST;
  UART_REG(CTL0)=0;
  UART_REG(CTL1)|=UCSSEL_1;
  
  //set baud rate to 9600
  UART_REG(BR0)=3;
  UART_REG(BR1)=0;
  UART_REG(MCTL)=UCBRS_3;

  #ifdef USE_UCA0
    //setup pins
    P3SEL|=BIT4|BIT5;
  #else
    //setup pins
    P3SEL|=BIT6|BIT7;
  #endif
  //take UCA1 out of reset mode
  UART_REG(CTL1)&=~UCSWRST;
  //enable UART interrupts
  enable_ints();
}

void UART_NAME(BR9600) (void){
  //enable UART interrupts
  disable_ints();
  //put module into reset mode
  UART_REG(CTL1)|=UCSWRST;
  //set clock source
  UART_REG(CTL1)&=~(UCSSEL0|UCSSEL1);
  UART_REG(CTL1)|=UCSSEL_1;
  //set baud rate to 9600
  UART_REG(BR0)=3;
  UART_REG(BR1)=0;
  UART_REG(MCTL)=UCBRS_3;
  //take UCA1 out of reset mode
  UART_REG(CTL1)&=~UCSWRST;
  //enable UART interrupts
  enable_ints();
}

void UART_NAME(BR38400) (void){
  //enable UART interrupts
  disable_ints();
  //put module into reset mode
  UART_REG(CTL1)|=UCSWRST;
  //set clock source
  UART_REG(CTL1)&=~(UCSSEL0|UCSSEL1);
  UART_REG(CTL1)|=UCSSEL_2;
  //set baud rate to 38400
  UART_REG(BR0)=26;
  UART_REG(BR1)=0;
  UART_REG(MCTL)=UCBRF_1|UCOS16;
  //take UCA1 out of reset mode
  UART_REG(CTL1)&=~UCSWRST;
  //enable UART interrupts
  enable_ints();
}

void UART_NAME(BR57600) (void){
  //enable UART interrupts
  disable_ints();
  //put module into reset mode
  UART_REG(CTL1)|=UCSWRST;
  //set clock source
  UART_REG(CTL1)&=~(UCSSEL0|UCSSEL1);
  UART_REG(CTL1)|=UCSSEL_2;
  //set baud rate to 57600
  UART_REG(BR0)=17;
  UART_REG(BR1)=0;
  UART_REG(MCTL)=UCBRF_6|UCBRS_0|UCOS16;
  //take UCA1 out of reset mode
  UART_REG(CTL1)&=~UCSWRST;
  //enable UART interrupts
  enable_ints();
}

//queue byte to get transmitted
int UART_NAME(TxChar) (unsigned char c){
  unsigned int t;
  int res=c;
  //disable interrupt
  int en=ctl_global_interrupts_disable();
  //check if transmitting
  if(UART_NAME(TxBuf).done){
    //bypass queue for first byte if not transmitting
    UART_REG(TXBUF)=c;
    //clear done flag
    UART_NAME(TxBuf).done=0;
  //queue byte
  }else{
    ctl_byte_queue_post(&UART_NAME(TxBuf).queue,c,CTL_TIMEOUT_NONE,0);
  }
  //enable interrupt
  if(en){
    ctl_global_interrupts_enable();
  }
  //return result
  return res;
}

//get byte from buffer
int UART_NAME(Getc) (void){
  unsigned char c;
  //recive a byte from the queue
  //TODO: posibly add timeout and the posibility to return EOF
  ctl_byte_queue_receive(&UART_NAME(RxBuf).queue,&c,CTL_TIMEOUT_NONE,0);
  //return byte from queue
  return c;
}

//get a byte from the buffer but don't wait
int UART_NAME(CheckKey) (void){
  unsigned char c;
  if(ctl_byte_queue_receive_nb(&UART_NAME(RxBuf).queue,&c)){
    return c;
  }else{
    return EOF;
  }
}

//setup events to watch for data
void UART_NAME(setup_events) (CTL_EVENT_SET_t *e,CTL_EVENT_SET_t txnotfull,CTL_EVENT_SET_t rxnotempty){
  //setup RX events
  ctl_byte_queue_setup_events(&UART_NAME(RxBuf).queue,e,rxnotempty,0);
  //setup TX events
  ctl_byte_queue_setup_events(&UART_NAME(TxBuf).queue,e,0,txnotfull);
}

