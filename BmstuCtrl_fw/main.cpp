/*
 * File:   main.cpp
 * Author: Kreyl
 * Project: ChibiArmlet-Atlantis
 *
 * Created on Feb 05, 2013, 20:27
 */

#include "main.h"

App_t App;

#if 1 // ============================ Timers ===================================
void TmrUartRxCallback(void *p) {
    chSysLockFromIsr();
    chEvtSignalI(App.PThread, EVTMSK_UART_RX_POLL);
    chVTSetI(&App.TmrUartRx, MS2ST(UART_RX_POLLING_MS), TmrUartRxCallback, nullptr);
    chSysUnlockFromIsr();
}
#endif

#if 1 // ============================ LEDs =====================================
void LedTmrCallback(void *p) {
    PinClear(LED_GPIO, LED_PIN);
}

void App_t::LedBlink(uint32_t Duration_ms) {
    PinSet(LED_GPIO, LED_PIN);
    chSysLock()
    if(chVTIsArmedI(&TmrLed)) chVTResetI(&TmrLed);
    chVTSetI(&TmrLed, MS2ST(Duration_ms), LedTmrCallback, nullptr);
    chSysUnlock();
}
#endif

int main(void) {
    // ==== Init Vcore & clock system ====
    SetupVCore(vcore1V5);
    Clk.SetupFlashLatency(16);
    uint8_t ClkRslt = Clk.SwitchToHSI();
    if(ClkRslt == OK) Clk.DisableMSI();
    Clk.UpdateFreqValues();

    // ==== Init OS ====
    halInit();
    chSysInit();

    // ==== Init Hard & Soft ====
    App.PThread = chThdSelf();

    Uart.Init(115200);
    Uart.Printf("\rBmstuCtrl AHB=%u", Clk.AHBFreqHz);
    if(ClkRslt != OK) Uart.Printf("\rClk switch failure");

    // Path init
    PinSetupOut(PATH_GPIO, PATH21_PIN, omPushPull);
    PinSetupOut(PATH_GPIO, PATH31_PIN, omPushPull);
    PinSetupOut(PATH_GPIO, PATH22_PIN, omPushPull);
    PinSetupOut(PATH_GPIO, PATH32_PIN, omPushPull);

    // Led init
    PinSetupOut(LED_GPIO, LED_PIN, omPushPull);

    App.UartImInit(); // Imitator's UART

    // Timers
    chSysLock();
    chVTSetI(&App.TmrUartRx,    MS2ST(UART_RX_POLLING_MS), TmrUartRxCallback, nullptr);
    chSysUnlock();

    // Main cycle
    while(true) App.ITask();
}

void App_t::ITask() {
    uint32_t EvtMsk = chEvtWaitAny(ALL_EVENTS);
    // ==== Uart cmd ====
    if(EvtMsk & EVTMSK_UART_RX_POLL) {
        // Check control UART
        while(Uart.ProcessRx() == pdrNewCmd) OnUartCmd(&Uart);
        // Check Imitator's UART
        UartImProcessRx();
    }
}

