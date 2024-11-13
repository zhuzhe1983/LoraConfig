#include "MapDisplay.h"

MapDisplay::MapDisplay() {
    centerX = M5.Lcd.width() / 2;
    centerY = M5.Lcd.height() / 2;
    currentZoom = 15;
    mapLat = 0;
    mapLng = 0;
}

void MapDisplay::begin() {
    M5.Lcd.fillScreen(TFT_BLACK);
    M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
}

void MapDisplay::latLngToScreen(float lat, float lng, int16_t& x, int16_t& y) {
    // 简单的等距圆柱投影
    float scale = pow(2, currentZoom) * M5.Lcd.width() / 360.0;
    
    x = centerX + (lng - mapLng) * scale;
    y = centerY - (lat - mapLat) * scale * cos(mapLat * PI / 180.0);
}

void MapDisplay::drawMarker(int16_t x, int16_t y, const char* label) {
    // 绘制位置标记
    M5.Lcd.fillTriangle(x, y-15, x-5, y-5, x+5, y-5, TFT_RED);
    M5.Lcd.fillCircle(x, y-5, 5, TFT_RED);
    
    // 绘制标签
    M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
    M5.Lcd.drawString(label, x+10, y-10, 2);
}

void MapDisplay::setCenter(float lat, float lng) {
    mapLat = lat;
    mapLng = lng;
    update();
}

void MapDisplay::setZoom(float zoom) {
    if (zoom < 1) zoom = 1;
    if (zoom > 20) zoom = 20;
    currentZoom = zoom;
    update();
}

void MapDisplay::update() {
    // 清除屏幕并重绘基本地图元素
    clear();
    
    // 绘制中心点标记
    drawMarker(centerX, centerY, "HERE");
    
    // 绘制比例尺
    int scaleWidth = 100;
    M5.Lcd.drawLine(10, M5.Lcd.height()-20, 10+scaleWidth, M5.Lcd.height()-20, TFT_WHITE);
    M5.Lcd.drawString(String(scaleWidth * 111.32 / pow(2, currentZoom), 1) + "km", 
                      10, M5.Lcd.height()-40, 2);
}

void MapDisplay::drawNode(float lat, float lng, const char* nodeId) {
    int16_t x, y;
    latLngToScreen(lat, lng, x, y);
    
    // 检查是否在屏幕范围内
    if (x >= 0 && x < M5.Lcd.width() && y >= 0 && y < M5.Lcd.height()) {
        drawMarker(x, y, nodeId);
    }
}

void MapDisplay::clear() {
    M5.Lcd.fillScreen(TFT_BLACK);
}