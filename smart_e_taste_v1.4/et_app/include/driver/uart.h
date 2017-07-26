/*
 *  Copyright (C) 2010 -2011  Espressif System
 *
 */

#ifndef __UART_H__
#define __UART_H__

#include "et_types.h"

//#define ETS_UART_INTR_ENABLE()  _xt_isr_unmask(1 << ETS_UART_INUM)
//#define ETS_UART_INTR_DISABLE() _xt_isr_mask(1 << ETS_UART_INUM)
#define UART_INTR_MASK          0x1ff
#define UART_LINE_INV_MASK      (0x3f<<19)
#define UART_BUF_SIZE    512
#define UART_MAX_READ_BUFFER  512
#define UART_MAX_SEND_BUFFER  512
#define FAC_MODE 				0x01
#define NORM_MODE 				0x02

typedef enum {
    UART_WordLength_5b = 0x0,
    UART_WordLength_6b = 0x1,
    UART_WordLength_7b = 0x2,
    UART_WordLength_8b = 0x3
} UART_WordLength;

typedef enum {
    USART_StopBits_1   = 0x1,
    USART_StopBits_1_5 = 0x2,
    USART_StopBits_2   = 0x3,
} UART_StopBits;

typedef enum {
    UART0 = 0x0,
    UART1 = 0x1,
} UART_Port;

typedef enum {
    USART_Parity_None = 0x2,
    USART_Parity_Even = 0x0,
    USART_Parity_Odd  = 0x1
} UART_ParityMode;

typedef enum {
    PARITY_DIS = 0x0,
    PARITY_EN  = 0x2
} UartExistParity;

typedef enum {
    BIT_RATE_300     = 300,
    BIT_RATE_600     = 600,
    BIT_RATE_1200    = 1200,
    BIT_RATE_2400    = 2400,
    BIT_RATE_4800    = 4800,
    BIT_RATE_9600    = 9600,
    BIT_RATE_19200   = 19200,
    BIT_RATE_38400   = 38400,
    BIT_RATE_57600   = 57600,
    BIT_RATE_74880   = 74880,
    BIT_RATE_115200  = 115200,
    BIT_RATE_230400  = 230400,
    BIT_RATE_460800  = 460800,
    BIT_RATE_921600  = 921600,
    BIT_RATE_1843200 = 1843200,
    BIT_RATE_3686400 = 3686400,
} UART_BautRate; //you can add any rate you need in this range

typedef enum {
    USART_HardwareFlowControl_None    = 0x0,
    USART_HardwareFlowControl_RTS     = 0x1,
    USART_HardwareFlowControl_CTS     = 0x2,
    USART_HardwareFlowControl_CTS_RTS = 0x3
} UART_HwFlowCtrl;

typedef enum {
    UART_None_Inverse = 0x0,
    UART_Rxd_Inverse  = UART_RXD_INV,
    UART_CTS_Inverse  = UART_CTS_INV,
    UART_Txd_Inverse  = UART_TXD_INV,
    UART_RTS_Inverse  = UART_RTS_INV,
} UART_LineLevelInverse;

typedef struct {
    UART_BautRate   baud_rate;
    UART_WordLength data_bits;
    UART_ParityMode parity;    // chip size in byte
    UART_StopBits   stop_bits;
    UART_HwFlowCtrl flow_ctrl;
    uint8           UART_RxFlowThresh ;
    uint32          UART_InverseMask;
} UART_ConfigTypeDef;

typedef struct {
    uint32 UART_IntrEnMask;
    uint8  UART_RX_TimeOutIntrThresh;
    uint8  UART_TX_FifoEmptyIntrThresh;
    uint8  UART_RX_FifoFullIntrThresh;
} UART_IntrConfTypeDef;
/*
typedef struct {
    uint32 rx_buf_size;
    uint8  *rx_buf;
    uint8  *out_pos;
    uint8 *in_pos;
    uint32 remanet_size;
}rx_buf_t;*/


enum {
    UART_EVENT_RX_CHAR,
    UART_EVENT_MAX
};

typedef struct _os_event_ {
    uint32 event;
    uint32 param;
} os_event_t;

//=======================================
void UART_WaitTxFifoEmpty(UART_Port uart_no); //do not use if tx flow control enabled
void UART_ResetFifo(UART_Port uart_no);
void UART_ClearIntrStatus(UART_Port uart_no, uint32 clr_mask);
void UART_SetIntrEna(UART_Port uart_no, uint32 ena_mask);
void UART_intr_handler_register(void *fn);
void UART_SetPrintPort(UART_Port uart_no);
void UART_ParamConfig(UART_Port uart_no,  UART_ConfigTypeDef *pUARTConfig);
void UART_IntrConfig(UART_Port uart_no,  UART_IntrConfTypeDef *pUARTIntrConf);
void UART_SetWordLength(UART_Port uart_no, UART_WordLength len);
void UART_SetStopBits(UART_Port uart_no, UART_StopBits bit_num);
void UART_SetParity(UART_Port uart_no, UART_ParityMode Parity_mode) ;
void UART_SetBaudrate(UART_Port uart_no, uint32 baud_rate);
void UART_SetFlowCtrl(UART_Port uart_no, UART_HwFlowCtrl flow_ctrl, uint8 rx_thresh);
void UART_SetLineInverse(UART_Port uart_no, UART_LineLevelInverse inverse_mask) ;
void uart_init_new(UART_BautRate baud_rate);
void uart_send(const et_char *str,et_int32 size);
et_int32 uart_buf_init(et_uint32 buf_size);
et_uint32 read_data_from_buf(et_char *data, et_uint32 data_len);
et_int32 recv_data_uart(et_char *buffer,et_int32 len,et_int32 time_out_ms);


#endif
