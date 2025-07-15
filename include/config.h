#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

// ESP32S3-WROOM-1 摄像头引脚配置 (适用于常见的OV2640模块)
#define PWDN_GPIO_NUM     -1   // Power down pin (不使用)
#define RESET_GPIO_NUM    -1   // Reset pin (不使用)
#define XCLK_GPIO_NUM     15   // External clock
#define SIOD_GPIO_NUM     4    // SDA
#define SIOC_GPIO_NUM     5    // SCL

#define Y9_GPIO_NUM       16   // Data 9
#define Y8_GPIO_NUM       17   // Data 8  
#define Y7_GPIO_NUM       18   // Data 7
#define Y6_GPIO_NUM       12   // Data 6
#define Y5_GPIO_NUM       10   // Data 5
#define Y4_GPIO_NUM       8    // Data 4
#define Y3_GPIO_NUM       9    // Data 3
#define Y2_GPIO_NUM       11   // Data 2

#define VSYNC_GPIO_NUM    6    // Vertical sync
#define HREF_GPIO_NUM     7    // Horizontal reference
#define PCLK_GPIO_NUM     13   // Pixel clock

// 摄像头配置参数
#define CAMERA_MODEL_OV2640
#define CAMERA_FRAME_SIZE FRAMESIZE_UXGA  // 1600x1200
#define CAMERA_JPEG_QUALITY 12            // JPEG质量 (0-63, 数值越小质量越高)
#define CAMERA_FB_COUNT 1                 // 帧缓冲区数量

#endif