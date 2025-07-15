#include "camera.h"
#include "config.h"

Camera::Camera() : initialized(false) {
    // 构造函数初始化
}

bool Camera::init() {
    if(initialized) {
        Serial.println("摄像头已经初始化");
        return true;
    }

    initConfig();

    // 初始化摄像头
    esp_err_t err = esp_camera_init(&config);
    if(err != ESP_OK) {
        Serial.printf("摄像头初始化失败，错误码: 0x%x\n", err);
        return false;   
    }

    Serial.println("摄像头初始化成功！");

    // 设置默认参数
    setDefaultParameters();

    initialized = true;
    return true;

}

camera_fb_t* Camera::captureImage() {
    if(!initialized) {
        Serial.println("摄像头未初始化，无法捕获图像");
        return nullptr;
    }

    Serial.println("开始捕获图像...");

    // 获取图像帧缓冲区
    camera_fb_t* fb = esp_camera_fb_get();
    if(!fb) {
        Serial.println("图像捕获失败");
        return nullptr;
    }

    Serial.println("图像捕获成功！");
    return fb;
}

void Camera::releaseFrameBuffer(camera_fb_t* fb) {
    if(fb) {
        esp_camera_fb_return(fb);
    }
}

sensor_t* Camera::getSensor() {
    if(!initialized) {
        return nullptr;
    }
    return esp_camera_sensor_get();
}

bool Camera::setFrameSize(framesize_t frameSize) {
    sensor_t* s = getSensor();
    if (s) {
        return s->set_framesize(s, frameSize) == 0;
    }
    return false;
}

bool Camera::setJpegQuality(int quality) {
    sensor_t* s = getSensor();
    if (s) {
        return s->set_quality(s, quality) == 0;
    }
    return false;
}

bool Camera::setBrightness(int brightness) {
    sensor_t* s = getSensor();
    if (s) {
        return s->set_brightness(s, brightness) == 0;
    }
    return false;
}

bool Camera::setContrast(int contrast) {
    sensor_t* s = getSensor();
    if (s) {
        return s->set_contrast(s, contrast) == 0;
    }
    return false;
}

bool Camera::setSaturation(int saturation) {
    sensor_t* s = getSensor();
    if (s) {
        return s->set_saturation(s, saturation) == 0;
    }
    return false;
}

bool Camera::setHorizontalMirror(bool enable) {
    sensor_t* s = getSensor();
    if (s) {
        return s->set_hmirror(s, enable ? 1 : 0) == 0;
    }
    return false;
}

bool Camera::setVerticalFlip(bool enable) {
    sensor_t* s = getSensor();
    if (s) {
        return s->set_vflip(s, enable ? 1 : 0) == 0;
    }
    return false;
}

void Camera::printStatus() {
    if (!initialized) {
        Serial.println("摄像头未初始化");
        return;
    }
    
    sensor_t* s = getSensor();
    if (s) {
        Serial.println("=== 摄像头状态信息 ===");
        Serial.printf("传感器ID: 0x%02X\n", s->id.PID);
        Serial.printf("像素格式: %s\n", (config.pixel_format == PIXFORMAT_JPEG) ? "JPEG" : "RAW");
        Serial.printf("帧大小: %d\n", config.frame_size);
        Serial.printf("JPEG质量: %d\n", config.jpeg_quality);
        Serial.printf("帧缓冲区数量: %d\n", config.fb_count);
        Serial.printf("时钟频率: %d Hz\n", config.xclk_freq_hz);
        Serial.println("=====================");
    }
}


void Camera::printImageInfo(camera_fb_t* fb) {
    if (!fb) {
        Serial.println("无效的图像帧缓冲区");
        return;
    }
    
    Serial.printf("图像大小: %u bytes\n", fb->len);
    Serial.printf("图像宽度: %d pixels\n", fb->width);
    Serial.printf("图像高度: %d pixels\n", fb->height);
    Serial.printf("图像格式: %s\n", (fb->format == PIXFORMAT_JPEG) ? "JPEG" : "RAW");
    Serial.printf("时间戳: %lld\n", fb->timestamp.tv_sec);
    
    // 打印图像数据的前16个字节（十六进制）
    Serial.print("图像数据开头: ");
    for(int i = 0; i < 16 && i < fb->len; i++) {
        Serial.printf("%02X ", fb->buf[i]);
    }
    Serial.println();
}

camera_config_t Camera::getConfig() {
    return config;
}

bool Camera::isInitialized() {
    return initialized;
}

void Camera::initConfig() {
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sccb_sda = SIOD_GPIO_NUM;
    config.pin_sccb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000; // 20MHz
    config.pixel_format = PIXFORMAT_JPEG;

    // 根据可用PSRAM调整帧大小和质量
    if(psramFound()){
        config.frame_size = CAMERA_FRAME_SIZE;
        config.jpeg_quality = CAMERA_JPEG_QUALITY;
        config.fb_count = 2;
        Serial.println("PSRAM检测到，使用高质量设置");
    } else {
        config.frame_size = FRAMESIZE_SVGA;
        config.jpeg_quality = 12;
        config.fb_count = 1;
        Serial.println("未检测到PSRAM，使用标准设置");
    }
}

void Camera::setDefaultParameters() {
    sensor_t* s = getSensor();
    if (s) {
        // 设置摄像头参数
        s->set_brightness(s, 0);     // 亮度 (-2 到 2)
        s->set_contrast(s, 0);       // 对比度 (-2 到 2)
        s->set_saturation(s, 0);     // 饱和度 (-2 到 2)
        s->set_special_effect(s, 0); // 特殊效果 (0-6)
        s->set_whitebal(s, 1);       // 白平衡开启
        s->set_awb_gain(s, 1);       // 自动白平衡增益开启
        s->set_wb_mode(s, 0);        // 白平衡模式
        s->set_exposure_ctrl(s, 1);  // 曝光控制开启
        s->set_aec2(s, 0);           // AEC算法
        s->set_ae_level(s, 0);       // AE级别 (-2 到 2)
        s->set_aec_value(s, 300);    // 曝光值
        s->set_gain_ctrl(s, 1);      // 增益控制开启
        s->set_agc_gain(s, 0);       // AGC增益
        s->set_gainceiling(s, (gainceiling_t)0); // 增益上限
        s->set_bpc(s, 0);            // 黑像素取消
        s->set_wpc(s, 1);            // 白像素取消
        s->set_raw_gma(s, 1);        // 原始伽马
        s->set_lenc(s, 1);           // 镜头校正
        s->set_hmirror(s, 0);        // 水平镜像
        s->set_vflip(s, 0);          // 垂直翻转
        s->set_dcw(s, 1);            // DCW (降采样)
        s->set_colorbar(s, 0);       // 颜色条测试模式
        
        Serial.println("摄像头参数设置完成");
    }
}