
#include "target.h"

const vpd_table_t vpd_table = {
    .calFreqs = {5600, 5650, 5700, 5750, 5800, 5850, 5900, 5950, 6000},
    .calDBm = {14, 20, 29, 34},  /* 25mW, 100mW, 800mW, MAX/2500mW */
    .calVpd = {
        /* 25mW (14dBm) */
        {470, 450, 435, 425, 420, 420, 420, 425, 425},
        /* 100mW (20dBm) */
        {745, 720, 690, 660, 660, 660, 660, 660, 660},
        /* 800mW (29dBm) */
        {1580, 1480, 1410, 1380, 1340, 1310, 1310, 1300, 1300},
        /* MAX/2500mW (34dBm) */
        {2540, 2460, 2390, 2320, 2280, 2280, 2280, 2280, 3300}
    }
};

/* Power levels: 25mW, 100mW, 800mW, MAX/2500mW (requires unlock) */
uint8_t saPowerLevelsLut[SA_NUM_POWER_LEVELS] = {14, 20, 29, 34};
uint8_t saPowerLevelsLabel[SA_NUM_POWER_LEVELS * POWER_LEVEL_LABEL_LENGTH] = {'2', '5', ' ',
                                                                              '1', '0', '0',
                                                                              '8', '0', '0',
                                                                              'M', 'A', 'X'};


