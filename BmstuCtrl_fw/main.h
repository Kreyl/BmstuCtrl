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

class App_t {
private:
public:
    Thread *PThread;
    VirtualTimer TmrUartRx, TmrLed;
    void Init();
    void LedBlink(uint32_t Duration_ms);
    // Events
    void OnUartCmd(CmdUart_t *PUart);
    // Inner use
    void ITask();
    App_t(): PThread(nullptr), TmrUartRx(), TmrLed() {}
};


#endif /* MAIN_H_ */
