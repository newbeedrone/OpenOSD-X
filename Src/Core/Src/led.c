
#include "main.h"
#include "led.h"


#define WS2812B_0   51          /* 0.3us */
#define WS2812B_1   111         /* 0.65us */

TIM_HandleTypeDef htim8;

// Green (R=0, G=51, B=0)
const uint32_t led_green128[] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,                        /* reset 1.25us*240 = 300us */
    /* LED 1 */
    WS2812B_0,WS2812B_0,WS2812B_1,WS2812B_1,WS2812B_0,WS2812B_0,WS2812B_1,WS2812B_1, /* G: 51 (20% brightness) */
    WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0, /* R: 0 */
    WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0, /* B: 0 */
    /* LED 2 */
    WS2812B_0,WS2812B_0,WS2812B_1,WS2812B_1,WS2812B_0,WS2812B_0,WS2812B_1,WS2812B_1, /* G: 51 */
    WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0, /* R: 0 */
    WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0, /* B: 0 */
    0xffffffff
};

// Blue (R=0, G=0, B=51)
const uint32_t led_blue128[] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,                        /* reset 1.25us*240 = 300us */
    /* LED 1 */
    WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0, /* G: 0 */
    WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0, /* R: 0 */
    WS2812B_0,WS2812B_0,WS2812B_1,WS2812B_1,WS2812B_0,WS2812B_0,WS2812B_1,WS2812B_1, /* B: 51 (20% brightness) */
    /* LED 2 */
    WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0, /* G: 0 */
    WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0, /* R: 0 */
    WS2812B_0,WS2812B_0,WS2812B_1,WS2812B_1,WS2812B_0,WS2812B_0,WS2812B_1,WS2812B_1, /* B: 51 */
    0xffffffff
};

// Red (R=51, G=0, B=0)
const uint32_t led_red128[] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,                        /* reset 1.25us*240 = 300us */
    /* LED 1 */
    WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0, /* G: 0 */
    WS2812B_0,WS2812B_0,WS2812B_1,WS2812B_1,WS2812B_0,WS2812B_0,WS2812B_1,WS2812B_1, /* R: 51 (20% brightness) */
    WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0, /* B: 0 */
    /* LED 2 */
    WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0, /* G: 0 */
    WS2812B_0,WS2812B_0,WS2812B_1,WS2812B_1,WS2812B_0,WS2812B_0,WS2812B_1,WS2812B_1, /* R: 51 */
    WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0, /* B: 0 */
    0xffffffff
};

const uint32_t led_off[] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,                        /* reset 1.25us*240 = 300us */
    /* LED 1 */
    WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0, /* G */
    WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0, /* R */
    WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0, /* B */
    /* LED 2 */
    WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0, /* G */
    WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0, /* R */
    WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0, /* B */
    0xffffffff
};

// White (R=51, G=51, B=51) - 20% brightness
const uint32_t led_white[] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    /* LED 1 */
    WS2812B_0,WS2812B_0,WS2812B_1,WS2812B_1,WS2812B_0,WS2812B_0,WS2812B_1,WS2812B_1, /* G: 51 */
    WS2812B_0,WS2812B_0,WS2812B_1,WS2812B_1,WS2812B_0,WS2812B_0,WS2812B_1,WS2812B_1, /* R: 51 */
    WS2812B_0,WS2812B_0,WS2812B_1,WS2812B_1,WS2812B_0,WS2812B_0,WS2812B_1,WS2812B_1, /* B: 51 */
    /* LED 2 */
    WS2812B_0,WS2812B_0,WS2812B_1,WS2812B_1,WS2812B_0,WS2812B_0,WS2812B_1,WS2812B_1, /* G: 51 */
    WS2812B_0,WS2812B_0,WS2812B_1,WS2812B_1,WS2812B_0,WS2812B_0,WS2812B_1,WS2812B_1, /* R: 51 */
    WS2812B_0,WS2812B_0,WS2812B_1,WS2812B_1,WS2812B_0,WS2812B_0,WS2812B_1,WS2812B_1, /* B: 51 */
    0xffffffff
};

// Pink (R=51, G=21, B=36)
const uint32_t led_pink[] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    /* LED 1 */
    WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_1,WS2812B_0,WS2812B_1,WS2812B_0,WS2812B_1, /* G: 21 */
    WS2812B_0,WS2812B_0,WS2812B_1,WS2812B_1,WS2812B_0,WS2812B_0,WS2812B_1,WS2812B_1, /* R: 51 */
    WS2812B_0,WS2812B_0,WS2812B_1,WS2812B_0,WS2812B_0,WS2812B_1,WS2812B_0,WS2812B_0, /* B: 36 */
    /* LED 2 */
    WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_1,WS2812B_0,WS2812B_1,WS2812B_0,WS2812B_1, /* G: 21 */
    WS2812B_0,WS2812B_0,WS2812B_1,WS2812B_1,WS2812B_0,WS2812B_0,WS2812B_1,WS2812B_1, /* R: 51 */
    WS2812B_0,WS2812B_0,WS2812B_1,WS2812B_0,WS2812B_0,WS2812B_1,WS2812B_0,WS2812B_0, /* B: 36 */
    0xffffffff
};

