#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH      128   // OLED display width
#define SCREEN_HEIGHT     64    // OLED display height
#define OLED_RESET        -1    // Reset pin
#define SCREEN_ADDRESS    0x3C  // I2C address

Adafruit_SSD1306 OLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// 表情状态
enum Expression {
  NORMAL,
  BLINKING,
  HAPPY
};

Expression currentExpression = NORMAL;
unsigned long lastBlinkTime = 0;
const unsigned long blinkInterval = 3000; // 眨眼间隔
const unsigned long blinkDuration = 200;  // 眨眼持续时间

void drawFace(Expression expr) {
  OLED.clearDisplay();
  
  // 绘制嘴巴 - 根据表情状态变化
  if (expr == HAPPY) {
    //开心——抛物线嘴巴
int mouthCenterX = 64;    // 嘴巴中心X坐标
int mouthBottomY = 50;    // 嘴巴最低点Y坐标
int mouthWidth = 28;      // 嘴巴宽度
int mouthHeight = 8;      // 嘴巴上翘高度

// 预计算抛物线参数
float a = mouthHeight / pow(mouthWidth/2.0, 2);

// 绘制抛物线
for(int x = mouthCenterX - mouthWidth/2; x <= mouthCenterX + mouthWidth/2; x++) {
    // 标准开口向下抛物线公式 y = -a*(x-h)² + k
    int relativeX = x - mouthCenterX;
    int y = mouthBottomY - a * relativeX * relativeX;
    
    // 绘制嘴巴线（加粗）
    OLED.drawPixel(x, y, SSD1306_WHITE);
    OLED.drawPixel(x, y+1, SSD1306_WHITE);
    OLED.drawPixel(x, y-1, SSD1306_WHITE);
}
  } else {
    // 普通嘴或眨眼时的嘴 - 直线
    OLED.drawLine(50, 45, 78, 45, SSD1306_WHITE);
  }
  
  // 绘制眼睛
  if (expr == BLINKING) {
    // 眨眼状态 - 画两条短线表示闭眼
    OLED.drawLine(45, 25, 55, 25, SSD1306_WHITE); // 左眼
    OLED.drawLine(73, 25, 83, 25, SSD1306_WHITE); // 右眼
  } else {
    // 正常状态 - 画圆圈眼睛
    OLED.fillCircle(50, 25, 5, SSD1306_WHITE); // 左眼
    OLED.fillCircle(78, 25, 5, SSD1306_WHITE); // 右眼
  }
  
  OLED.display();
}

void setup() {
  // OLED初始化
  if(!OLED.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    while(1); // 初始化失败，停止执行
  }
  
  // 绘制初始表情
  drawFace(NORMAL);
  randomSeed(analogRead(0));
}

void loop() {
  unsigned long currentTime = millis();
  
  // 随机决定是否变成开心表情
  if (random(1000) < 5) { // 大约每10秒有0.5%的几率
    currentExpression = HAPPY;
    drawFace(currentExpression);
    delay(1000); // 保持开心表情1秒
    currentExpression = NORMAL;
    drawFace(currentExpression);
    lastBlinkTime = currentTime; // 重置眨眼计时
    return;
  }
  
  // 处理眨眼逻辑
  if (currentExpression == NORMAL && currentTime - lastBlinkTime > blinkInterval) {
    currentExpression = BLINKING;
    drawFace(currentExpression);
    lastBlinkTime = currentTime;
  } else if (currentExpression == BLINKING && currentTime - lastBlinkTime > blinkDuration) {
    currentExpression = NORMAL;
    drawFace(currentExpression);
  }
  
  delay(50); 
}