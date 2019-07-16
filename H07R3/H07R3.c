/*
    BitzOS (BOS) V0.1.6 - Copyright (C) 2017-2019 Hexabitz
    All rights reserved

    File Name     : H07R3.c
    Description   : Source code for module H07R3.
										Sound speaker (Knowles) with audio amp (TS4990IST) and headset port 
		
		Required MCU resources : 
		
			>> USARTs 1,2,3,4,5 for module ports.
			>> DMA1 Ch5, DMA1 Ch6, DMA2 Ch2 for port-to-memory messaging.
			>> DMA1 Ch1, DMA2 Ch3 for port-to-port streaming.
			>> DAC_OUT 1 for TS4990IST.
			>> DMA1 Ch3 for DAC Ch 1 DMA.
			>> PB0 for TS4990IST STDBY.
			>> Timer 6 for DAC Ch 1 trigger.
			
*/
	
/* Includes ------------------------------------------------------------------*/
#include "BOS.h"
#include "wave.h"


/* Define UART variables */
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
UART_HandleTypeDef huart4;
UART_HandleTypeDef huart5;

/* Module exported parameters ------------------------------------------------*/
module_param_t modParam[NUM_MODULE_PARAMS] = {{.paramPtr=NULL, .paramFormat=FMT_FLOAT, .paramName=""}};

AudioDesc_t currentAudioDesc;

typedef enum audioPlayTaskState_e {
	STATE_DEQUE = 0,
	STATE_PLAY_AUDIO,
	STATE_WAIT
} audioPlayTaskState_t;


/* Private variables ---------------------------------------------------------*/

QueueHandle_t audioDescQueue = NULL;
TaskHandle_t AudioPlayTaskHandle = NULL;
TaskHandle_t playTask = NULL;
bool isInitilized = false;

const char sineModeString[] = "sine";
const char waveModeString[] = "wave";
const char tuneModeString[] = "tune";

