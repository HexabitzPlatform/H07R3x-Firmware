#ifndef WAVE_H
#define WAVE_H

#include <stdint.h>


/* Maximum number of embedded waves */
#define MAX_WAVES		2

/* Length of embedded waves */
#define WAVEBYTECODE_HOWAREYOU_LENGTH 				14202
#define WAVEBYTECODE_HITHERE_LENGTH						9028

/* Resolution of embedded waves (bits per sample) */
#define WAVEBYTECODE_HOWAREYOU_BPS 						8
#define WAVEBYTECODE_HITHERE_BPS							8

/* Rate of embedded waves (sample per second) */
#define WAVEBYTECODE_HOWAREYOU_SPS 						22050
#define WAVEBYTECODE_HITHERE_SPS							22050

/* Export embedded waves definition */
extern const uint8_t waveByteCode_HiThere[WAVEBYTECODE_HITHERE_LENGTH];
extern const uint8_t waveByteCode_HowAreYou[WAVEBYTECODE_HOWAREYOU_LENGTH];

/* Export wave variables */
extern uint32_t *waveAdd[MAX_WAVES];
extern const uint32_t waveLength[MAX_WAVES];
extern const uint32_t waveResolution[MAX_WAVES];
extern const uint32_t waveRate[MAX_WAVES];
extern const char *waveName[MAX_WAVES];

#endif
