#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "main.h"
#include "log.h"
#include "flash.h"
#include "mspvtx.h"
#include "setting.h"


const openosdx_setting_t openosdx_setting_default = {
        1,              // version
        27,             // channel
        2,              // powerIndex
        1,              // videoFormat
        1800,           // vref_init
        0x3c5e,         // magic
        0,              // max_power_unlocked (default: locked)
        0               // pad2    
    };

openosdx_setting_t openosdx_setting  __attribute__((aligned(8)));
openosdx_setting_t openosdx_setting_backup  __attribute__((aligned(8)));

__attribute__((section(".setting")))
openosdx_setting_t flash_setting;



void setting_print(void)
{
#ifdef DEV_MODE
    UNUSED(setting);
#endif
    DEBUG_PRINTF(" version:%d", openosdx_setting.version);
    DEBUG_PRINTF(" channel:%d (%c%d)", openosdx_setting.channel, getBandLetterByIdx(openosdx_setting.channel/8), (openosdx_setting.channel%8)+1);
    DEBUG_PRINTF(" powerIndex:%d", openosdx_setting.powerIndex);
    DEBUG_PRINTF(" videoFormat:%d", openosdx_setting.videoFormat);
    DEBUG_PRINTF(" vref_init:%d", openosdx_setting.vref_init);
    DEBUG_PRINTF(" max_power_unlocked:%s", openosdx_setting.max_power_unlocked == 0x5A5A ? "YES" : "NO");
    

}

void setting_update(void)
{
    static uint32_t update_time = 0;
    uint32_t now = HAL_GetTick();

    if (memcmp(&openosdx_setting_backup, &openosdx_setting, sizeof(openosdx_setting_t))){
        memcpy(&openosdx_setting_backup, &openosdx_setting, sizeof(openosdx_setting_t));
        update_time = now;
        DEBUG_PRINTF("setting_update");
        setting_print();
    }
    if ( update_time ){
        if ( now - update_time > 1000){ // 1sec
            update_time = 0;
            flash_erase((uint32_t)&flash_setting, sizeof(openosdx_setting_t));
            flash_write((uint32_t)&flash_setting, (uint8_t*)&openosdx_setting, sizeof(openosdx_setting_t));
            DEBUG_PRINTF("setting_flash");
            setting_print();
        }
    }
}

openosdx_setting_t* setting(void)
{
    return &openosdx_setting;
}

void setting_init(void)
{
    memcpy(&openosdx_setting, &flash_setting, sizeof(openosdx_setting));
    memcpy(&openosdx_setting_backup, &openosdx_setting, sizeof(openosdx_setting));

    if (    openosdx_setting.version != openosdx_setting_default.version ||
            openosdx_setting.magic != openosdx_setting_default.magic) {
                DEBUG_PRINTF("vtx_default");
                memcpy(&openosdx_setting, &openosdx_setting_default, sizeof(openosdx_setting));
                memcpy(&openosdx_setting_backup, &openosdx_setting_default, sizeof(openosdx_setting_t));
                flash_erase((uint32_t)&flash_setting, sizeof(openosdx_setting_t));
                flash_write((uint32_t)&flash_setting, (uint8_t*)&openosdx_setting, sizeof(openosdx_setting_t));
    }
    
    // Check power limit at startup based on ENABLE_MAX_POWER_UNLOCK macro
#if ENABLE_MAX_POWER_UNLOCK
    // When macro is enabled, max power is allowed by default (no unlock needed)
    // No need to limit power index
#else
    // When macro is disabled, max power requires button unlock
    bool maxPowerUnlocked = (openosdx_setting.max_power_unlocked == 0x5A5A);
    if (!maxPowerUnlocked && openosdx_setting.powerIndex >= 3) {
        DEBUG_PRINTF("Max power locked, forcing powerIndex from %d to 2", openosdx_setting.powerIndex);
        openosdx_setting.powerIndex = 2;  // Force to 100mW
        // No immediate flash write, wait for setting_update to auto-save
    }
#endif

    DEBUG_PRINTF("vtx_init");
    setting_print();
}