#if (MusicNotesNumOfSamples == 10)
/* 10 samples 12-bit amplitude sine wave: 1.650, 2.620, 3.219, 3.220, 2.622, 1.653, 0.682, 0.081, 0.079, 0.676 */
const uint16_t sineDigital[10] = {2048, 3251, 3995, 3996, 3253, 2051, 847, 101, 98, 839};
//const uint16_t sineDigital[10] = {120,179,205,205,179,120,61,25,25,61};
#elif (MusicNotesNumOfSamples == 100)
/* 100 samples 12-bit amplitude sine wave <<http://www.daycounter.com/Calculators/Sine-Generator-Calculator.phtml>> */
const uint16_t sineDigital[100] = {
2048,2176,2304,2431,2557,2680,2801,2919,3034,3145,3251,3353,3449,3540,3625,3704,3776,3842,3900,3951,
3995,4031,4059,4079,4091,4095,4091,4079,4059,4031,3995,3951,3900,3842,3776,3704,3625,3540,3449,3353,
3251,3145,3034,2919,2801,2680,2557,2431,2304,2176,2048,1919,1791,1664,1538,1415,1294,1176,1061,950,
844,742,646,555,470,391,319,253,195,144,100,64,36,16,4,0,4,16,36,64,
100,144,195,253,319,391,470,555,646,742,844,950,1061,1176,1294,1415,1538,1664,1791,1919
};
#elif (MusicNotesNumOfSamples == 256)
/* 256 samples 12-bit amplitude sine wave <<http://www.daycounter.com/Calculators/Sine-Generator-Calculator.phtml>> */
const uint16_t sineDigital[256] = {
2048,2098,2148,2199,2249,2299,2349,2399,2448,2498,2547,2596,2644,2692,2740,2787,2834,2880,2926,2971,3016,3060,3104,3147,3189,3230,3271,3311,3351,3389,3427,3464,
3500,3535,3569,3602,3635,3666,3697,3726,3754,3782,3808,3833,3857,3880,3902,3923,3943,3961,3979,3995,4010,4024,4036,4048,4058,4067,4074,4081,4086,4090,4093,4095,
4095,4094,4092,4088,4084,4078,4071,4062,4053,4042,4030,4017,4002,3987,3970,3952,3933,3913,3891,3869,3845,3821,3795,3768,3740,3711,3681,3651,3619,3586,3552,3517,
3482,3445,3408,3370,3331,3291,3251,3210,3168,3125,3082,3038,2994,2949,2903,2857,2811,2764,2716,2668,2620,2571,2522,2473,2424,2374,2324,2274,2224,2174,2123,2073,
2022,1972,1921,1871,1821,1771,1721,1671,1622,1573,1524,1475,1427,1379,1331,1284,1238,1192,1146,1101,1057,1013,970,927,885,844,804,764,725,687,650,613,
578,543,509,476,444,414,384,355,327,300,274,250,226,204,182,162,143,125,108,93,78,65,53,42,33,24,17,11,7,3,1,0,
0,2,5,9,14,21,28,37,47,59,71,85,100,116,134,152,172,193,215,238,262,287,313,341,369,398,429,460,493,526,560,595,
631,668,706,744,784,824,865,906,948,991,1035,1079,1124,1169,1215,1261,1308,1355,1403,1451,1499,1548,1597,1647,1696,1746,1796,1846,1896,1947,1997,2048
};
#elif (MusicNotesNumOfSamples == 1024)
/* 1024 samples sine wave 12-bit amplitude */
const uint16_t sineDigital[1024] = {
2048,2060,2073,2085,2098,2110,2123,2136,2148,2161,2173,2186,2198,2211,2223,2236,2248,2261,2273,2286,2298,2311,2323,2336,2348,2361,2373,2385,2398,2410,2423,2435,
2447,2460,2472,2484,2497,2509,2521,2533,2545,2558,2570,2582,2594,2606,2618,2630,2642,2654,2666,2678,2690,2702,2714,2726,2738,2750,2762,2773,2785,2797,2808,2820,
2832,2843,2855,2866,2878,2889,2901,2912,2924,2935,2946,2958,2969,2980,2991,3002,3014,3025,3036,3047,3058,3069,3079,3090,3101,3112,3123,3133,3144,3154,3165,3176,
3186,3196,3207,3217,3227,3238,3248,3258,3268,3278,3288,3298,3308,3318,3328,3338,3347,3357,3367,3376,3386,3395,3405,3414,3424,3433,3442,3451,3460,3470,3479,3487,
3496,3505,3514,3523,3532,3540,3549,3557,3566,3574,3582,3591,3599,3607,3615,3623,3631,3639,3647,3655,3663,3670,3678,3686,3693,3701,3708,3715,3723,3730,3737,3744,
3751,3758,3765,3772,3778,3785,3792,3798,3805,3811,3818,3824,3830,3836,3842,3848,3854,3860,3866,3872,3877,3883,3889,3894,3899,3905,3910,3915,3920,3925,3930,3935,
3940,3945,3949,3954,3959,3963,3968,3972,3976,3980,3984,3988,3992,3996,4000,4004,4008,4011,4015,4018,4022,4025,4028,4031,4034,4037,4040,4043,4046,4049,4051,4054,
4056,4059,4061,4063,4065,4067,4069,4071,4073,4075,4077,4078,4080,4081,4083,4084,4085,4087,4088,4089,4090,4091,4091,4092,4093,4093,4094,4094,4094,4095,4095,4095,
4095,4095,4095,4095,4094,4094,4093,4093,4092,4092,4091,4090,4089,4088,4087,4086,4085,4084,4082,4081,4079,4078,4076,4074,4072,4070,4068,4066,4064,4062,4060,4057,
4055,4052,4050,4047,4044,4042,4039,4036,4033,4030,4026,4023,4020,4016,4013,4009,4006,4002,3998,3994,3990,3986,3982,3978,3974,3970,3965,3961,3956,3952,3947,3942,
3938,3933,3928,3923,3918,3913,3907,3902,3897,3891,3886,3880,3875,3869,3863,3857,3851,3845,3839,3833,3827,3821,3814,3808,3802,3795,3788,3782,3775,3768,3761,3755,
3748,3740,3733,3726,3719,3712,3704,3697,3689,3682,3674,3667,3659,3651,3643,3635,3627,3619,3611,3603,3595,3587,3578,3570,3561,3553,3544,3536,3527,3518,3510,3501,
3492,3483,3474,3465,3456,3447,3438,3428,3419,3410,3400,3391,3381,3372,3362,3352,3343,3333,3323,3313,3303,3293,3283,3273,3263,3253,3243,3233,3222,3212,3202,3191,
3181,3170,3160,3149,3139,3128,3117,3106,3096,3085,3074,3063,3052,3041,3030,3019,3008,2997,2986,2975,2963,2952,2941,2929,2918,2907,2895,2884,2872,2861,2849,2838,
2826,2814,2803,2791,2779,2767,2756,2744,2732,2720,2708,2696,2684,2672,2660,2648,2636,2624,2612,2600,2588,2576,2564,2552,2539,2527,2515,2503,2490,2478,2466,2453,
2441,2429,2416,2404,2392,2379,2367,2354,2342,2330,2317,2305,2292,2280,2267,2255,2242,2230,2217,2205,2192,2179,2167,2154,2142,2129,2117,2104,2092,2079,2066,2054,
2041,2029,2016,2003,1991,1978,1966,1953,1941,1928,1916,1903,1890,1878,1865,1853,1840,1828,1815,1803,1790,1778,1765,1753,1741,1728,1716,1703,1691,1679,1666,1654,
1642,1629,1617,1605,1592,1580,1568,1556,1543,1531,1519,1507,1495,1483,1471,1459,1447,1435,1423,1411,1399,1387,1375,1363,1351,1339,1328,1316,1304,1292,1281,1269,
1257,1246,1234,1223,1211,1200,1188,1177,1166,1154,1143,1132,1120,1109,1098,1087,1076,1065,1054,1043,1032,1021,1010,999,989,978,967,956,946,935,925,914,
904,893,883,873,862,852,842,832,822,812,802,792,782,772,762,752,743,733,723,714,704,695,685,676,667,657,648,639,630,621,612,603,
594,585,577,568,559,551,542,534,525,517,508,500,492,484,476,468,460,452,444,436,428,421,413,406,398,391,383,376,369,362,355,347,
340,334,327,320,313,307,300,293,287,281,274,268,262,256,250,244,238,232,226,220,215,209,204,198,193,188,182,177,172,167,162,157,
153,148,143,139,134,130,125,121,117,113,109,105,101,97,93,89,86,82,79,75,72,69,65,62,59,56,53,51,48,45,43,40,
38,35,33,31,29,27,25,23,21,19,17,16,14,13,11,10,9,8,7,6,5,4,3,3,2,2,1,1,0,0,0,0,
0,0,0,1,1,1,2,2,3,4,4,5,6,7,8,10,11,12,14,15,17,18,20,22,24,26,28,30,32,34,36,39,
41,44,46,49,52,55,58,61,64,67,70,73,77,80,84,87,91,95,99,103,107,111,115,119,123,127,132,136,141,146,150,155,
160,165,170,175,180,185,190,196,201,206,212,218,223,229,235,241,247,253,259,265,271,277,284,290,297,303,310,317,323,330,337,344,
351,358,365,372,380,387,394,402,409,417,425,432,440,448,456,464,472,480,488,496,504,513,521,529,538,546,555,563,572,581,590,599,
608,616,625,635,644,653,662,671,681,690,700,709,719,728,738,748,757,767,777,787,797,807,817,827,837,847,857,868,878,888,899,909,
919,930,941,951,962,972,983,994,1005,1016,1026,1037,1048,1059,1070,1081,1093,1104,1115,1126,1137,1149,1160,1171,1183,1194,1206,1217,1229,1240,1252,1263,
1275,1287,1298,1310,1322,1333,1345,1357,1369,1381,1393,1405,1417,1429,1441,1453,1465,1477,1489,1501,1513,1525,1537,1550,1562,1574,1586,1598,1611,1623,1635,1648,
1660,1672,1685,1697,1710,1722,1734,1747,1759,1772,1784,1797,1809,1822,1834,1847,1859,1872,1884,1897,1909,1922,1934,1947,1959,1972,1985,1997,2010,2022,2035,2048
};
#endif


