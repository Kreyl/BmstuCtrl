/*
 * main.h
 *
 *  Created on: 21 дек. 2014 г.
 *      Author: Kreyl
 */

#ifndef MAIN_H_
#define MAIN_H_

#include "kl_lib_L15x.h"
#include "ch.h"
#include "hal.h"
#include "clocking_L1xx.h"
#include "evt_mask.h"
#include "uart.h"

#if 1 // ============================ Path control =============================
#define PATH_GPIO       GPIOB
#define PATH21_PIN      4
#define PATH31_PIN      5
#define PATH22_PIN      6
#define PATH32_PIN      7
#endif

#define LED_GPIO        GPIOB
#define LED_PIN         0

#define IM_RX_BUF_SZ    99

#define UART_IM_DMA_RX      STM32_DMA1_STREAM3
#define UART_IM_DMA_RX_MODE DMA_PRIORITY_LOW | \
                            STM32_DMA_CR_MSIZE_BYTE | \
                            STM32_DMA_CR_PSIZE_BYTE | \
                            STM32_DMA_CR_MINC |       /* Memory pointer increase */ \
                            STM32_DMA_CR_DIR_P2M |    /* Direction is peripheral to memory */ \
                            STM32_DMA_CR_CIRC         /* Circular buffer enable */


class App_t {
private:
    int32_t SzOld=0, RIndx=0;
public:
    Thread *PThread;
    VirtualTimer TmrUartRx, TmrLed;
    uint8_t ImRxBuf[IM_RX_BUF_SZ];
    void Init();
    void UartImInit();
    void UartImProcessRx();
    void LedBlink(uint32_t Duration_ms);
    // Events
    void OnUartCmd(CmdUart_t *PUart);
    // Inner use
    void ITask();
    App_t(): PThread(nullptr), TmrUartRx(), TmrLed() {}
};


#endif /* MAIN_H_ */
