
#include "target.h"

const vpd_table_t vpd_table = {
    .calFreqs = {5600, 5650, 5700, 5750, 5800, 5850, 5900, 5950, 6000},
    .calDBm = {0, 14, 20, 26},  /* 0=0dBm(0mW/off), 1=14dBm(25mW), 2=20dBm(100mW), 3=26dBm(400mW) */
    .calVpd = {
        /* 0mW (off) - VPD=0 */
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        /* 25mW (14dBm) - VPD tracked */
        {135, 140, 155, 175, 200, 230, 270, 320, 365},
        /* 100mW (20dBm) - VPD tracked */
        {235, 245, 270, 295, 340, 380, 425, 480, 540},
        /* 400mW (26dBm) - VPD tracked */
        {342, 362, 400, 450, 515, 590, 680, 780, 860}
    }
};

/* Power levels: 0=0mW(off), 1=25mW(VPD), 2=100mW(VPD), 3=400mW(VPD, requires unlock) */
/* dBm values: 0=0(off), 1=14dBm(25mW), 2=20dBm(100mW), 3=26dBm(400mW) */
uint8_t saPowerLevelsLut[SA_NUM_POWER_LEVELS] = {0, 14, 20, 26};
uint8_t saPowerLevelsLabel[SA_NUM_POWER_LEVELS * POWER_LEVEL_LABEL_LENGTH] = {'0', ' ', ' ',
                                                                              '2', '5', ' ',
                                                                              '1', '0', '0',
                                                                              '4', '0', '0'};