/* Musical notes: 12 Tone Equal Tempered Scale. Frequency Table based on A4 = 440Hz 
   (Middle C = C4)
									Octave Number: 		0				1			 2			 3			4				 5			 6				7				8
*/
const float notesFreq[12][9] = {	{16.35, 32.70, 65.41, 130.81, 261.63, 523.25, 1046.50, 2093.00, 4186.01},			// C
																	{17.32,	34.65, 69.30,	138.59,	277.18,	554.37,	1108.73, 2217.46,	4434.92},			// C#
																	{18.35,	36.71, 73.42,	146.83,	293.66,	587.33,	1174.66, 2349.32,	4698.64},			// D
																	{19.45,	38.89, 77.78,	155.56,	311.13,	622.25,	1244.51, 2489.02,	4978.03},			// D#
																	{20.60,	41.20, 82.41,	164.81,	329.63,	659.26,	1318.51, 2637.02,	5274.04},			// E
																	{21.83,	43.65, 87.31,	174.61,	349.23,	698.46,	1396.91, 2793.83,	5587.65},			// F		
																	{23.12,	46.25, 92.50,	185.00,	369.99,	739.99,	1479.98, 2959.96,	5919.91},			// F#
																	{24.50,	49.00, 98.00,	196.00,	392.00,	783.99,	1567.98, 3135.96,	6271.93},			// G
																	{25.96,	51.91, 103.83, 207.65, 415.30, 830.61, 1661.22,	3322.44, 6644.88},		// G#
																	{27.50,	55.00, 110.00, 220.00, 440.00, 880.00, 1760.00,	3520.00, 7040.00},		// A
																	{29.14,	58.27, 116.54, 233.08, 466.16, 932.33, 1864.66,	3729.31, 7458.62},		// A#
																	{30.87,	61.74, 123.47, 246.94, 493.88, 987.77, 1975.53,	3951.07, 7902.13} };	// B


/* Private function prototypes -----------------------------------------------*/	
bool TS4990_Init(void);
bool TS4990_DeInit(void);																	
bool AddAudioDescToPlaylist(AudioDesc_t *pDesc);
bool PlayAudioNonBlock(AudioDesc_t *pDesc);
void AudioPlayTask(void *argument);
float ParseNoteTime(uint8_t start, char *noteParams, portBASE_TYPE noteStringParamLen);
uint8_t LookupWave(char *name);
																	
static portBASE_TYPE PlayCommand(int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString);
static portBASE_TYPE ListCommand(int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString);
static portBASE_TYPE demoCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString );
																	
/* Create CLI commands --------------------------------------------------------*/
										
