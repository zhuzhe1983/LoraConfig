#ifndef MAP_DISPLAY_H
#define MAP_DISPLAY_H

#include <M5CoreS3.h>

class MapDisplay {
private:
    int16_t centerX;
    int16_t centerY;
    float currentZoom;
    float mapLat;
    float mapLng;
    
    // 绘制节点标记
    void drawMarker(int16_t x, int16_t y, const char* label);
    // 计算屏幕坐标
    void latLngToScreen(float lat, float lng, int16_t& x, int16_t& y);

public:
    MapDisplay();
    void begin();
    void setCenter(float lat, float lng);
    void setZoom(float zoom);
    void update();
    void drawNode(float lat, float lng, const char* nodeId);
    void clear();
};

#endif