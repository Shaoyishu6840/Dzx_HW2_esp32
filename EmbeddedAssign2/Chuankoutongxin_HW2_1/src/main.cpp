#include <Arduino.h>

// 定义LED连接的引脚
const int ledPins[] = {32, 14, 21, 25, 22, 23};
// 定义LED的数量
const int numLeds = sizeof(ledPins) / sizeof(ledPins[0]);
// 定义延时时间（毫秒）
const int delayTime = 500;
bool isFlashing = true;
bool ledsState[numLeds] = {false}; // 存储每个LED的状态

void setup() {
  // 将所有LED引脚设置为输出模式
  for (int i = 0; i < numLeds; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW); // 确保初始状态为关闭
  }
  Serial.begin(9600); // 初始化串口通信，波特率115200
  Serial.println("ESP32 LED Controller Ready");
  Serial.println("Commands:");
  Serial.println("  0-5: 点亮对应的LED灯");
  Serial.println("  f: 开始流水灯模式");
  Serial.println("  s: 关闭流水灯并熄灭所有灯");
  Serial.println("  a: 打卡所有灯");
  Serial.println("  n: 关闭所有灯");
}

void loop() {
  // 处理串口命令
  if (Serial.available() > 0) {
    char command = Serial.read();
    
    // 控制单个LED 
    if (command >= '0' && command <= '5') {
      isFlashing = false; // 停止闪烁模式
      int ledIndex = command -'0';
      if (ledIndex < numLeds) {
        ledsState[ledIndex] = !ledsState[ledIndex]; // 切换状态
        digitalWrite(ledPins[ledIndex], ledsState[ledIndex]);
        Serial.print("LED ");
        Serial.print(ledIndex);
        Serial.println(ledsState[ledIndex] ? " ON" : " OFF");
      }
    }
    // 开始闪烁模式
    else if (command == 'f') {
      isFlashing = true;
      Serial.println("Starting flash mode");
    }
    // 停止闪烁并关闭所有LED
    else if (command == 's') {
      isFlashing = false;
      for (int i = 0; i < numLeds; i++) {
        ledsState[i] = false;
        digitalWrite(ledPins[i], LOW);
      }
      Serial.println("all LEDs OFF");
    }
    // 打开所有LED
    else if (command == 'a') {
      isFlashing = false;
      for (int i = 0; i < numLeds; i++) {
        ledsState[i] = true;
        digitalWrite(ledPins[i], HIGH);
      }
      Serial.println("all LEDs ON");
    }
    // 关闭所有LED
    else if (command == 'n') {
      isFlashing = false;
      for (int i = 0; i < numLeds; i++) {
        ledsState[i] = false;
        digitalWrite(ledPins[i], LOW);
      }
      Serial.println("all LEDs OFF");
    }
  }

  // 闪烁模式
  if (isFlashing) {
    for (int i = 0; i < numLeds; i++) {
      digitalWrite(ledPins[i], HIGH); // 点亮当前LED
      delay(delayTime); // 延时
      digitalWrite(ledPins[i], LOW);  // 熄灭当前LED
      delay(delayTime); // 延时
      
      // 检查是否有新命令，允许中断闪烁
      if (Serial.available() > 0) break;
    }
  }
}