void App_t::OnUartCmd(CmdUart_t *PUart) {
    LedBlink(54);
    UartCmd_t *PCmd = &PUart->Cmd;
    Uart.Printf("\r%S\r", PCmd->Name);
    // Handle command
    if(PCmd->NameIs("#Ping")) PUart->Ack(OK);

    else if(PCmd->NameIs("#SetLines")) {
        int32_t L[4];
        // Receive desired state
        if(PCmd->GetNextToken() != OK) { PUart->Ack(CMD_ERROR); return; }
        if(PCmd->TryConvertTokenToNumber(&L[0]) != OK) { PUart->Ack(CMD_ERROR); return; }
        if(PCmd->GetNextToken() != OK) { PUart->Ack(CMD_ERROR); return; }
        if(PCmd->TryConvertTokenToNumber(&L[1]) != OK) { PUart->Ack(CMD_ERROR); return; }
        if(PCmd->GetNextToken() != OK) { PUart->Ack(CMD_ERROR); return; }
        if(PCmd->TryConvertTokenToNumber(&L[2]) != OK) { PUart->Ack(CMD_ERROR); return; }
        if(PCmd->GetNextToken() != OK) { PUart->Ack(CMD_ERROR); return; }
        if(PCmd->TryConvertTokenToNumber(&L[3]) != OK) { PUart->Ack(CMD_ERROR); return; }
        // Setup state
        if(L[0] == 0) PinClear(PATH_GPIO, PATH21_PIN);
        else PinSet(PATH_GPIO, PATH21_PIN);
        if(L[1] == 0) PinClear(PATH_GPIO, PATH31_PIN);
        else PinSet(PATH_GPIO, PATH31_PIN);
        if(L[2] == 0) PinClear(PATH_GPIO, PATH22_PIN);
        else PinSet(PATH_GPIO, PATH22_PIN);
        if(L[3] == 0) PinClear(PATH_GPIO, PATH32_PIN);
        else PinSet(PATH_GPIO, PATH32_PIN);
        PUart->Ack(OK);
    }

    else if(PCmd->NameIs("#GetLines")) {
        char c[4];
        // Collect data
        c[0] = PinIsSet(PATH_GPIO, PATH21_PIN)? '1' : '0';
        c[1] = PinIsSet(PATH_GPIO, PATH31_PIN)? '1' : '0';
        c[2] = PinIsSet(PATH_GPIO, PATH22_PIN)? '1' : '0';
        c[3] = PinIsSet(PATH_GPIO, PATH32_PIN)? '1' : '0';
        // Send reply
        PUart->Printf("#Lines %c %c %c %c\r\n", c[0], c[1], c[2], c[3]);
    }

    else if(PCmd->NameIs("#SendToIm")) {
        if(PCmd->GetNextToken() != OK) { PUart->Ack(CMD_ERROR); return; }
        int32_t dw=0;
        do {
            if(PCmd->TryConvertTokenToNumber(&dw) != OK) { PUart->Ack(CMD_ERROR); return; }
            uint8_t b = (uint8_t)(0xFF & dw);
            while(!(USART3->SR & USART_SR_TXE));
            USART3->DR = b;
        } while(PCmd->GetNextToken() == OK);
    }

    else if(*PCmd->Name == '#') PUart->Ack(CMD_UNKNOWN);  // reply only #-started stuff
}

void App_t::UartImInit() {
    // GPIO
    PinSetupAlterFunc(GPIOB, 10, omPushPull, pudNone, AF7);
    PinSetupAlterFunc(GPIOB, 11, omOpenDrain, pudPullUp, AF7);
    // UART
    rccEnableUSART3(FALSE);
    USART3->CR1 = USART_CR1_UE;     // Enable USART
    USART3->CR1 = USART_CR1_TE | USART_CR1_RE;        // TX & RX enable
    USART3->BRR = Clk.APB1FreqHz / 115200;
    USART3->CR2 = 0;
    // RX DMA
    USART3->CR3 = USART_CR3_DMAR;    // Enable DMA RX
    dmaStreamAllocate     (UART_IM_DMA_RX, IRQ_PRIO_LOW, nullptr, NULL);
    dmaStreamSetPeripheral(UART_IM_DMA_RX, &USART3->DR);
    dmaStreamSetMemory0   (UART_IM_DMA_RX, ImRxBuf);
    dmaStreamSetTransactionSize(UART_IM_DMA_RX, IM_RX_BUF_SZ);
    dmaStreamSetMode      (UART_IM_DMA_RX, UART_IM_DMA_RX_MODE);
    dmaStreamEnable       (UART_IM_DMA_RX);
    USART3->CR1 |= USART_CR1_UE;    // Enable USART
}

void App_t::UartImProcessRx() {
    // Number of bytes copied to buffer since restart
    int32_t Sz = IM_RX_BUF_SZ - UART_IM_DMA_RX->channel->CNDTR;
    if(Sz != SzOld) {
        int32_t ByteCnt = Sz - SzOld;
        if(ByteCnt < 0) ByteCnt += IM_RX_BUF_SZ;   // Handle buffer circulation
        SzOld = Sz;
        if(ByteCnt != 0) Uart.Printf("#ReplyFromIm");
        for(int32_t i=0; i<ByteCnt; i++) {          // Iterate received bytes
            uint8_t b = ImRxBuf[RIndx++];
            if(RIndx >= IM_RX_BUF_SZ) RIndx = 0;
            // Print byte
            Uart.Printf(" %X", b);
        } // for
        Uart.Printf("\r\n");
    } // if sz
}