static const CLI_Command_Definition_t PlayCommandDefinition = {
		(const int8_t *)"play",
		(const int8_t *)"(H07R3) play:\r\n Syntax: play [tune]/[sine]/[wave] [note]/[freq]/[file]\r\n \
Play a musical tune or a sine wave or a wave file.\n\r Musical notes are:\n\r Cx, Dx, Ex, Fx, Gx, Ax, Bx OR:\n\r \
DOx, REx, MIx, FAx, SOLx, LAx, SIx where x is octave number 1 to 9\n\r - Separate musical notes by a space.\n\r \
- Add # after the note to raise it by a semitone (half-step).\n\r \
- Add note time in seconds with [t]. If ommited, default is t = 1.\n\r \
- Add silence with [t] without a note.\n\r Examples:\n\r\t\
DO4 RE4 MI4[2] FA4 SOL4[0.5] LA4[3]\n\r\tC4 C4# D4 D4# [1] E4[2] F[0.25]\r\n\r\n",
		PlayCommand,
		-1,
};

/*-----------------------------------------------------------*/

static const CLI_Command_Definition_t ListCommandDefinition = {
		(const int8_t *)"list",
		(const int8_t *)"(H07R3) list:\r\n List embedded WAVE files\r\n\r\n",
		ListCommand,
		0,
};

/*-----------------------------------------------------------*/

const CLI_Command_Definition_t demoCommandDefinition =
{
	( const int8_t * ) "demo", /* The command string to type. */
	( const int8_t * ) "(H07R3) demo:\r\n Run a demo program to test module functionality\r\n\r\n",
	demoCommand, /* The function to run. */
	0 /* No parameters are expected. */
};
/*-----------------------------------------------------------*/

/* -----------------------------------------------------------------------
	|												 Private Functions	 														|
   ----------------------------------------------------------------------- 
*/


/* --- H07R3 module initialization. 
*/
void Module_Init(void)
{
	/* Array ports */
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_USART4_UART_Init();
  MX_USART5_UART_Init();
	/* DAC */
	MX_DAC_Init();
	
	/* Initialize the Amplifier */
	TS4990_Init();
	// TS4990_DISABLE();
}
/*-----------------------------------------------------------*/

/* --- H07R3 message processing task. 
*/
Module_Status Module_MessagingTask(uint16_t code, uint8_t port, uint8_t src, uint8_t dst)
{
	Module_Status result = H07R3_OK;
	uint16_t temp16;
	
	switch (code) {
		case CODE_H07R3_PLAY_SINE:
		{
			float freq = 0.0;
			freq = (float)( (uint32_t) cMessage[port-1][4] << 24 ) + ( (uint32_t) cMessage[port-1][5] << 16 ) + ( (uint32_t) cMessage[port-1][6] << 8 ) + cMessage[port-1][7];
			PlaySine(freq, cMessage[port-1][8], (float) cMessage[port-1][9] / 16.0f);
			break;
		}
		case CODE_H07R3_PLAY_WAVE:
		{
			temp16 = (((uint16_t)cMessage[port-1][5])<<8) + (uint16_t)cMessage[port-1][6];
			cMessage[port-1][messageLength[port-1]-1] = 0; 	// Terminate the wave name string
			PlayWave((char *)&cMessage[port-1][7], cMessage[port-1][4], temp16);
			break;
		}
		case CODE_H07R3_PLAY_Tone://notesFreq[note][octave]
		{
			PlaySine(notesFreq[cMessage[port-1][4]][cMessage[port-1][5]], MusicNotesNumOfSamples, (float) cMessage[port-1][6] / 16.0f);
		 	break;
		}
		default:
			result = H07R3_ERR_UnknownMessage;
			break;
	}			

	return result;	
}

/*-----------------------------------------------------------*/

/* --- Register this module CLI Commands 
*/
void RegisterModuleCLICommands(void)
{
	// Todo: Check return values of register commands
	FreeRTOS_CLIRegisterCommand(&PlayCommandDefinition);
	FreeRTOS_CLIRegisterCommand(&ListCommandDefinition);
	FreeRTOS_CLIRegisterCommand(&demoCommandDefinition);
}

/*-----------------------------------------------------------*/

/* --- Get the port for a given UART. 
*/
uint8_t GetPort(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART4)
			return P1;
	else if (huart->Instance == USART2)
			return P2;
	else if (huart->Instance == USART3)
			return P3;
	else if (huart->Instance == USART1)
			return P4;
	else if (huart->Instance == USART5)
			return P5;
		
	return 0;
}

/*-----------------------------------------------------------*/

/* --- Get the port for a given UART. 
*/
float ParseNoteTime(uint8_t start, char *noteParams, portBASE_TYPE noteStringParamLen)
{
	// Locate a space to find out if this is a single parameter or more
	char *space = strchr(&noteParams[start-1],' ');
	
	if (noteParams[start-1] == '[' && (noteParams[noteStringParamLen-1] == ']' || (space != NULL && *(space-1) == ']')) ) {
		return atof(&noteParams[start]);
	} else {
		return 1.0f;
	}
}

/*-----------------------------------------------------------*/

