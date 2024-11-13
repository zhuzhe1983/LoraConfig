#include "LocationManager.h"

LocationManager::LocationManager(HardwareSerial& serial, TinyGPSPlus& gpsInstance)
    : gpsSerial(serial), gps(gpsInstance) {
    currentLat = 0;
    currentLng = 0;
    hasValidFix = false;
    hasGpsModule = HAS_GPS;
    lastIpGeoUpdate = 0;
}

void LocationManager::begin() {
    if (hasGpsModule) {
        gpsSerial.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);
    }
    
    if (!hasValidFix) {
        updateFromIpGeo();
    }
}

void LocationManager::update() {
    if (hasGpsModule) {
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
    }
    
    if (!hasValidFix && (millis() - lastIpGeoUpdate > IP_GEO_UPDATE_INTERVAL)) {
        updateFromIpGeo();
    }
}

// ... (rest of the LocationManager.cpp implementation remains the same)