// Cyan (R=0, G=51, B=51)
const uint32_t led_cyan[] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    /* LED 1 */
    WS2812B_0,WS2812B_0,WS2812B_1,WS2812B_1,WS2812B_0,WS2812B_0,WS2812B_1,WS2812B_1, /* G: 51 */
    WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0, /* R: 0 */
    WS2812B_0,WS2812B_0,WS2812B_1,WS2812B_1,WS2812B_0,WS2812B_0,WS2812B_1,WS2812B_1, /* B: 51 */
    /* LED 2 */
    WS2812B_0,WS2812B_0,WS2812B_1,WS2812B_1,WS2812B_0,WS2812B_0,WS2812B_1,WS2812B_1, /* G: 51 */
    WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0, /* R: 0 */
    WS2812B_0,WS2812B_0,WS2812B_1,WS2812B_1,WS2812B_0,WS2812B_0,WS2812B_1,WS2812B_1, /* B: 51 */
    0xffffffff
};

// Yellow (R=51, G=51, B=0)
const uint32_t led_yellow[] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    /* LED 1 */
    WS2812B_0,WS2812B_0,WS2812B_1,WS2812B_1,WS2812B_0,WS2812B_0,WS2812B_1,WS2812B_1, /* G: 51 */
    WS2812B_0,WS2812B_0,WS2812B_1,WS2812B_1,WS2812B_0,WS2812B_0,WS2812B_1,WS2812B_1, /* R: 51 */
    WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0, /* B: 0 */
    /* LED 2 */
    WS2812B_0,WS2812B_0,WS2812B_1,WS2812B_1,WS2812B_0,WS2812B_0,WS2812B_1,WS2812B_1, /* G: 51 */
    WS2812B_0,WS2812B_0,WS2812B_1,WS2812B_1,WS2812B_0,WS2812B_0,WS2812B_1,WS2812B_1, /* R: 51 */
    WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0, /* B: 0 */
    0xffffffff
};

// Purple (R=26, G=0, B=26)
const uint32_t led_purple[] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    /* LED 1 */
    WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0, /* G: 0 */
    WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_1,WS2812B_1,WS2812B_0,WS2812B_1,WS2812B_0, /* R: 26 */
    WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_1,WS2812B_1,WS2812B_0,WS2812B_1,WS2812B_0, /* B: 26 */
    /* LED 2 */
    WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_0, /* G: 0 */
    WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_1,WS2812B_1,WS2812B_0,WS2812B_1,WS2812B_0, /* R: 26 */
    WS2812B_0,WS2812B_0,WS2812B_0,WS2812B_1,WS2812B_1,WS2812B_0,WS2812B_1,WS2812B_0, /* B: 26 */
    0xffffffff
};


static void sendLed(const uint32_t *led, uint16_t len)
{
    // The PWM generation timer will not be stopped after the transfer is complete - it will simply be left running.
    // Since the output remains high until the next transfer, this poses no problem.
    // This is intended to reduce interrupts and processing overhead.

    LL_TIM_DisableCounter(TIM8);
    LL_DMA_DisableChannel(DMA2, LL_DMA_CHANNEL_2);
    LL_TIM_EnableAllOutputs(TIM8);
    LL_DMA_ConfigTransfer(DMA2,
                            LL_DMA_CHANNEL_2,
                            LL_DMA_DIRECTION_MEMORY_TO_PERIPH | LL_DMA_PRIORITY_LOW | LL_DMA_MODE_NORMAL |
                            LL_DMA_PERIPH_NOINCREMENT | LL_DMA_MEMORY_INCREMENT |
                            LL_DMA_PDATAALIGN_WORD | LL_DMA_MDATAALIGN_WORD);
    LL_DMA_ConfigAddresses(DMA2,
                            LL_DMA_CHANNEL_2,
                            (uint32_t)led, (uint32_t)&(TIM8->CCR1),
                            LL_DMA_GetDataTransferDirection(DMA2, LL_DMA_CHANNEL_2));

    LL_DMA_SetDataLength(DMA2, LL_DMA_CHANNEL_2, len/4);
    LL_DMA_EnableChannel(DMA2, LL_DMA_CHANNEL_2);
    LL_TIM_ConfigDMABurst(TIM8, LL_TIM_DMABURST_BASEADDR_CCR1, LL_TIM_DMABURST_LENGTH_1TRANSFER);
    LL_TIM_EnableDMAReq_UPDATE(TIM8);
    TIM8->CCR1 = 0;
    LL_TIM_CC_EnableChannel(TIM8, LL_TIM_CHANNEL_CH1);
    LL_TIM_EnableCounter(TIM8);
}


