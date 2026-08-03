#ifndef __SETTING_H
#define __SETTING_H

typedef struct
{
    uint16_t version;
    uint16_t channel;
    uint16_t powerIndex;
    uint16_t videoFormat;
    uint16_t vref_init;
    uint16_t magic;
    uint16_t pad1;
    uint16_t pad2;
    uint16_t max_power_unlocked;  // 0x5A5A=MAX unlocked; any other value=locked
} openosdx_setting_t;


// Direct access to variables for faster performance
extern openosdx_setting_t openosdx_setting;


void setting_update(void);
openosdx_setting_t* setting(void);
void setting_init(void);


#endif
