/*
 * battery_consts.h
 *
 *  Created on: 30 дек. 2013 г.
 *      Author: kreyl
 */

#ifndef BATTERY_CONSTS_H_
#define BATTERY_CONSTS_H_

#ifndef countof
#define countof(A)  (sizeof(A)/sizeof(A[0]))
#endif

// Table of charge
struct mVPercent_t {
    uint16_t mV;
    uint8_t Percent;
};

#if 1 // ========================= Alkaline 1.5V ===============================
static const mVPercent_t mVPercentTableAlkaline[] = {
        {1440, 100},
        {1370, 80},
        {1270, 60},
        {1170, 40},
        {1080, 20},
        {930,  10}
};
#define mVPercentTableAlkalineSz    countof(mVPercentTableAlkaline)

static uint8_t mV2PercentAlkaline(uint16_t mV) {
    for(uint8_t i=0; i<mVPercentTableAlkalineSz; i++)
        if(mV >= mVPercentTableAlkaline[i].mV) return mVPercentTableAlkaline[i].Percent;
    return 0;
}
#endif

#if 0 // ============================ Li-Ion ===================================
static const mVPercent_t mVPercentTableLiIon[] = {
        {4100, 100},
        {4000, 90},
        {3900, 80},
        {3850, 70},
        {3800, 60},
        {3780, 50},
        {3740, 40},
        {3700, 30},
        {3670, 20},
        {3640, 10}
};
#define mVPercentTableLiIonSz    countof(mVPercentTableLiIon)

static uint8_t mV2PercentLiIon(uint16_t mV) {
    for(uint8_t i=0; i<mVPercentTableLiIonSz; i++)
        if(mV >= mVPercentTableLiIon[i].mV) return mVPercentTableLiIon[i].Percent;
    return 0;
}
#endif

#if 0 // ============================ 3V Li ====================================
static const mVPercent_t mVPercentTableLi3V[] = {
        {2640, 100},
        {2550, 80},
        {2410, 60},
        {2210, 40},
        {2080, 20},
        {1950, 10}
};
#endif

#endif /* BATTERY_CONSTS_H_ */
