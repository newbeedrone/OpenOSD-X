#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "main.h"
#include "log.h"
#include "flash.h"
#include "mspvtx.h"
#include "setting.h"
#include "target.h"


const openosdx_setting_t openosdx_setting_default = {
        1,              // version
        27,             // channel
        POWER_LEVEL_25MW, // powerIndex (25mW)
        1,              // videoFormat
        1800,           // vref_init
        0x3c5e,         // magic
        0,              // pad1
        0,              // pad2
        0               // max_power_unlocked (default: locked)
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

    // Validate settings before using them as table indices.
    if (openosdx_setting.channel >= getFreqTableSize()) {
        DEBUG_PRINTF("Invalid channel %d, restoring default channel %d",
                     openosdx_setting.channel, openosdx_setting_default.channel);
        openosdx_setting.channel = openosdx_setting_default.channel;
    }

    if (openosdx_setting.powerIndex >= SA_NUM_POWER_LEVELS) {
        DEBUG_PRINTF("Invalid powerIndex %d, restoring default powerIndex %d",
                     openosdx_setting.powerIndex, openosdx_setting_default.powerIndex);
        openosdx_setting.powerIndex = openosdx_setting_default.powerIndex;
    }
    
    // Check power limit at startup based on ENABLE_MAX_POWER_UNLOCK macro
#if ENABLE_MAX_POWER_UNLOCK
    // When macro is enabled, max power is allowed by default (no unlock needed)
    // No need to limit power index
#else
    // When macro is disabled, max power requires button unlock
    bool maxPowerUnlocked = (openosdx_setting.max_power_unlocked == 0x5A5A);
    if (!maxPowerUnlocked && openosdx_setting.powerIndex >= POWER_LEVEL_MAX) {
        DEBUG_PRINTF("Max power locked, forcing powerIndex from %d to 2", openosdx_setting.powerIndex);
        openosdx_setting.powerIndex = POWER_LEVEL_800MW;
        // No immediate flash write, wait for setting_update to auto-save
    }
#endif

    DEBUG_PRINTF("vtx_init");
    setting_print();
}