bool TS4990_Init(void)
{
	if (isInitilized)
		return true;
	
	audioDescQueue = xQueueCreate(AUDIO_DESC_QUE_SIZE, sizeof(AudioDesc_t));
	if (audioDescQueue == NULL)
		return false;
	
	if (xTaskCreate(AudioPlayTask, (const char *)"AudioPlayTask", (2 * configMINIMAL_STACK_SIZE), 
									NULL, osPriorityNormal, &AudioPlayTaskHandle) != pdPASS)
		return false;
	
	TS4990_ENABLE();
	isInitilized = true;
	return true;
}

/*-----------------------------------------------------------*/

bool TS4990_DeInit(void)
{
	if (!isInitilized)
		return true;
	
	// TODO: Delete Task
	vQueueDelete(audioDescQueue);
	TS4990_DISABLE();
	isInitilized = false;
	return true;
}

/*-----------------------------------------------------------*/

bool AddAudioDescToPlaylist(AudioDesc_t *pDesc)
{
	// TODO: Check if playing is already in process
	if (pDesc == NULL)
		return false;
	if (xQueueSend(audioDescQueue, (const void *)pDesc, 0) != pdTRUE)
		return false;
	
	return true;
}

/*-----------------------------------------------------------*/

void AudioPlayTask(void *argument)
{
	const TickType_t TICKS_TO_WAIT = pdMS_TO_TICKS(1000);
	audioPlayTaskState_t state = STATE_DEQUE;
	
	// TODO: Make Sure the Audio Amplifier is activated
	// TODO: Make sure DAC is zero when not playing
	// TODO: Initialize currentAudioDesc
	for (;;) {
		switch (state) {
			case STATE_DEQUE:
			{
				if (xQueueReceive(audioDescQueue, (void *)&currentAudioDesc, TICKS_TO_WAIT) != pdTRUE)
					break;
				
				state = STATE_PLAY_AUDIO;
				// Fall Though!
			}
			case STATE_PLAY_AUDIO:
			{
				if (currentAudioDesc.delay && (currentAudioDesc.numOfRepeats > 0)) {
					state = STATE_WAIT;
				} else {
					state = STATE_DEQUE;			
					if (PlayAudioNonBlock(&currentAudioDesc) == false) {
						// TODO: Add a delay
						break;
					}
					/* Wait indefinitly until DMA transfer is finished */
					ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
				}		
				break;
			}
			case STATE_WAIT:
			{
				Delay_ms(currentAudioDesc.delay);	
				currentAudioDesc.numOfRepeats--;			
				if (currentAudioDesc.numOfRepeats > 0)
					state = STATE_PLAY_AUDIO;
				else
					state = STATE_DEQUE;
				
				break;
			}
			
			default:
			{
				// TODO: Handle Error
				break;
			}
			
		}
	}
}

/*-----------------------------------------------------------*/

void AudioDescInit(AudioDesc_t *pDesc)
{
	if (pDesc == NULL)
		return;
	
	pDesc->pBuffer = NULL;
	pDesc->lenOfBuf = 0;
	pDesc->numOfBitsInACode = 0;
	pDesc->numOfRepeats = 0;
	pDesc->delay = 0;
	pDesc->rate = 0.0;
}

/*-----------------------------------------------------------*/

bool AddAudioToPlaylist(uint32_t *pBuffer, uint32_t length, uint32_t numOfRepeats, uint8_t dataPointSize, float rate, uint32_t delay)
{
	// TODO: Check if playing is alread in process
	AudioDesc_t desc;
	
	AudioDescInit(&desc);
	
	desc.pBuffer = pBuffer;
	desc.lenOfBuf = length;
	desc.numOfBitsInACode = dataPointSize;
	desc.numOfRepeats = numOfRepeats;
	desc.rate = rate;
	desc.delay = delay;
	
	return AddAudioDescToPlaylist(&desc);
}

/*-----------------------------------------------------------*/

/* -- Lookup WAVE file index based on its name
*/
uint8_t LookupWave(char *name)
{
	for(uint8_t i=0 ; i<MAX_WAVES ; i++) {
		if (!strcmp(waveName[i], name)) {
			return i;
		}
	}
	return	0;
}

/*-----------------------------------------------------------*/

/* -----------------------------------------------------------------------
	|																APIs	 																 	|
   ----------------------------------------------------------------------- 
*/

bool PlayAudioNonBlock(AudioDesc_t *pDesc)
{
	/* Setup Tim 6: Prescaler = (SystemCoreClock / TIM6 trigger clock) - 1, ARR = TIM6 trigger clock - 1 */
	HAL_TIM_Base_Stop(&htim6);
	HAL_DAC_Stop_DMA(&hdac, DAC_CHANNEL_1);
	
	htim6.Instance->ARR = 1;
	htim6.Instance->PSC = (uint16_t)(((SystemCoreClock / pDesc->rate) / 2) - 1);
	HAL_TIM_Base_Start(&htim6);
	
	uint32_t alignment = DAC_ALIGN_8B_R;
	if (pDesc->numOfBitsInACode > 8)
		alignment = DAC_ALIGN_12B_R;
	
	/* Start the DAC DMA */
	HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, pDesc->pBuffer, pDesc->lenOfBuf, alignment);

	return true;
}

