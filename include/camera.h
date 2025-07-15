#ifndef CAMERA_H
#define CAMERA_H

#include <Arduino.h>
#include "esp_camera.h"

/**
 * ESP32S3 OV2640摄像头模块
 * 提供摄像头初始化、配置和图像采集功能
 */
class Camera {
private:
    camera_config_t config;
    bool initialized;

    void initConfig(); // 初始化摄像头配置
    void setDefaultParameters(); // 设置默认摄像头参数

public:
    Camera();

    /**
     * 初始化摄像头
     * @return true 初始化成功，false 初始化失败
     */
    bool init();

    /**
     * 捕获图像
     * @return 图像帧缓冲区指针，失败返回nullptr
     */
    camera_fb_t* captureImage();

    /**
     * 释放图像帧缓冲区
     * @param fb 要释放的帧缓冲区
     */
    void releaseFrameBuffer(camera_fb_t* fb);

    /**
     * 获取摄像头传感器
     * @return 传感器指针
     */
    sensor_t* getSensor();

    /**
     * 设置图像分辨率
     * @param frameSize 帧大小
     * @return true 设置成功，false 设置失败
     */
    bool setFrameSize(framesize_t frameSize);

    /**
     * 设置JPEG质量
     * @param quality 质量值 (0-63, 数值越小质量越高)
     * @return true 设置成功，false 设置失败
     */
    bool setJpegQuality(int quality);

    /**
     * 设置图像亮度
     * @param brightness 亮度值 (-2 到 2)
     * @return true 设置成功，false 设置失败
     */
    bool setBrightness(int brightness);
    
    /**
     * 设置图像对比度
     * @param contrast 对比度值 (-2 到 2)
     * @return true 设置成功，false 设置失败
     */
    bool setContrast(int contrast);
    
    /**
     * 设置图像饱和度
     * @param saturation 饱和度值 (-2 到 2)
     * @return true 设置成功，false 设置失败
     */
    bool setSaturation(int saturation);
    
    /**
     * 设置水平镜像
     * @param enable true 启用镜像，false 禁用镜像
     * @return true 设置成功，false 设置失败
     */
    bool setHorizontalMirror(bool enable);
    
    /**
     * 设置垂直翻转
     * @param enable true 启用翻转，false 禁用翻转
     * @return true 设置成功，false 设置失败
     */
    bool setVerticalFlip(bool enable);

    /**
     * 获取摄像头状态
     */
    void printStatus();

    /**
     * 打印图像信息
     * @param fb 图像帧缓冲区
     */
    void printImageInfo(camera_fb_t* fb);

    /**
     * 获取摄像头配置
     * @return 摄像头配置结构体
     */
    camera_config_t getConfig();

    /**
     * 检查摄像头是否已初始化
     * @return true 已初始化，false 未初始化
     */
    bool isInitialized();

};


#endif