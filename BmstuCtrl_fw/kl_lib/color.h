/*
 * color.h
 *
 *  Created on: 05 апр. 2014 г.
 *      Author: Kreyl
 */

#ifndef COLOR_H_
#define COLOR_H_

#include "inttypes.h"
#include "stdbool.h"

struct Color_t {
    uint8_t Red, Green, Blue;
    bool operator == (const Color_t AColor) { return ((Red == AColor.Red) and (Green == AColor.Green) and (Blue == AColor.Blue)); }
    bool operator != (const Color_t AColor) { return ((Red != AColor.Red) or  (Green != AColor.Green) or  (Blue != AColor.Blue)); }
    void operator = (const Color_t AColor) { Red = AColor.Red; Green = AColor.Green; Blue = AColor.Blue; }
    void Adjust(const Color_t *PColor) {
        if     (Red   < PColor->Red  ) Red++;
        else if(Red   > PColor->Red  ) Red--;
        if     (Green < PColor->Green) Green++;
        else if(Green > PColor->Green) Green--;
        if     (Blue  < PColor->Blue ) Blue++;
        else if(Blue  > PColor->Blue ) Blue--;
    }
    void Set(uint8_t R, uint8_t G, uint8_t B) { Red = R; Green = G; Blue = B; }
    void Get(uint8_t *PR, uint8_t *PG, uint8_t *PB) { *PR = Red; *PG = Green; *PB = Blue; }
};

// ==== Colors ====
#define clBlack     ((Color_t){0,   0,   0})
#define clRed       ((Color_t){255, 0,   0})
#define clGreen     ((Color_t){0,   255, 0})
#define clBlue      ((Color_t){0,   0,   255})
#define clYellow    ((Color_t){255, 255, 0})
#define clMagenta   ((Color_t){255, 0, 255})
#define clCyan      ((Color_t){0, 255, 255})
#define clWhite     ((Color_t){255, 255, 255})

#if 0 // ============================ Color table ==============================
const Color_t ColorTable[] = {
        {255,0,0},
        {255,24,0},
        {255,48,0},
        {255,71,0},
        {255,94,0},
        {255,117,0},
        {255,138,0},
        {255,158,0},
        {255,177,0},
        {255,195,0},
        {255,210,0},
        {255,224,0},
        {255,235,0},
        {255,244,0},
        {255,250,0},
        {255,254,0},
        {254,255,0},
        {250,255,0},
        {241,255,0},
        {228,255,0},
        {212,255,0},
        {193,255,0},
        {172,255,0},
        {150,255,0},
        {127,255,0},
        {104,255,0},
        {82,255,0},
        {61,255,0},
        {42,255,0},
        {26,255,0},
        {13,255,0},
        {4,255,0},
        {0,255,0},
        {0,255,1},
        {0,255,7},
        {0,255,17},
        {0,255,31},
        {0,255,48},
        {0,255,68},
        {0,255,89},
        {0,255,112},
        {0,255,135},
        {0,255,157},
        {0,255,179},
        {0,255,199},
        {0,255,217},
        {0,255,233},
        {0,255,244},
        {0,255,252},
        {0,255,255},
        {0,252,255},
        {0,244,255},
        {0,233,255},
        {0,217,255},
        {0,199,255},
        {0,179,255},
        {0,157,255},
        {0,135,255},
        {0,112,255},
        {0,89,255},
        {0,68,255},
        {0,48,255},
        {0,31,255},
        {0,17,255},
        {0,7,255},
        {0,1,255},
        {0,0,255},
        {4,0,255},
        {13,0,255},
        {26,0,255},
        {42,0,255},
        {61,0,255},
        {82,0,255},
        {104,0,255},
        {127,0,255},
        {150,0,255},
        {172,0,255},
        {193,0,255},
        {212,0,255},
        {228,0,255},
        {241,0,255},
        {250,0,255},
        {254,0,255},
        {255,0,254},
        {255,0,251},
        {255,0,245},
        {255,0,237},
        {255,0,227},
        {255,0,215},
        {255,0,201},
        {255,0,186},
        {255,0,169},
        {255,0,151},
        {255,0,131},
        {255,0,111},
        {255,0,90},
        {255,0,68},
        {255,0,45},
        {255,0,22},
        {255,0,0}
};
#define COLOR_TABLE_SZ  countof(ColorTable)
#endif

#endif /* COLOR_H_ */