/*-----------------------------------------------------------*/

void PlayAudio(uint32_t *pBuffer, uint32_t length, uint32_t numOfRepeats, uint8_t dataPointSize, float rate)
{
	// TODO: Check if playing is already in process
	// NumberOfTuneWaves = numOfRepeats;
	playTask = xTaskGetCurrentTaskHandle();
	
	/* Setup Tim 6: Prescaler = (SystemCoreClock / TIM6 trigger clock) - 1, ARR = TIM6 trigger clock - 1 */
	HAL_TIM_Base_Stop(&htim6);
	HAL_DAC_Stop_DMA(&hdac, DAC_CHANNEL_1);
	
	htim6.Instance->ARR = 1;
	htim6.Instance->PSC = (uint16_t)(((SystemCoreClock / rate) / 2) - 1);
	HAL_TIM_Base_Start(&htim6);
	
	uint32_t alignment = DAC_ALIGN_8B_R;
	if (dataPointSize > 8)
		alignment = DAC_ALIGN_12B_R;
	
	/* Start the DAC DMA */
	HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, pBuffer, length, alignment); 
		
	/* Wait indefinitly until DMA transfer is finished */
	ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
}

/*-----------------------------------------------------------*/

bool PlaySine(float freq, uint16_t NumOfSamples, float durationInSeconds)
{
	if (!freq) {			// Play silence
		return AddAudioToPlaylist((uint32_t *)sineDigital, NumOfSamples, 1, 0, 0, durationInSeconds * 1000);			
	} else {					// Play a note
		return AddAudioToPlaylist((uint32_t *)sineDigital, NumOfSamples, freq * durationInSeconds, 12, freq * NumOfSamples, 0);		
	}	
}

/*-----------------------------------------------------------*/

/* --- Play an embedded WAVE Format Data. 
*/
bool PlayWave(char *name, int32_t repeats, uint16_t delayInMs)
{
	uint8_t index = LookupWave(name);
	return AddAudioToPlaylist(waveAdd[index], waveLength[index], repeats, waveResolution[index], waveRate[index], delayInMs);
}

/*-----------------------------------------------------------*/

/* -----------------------------------------------------------------------
	|															Commands																 	|
   ----------------------------------------------------------------------- 
*/

