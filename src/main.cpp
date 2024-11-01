#include <Arduino.h>

#include <Wire.h>

#define I2C_ADDRESS 0x32
#define I2C_SDA_PIN 21
#define I2C_SCK_PIN 22
#define I2C_FREQ 400000

typedef struct
{

  uint8_t DUMMY;
  uint8_t CHANNEL : 4;
  uint8_t COMMAND : 4;
  uint8_t MIDI_0;
  uint8_t MIDI_1;

} USB_AUDIO_MIDI_EVENT_PACKET;

USB_AUDIO_MIDI_EVENT_PACKET mididata;
uint8_t* p_mididata=(uint8_t*)&mididata;

bool midi_update=0;

// I2C受信割り込みハンドラ
void i2c_recv(int len)
{
  if(len==4){
    *(p_mididata)=Wire.read();
    *(p_mididata+1)=Wire.read();
    *(p_mididata+2)=Wire.read();
    *(p_mididata+3)=Wire.read();

    midi_update=true;
  }
  
}

void setup()
{
  Serial.begin(115200); // デバッグ用

  Wire.onReceive(i2c_recv); // 割り込みハンドラ設定

  Wire.begin(I2C_ADDRESS, I2C_SDA_PIN, I2C_SCK_PIN, I2C_FREQ); // I2C通信初期化

  Serial.println("START");
}

void loop()
{
  // put your main code here, to run repeatedly:
  if(midi_update){
    midi_update=false;
    Serial.printf("channel=%d,num=%d,volume=%d\r\n",mididata.CHANNEL,mididata.MIDI_0,mididata.MIDI_1);
  }
}
