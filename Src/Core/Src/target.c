
#include "target.h"

const vpd_table_t vpd_table = {
    .calFreqs = {5600, 5650, 5700, 5750, 5800, 5850, 5900, 5950, 6000},
    .calDBm = {29, 26},  /* 0=29dBm(800mW), 1=26dBm(400mW/MAX) */
    .calVpd = {
        /* 800mW (29dBm) - VPD tracked */
        {2400, 2350, 2300, 2250, 2200, 2150, 2100, 2050, 2000},
        /* 400mW (26dBm/MAX) - VPD tracked */
        {2200, 1700, 1690, 1690, 1740, 1800, 2020, 2300, 2400}
    }
};

/* Power levels: 0=25mW(fixed DAC), 1=100mW(fixed DAC), 2=800mW(VPD), 3=400mW/MAX(VPD, requires unlock) */
/* dBm values: 0=14dBm(25mW), 1=20dBm(100mW), 2=29dBm(800mW), 3=26dBm(400mW/MAX) */
uint8_t saPowerLevelsLut[SA_NUM_POWER_LEVELS] = {14, 20, 29, 26};
uint8_t saPowerLevelsLabel[SA_NUM_POWER_LEVELS * POWER_LEVEL_LABEL_LENGTH] = {'2', '5', ' ',
                                                                              '1', '0', '0',
                                                                              '8', '0', '0',
                                                                              'M', 'A', 'X'};