static bool PlayCommandLineParser(const int8_t *pcCommandString, char **ppModeString, 
										portBASE_TYPE *pModeStrParamLen, float *pFreq, float *pLength, bool *tuneMode)
{
	char *modeParams = NULL;
	char *freqParams = NULL;
	char *lengthParams = NULL;
	char *noteParams = NULL;
	char *nameParams = NULL;
	portBASE_TYPE modeStringParamLen = 0;
	portBASE_TYPE freqStringParamLen = 0;
	portBASE_TYPE lengthStringParamLen = 0;
	portBASE_TYPE noteStringParamLen = 0;
	static char lastNote = 1; uint8_t octave = 0;
	
	// Parse mode parameter
	if (lastNote == 1) 
	{
		modeParams = (char *)FreeRTOS_CLIGetParameter(pcCommandString, 1, &modeStringParamLen);
		if (modeParams == NULL)
			return false;	
		
		*ppModeString = modeParams;
		*pModeStrParamLen = modeStringParamLen;
	} 
	
	// Wave mode
	if (!strncmp(modeParams, waveModeString, max(strlen(waveModeString), modeStringParamLen)))
	{
		// Parse WAVE name
		nameParams = (char *)FreeRTOS_CLIGetParameter(pcCommandString, 2, &lengthStringParamLen);
		if (nameParams == NULL)
			return false;
		
		*pFreq = LookupWave(nameParams);
		
		return true;
	}
	// Sine mode
	else if (*tuneMode == false && strncmp(modeParams, tuneModeString, max(strlen(tuneModeString), modeStringParamLen)) != 0)
	{	
		freqParams = (char *)FreeRTOS_CLIGetParameter(pcCommandString, 2, &freqStringParamLen);
		lengthParams = (char *)FreeRTOS_CLIGetParameter(pcCommandString, 3, &lengthStringParamLen);
		if ((freqParams == NULL) || (lengthParams == NULL))
			return false;
		
		*pFreq = atoi(freqParams);
		*pLength = atoi(lengthParams);
		
		return true;
	}
	// tune mode
	else
	{
		// Parse musical notes one by one
		noteParams = (char *)FreeRTOS_CLIGetParameter(pcCommandString, ++lastNote, &noteStringParamLen);

		if (noteParams == NULL) 
		{
			lastNote = 1;		// Reset this for next command
			return false;
		// Parse the musical note
		} 
		else 
		{		
			if (noteParams[0] == '[') {	// silence note
				if (strchr(&noteParams[0],' ') != NULL) {
					*pFreq = 0;
					*pLength = ParseNoteTime(1, noteParams, noteStringParamLen);
				} else {
					lastNote = 1;		// Reset this for next command
					return false;										
				}
			} else if (noteParams[0] == 'c') {
				octave = atoi(&noteParams[1]);
				if (!octave)	octave = 5;
				if (noteParams[2] == '#') {
					*pFreq = notesFreq[Csharp][octave-1];
					*pLength = ParseNoteTime(3, noteParams, noteStringParamLen);					
				} else {
					*pFreq = notesFreq[C][octave-1];
					*pLength = ParseNoteTime(2, noteParams, noteStringParamLen);
				}
			} else if (!strncmp(noteParams, "do", 2)) {
				octave = atoi(&noteParams[2]);
				if (!octave)	octave = 5;
				if (noteParams[3] == '#') {
					*pFreq = notesFreq[DOsharp][octave-1];
					*pLength = ParseNoteTime(4, noteParams, noteStringParamLen);					
				} else {
					*pFreq = notesFreq[DO][octave-1];
					*pLength = ParseNoteTime(3, noteParams, noteStringParamLen);
				}
			} else if (noteParams[0] == 'd') {
				octave = atoi(&noteParams[1]);
				if (!octave)	octave = 5;
				if (noteParams[2] == '#') {
					*pFreq = notesFreq[Dsharp][octave-1];
					*pLength = ParseNoteTime(3, noteParams, noteStringParamLen);					
				} else {
					*pFreq = notesFreq[D][octave-1];
					*pLength = ParseNoteTime(2, noteParams, noteStringParamLen);
				}
			} else if (!strncmp(noteParams, "re", 2)) {
				octave = atoi(&noteParams[2]);
				if (!octave)	octave = 5;
				if (noteParams[3] == '#') {
					*pFreq = notesFreq[REsharp][octave-1];
					*pLength = ParseNoteTime(4, noteParams, noteStringParamLen);					
				} else {
					*pFreq = notesFreq[RE][octave-1];
					*pLength = ParseNoteTime(3, noteParams, noteStringParamLen);
				}
			} else if (noteParams[0] == 'e') {
				octave = atoi(&noteParams[1]);
				if (!octave)	octave = 5;
				if (noteParams[2] == '#') {
					*pFreq = notesFreq[E][octave-1];
					*pLength = ParseNoteTime(3, noteParams, noteStringParamLen);					
				} else {
					*pFreq = notesFreq[E][octave-1];
					*pLength = ParseNoteTime(2, noteParams, noteStringParamLen);
				}
			} else if (!strncmp(noteParams, "mi", 2)) {
				octave = atoi(&noteParams[2]);
				if (!octave)	octave = 5;
				if (noteParams[3] == '#') {
					*pFreq = notesFreq[MI][octave-1];
					*pLength = ParseNoteTime(4, noteParams, noteStringParamLen);					
				} else {
					*pFreq = notesFreq[MI][octave-1];
					*pLength = ParseNoteTime(3, noteParams, noteStringParamLen);
				}
			} else if (noteParams[0] == 'f') {
				octave = atoi(&noteParams[1]);
				if (!octave)	octave = 5;
				if (noteParams[2] == '#') {
					*pFreq = notesFreq[Fsharp][octave-1];
					*pLength = ParseNoteTime(3, noteParams, noteStringParamLen);					
				} else {
					*pFreq = notesFreq[F][octave-1];
					*pLength = ParseNoteTime(2, noteParams, noteStringParamLen);
				}
			} else if (!strncmp(noteParams, "fa", 2)) {
				octave = atoi(&noteParams[2]);
				if (!octave)	octave = 5;
				if (noteParams[3] == '#') {
					*pFreq = notesFreq[FAsharp][octave-1];
					*pLength = ParseNoteTime(4, noteParams, noteStringParamLen);					
				} else {
					*pFreq = notesFreq[FA][octave-1];
					*pLength = ParseNoteTime(3, noteParams, noteStringParamLen);
				}
			} else if (noteParams[0] == 'g') {
				octave = atoi(&noteParams[1]);
				if (!octave)	octave = 5;
				if (noteParams[2] == '#') {
					*pFreq = notesFreq[Gsharp][octave-1];
					*pLength = ParseNoteTime(3, noteParams, noteStringParamLen);					
				} else {
					*pFreq = notesFreq[G][octave-1];
					*pLength = ParseNoteTime(2, noteParams, noteStringParamLen);
				}
			} else if (!strncmp(noteParams, "sol", 2)) {
				octave = atoi(&noteParams[2]);
				if (!octave)	octave = 5;
				if (noteParams[3] == '#') {
					*pFreq = notesFreq[SOLsharp][octave-1];
					*pLength = ParseNoteTime(4, noteParams, noteStringParamLen);					
				} else {
					*pFreq = notesFreq[SOL][octave-1];
					*pLength = ParseNoteTime(3, noteParams, noteStringParamLen);
				}
			} else if (noteParams[0] == 'a') {
				octave = atoi(&noteParams[1]);
				if (!octave)	octave = 5;
				if (noteParams[2] == '#') {
					*pFreq = notesFreq[Asharp][octave-1];
					*pLength = ParseNoteTime(3, noteParams, noteStringParamLen);					
				} else {
					*pFreq = notesFreq[A][octave-1];
					*pLength = ParseNoteTime(2, noteParams, noteStringParamLen);
				}
			} else if (!strncmp(noteParams, "la", 2)) {
				octave = atoi(&noteParams[2]);
				if (!octave)	octave = 5;
				if (noteParams[3] == '#') {
					*pFreq = notesFreq[LAsharp][octave-1];
					*pLength = ParseNoteTime(4, noteParams, noteStringParamLen);					
				} else {
					*pFreq = notesFreq[LA][octave-1];
					*pLength = ParseNoteTime(3, noteParams, noteStringParamLen);
				}
			} else if (noteParams[0] == 'b') {
				octave = atoi(&noteParams[1]);
				if (!octave)	octave = 5;
				if (noteParams[2] == '#') {
					*pFreq = notesFreq[B][octave-1];
					*pLength = ParseNoteTime(3, noteParams, noteStringParamLen);					
				} else {
					*pFreq = notesFreq[B][octave-1];
					*pLength = ParseNoteTime(2, noteParams, noteStringParamLen);
				}
			} else if (!strncmp(noteParams, "si", 2)) {
				octave = atoi(&noteParams[2]);
				if (!octave)	octave = 5;
				if (noteParams[3] == '#') {
					*pFreq = notesFreq[SI][octave-1];
					*pLength = ParseNoteTime(4, noteParams, noteStringParamLen);					
				} else {
					*pFreq = notesFreq[SI][octave-1];
					*pLength = ParseNoteTime(3, noteParams, noteStringParamLen);
				}
			}
			
			*tuneMode = true;		// Use this flag to avoid comparing mode strings for each note
			return true;
		}
	}
}

