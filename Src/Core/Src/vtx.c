#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include "target.h"
#include "main.h"
#include "char_canvas.h"
#include "rtc6705.h"
#include "log.h"
#include "setting.h"
#include "temp.h"
#include "vtx.h"

#ifndef TARGET_NOVTX

#define PLL_STABLE_TIME_MS      500
#define POWER_STABLE_TIME_MS    100
#define ADC_CONV_TIME_MS        10         // ms

typedef enum {
    VTX_STATE_INIT = 0,
    VTX_STATE_INIT_RTC6705,
    VTX_STATE_PLL_STABLE,
    VTX_STATE_POWER_STABLE,
    VTX_STATE_IDLE,
    VTX_STATE_CALIB_START,
    VTX_STATE_CALIB_DELAY,
    VTX_STATE_ADC_ENABLE,
    VTX_STATE_ADC_CONVERSION,
    VTX_STATE_DISABLE,

}VTX_STATE;

char * vtxstate_str[] =
{
    "VTX_STATE_INIT",
    "VTX_STATE_INIT_RTC6705",
    "VTX_STATE_PLL_STABLE",
    "VTX_STATE_POWER_STABLE",
    "VTX_STATE_IDLE",
    "VTX_STATE_CALIB_START",
    "VTX_STATE_CALIB_DELAY",
    "VTX_STATE_ADC_ENABLE",
    "VTX_STATE_ADC_CONVERSION",
    "VTX_STATE_DISABLE"
};

const vpd_table_t *vpdt = &vpd_table;
uint16_t target_vpd = 0;
uint16_t target_vpd_normal = 0;
uint16_t vpd = 0;
uint16_t vpd_max = 0;
uint16_t vpd_min = UINT16_MAX;
int32_t vref = 0;
int32_t vpd_error;
uint16_t vpd_save_count = 0;
uint16_t vtx_freq = 0;
uint32_t next_state_timer = 0;
uint32_t temperature = 0;
VTX_STATE vtx_state = VTX_STATE_INIT;
static uint8_t dac_power_index = VPD_INDEX_25MW;

static uint16_t interpolateCalibration(uint16_t freq, uint16_t left, uint16_t right,
                                      uint8_t column)
{
    uint32_t low = vpdt->calFreqs[column];
    uint32_t high = vpdt->calFreqs[column + 1];
    if (freq <= low) {
        return left;
    }
    if (freq >= high) {
        return right;
    }
    uint32_t span = high - low;
    uint32_t weight = freq - low;
    return (left * (span - weight) + right * weight + span / 2U) / span;
}

static uint8_t calibrationColumn(uint16_t freq)
{
    uint8_t column = 0;
    while (column < CAL_FREQ_SIZE - 2 && freq > vpdt->calFreqs[column + 1]) {
        ++column;
    }
    return column;
}

static bool validDacRange(dac_voltage_range_t range)
{
    uint32_t min_code = ((uint32_t)range.min_mv * 4095U + ANALOG_REFERENCE_MV - 1U)
                        / ANALOG_REFERENCE_MV;
    uint32_t max_code = (uint32_t)range.max_mv * 4095U / ANALOG_REFERENCE_MV;
    return range.min_mv <= range.max_mv && range.max_mv <= VREF_MAX_MV
           && min_code <= max_code;
}

static bool validCalibrationTable(const volatile vpd_table_t *table)
{
    if (table->magic[0] != 'V' || table->magic[1] != 'P'
        || table->magic[2] != 'D' || table->magic[3] != '2') {
        return false;
    }
    for (uint8_t column = 0; column < CAL_FREQ_SIZE; ++column) {
        if (table->calFreqs[column] < 5000 || table->calFreqs[column] > 6500
            || (column && table->calFreqs[column] <= table->calFreqs[column - 1])) {
            return false;
        }
        for (uint8_t row = 0; row < CAL_DBM_SIZE; ++row) {
            if (table->calDBm[row] != vpd_table.calDBm[row]
                || table->calVpd[row][column] > ANALOG_REFERENCE_MV
                || !validDacRange(table->calDacMv[row][column])) {
                return false;
            }
        }
    }
    return true;
}

static dac_voltage_range_t dacVoltageLimits(void)
{
    if (dac_power_index >= CAL_DBM_SIZE || target_vpd_normal == 0) {
        return (dac_voltage_range_t){0, 0};
    }
    uint8_t column = calibrationColumn(vtx_freq);
    dac_voltage_range_t left = vpdt->calDacMv[dac_power_index][column];
    dac_voltage_range_t right = vpdt->calDacMv[dac_power_index][column + 1];
    return (dac_voltage_range_t){
        interpolateCalibration(vtx_freq, left.min_mv, right.min_mv, column),
        interpolateCalibration(vtx_freq, left.max_mv, right.max_mv, column)
    };
}

