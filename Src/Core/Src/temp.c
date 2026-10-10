#include <stdbool.h>
#include <stdio.h>
#include "target.h"
#include "main.h"
#include "char_canvas.h"
#include "rtc6705.h"
#include "log.h"
#include "setting.h"
#include "temp.h"

#ifndef TARGET_NOVTX

#define TEMP_ADC_CONV_TIME_MS       2000         // ms

typedef enum {
    TEMP_STATE_INIT = 0,
    TEMP_STATE_IDLE,
    TEMP_STATE_CALIB_START,
    TEMP_STATE_CALIB_DELAY,
    TEMP_STATE_ADC_ENABLE,
    TEMP_STATE_ADC_CONVERSION,
    TEMP_STATE_DISABLE,

}TEMP_STATE;

char * temptate_str[] =
{
    "TEMP_STATE_INIT",
    "TEMP_STATE_IDLE",
    "TEMP_STATE_CALIB_START",
    "TEMP_STATE_CALIB_DELAY",
    "TEMP_STATE_ADC_ENABLE",
    "TEMP_STATE_ADC_CONVERSION",
    "TEMP_STATE_DISABLE"
};



TEMP_STATE temp_state = TEMP_STATE_INIT;
static uint32_t next_state_timer = 0;
static int32_t temp_val = 0;



void initTemp(void)
{
    temp_state = TEMP_STATE_IDLE;
    temp_val = 0;
}



int32_t getTemp(void)
{
    return temp_val;
}



bool procTemp(void)
{
    bool newdata = false;

    uint32_t now = HAL_GetTick();

    switch(temp_state){
        case TEMP_STATE_INIT:
            break;
        case TEMP_STATE_IDLE:
            if ( (now - next_state_timer) >= TEMP_ADC_CONV_TIME_MS ){
                //DEBUG_PRINTF("adc_calibration");
                LL_ADC_StartCalibration(ADC1, LL_ADC_SINGLE_ENDED);
                temp_state = TEMP_STATE_CALIB_START;
                //DEBUG_PRINTF("temp_state:TEMP_STATE_CALIB_START");
            }
            break;
        case TEMP_STATE_CALIB_START:
            if ( LL_ADC_IsCalibrationOnGoing(ADC1) == 0){
                temp_state = TEMP_STATE_CALIB_DELAY;
                next_state_timer = now;
            }
            break;
        case TEMP_STATE_CALIB_DELAY:
            if ( (now - next_state_timer) > 2 ){
                LL_ADC_Enable(ADC1);
                temp_state = TEMP_STATE_ADC_ENABLE;
            }
            break;
        case TEMP_STATE_ADC_ENABLE:
            if (LL_ADC_IsActiveFlag_ADRDY(ADC1) == 1){
                LL_ADC_ClearFlag_EOC(ADC1);
                LL_ADC_REG_StartConversion(ADC1);
                temp_state = TEMP_STATE_ADC_CONVERSION;
            }
            break;
        case TEMP_STATE_ADC_CONVERSION:
            if (LL_ADC_IsActiveFlag_EOC(ADC1) == 1){
                LL_ADC_ClearFlag_EOC(ADC1);
                uint32_t val = (LL_ADC_REG_ReadConversionData12(ADC1)) >> 4;
                temp_val = __HAL_ADC_CALC_TEMPERATURE(ANALOG_REFERENCE_MV, val, ADC_RESOLUTION_12B);
                DEBUG_PRINTF("temp:%d", temp_val);
                temp_state = TEMP_STATE_DISABLE;
                LL_ADC_Disable(ADC1);
                newdata = true;
            }
            break;
        case TEMP_STATE_DISABLE:
            if (!LL_ADC_IsEnabled(ADC1)){
                temp_state = TEMP_STATE_IDLE;
            }
            break;
    }
    return newdata;
}

#endif

