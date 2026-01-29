
#include "target.h"

const vpd_table_t vpd_table = {
    .calFreqs = {5600, 5650, 5700, 5750, 5800, 5850, 5900, 5950, 6000},
    .calDBm = {14, 20, 29, 34},  /* 0=14dBm(25mW), 1=20dBm(100mW), 2=29dBm(800mW), 3=34dBm(2500mW/MAX) */
    .calVpd = {
        /* 25mW (14dBm) - VPD tracked */
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        /* 100mW (20dBm) - VPD tracked */
        {570, 565, 550, 538, 510, 510, 480, 470, 310},
        /* 800mW (29dBm) - VPD tracked */
        {859, 890, 880, 870, 851, 850, 800, 790, 835},
        /* 2500mW (34dBm/MAX) - VPD tracked */
        {1400, 1400, 1400, 1400, 1400, 1400, 1400, 1400, 1400}
    }
};

/* Power levels: 0=25mW(VPD), 1=100mW(VPD), 2=800mW(VPD), 3=2500mW/MAX(VPD, requires unlock) */
/* dBm values: 0=14dBm(25mW), 1=20dBm(100mW), 2=29dBm(800mW), 3=34dBm(2500mW/MAX) */
uint8_t saPowerLevelsLut[SA_NUM_POWER_LEVELS] = {14, 20, 29, 34};
uint8_t saPowerLevelsLabel[SA_NUM_POWER_LEVELS * POWER_LEVEL_LABEL_LENGTH] = {'2', '5', ' ',
                                                                              '1', '0', '0',
                                                                              '8', '0', '0',
                                                                              'M', 'A', 'X'};


