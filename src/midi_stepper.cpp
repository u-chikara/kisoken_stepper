#include "midi_stepper.h"
#include <Arduino.h>

//タイマー割り込みハンドル
static hw_timer_t *timer = NULL;


static uint16_t step_data[2][SAMPLE_SIZE] = {0};//常に流し続けるバッファ
static bool buf_num_client = 0;//バッファのデーター生成用
static bool buf_num = 0;//バッファ選択用

static uint16_t sample_val = 0;//サンプル位置

static uint16_t kukei_val[MAX_SOURCE] = {0};//バッファデーター生成用で使う

static uint8_t kukei[MAX_SOURCE] = {0};//バッファデーター生成用で使う

const static uint8_t step_pulse[4]={0b0001,0b0010,0b0100,0b1000};//ステッピングモーターのパターン

static USB_AUDIO_MIDI_EVENT_PACKET midi_source[MAX_SOURCE] = {0};//MIDIデーターのバッファ

static void AllStepping(uint16_t dat)
{
    digitalWrite(MOTOR0_0, (dat) & 1);
    digitalWrite(MOTOR0_1, (dat >> 1) & 1);
    digitalWrite(MOTOR0_2, (dat >> 2) & 1);
    digitalWrite(MOTOR0_3, (dat >> 3) & 1);
    digitalWrite(MOTOR1_0, (dat >> 4) & 1);
    digitalWrite(MOTOR1_1, (dat >> 5) & 1);
    digitalWrite(MOTOR1_2, (dat >> 6) & 1);
    digitalWrite(MOTOR1_3, (dat >> 7) & 1);
    digitalWrite(MOTOR2_0, (dat >> 8) & 1);
    digitalWrite(MOTOR2_1, (dat >> 9) & 1);
    digitalWrite(MOTOR2_2, (dat >> 10) & 1);
    digitalWrite(MOTOR2_3, (dat >> 11) & 1);
    digitalWrite(MOTOR3_0, (dat >> 12) & 1);
    digitalWrite(MOTOR3_1, (dat >> 13) & 1);
    digitalWrite(MOTOR3_2, (dat >> 14) & 1);
    digitalWrite(MOTOR3_3, (dat >> 15) & 1);
}


static uint8_t step=0;
// please put 0 or 1
void Stepping(char direction)
{
    step += direction;
    step %= 4;

    switch (step)
    {
    case 0:
        digitalWrite(MOTOR0_0, HIGH);
        digitalWrite(MOTOR0_1, LOW);
        digitalWrite(MOTOR0_2, LOW);
        digitalWrite(MOTOR0_3, LOW);
        break;
    case 1:
        digitalWrite(MOTOR0_0, LOW);
        digitalWrite(MOTOR0_1, HIGH);
        digitalWrite(MOTOR0_2, LOW);
        digitalWrite(MOTOR0_3, LOW);
        break;
    case 2:
        digitalWrite(MOTOR0_0, LOW);
        digitalWrite(MOTOR0_1, LOW);
        digitalWrite(MOTOR0_2, HIGH);
        digitalWrite(MOTOR0_3, LOW);
        break;
    case 3:
        digitalWrite(MOTOR0_0, LOW);
        digitalWrite(MOTOR0_1, LOW);
        digitalWrite(MOTOR0_2, LOW);
        digitalWrite(MOTOR0_3, HIGH);
        break;
    }
}

void IRAM_ATTR Speaker()
{
    AllStepping(step_data[buf_num][sample_val]);
    sample_val++;
    if (sample_val >= SAMPLE_SIZE)
    {
        sample_val = 0;
        buf_num = !buf_num;
    }
}

void SourceQueue(USB_AUDIO_MIDI_EVENT_PACKET mep) {
    if (mep.COMMAND == MIDI_CIN_NOTE_OFF) {
        for (uint8_t i = 0; i < MAX_SOURCE; i++) {
            //printf("%d,%d\r\n",midi_source[i].MIDI_0 ,mep.MIDI_0);
            if (midi_source[i].MIDI_0 == mep.MIDI_0) {
                midi_source[i] = mep;
                return;
            }
        }
    }
    if (mep.COMMAND == MIDI_CIN_NOTE_ON) {
        for (uint8_t i = 0; i < MAX_SOURCE; i++) {
            if (midi_source[i].COMMAND != MIDI_CIN_NOTE_ON) {
                midi_source[i] = mep;
                return;
            }
        }
    }
    memset(midi_source,0,sizeof(USB_AUDIO_MIDI_EVENT_PACKET)*MAX_SOURCE);
}

void midi_process()
{
    if (buf_num_client ^ buf_num)
    {

        // printf("%d\r\n", source_num);
        // printf("%d,%d,%d,%d\r\n",midi_source[0].CN,midi_source[1].CN,midi_source[2].CN,midi_source[3].CN);
        buf_num_client = buf_num;

        uint16_t calculation_val = SAMPLE_SIZE * HOKAN;

        for (uint16_t i = 0; i < calculation_val; i++)
        {

            step_data[!buf_num][i / HOKAN] = 0;

            for (uint8_t j = 0; j < MAX_SOURCE; j++)
            {
                if (kukei_val[j] > scale_data[midi_source[j].MIDI_0 - 9])
                {
                    kukei[j]++;
                    kukei[j]%=4;
                    kukei_val[j] = 0;
                }
                kukei_val[j]++;

                if (midi_source[j].COMMAND == MIDI_CIN_NOTE_ON)
                {
                    step_data[!buf_num][i / HOKAN] |= (step_pulse[kukei[j]]<<(j*4)) ;
                }
            }
        }
    }
}

void midi_init()
{
    pinMode(MOTOR0_0, OUTPUT);
    pinMode(MOTOR0_1, OUTPUT);
    pinMode(MOTOR0_2, OUTPUT);
    pinMode(MOTOR0_3, OUTPUT);
    pinMode(MOTOR1_0, OUTPUT);
    pinMode(MOTOR1_1, OUTPUT);
    pinMode(MOTOR1_2, OUTPUT);
    pinMode(MOTOR1_3, OUTPUT);
    pinMode(MOTOR2_0, OUTPUT);
    pinMode(MOTOR2_1, OUTPUT);
    pinMode(MOTOR2_2, OUTPUT);
    pinMode(MOTOR2_3, OUTPUT);
    pinMode(MOTOR3_0, OUTPUT);
    pinMode(MOTOR3_1, OUTPUT);
    pinMode(MOTOR3_2, OUTPUT);
    pinMode(MOTOR3_3, OUTPUT);

    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &Speaker, true);
    timerAlarmWrite(timer, 50, true);
    timerAlarmEnable(timer);
}