#include <Arduino.h>

// 定义LED连接的引脚
const int ledPins[] = {32,14,21,25, 22, 23};
// 定义LED的数量
const int numLeds = sizeof(ledPins) / sizeof(ledPins[0]);
// 定义延时时间（毫秒）
const int delayTime = 500;
bool isFlashing = true;

void setup() {
  // 将所有LED引脚设置为输出模式
  for (int i = 0; i < numLeds; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
  Serial.begin(115200); // 初始化串口通信
}

void loop() {
  if(Serial.available()>0){
    char command = Serial.read();
    if(command =='s'){
      isFlashing = false;
    }
  }
  // 点亮LED
  if(isFlashing){
    for (int i = 0; i < numLeds; i++) {
      digitalWrite(ledPins[i], HIGH); // 点亮当前LED
      delay(delayTime); // 延时
      digitalWrite(ledPins[i], LOW);  // 熄灭当前LED
      delay(delayTime); // 延时
    }
  }else{
    for(int i = 0; i < numLeds; i++) {
      digitalWrite(ledPins[i], LOW); 
    }
  }
}
