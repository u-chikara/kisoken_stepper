#include <Arduino.h>

#include <Wire.h>

#define I2C_ADDRESS 0x32
#define I2C_SDA_PIN 21
#define I2C_SCK_PIN 22
#define I2C_FREQ 400000

#define MOTOR_0 25
#define MOTOR_1 26
#define MOTOR_2 27
#define MOTOR_3 14

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

typedef struct
{

  uint8_t DUMMY;
  uint8_t CHANNEL : 4;
  uint8_t COMMAND : 4;
  uint8_t MIDI_0;
  uint8_t MIDI_1;

} USB_AUDIO_MIDI_EVENT_PACKET;

USB_AUDIO_MIDI_EVENT_PACKET mididata;
uint8_t *p_mididata = (uint8_t *)&mididata;

bool midi_update = 0;

// I2C受信割り込みハンドラ
void i2c_recv(int len)
{
  if (len == 4)
  {
    *(p_mididata) = Wire.read();
    *(p_mididata + 1) = Wire.read();
    *(p_mididata + 2) = Wire.read();
    *(p_mididata + 3) = Wire.read();

    midi_update = true;
  }
}

static uint8_t step = 0;

// please put 0 or 1
void Stepping(char direction)
{
  step += direction;
  step %= 4;

  switch (step)
  {
  case 0:
    digitalWrite(MOTOR_0, HIGH);
    digitalWrite(MOTOR_1, LOW);
    digitalWrite(MOTOR_2, LOW);
    digitalWrite(MOTOR_3, LOW);
    break;
  case 1:
    digitalWrite(MOTOR_0, LOW);
    digitalWrite(MOTOR_1, HIGH);
    digitalWrite(MOTOR_2, LOW);
    digitalWrite(MOTOR_3, LOW);
    break;
  case 2:
    digitalWrite(MOTOR_0, LOW);
    digitalWrite(MOTOR_1, LOW);
    digitalWrite(MOTOR_2, HIGH);
    digitalWrite(MOTOR_3, LOW);
    break;
  case 3:
    digitalWrite(MOTOR_0, LOW);
    digitalWrite(MOTOR_1, LOW);
    digitalWrite(MOTOR_2, LOW);
    digitalWrite(MOTOR_3, HIGH);
    break;
  }
}

void setup()
{
  Serial.begin(115200); // デバッグ用

  Wire.onReceive(i2c_recv); // 割り込みハンドラ設定

  Wire.begin(I2C_ADDRESS, I2C_SDA_PIN, I2C_SCK_PIN, I2C_FREQ); // I2C通信初期化

  Serial.println("START");

  pinMode(MOTOR_0, OUTPUT);
  pinMode(MOTOR_1, OUTPUT);
  pinMode(MOTOR_2, OUTPUT);
  pinMode(MOTOR_3, OUTPUT);
}

void loop()
{
  // put your main code here, to run repeatedly:
  if (midi_update)
  {
    midi_update = false;
    Serial.printf("channel=%d,num=%d,volume=%d\r\n", mididata.CHANNEL, mididata.MIDI_0, mididata.MIDI_1);
  }
  if (mididata.COMMAND == MIDI_CIN_NOTE_ON)
  {
    Stepping(1);
    delayMicroseconds(onkai[mididata.MIDI_0 + 3]);
  }
}
