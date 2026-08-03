#ifndef __TARGET_H
#define __TARGET_H

#include <stdint.h>


#define CAL_FREQ_SIZE 9
#define POWER_LEVEL_LABEL_LENGTH    3



#define TEMP_WARNING_DEG        70  /* warning temperature (degrees Celsius) */
#define TEMP_WARNING_POWERINDEX VPD_INDEX_MAX   /* Reduce from MAX as temperature rises */
#define TEMP_DANGER_DEG         90  /* danger temperature (degrees Celsius) */
#define TEMP_DANGER_POWERINDEX  VPD_INDEX_800MW /* Limit to 800mW at danger temperature */
#define VTX_DEFAULT_POWER_INDEX     1    /* MSP power index is 1-based: 1 = 25mW */
#define VTX_TABLE_NEW_POWER_COUNT   4    /* Number of VTX power levels */
#define SA_NUM_POWER_LEVELS         VTX_TABLE_NEW_POWER_COUNT
#define CAL_DBM_SIZE 4
#define VREF_MAX_MV  2800           /* Maximum DAC reference voltage in mV */

/* Power level indices */
#define POWER_LEVEL_25MW     0
#define POWER_LEVEL_100MW    1
#define POWER_LEVEL_800MW    2
#define POWER_LEVEL_MAX      3      /* 2500mW, requires button unlock */

/* VPD table indices */
#define VPD_INDEX_25MW       0
#define VPD_INDEX_100MW      1
#define VPD_INDEX_800MW      2
#define VPD_INDEX_MAX        3

/* Configure whether the 2500mW power level is available without button unlock. */
/* Set to 1: 2500mW is available by default. */
/* Set to 0: both buttons must be held for 5 seconds to unlock 2500mW. */
#ifndef ENABLE_MAX_POWER_UNLOCK
#define ENABLE_MAX_POWER_UNLOCK    0
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
