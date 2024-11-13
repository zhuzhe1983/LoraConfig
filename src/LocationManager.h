#ifndef LOCATION_MANAGER_H
#define LOCATION_MANAGER_H

#include "BoardManager.h"
#include <TinyGPS++.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

class LocationManager {
private:
    HardwareSerial& gpsSerial;
    TinyGPSPlus& gps;
    
    float currentLat;
    float currentLng;
    bool hasValidFix;
    bool hasGpsModule;
    unsigned long lastIpGeoUpdate;
    
    static const unsigned long IP_GEO_UPDATE_INTERVAL = 300000; // 5 minutes
    
    bool updateFromIpGeo();
    void parseIpGeoResponse(const String& response);

public:
    LocationManager(HardwareSerial& serial, TinyGPSPlus& gpsInstance);
    void begin();
    void update();
    bool hasValidLocation() const { return hasValidFix; }
    float getLatitude() const { return currentLat; }
    float getLongitude() const { return currentLng; }
    void setLocation(float lat, float lng);
    bool hasGPS() const { return hasGpsModule; }
};

#endif