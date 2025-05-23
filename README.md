
# Dzx_HW2_esp32  

## 说明  
xlab嵌入式作业（2025.5.2-2025.5.15）  


## 第一部分：流水灯  
1. **电路连接**  
   - 注意LED正负极，接地为面包板边缘整条线（中间有断口，需确认连通性）。  
2. **代码实现**  
   - 控制每个串口的输出状态，注意引脚连接顺序与代码逻辑对应。  


## 第二部分：串口通信  
1. **电路连接**  
   - 确保串口模块与ESP32正确连接，使用串行监视器观察数据。  
2. **功能实现**  
   - 保持波特率与代码一致，通过即时命令控制流水灯亮灭。  


## 第三部分：OLED显示  
1. **基础功能**  
   - 成功在OLED屏幕上打印字符，参考坐标设置示例：  

2. **动效设计**  
   - **眨眼效果**：通过两种面部状态（睁眼/闭眼）切换实现，控制眼睑坐标变化频率。  
   - **开心触发**：设定随机概率（如10%）触发开心表情，调整嘴角曲线方程：
   -      ```c  
      // 标准开口向下抛物线公式 y = -a*(x-h)² + k
       int relativeX = x - mouthCenterX;
       int y = mouthBottomY - a * relativeX * relativeX;
     ```     
   - 关键参数：坐标计算、状态切换间隔、随机数生成逻辑。