static void writeAutomaticVref(void)
{
    dac_voltage_range_t range = dacVoltageLimits();
    vref = (vref < range.min_mv) ? range.min_mv : vref;
    vref = (vref > range.max_mv) ? range.max_mv : vref;
    uint32_t code = ((uint32_t)vref * 4095U + ANALOG_REFERENCE_MV / 2U) / ANALOG_REFERENCE_MV;
    uint32_t min_code = ((uint32_t)range.min_mv * 4095U + ANALOG_REFERENCE_MV - 1U)
                        / ANALOG_REFERENCE_MV;
    uint32_t max_code = (uint32_t)range.max_mv * 4095U / ANALOG_REFERENCE_MV;
    code = (code < min_code) ? min_code : code;
    code = (code > max_code) ? max_code : code;
    LL_DAC_ConvertData12RightAligned(DAC1, LL_DAC_CHANNEL_2, code);
}

__attribute__((section(".vpdtable")))
const volatile vpd_table_t adj_vpdtable;




void initVtx(void)
{
    target_vpd = 0;
    target_vpd_normal = 0;
    vtx_freq = 0;
    dac_power_index = VPD_INDEX_25MW;
    vpdt = &vpd_table;

    // Check if there is an adjustment table.
    if (validCalibrationTable(&adj_vpdtable)) {
        vpdt = (const vpd_table_t*)&adj_vpdtable;
    }

    vtx_state = VTX_STATE_INIT;
    HAL_DAC_Start(&hdac1, DAC_CHANNEL_2);
    rtc6705PowerAmpOff();
    initRtc6705();
}


uint16_t bilinearInterpolation(uint16_t freq, uint8_t calDBmIndex)
{
    uint8_t column = calibrationColumn(freq);
    return interpolateCalibration(freq, vpdt->calVpd[calDBmIndex][column],
                                  vpdt->calVpd[calDBmIndex][column + 1], column);
}


void update_vpd(void)
{
    uint16_t vpd_danger, vpd_warning, vpd_limit;

    temperature = getTemp();

    vpd_danger  = bilinearInterpolation(vtx_freq, TEMP_DANGER_POWERINDEX);
    vpd_warning = bilinearInterpolation(vtx_freq, TEMP_WARNING_POWERINDEX);
    if (temperature <= TEMP_WARNING_DEG){
        vpd_limit = UINT16_MAX;
    }else if (temperature >= TEMP_DANGER_DEG){
        vpd_limit = vpd_danger;
    }else{
        float ratio = (float)(temperature - TEMP_WARNING_DEG) / (TEMP_DANGER_DEG - TEMP_WARNING_DEG);
        vpd_limit = vpd_warning - (uint16_t)((vpd_warning - vpd_danger) * ratio);
    }

    target_vpd = (vpd_limit < target_vpd_normal) ? vpd_limit : target_vpd_normal;
}

void setVtx_vpd(uint16_t freq, uint16_t vpd)
{
    bool renew = false;

    if (vtx_freq != freq){
        vtx_freq = freq;
        rtc6705PowerAmpOff();
        initRtc6705();
        vtx_state = VTX_STATE_INIT_RTC6705;
        renew = true;
    }
    if (target_vpd_normal != vpd){
        target_vpd_normal = vpd;
        update_vpd();
        renew = true;
    }
    writeAutomaticVref();
    if (renew){
        debuglogVtx("setVtx_vpd");
    }
}

uint8_t db2caldbmindex(uint8_t dB)
{
    uint8_t index;

    for (index = 0; index < CAL_DBM_SIZE; index++){
        if (dB == vpdt->calDBm[index]){
            break;
        }
    }
    if (index >= CAL_DBM_SIZE){
        index = 0;    // default
    }
    return index;
}

void setVtx(uint16_t freq, uint8_t dB)
{
    if (dB < 10){
        dac_power_index = UINT8_MAX;
        setVtx_vpd(freq, 0);
    }else{
        dac_power_index = db2caldbmindex(dB);
        setVtx_vpd(freq, bilinearInterpolation(freq, dac_power_index));
    }
    /* TIM3 CH1 has HIGH polarity; the PB4 fan is ON when the pin is LOW. */
    LL_TIM_OC_SetMode(TIM3, LL_TIM_CHANNEL_CH1,
        (dac_power_index >= POWER_LEVEL_800MW && dac_power_index < CAL_DBM_SIZE)
        ? LL_TIM_OCMODE_FORCED_INACTIVE : LL_TIM_OCMODE_FORCED_ACTIVE);
    LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH1);
    LL_TIM_EnableCounter(TIM3);
}

uint16_t getVtxFreq(void)
{
    return vtx_freq;
}

uint16_t getVpdTarget(void)
{
    return target_vpd_normal;
}

uint16_t getVpd(void)
{
    return vpd;
}

uint16_t getVref(void)
{
    return vref;
}

