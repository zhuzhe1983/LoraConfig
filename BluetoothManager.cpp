#include "BluetoothManager.h"

// ... (保持之前的代码不变)

void BluetoothManager::handleConfig(const String& jsonStr) {
    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, jsonStr);
    
    if (error) {
        Serial.println("配置解析错误");
        return;
    }
    
    // 处理位置信息
    if (doc.containsKey("type") && doc["type"] == "location") {
        float latitude = doc["latitude"];
        float longitude = doc["longitude"];
        
        // 更新设备的位置信息
        StaticJsonDocument<200> statusDoc;
        statusDoc["nodeId"] = nodeId;
        statusDoc["latitude"] = latitude;
        statusDoc["longitude"] = longitude;
        
        String status;
        serializeJson(statusDoc, status);
        sendStatus(status);
        return;
    }
    
    // 处理其他配置
    if (onConfigReceived) {
        onConfigReceived(doc);
    }
}

// ... (保持其他代码不变)