
#include "target.h"

const vpd_table_t vpd_table = {
    .calFreqs = {5600, 5650, 5700, 5750, 5800, 5850, 5900, 5950, 6000},
    .calDBm = {0, 14, 20, 26},  /* 0=0mW, 1=14dBm(25mW), 2=20dBm(100mW), 3=26dBm(400mW) */
    .calVpd = {
        /* 0mW (0dBm) - all zeros */
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        /* 25mW (14dBm) */
        {1425,1425,1425,1430,1430,1445,1465,1465,1475},     
        /* 100mW (20dBm) */
        {1550,1515,1515,1500,1510,1520,1665,1670,1675},
        /* 400mW (26dBm) - estimated based on power scaling */
        {3300,3300,3300,3300,3300,3300,3300,3300,3300}
    }
};

/* Power levels: 0=0mW(off), 1=25mW, 2=100mW, 3=400mW(max, requires unlock) */
/* dBm values: 0=0mW, 1=14dBm(25mW), 2=20dBm(100mW), 3=26dBm(400mW) */
uint8_t saPowerLevelsLut[SA_NUM_POWER_LEVELS] = {0, 14, 20, 26};
uint8_t saPowerLevelsLabel[SA_NUM_POWER_LEVELS * POWER_LEVEL_LABEL_LENGTH] = {'0', ' ', ' ',
                                                                              '2', '5', ' ',
                                                                              '1', '0', '0',
                                                                              '4', '0', '0'};


