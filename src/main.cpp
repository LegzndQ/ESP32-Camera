/**
 * ESP32S3控制OV2640摄像头
 */

#include <Arduino.h>
#include "camera.h"

// 创建摄像头模块实例
Camera camera;

void setup() {
    // 初始化串口
    Serial.begin(115200);
    Serial.setDebugOutput(true);
    Serial.println();
    Serial.println("ESP32S3 OV2640摄像头模块化初始化开始...");
    
    // 打印芯片信息
    Serial.printf("ESP32芯片型号: %s\n", ESP.getChipModel());
    Serial.printf("芯片版本: %d\n", ESP.getChipRevision());
    Serial.printf("CPU频率: %d MHz\n", ESP.getCpuFreqMHz());
    Serial.printf("Flash大小: %d bytes\n", ESP.getFlashChipSize());
    Serial.printf("可用内存: %d bytes\n", ESP.getFreeHeap());
    Serial.printf("PSRAM大小: %d bytes\n", ESP.getPsramSize());
    
    // 初始化摄像头模块
    if (camera.init()) {
        Serial.println("摄像头模块初始化成功，系统就绪！");
        camera.printStatus();
    } else {
        Serial.println("摄像头模块初始化失败，系统停止");
        while(1) {
            delay(1000);
        }
    }
    
    delay(2000); // 等待2秒让摄像头稳定
}

void loop() {
    // 每5秒捕获一次图像
    static unsigned long lastCapture = 0;
    unsigned long currentTime = millis();
    
    if (currentTime - lastCapture >= 5000) {
        // 捕获图像
        camera_fb_t* fb = camera.captureImage();
        
        if (fb) {
            // 打印图像信息
            camera.printImageInfo(fb);
            
            // 在这里可以处理图像数据
            // 例如：保存到SD卡、通过WiFi传输、进行图像处理等
            
            // 释放帧缓冲区
            camera.releaseFrameBuffer(fb);
        }
        
        lastCapture = currentTime;
        
        // 打印内存使用情况
        Serial.printf("可用堆内存: %d bytes\n", ESP.getFreeHeap());
        if (ESP.getPsramSize() > 0) {
            Serial.printf("可用PSRAM: %d bytes\n", ESP.getFreePsram());
        }
        Serial.println("---");
    }
    
    delay(100);
}
