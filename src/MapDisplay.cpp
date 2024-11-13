#include "MapDisplay.h"

MapDisplay::MapDisplay() : tft(BOARD.getDisplay()) {
    centerX = BOARD.getDisplayWidth() / 2;
    centerY = BOARD.getDisplayHeight() / 2;
    currentZoom = 15;
    mapLat = 0;
    mapLng = 0;
    hasMapSupport = HAS_SD_CARD;
}

void MapDisplay::begin() {
    clear();
    if (!hasMapSupport) {
        drawNoMapMessage();
    }
}

void MapDisplay::drawNoMapMessage() {
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextDatum(MC_DATUM);
    tft.drawString("Map not supported", centerX, centerY - 20, 4);
    tft.drawString("on this device", centerX, centerY + 20, 4);
}

void MapDisplay::setCenter(float lat, float lng) {
    if (!hasMapSupport) return;
    mapLat = lat;
    mapLng = lng;
    update();
}

void MapDisplay::setZoom(float zoom) {
    if (!hasMapSupport) return;
    if (zoom < 1) zoom = 1;
    if (zoom > 20) zoom = 20;
    currentZoom = zoom;
    update();
}

void MapDisplay::update() {
    if (!hasMapSupport) {
        drawNoMapMessage();
        return;
    }
    
    clear();
    drawMarker(centerX, centerY, "HERE");
    
    // Draw scale
    int scaleWidth = 100;
    tft.drawLine(10, BOARD.getDisplayHeight()-20, 10+scaleWidth, BOARD.getDisplayHeight()-20, TFT_WHITE);
    tft.drawString(String(scaleWidth * 111.32 / pow(2, currentZoom), 1) + "km", 
                  10, BOARD.getDisplayHeight()-40, 2);
}

void MapDisplay::drawNode(float lat, float lng, const char* nodeId) {
    if (!hasMapSupport) return;
    
    int16_t x, y;
    latLngToScreen(lat, lng, x, y);
    
    if (x >= 0 && x < BOARD.getDisplayWidth() && y >= 0 && y < BOARD.getDisplayHeight()) {
        drawMarker(x, y, nodeId);
    }
}

void MapDisplay::latLngToScreen(float lat, float lng, int16_t& x, int16_t& y) {
    float scale = pow(2, currentZoom) * BOARD.getDisplayWidth() / 360.0;
    x = centerX + (lng - mapLng) * scale;
    y = centerY - (lat - mapLat) * scale * cos(mapLat * PI / 180.0);
}

void MapDisplay::drawMarker(int16_t x, int16_t y, const char* label) {
    tft.fillTriangle(x, y-15, x-5, y-5, x+5, y-5, TFT_RED);
    tft.fillCircle(x, y-5, 5, TFT_RED);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawString(label, x+10, y-10, 2);
}

void MapDisplay::clear() {
    tft.fillScreen(TFT_BLACK);
}