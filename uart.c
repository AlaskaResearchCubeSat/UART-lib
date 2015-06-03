#include <msp430.h>
#include <stdio.h>
#include "uart.h"

//include all header files here to make sure prorotypes match
#include "UCA0_uart.h"
#include "UCA1_uart.h"
#include "UCA2_uart.h"
#include "UCA3_uart.h"

//buffer structures
extern struct UART_Tx UART_NAME(TxBuf);
extern struct UART_Rx UART_NAME(RxBuf);

static void enable_ints(void){
  //enable interrupts
  UART_REG(IE)|=UCTXIE|UCRXIE;
}

static void disable_ints(void){
  //disable interrupts
  UART_REG(IE)&=~(UCTXIE|UCRXIE);
}

void UART_NAME(init_UART) (unsigned int port,unsigned int tx,unsigned int rx){
  volatile unsigned char *port_sel,*base_map;
  //init queues
  ctl_byte_queue_init(&UART_NAME(TxBuf).queue,UART_NAME(TxBuf).buf,UART_TX_SIZE);
  ctl_byte_queue_init(&UART_NAME(RxBuf).queue,UART_NAME(RxBuf).buf,UART_RX_SIZE);
  UART_NAME(TxBuf).done=0;
   //setup UART operation
  UART_REG(CTLW0)=UCSWRST;
  UART_REG(CTLW0)|=UCSSEL_1;
  
  //set baud rate to 9600
  UART_REG(BRW)=3;
  UART_REG(MCTLW)=(0x92<<8);

  //check port and pins for validity
  if((port>=2 && port<=4) && ((tx<8) || (rx<8)) ){
    switch(port){
      case 2:
        port_sel=&P2SEL0;
        base_map=&P2MAP0;
      break;
      case 3:
        port_sel=&P4SEL0;
        base_map=&P4MAP0;
      break;
      case 4:
        port_sel=&P4SEL0;
        base_map=&P4MAP0;
      break;
    }
    //unlock port maping
    PMAPKEYID=PMAPKEY;
    //allow reconfiguration
    PMAPCTL|=PMAPRECFG;
    //check if tx pin is valid
    if(tx<8){
      //setup port mapping for tx pin
      base_map[tx]=PM_UART_TXD;
      //select pin special function
      *port_sel|=1<<tx;
    }
    //check if rx pin is valid
    if(rx<8){
      //setup port mapping for rx pin
      base_map[rx]=PM_UART_RXD;
      //select pin special function
      *port_sel|=1<<rx;
    }
    //lock port maping
    PMAPKEYID=0;
  }
    

  //take UCA1 out of reset mode
  UART_REG(CTLW0)&=~UCSWRST;
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
  UART_REG(MCTLW)=0x92;
  //take UCA1 out of reset mode
  UART_REG(CTL1)&=~UCSWRST;
  //enable UART interrupts
  enable_ints();
}

/* disable high baud rate functions until system clock is chosen
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
  UART_REG(MCTLW)=UCBRF_1|UCOS16;
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
  UART_REG(MCTLW)=UCBRF_6|UCBRS_0|UCOS16;
  //take UCA1 out of reset mode
  UART_REG(CTL1)&=~UCSWRST;
  //enable UART interrupts
  enable_ints();
}*/

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