/*-----------------------------------------------------------*/

BaseType_t PlayCommand(int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString)
{
	// TODO: Change variable names
	char *modeParams = NULL;
	portBASE_TYPE modeStringParamLen = 0;
	float freq = 0, length = 0;
	bool tuneMode = false;
	*pcWriteBuffer = '\0';
	
	do {
		if (PlayCommandLineParser(pcCommandString, &modeParams, &modeStringParamLen, &freq, &length, &tuneMode) == false)
			break;
		
		if (tuneMode || !strncmp(modeParams, tuneModeString, max(strlen(tuneModeString), modeStringParamLen))) {		// Loop over this mode until all notes are proccessed
			PlaySine(freq, MusicNotesNumOfSamples, length);		
		} else if (!strncmp(modeParams, sineModeString, max(strlen(sineModeString), modeStringParamLen))) {		// Execute this mode once
			PlaySine(freq, MusicNotesNumOfSamples, length);
			return pdFALSE;
		} else if (!strncmp(modeParams, waveModeString, max(strlen(waveModeString), modeStringParamLen))) {		// Execute this mode once
			AddAudioToPlaylist(waveAdd[(uint8_t)freq], waveLength[(uint8_t)freq], 0, waveResolution[(uint8_t)freq], 16000, 0);
			return pdFALSE;
		} else {
			strncat((char *)pcWriteBuffer, "Error: Invalid Params\r\n", xWriteBufferLen);
			break;
		}
		
	} while (1);
	
	return pdFALSE;
}


/*-----------------------------------------------------------*/

BaseType_t ListCommand(int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString)
{
	const int8_t * const pcWavesTableHeader = ( int8_t * ) "\nName    Length (seconds)\r\n==================\r\n\n";
	const int8_t * const pcWavesTableRow = ( int8_t * ) "%s%s    %.1f\r\n";
	
	/* Remove compile time warnings about unused parameters, and check the
	write buffer is not NULL.  NOTE - for simplicity, this example assumes the
	write buffer length is adequate, so does not check for buffer overflows. */
	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );

	/* Respond to the command */
	strcpy( ( char * ) pcWriteBuffer, ( char * ) pcWavesTableHeader );
	for(uint8_t i=0 ; i<MAX_WAVES ; i++)
	{
		sprintf( ( char * ) pcWriteBuffer, ( char * ) pcWavesTableRow, ( char * ) pcWriteBuffer, waveName[i], (float)waveLength[i]*2/16000.0f);
	}
	strcat( ( char * ) pcWriteBuffer, "\n\r"); 
	
	/* There is no more data to return after this single string, so return
	pdFALSE. */
	return pdFALSE;	
}

/*-----------------------------------------------------------*/

portBASE_TYPE demoCommand( int8_t *pcWriteBuffer, size_t xWriteBufferLen, const int8_t *pcCommandString )
{
	/* Remove compile time warnings about unused parameters, and check the
	write buffer is not NULL.  NOTE - for simplicity, this example assumes the
	write buffer length is adequate, so does not check for buffer overflows. */
	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );

	C5(0.25); D5(0.25); E5(0.25); F5(0.25); G5(0.25); A5(0.25); B5(0.25);
			
	/* There is no more data to return after this single string, so return
	pdFALSE. */
	return pdFALSE;
}

/*-----------------------------------------------------------*/

/************************ (C) COPYRIGHT HEXABITZ *****END OF FILE****/
