#ifndef MIDI_STEPPER
#define MIDI_STEPPER

#include <Arduino.h>

#define SAMPLE_SIZE 250
#define MAX_SOURCE 3
#define HOKAN 5

//OK
// #define MOTOR0_0 27
// #define MOTOR0_1 14
// #define MOTOR0_2 12
// #define MOTOR0_3 13

#define MOTOR0_0 25
#define MOTOR0_1 26
#define MOTOR0_2 27
#define MOTOR0_3 14

//OK
#define MOTOR1_0 32
#define MOTOR1_1 33
#define MOTOR1_2 25
#define MOTOR1_3 26

//OK
#define MOTOR2_0 15
#define MOTOR2_1 2
#define MOTOR2_2 4
#define MOTOR2_3 16


#define MOTOR3_0 17
#define MOTOR3_1 5
#define MOTOR3_2 18
#define MOTOR3_3 19



#define MIDI_CIN_MISC_FUNCTION_RESERVED 0x0
#define MIDI_CIN_CABLE_EVENTS_RESERVED 0x1
#define MIDI_CIN_2_uint8_t_MESSAGE 0x2
#define MIDI_CIN_MTC 0x2
#define MIDI_CIN_SONG_SELECT 0x2
#define MIDI_CIN_3_uint8_t_MESSAGE 0x3
#define MIDI_CIN_SSP 0x3
#define MIDI_CIN_SYSEX_START 0x4
#define MIDI_CIN_SYSEX_CONTINUE 0x4
#define MIDI_CIN_1_uint8_t_MESSAGE 0x5
#define MIDI_CIN_SYSEX_ENDS_1 0x5
#define MIDI_CIN_SYSEX_ENDS_2 0x6
#define MIDI_CIN_SYSEX_ENDS_3 0x7
#define MIDI_CIN_NOTE_OFF 0x8
#define MIDI_CIN_NOTE_ON 0x9
#define MIDI_CIN_POLY_KEY_PRESS 0xA
#define MIDI_CIN_CONTROL_CHANGE 0xB
#define MIDI_CIN_PROGRAM_CHANGE 0xC
#define MIDI_CIN_CHANNEL_PREASURE 0xD
#define MIDI_CIN_PITCH_BEND_CHANGE 0xE
#define MIDI_CIN_SINGLE_uint8_t 0xF

#define USB_DEVICE_AUDIO_MIDI_ENDPOINT 1

typedef struct
{

  uint8_t DUMMY;
  uint8_t CHANNEL : 4;
  uint8_t COMMAND : 4;
  uint8_t MIDI_0;
  uint8_t MIDI_1;

} USB_AUDIO_MIDI_EVENT_PACKET;

const uint16_t onkai[88] = {
    35714,
    34482,
    32258,
    30303,
    28571,
    27027,
    25641,
    24390,
    22727,
    21739,
    20408,
    19230,
    18181,
    17241,
    16129,
    15384,
    14492,
    13698,
    12820,
    12195,
    11494,
    10869,
    10204,
    9615,
    9090,
    8547,
    8130,
    7633,
    7194,
    6802,
    6410,
    6060,
    5714,
    5405,
    5102,
    4807,
    4545,
    4291,
    4048,
    3816,
    3610,
    3401,
    3215,
    3030,
    2865,
    2702,
    2551,
    2409,
    2272,
    2145,
    2024,
    1912,
    1805,
    1703,
    1607,
    1517,
    1432,
    1351,
    1275,
    1203,
    1136,
    1072,
    1012,
    955,
    901,
    851,
    803,
    758,
    715,
    675,
    637,
    602,
    568,
    536,
    506,
    477,
    451,
    425,
    401,
    379,
    357,
    337,
    318,
    301,
    284,
    268,
    253,
    238};

const uint16_t scale_data[88] = {
1818,
1716,
1620,
1529,
1443,
1362,
1286,
1213,
1145,
1081,
1020,
963,
909,
858,
810,
764,
722,
681,
643,
607,
573,
541,
510,
482,
455,
429,
405,
382,
361,
341,
321,
303,
286,
270,
255,
241,
227,
215,
202,
191,
180,
170,
161,
152,
143,
135,
128,
120,
114,
107,
101,
96,
90,
85,
80,
76,
72,
68,
64,
60,
57,
54,
51,
48,
45,
43,
40,
38,
36,
34,
32,
30,
28,
27,
25,
24,
23,
21,
20,
19,
18,
17,
16,
15,
14,
13,
13,
12
};

void midi_init();

void Stepping(char direction);

extern void SourceQueue(USB_AUDIO_MIDI_EVENT_PACKET mep);

void midi_process();

#endif