#ifndef __TARGET_H
#define __TARGET_H

#include <stdint.h>


#define CAL_FREQ_SIZE 9
#define POWER_LEVEL_LABEL_LENGTH    3



#define TEMP_WARNING_DEG        70  /* warning temperrature (degree) */
#define TEMP_WARNING_POWERINDEX VPD_INDEX_MAX   /* warning power (vpd_table.calVpd[TEMP_WARNING_POWERINDEX]) - use MAX */
#define TEMP_DANGER_DEG         90  /* danger temparature (degree) */
#define TEMP_DANGER_POWERINDEX  VPD_INDEX_800MW /* danger power (vpd_table.calVpd[TEMP_DANGER_POWERINDEX]) - use 800mW */
#define VTX_DEFAULT_POWER_INDEX     0    /* Default power level index (25mW) */
#define VTX_TABLE_NEW_POWER_COUNT   4    /* vtx table power count */
#define SA_NUM_POWER_LEVELS         VTX_TABLE_NEW_POWER_COUNT
#define CAL_DBM_SIZE 4              /* VPD table size: all power levels use VPD tracking */
#define VREF_MAX_MV  3300           /* max vref voltage */

/* Power level indices - all use VPD tracking */
#define POWER_LEVEL_25MW     0      /* 25mW - uses VPD tracking (index 0 in VPD table) */
#define POWER_LEVEL_100MW    1      /* 100mW - uses VPD tracking (index 1 in VPD table) */
#define POWER_LEVEL_800MW    2      /* 800mW - uses VPD tracking (index 2 in VPD table) */
#define POWER_LEVEL_MAX      3      /* 2500mW/MAX - uses VPD tracking (index 3 in VPD table) */

/* VPD table indices for power levels that use VPD tracking */
#define VPD_INDEX_25MW       0      /* VPD table index for 25mW */
#define VPD_INDEX_100MW      1      /* VPD table index for 100mW */
#define VPD_INDEX_800MW      2      /* VPD table index for 800mW */
#define VPD_INDEX_MAX        3      /* VPD table index for MAX/2500mW */

/* TIM3 PWM Configuration */
#define TIM3_PWM_FREQ_HZ           1000        /* PWM frequency: 1KHz */
#define TIM3_PWM_PERIOD            10000       /* PWM period for 0.01% duty cycle resolution */
#define TIM3_PWM_PRESCALER         16          /* Prescaler: (170MHz / (16+1) / 10000) = 1000Hz */
#define TIM3_PWM_MAX_DUTY          TIM3_PWM_PERIOD  /* Maximum duty cycle value */

/* DAC to PWM conversion macros */
#define DAC_MAX_VALUE              4095        /* 12-bit DAC maximum value (0xFFF) */
#define DAC_VOLTAGE_MAX_MV         VREF_MAX_MV /* Maximum DAC output voltage in mV */
#define PWM_DUTY_FROM_DAC_VOLTAGE(mv)  ((uint32_t)((mv) * TIM3_PWM_PERIOD / DAC_VOLTAGE_MAX_MV))
#define PWM_DUTY_FROM_DAC_VALUE(dac_val)  ((uint32_t)((dac_val) * TIM3_PWM_PERIOD / DAC_MAX_VALUE))

/* Macro to update TIM3 PWM duty cycle from VTX power DAC output (for fan control, use in main loop) */
/* Note: Should use DAC1 CHANNEL_2 (VTX power control), not CHANNEL_1 (OSD) */
#define TIM3_PWM_UPDATE_FROM_DAC(dac_handle, dac_channel) \
    do { \
        uint32_t _dac_val = HAL_DAC_GetValue(dac_handle, dac_channel); \
        uint32_t _pwm_duty = PWM_DUTY_FROM_DAC_VALUE(_dac_val); \
        if (_pwm_duty > TIM3_PWM_MAX_DUTY) _pwm_duty = TIM3_PWM_MAX_DUTY; \
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 10000-_pwm_duty); \
    } while(0)

/* Macro to update TIM3 PWM duty cycle from VTX power DAC voltage in mV (for fan control) */
#define TIM3_PWM_UPDATE_FROM_DAC_VOLTAGE(mv) \
    do { \
        uint32_t _pwm_duty = PWM_DUTY_FROM_DAC_VOLTAGE(mv); \
        if (_pwm_duty > TIM3_PWM_MAX_DUTY) _pwm_duty = TIM3_PWM_MAX_DUTY; \
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 10000-_pwm_duty); \
    } while(0)

/* Macro to set VTX power DAC value and automatically update TIM3 PWM for fan control (replaces HAL_DAC_SetValue) */
/* Note: Should use DAC1 CHANNEL_2 (VTX power control), not CHANNEL_1 (OSD) */
#define HAL_DAC_SetValue_AND_UPDATE_PWM(dac_handle, dac_channel, alignment, value) \
    do { \
        HAL_DAC_SetValue(dac_handle, dac_channel, alignment, value); \
        TIM3_PWM_UPDATE_FROM_DAC(dac_handle, dac_channel); \
    } while(0)

/* Enable/disable max power unlock feature via button press */
/* Set to 1: max power (400mW) is allowed by default, no button unlock needed */
/* Set to 0: max power requires button unlock (both keys pressed for 5 seconds) */
/* Button unlock feature is only enabled when macro is 0 */
#ifndef ENABLE_MAX_POWER_UNLOCK
#define ENABLE_MAX_POWER_UNLOCK    1    /* 1=default unlocked, 0=requires button unlock */
#endif


#ifndef TARGET_NOVTX
typedef struct vpd_table_def {
    char magic[4];
    uint16_t calFreqs[CAL_FREQ_SIZE];
    uint8_t calDBm[CAL_DBM_SIZE];
    uint16_t calVpd[CAL_DBM_SIZE][CAL_FREQ_SIZE];
} vpd_table_t;
extern const vpd_table_t vpd_table;
extern uint8_t saPowerLevelsLut[SA_NUM_POWER_LEVELS];
extern uint8_t saPowerLevelsLabel[SA_NUM_POWER_LEVELS * POWER_LEVEL_LABEL_LENGTH];
#endif


#endif