void initLed(void)
{


  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  htim8.Instance = TIM8;
  htim8.Init.Prescaler = 0;
  htim8.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim8.Init.Period = 212;
  htim8.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim8.Init.RepetitionCounter = 0;
  htim8.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim8) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim8, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim8) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim8, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_SET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim8, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.BreakFilter = 0;
  sBreakDeadTimeConfig.BreakAFMode = TIM_BREAK_AFMODE_INPUT;
  sBreakDeadTimeConfig.Break2State = TIM_BREAK2_DISABLE;
  sBreakDeadTimeConfig.Break2Polarity = TIM_BREAK2POLARITY_HIGH;
  sBreakDeadTimeConfig.Break2Filter = 0;
  sBreakDeadTimeConfig.Break2AFMode = TIM_BREAK_AFMODE_INPUT;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim8, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_TIM_MspPostInit(&htim8);

    HAL_TIM_Base_MspInit(&htim8);
    LL_TIM_CC_EnableChannel(TIM8, LL_TIM_CHANNEL_CH1);
    LL_TIM_EnableARRPreload(TIM8);
    LL_TIM_EnableCounter(TIM8);


    setLed(LED_OFF);
}


// RGB color value lookup table (G, R, B)
typedef struct {
    uint8_t g;
    uint8_t r;
    uint8_t b;
} RGB_Color;

static const RGB_Color color_table[] = {
    {0,   0,   0},   // LED_OFF
    {51,  51,  51},  // LED_WHITE
    {21,  51,  36},  // LED_PINK
    {51,  0,   51},  // LED_CYAN
    {51,  51,  0},   // LED_YELLOW
    {51,  0,   0},   // LED_GREEN128
    {0,   0,   51},  // LED_BLUE128
    {0,   26,  26},  // LED_PURPLE
    {0,   51,  0}    // LED_RED128
};

// Convert single byte to WS2812B PWM data
static void byte_to_pwm(uint8_t byte, uint32_t *buffer) {
    for(int i = 0; i < 8; i++) {
        if(byte & (0x80 >> i)) {
            buffer[i] = WS2812B_1;
        } else {
            buffer[i] = WS2812B_0;
        }
    }
}

// Control two LEDs independently
void setLedDual(LED_STATE led1, LED_STATE led2)
{
    static uint32_t dual_led_buffer[240 + 48 + 1];  // reset(240) + LED1(24) + LED2(24) + end(1)
    
    // Fill reset time (300us)
    for(int i = 0; i < 240; i++) {
        dual_led_buffer[i] = 0;
    }
    
    // LED1 (controlled by band key)
    RGB_Color c1 = color_table[led1];
    byte_to_pwm(c1.g, &dual_led_buffer[240]);      // G
    byte_to_pwm(c1.r, &dual_led_buffer[240 + 8]);  // R
    byte_to_pwm(c1.b, &dual_led_buffer[240 + 16]); // B
    
    // LED2 (controlled by power key)
    RGB_Color c2 = color_table[led2];
    byte_to_pwm(c2.g, &dual_led_buffer[240 + 24]);      // G
    byte_to_pwm(c2.r, &dual_led_buffer[240 + 24 + 8]);  // R
    byte_to_pwm(c2.b, &dual_led_buffer[240 + 24 + 16]); // B
    
    // End marker
    dual_led_buffer[240 + 48] = 0xffffffff;
    
    sendLed(dual_led_buffer, sizeof(dual_led_buffer));
}

void setLed(LED_STATE led)
{
    switch(led){
        case LED_OFF:
            sendLed(led_off, sizeof(led_off));
            break;
        case LED_WHITE:
            sendLed(led_white, sizeof(led_white));
            break;
        case LED_PINK:
            sendLed(led_pink, sizeof(led_pink));
            break;
        case LED_CYAN:
            sendLed(led_cyan, sizeof(led_cyan));
            break;
        case LED_YELLOW:
            sendLed(led_yellow, sizeof(led_yellow));
            break;
        case LED_GREEN128:
            sendLed(led_green128, sizeof(led_green128));
            break;
        case LED_BLUE128:
            sendLed(led_blue128, sizeof(led_blue128));
            break;
        case LED_PURPLE:
            sendLed(led_purple, sizeof(led_purple));
            break;
        case LED_RED128:
            sendLed(led_red128, sizeof(led_red128));
            break;
    }
}
