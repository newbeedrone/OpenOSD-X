#ifndef __LED_H
#define __LED_H

typedef enum {
    LED_OFF=0,
    LED_WHITE,       // 白色
    LED_PINK,        // 粉色
    LED_CYAN,        // 青色
    LED_YELLOW,      // 黄色
    LED_GREEN128,    // 绿色
    LED_BLUE128,     // 蓝色
    LED_PURPLE,      // 紫色
    LED_RED128       // 红色
} LED_STATE;

void initLed(void);
void setLed(LED_STATE led);
void setLedDual(LED_STATE led1, LED_STATE led2);  // LED1: band, LED2: power

#endif
