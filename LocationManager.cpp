#include "LocationManager.h"

LocationManager::LocationManager(HardwareSerial& serial, TinyGPSPlus& gpsInstance)
    : gpsSerial(serial), gps(gpsInstance) {
    currentLat = 0;
    currentLng = 0;
    hasValidFix = false;
    lastIpGeoUpdate = 0;
}

void LocationManager::begin() {
    // 如果没有GPS定位，尝试IP地理位置
    if (!hasValidFix) {
        updateFromIpGeo();
    }
}

void LocationManager::update() {
    // 处理GPS数据
    while (gpsSerial.available() > 0) {
        if (gps.encode(gpsSerial.read())) {
            if (gps.location.isValid()) {
                currentLat = gps.location.lat();
                currentLng = gps.location.lng();
                hasValidFix = true;
                return;
            }
        }
    }
    
    // 如果没有GPS信号，且距离上次IP地理位置更新超过间隔时间
    if (!hasValidFix && (millis() - lastIpGeoUpdate > IP_GEO_UPDATE_INTERVAL)) {
        updateFromIpGeo();
    }
}

bool LocationManager::updateFromIpGeo() {
    if (WiFi.status() != WL_CONNECTED) {
        return false;
    }
    
    HTTPClient http;
    http.begin("http://ip-api.com/json/?fields=lat,lon");
    
    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK) {
        String response = http.getString();
        parseIpGeoResponse(response);
        lastIpGeoUpdate = millis();
        http.end();
        return true;
    }
    
    http.end();
    return false;
}

void LocationManager::parseIpGeoResponse(const String& response) {
    StaticJsonDocument<128> doc;
    DeserializationError error = deserializeJson(doc, response);
    
    if (!error) {
        currentLat = doc["lat"].as<float>();
        currentLng = doc["lon"].as<float>();
        hasValidFix = true;
    }
}

// 添加设置位置的方法
void LocationManager::setLocation(float lat, float lng) {
    if (!hasValidFix) {
        currentLat = lat;
        currentLng = lng;
        hasValidFix = true;
    }
}