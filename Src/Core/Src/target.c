
#include "target.h"

const vpd_table_t vpd_table = {
    .calFreqs = {5600, 5650, 5700, 5750, 5800, 5850, 5900, 5950, 6000},
    .calDBm = {14, 20, 29, 34},  /* 0=14dBm(25mW), 1=20dBm(100mW), 2=29dBm(800mW), 3=34dBm(2500mW/MAX) */
    .calVpd = {
        /* 25mW (14dBm) - VPD tracked */
        {480, 460, 445, 435, 430,430, 430, 435, 435},
        /* 100mW (20dBm) - VPD tracked */
        {735, 710, 680, 650, 650, 650, 650, 650, 650},
        /* 800mW (29dBm) - VPD tracked */
        {1500, 1400, 1330, 1300, 1260, 1230, 1230,1220, 1220},
        /* 2500mW (34dBm/MAX) - VPD tracked */
        {2500, 2420, 2350, 2280, 2240, 2240, 2240, 2240, 3290}
    }
};

/* Power levels: 0=25mW(VPD), 1=100mW(VPD), 2=800mW(VPD), 3=2500mW/MAX(VPD, requires unlock) */
/* dBm values: 0=14dBm(25mW), 1=20dBm(100mW), 2=29dBm(800mW), 3=34dBm(2500mW/MAX) */
uint8_t saPowerLevelsLut[SA_NUM_POWER_LEVELS] = {14, 20, 29, 34};
uint8_t saPowerLevelsLabel[SA_NUM_POWER_LEVELS * POWER_LEVEL_LABEL_LENGTH] = {'2', '5', ' ',
                                                                              '1', '0', '0',
                                                                              '8', '0', '0',
                                                                              'M', 'A', 'X'};