void vrefUpdate(void)
{
    LL_ADC_ClearFlag_EOC(ADC2);
    vpd = ((uint32_t)LL_ADC_REG_ReadConversionData12(ADC2) * ANALOG_REFERENCE_MV)
            / VPD_ADC_FULL_SCALE;
    if (vpd_max < vpd){
        vpd_max = vpd;
    }
    if (vpd_min > vpd){
        vpd_min = vpd;
    }

    vpd_error = (int32_t)vpd - target_vpd;
    if (vpd_error < -100){
        DEBUG_PRINTF("vpd_error %d", vpd_error);
        vpd_save_count = 100;
        vref += 10;
        //debuglogVtx();
    }else if (vpd_error < 0){
        vref += 1;
    }else if (vpd_error == 0){
        ;
    }else if (vpd_error < 100){
        vref -= 1;
    }else{
        DEBUG_PRINTF("vpd_error %d", vpd_error);
        vpd_save_count = 100;
        vref -= 10;
	    //debuglogVtx();
    }
    writeAutomaticVref();

	// save vref
    if (vpd_save_count != 0){
        if (--vpd_save_count == 0){
            if (temperature < TEMP_WARNING_DEG){
                setting()->vref_init = ((uint32_t)vref * LEGACY_ANALOG_REFERENCE_MV
                                       + ANALOG_REFERENCE_MV / 2U) / ANALOG_REFERENCE_MV;
                DEBUG_PRINTF("save vref");
            }
        }
    }
}

void debuglogVtx(char* head)
{
#if USE_RTT
    DEBUG_PRINTF("%s %s vpd:%d(%d-%d) target:%d(%d) error:%d vref:%d temp:%d vtx_freq:%d %s", 
        head, 
        vtxstate_str[vtx_state], 
        vpd,  
        vpd_min,
        vpd_max,
        target_vpd, 
        target_vpd_normal, 
        vpd_error, 
        vref, 
        temperature, 
        vtx_freq,
        (vpdt == &adj_vpdtable) ? "adj_vpdtable" : ""
        );
    vpd_min = UINT16_MAX;
    vpd_max = 0;
#else
    UNUSED(head);
#endif
}

void procVtx(void)
{

    uint32_t now = HAL_GetTick();

    switch(vtx_state){
        case VTX_STATE_INIT:
            break;
        case VTX_STATE_INIT_RTC6705:
            if (initRtc6705check()){
                rtc6705PowerAmpOff();
                rtc6705WriteFrequency(vtx_freq);
                next_state_timer = now;
                vtx_state = VTX_STATE_PLL_STABLE;
                DEBUG_PRINTF("vtx_state:VTX_STATE_PLL_STABLE");
            }
            break;
        case VTX_STATE_PLL_STABLE:
            if ( (now - next_state_timer) >= PLL_STABLE_TIME_MS ){
                dac_voltage_range_t range = dacVoltageLimits();
                vref = ((uint32_t)range.min_mv + range.max_mv) / 2U;
                vpd_save_count = 0;
                writeAutomaticVref();
                if (target_vpd_normal != 0) {
                    rtc6705PowerAmpOn();
                }
                vtx_state = VTX_STATE_POWER_STABLE;
                DEBUG_PRINTF("vtx_state:VTX_STATE_POWER_STABLE");
                next_state_timer = now;
            }
            break;
        case VTX_STATE_POWER_STABLE:
            if ( (now - next_state_timer) >= POWER_STABLE_TIME_MS ){
                vtx_state = VTX_STATE_IDLE;
                DEBUG_PRINTF("vtx_state:VTX_STATE_IDLE");
                next_state_timer = now;
            }
            break;
        case VTX_STATE_IDLE:
            if ( (now - next_state_timer) >= ADC_CONV_TIME_MS ){
                //HAL_GPIO_WritePin(DEBUG_GPIO_Port, DEBUG_Pin, GPIO_PIN_SET);
                //DEBUG_PRINTF("adc_calibration");
                LL_ADC_StartCalibration(ADC2, LL_ADC_SINGLE_ENDED);
                vtx_state = VTX_STATE_CALIB_START;
                //DEBUG_PRINTF("vtx_state:VTX_STATE_CALIB_START");
            }
            break;
        case VTX_STATE_CALIB_START:
            if ( LL_ADC_IsCalibrationOnGoing(ADC2) == 0){
                vtx_state = VTX_STATE_CALIB_DELAY;
                next_state_timer = now;
            }
            break;
        case VTX_STATE_CALIB_DELAY:
            if ( (now - next_state_timer) > 2 ){
                LL_ADC_Enable(ADC2);
                vtx_state = VTX_STATE_ADC_ENABLE;
            }
            break;
        case VTX_STATE_ADC_ENABLE:
            if (LL_ADC_IsActiveFlag_ADRDY(ADC2) == 1){
                LL_ADC_ClearFlag_EOC(ADC2);
                LL_ADC_REG_StartConversion(ADC2);
                vtx_state = VTX_STATE_ADC_CONVERSION;
            }
            break;
        case VTX_STATE_ADC_CONVERSION:
            if (LL_ADC_IsActiveFlag_EOC(ADC2) == 1){
                update_vpd();
                vrefUpdate();
                vtx_state = VTX_STATE_DISABLE;
                LL_ADC_Disable(ADC2);
            }
            break;
        case VTX_STATE_DISABLE:
            if (!LL_ADC_IsEnabled(ADC2)){
                vtx_state = VTX_STATE_IDLE;
                //HAL_GPIO_WritePin(DEBUG_GPIO_Port, DEBUG_Pin, GPIO_PIN_RESET);
            }
            break;
    }

}

#endif

