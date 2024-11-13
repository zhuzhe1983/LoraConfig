#include <M5CoreS3.h>
#include <SPI.h>
#include <LoRa.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include "MapDisplay.h"
#include "VoiceInput.h"
#include "LocationManager.h"
#include "BluetoothManager.h"

// ... (前面的引脚定义和变量保持不变)

// 添加蓝牙管理器
BluetoothManager* btManager;

void setup() {
    // ... (之前的初始化代码保持不变)
    
    // 初始化蓝牙管理器
    btManager = new BluetoothManager(nodeId);
    btManager->begin();
    
    // 设置蓝牙配置回调
    btManager->setConfigCallback([](JsonDocument& config) {
        // 处理LoRa配置
        if (config.containsKey("frequency")) {
            long freq = config["frequency"];
            LoRa.setFrequency(freq);
        }
        if (config.containsKey("txPower")) {
            int power = config["txPower"];
            LoRa.setTxPower(power);
        }
        if (config.containsKey("spreadingFactor")) {
            int sf = config["spreadingFactor"];
            LoRa.setSpreadingFactor(sf);
        }
        if (config.containsKey("signalBandwidth")) {
            long bw = config["signalBandwidth"];
            LoRa.setSignalBandwidth(bw);
        }
        if (config.containsKey("codingRate")) {
            int cr = config["codingRate"];
            LoRa.setCodingRate4(cr);
        }
        
        // 处理WiFi配置
        if (config.containsKey("wifi")) {
            JsonObject wifi = config["wifi"];
            if (wifi.containsKey("ssid") && wifi.containsKey("password")) {
                const char* ssid = wifi["ssid"];
                const char* password = wifi["password"];
                WiFi.begin(ssid, password);
            }
        }
        
        // 处理节点ID配置
        if (config.containsKey("nodeId")) {
            nodeId = config["nodeId"].as<String>();
        }
    });
    
    // 设置蓝牙消息回调
    btManager->setMessageCallback([](const String& receiver, const String& content) {
        StaticJsonDocument<200> doc;
        doc["type"] = receiver == "ALL" ? CHAT_GROUP : CHAT_PRIVATE;
        doc["sender"] = nodeId;
        doc["receiver"] = receiver;
        doc["content"] = content;
        
        String jsonString;
        serializeJson(doc, jsonString);
        sendLoRaMessage(jsonString);
    });
}

void loop() {
    // ... (之前的loop代码保持不变)
    
    // 更新蓝牙状态
    btManager->update();
    
    // 在处理LoRa消息时，同时通过蓝牙通知状态
    if (packetSize) {
        String message = "";
        while (LoRa.available()) {
            message += (char)LoRa.read();
        }
        processLoRaMessage(message);
        
        // 通过蓝牙发送消息状态
        StaticJsonDocument<200> doc;
        doc["type"] = "messageReceived";
        doc["rssi"] = LoRa.packetRssi();
        doc["snr"] = LoRa.packetSnr();
        
        String status;
        serializeJson(doc, status);
        btManager->sendStatus(status);
    }
}