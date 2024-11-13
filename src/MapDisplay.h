#ifndef MAP_DISPLAY_H
#define MAP_DISPLAY_H

#include "BoardManager.h"

class MapDisplay {
private:
    TFT_eSPI& tft;
    int16_t centerX;
    int16_t centerY;
    float currentZoom;
    float mapLat;
    float mapLng;
    bool hasMapSupport;
    
    void drawMarker(int16_t x, int16_t y, const char* label);
    void latLngToScreen(float lat, float lng, int16_t& x, int16_t& y);
    void drawNoMapMessage();

public:
    MapDisplay();
    void begin();
    void setCenter(float lat, float lng);
    void setZoom(float zoom);
    void update();
    void drawNode(float lat, float lng, const char* nodeId);
    void clear();
    bool isMapSupported() const { return hasMapSupport; }
};

#endif