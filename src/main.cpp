#include <Arduino.h>
#include "midi_stepper.h"
#include <Wire.h>

#define I2C_ADDRESS 0x32
#define I2C_SDA_PIN 21
#define I2C_SCK_PIN 22
#define I2C_FREQ 400000

#define QUEUE_SIZE 8

uint8_t enqueue=0;
uint8_t dequeue=0;
USB_AUDIO_MIDI_EVENT_PACKET mididata;
USB_AUDIO_MIDI_EVENT_PACKET mididata_out[QUEUE_SIZE]={0};
uint8_t *p_mididata = (uint8_t *)&mididata;


// I2C受信割り込みハンドラ
void i2c_recv(int len)
{
  if (len == 4)
  {
    *(p_mididata) = Wire.read();
    *(p_mididata + 1) = Wire.read();
    *(p_mididata + 2) = Wire.read();
    *(p_mididata + 3) = Wire.read();

    mididata_out[enqueue]=mididata;
    enqueue++;
    enqueue%=QUEUE_SIZE;
  }
  //printf("update\r\n");
}

void setup()
{
  //disableCore0WDT();
  //xTaskCreatePinnedToCore(Core0a, "Core0a", 8192, NULL, 1, NULL, 0);
  delay(100);
  Serial.begin(115200); // デバッグ用
  delay(100);
  Wire.onReceive(i2c_recv); // 割り込みハンドラ設定
  delay(100);
  Wire.begin(I2C_ADDRESS, I2C_SDA_PIN, I2C_SCK_PIN, I2C_FREQ); // I2C通信初期化
  delay(500);
  Serial.println("START");
  Serial.printf( "getXtalFrequencyMhz() : %d\n", getXtalFrequencyMhz() );
  Serial.printf( "getCpuFrequencyMhz() : %d\n", getCpuFrequencyMhz() );
  Serial.printf( "getApbFrequency() : %d\n", getApbFrequency() );
  Serial.printf( "\n" );
  delay(100);
  //midi_init();
  
  pinMode(MOTOR0_0, OUTPUT);
    pinMode(MOTOR0_1, OUTPUT);
    pinMode(MOTOR0_2, OUTPUT);
    pinMode(MOTOR0_3, OUTPUT);
}

void loop()
{
  // put your main code here, to run repeatedly:
  if (enqueue!=dequeue)
  {
    Serial.printf("CNL=%d,CMD=%d,num=%d\r\n", mididata.CHANNEL,mididata.COMMAND, mididata.MIDI_0);
    //SourceQueue(mididata_out[dequeue]);
    dequeue++;
    dequeue%=QUEUE_SIZE;
    
  } 
  //midi_process();

  if (mididata.COMMAND == MIDI_CIN_NOTE_ON)
  {
    Stepping(1);
    delayMicroseconds(onkai[mididata.MIDI_0 + 3]);
  }else{
    digitalWrite(MOTOR0_0,LOW);
    digitalWrite(MOTOR0_1,LOW);
    digitalWrite(MOTOR0_2,LOW);
    digitalWrite(MOTOR0_3,LOW);
  }

  
}
