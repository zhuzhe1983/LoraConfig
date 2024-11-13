#ifndef BLUETOOTH_MANAGER_H
#define BLUETOOTH_MANAGER_H

#include <M5CoreS3.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <ArduinoJson.h>

// 服务和特征值UUID
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CONFIG_CHAR_UUID    "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define MESSAGE_CHAR_UUID   "beb5483e-36e1-4688-b7f5-ea07361b26a9"
#define STATUS_CHAR_UUID    "beb5483e-36e1-4688-b7f5-ea07361b26aa"

class BluetoothManager;

// 回调处理类
class ConfigCharacteristicCallbacks: public BLECharacteristicCallbacks {
private:
    BluetoothManager* manager;
public:
    ConfigCharacteristicCallbacks(BluetoothManager* mgr) : manager(mgr) {}
    void onWrite(BLECharacteristic *pCharacteristic);
};

class MessageCharacteristicCallbacks: public BLECharacteristicCallbacks {
private:
    BluetoothManager* manager;
public:
    MessageCharacteristicCallbacks(BluetoothManager* mgr) : manager(mgr) {}
    void onWrite(BLECharacteristic *pCharacteristic);
};

class BluetoothManager {
private:
    BLEServer* pServer;
    BLECharacteristic* pConfigCharacteristic;
    BLECharacteristic* pMessageCharacteristic;
    BLECharacteristic* pStatusCharacteristic;
    
    bool deviceConnected;
    String nodeId;
    
    // 配置回调函数指针
    typedef void (*ConfigCallback)(JsonDocument&);
    typedef void (*MessageCallback)(const String&, const String&);
    
    ConfigCallback onConfigReceived;
    MessageCallback onMessageReceived;
    
    friend class ConfigCharacteristicCallbacks;
    friend class MessageCharacteristicCallbacks;

public:
    BluetoothManager(const String& node_id);
    
    void begin();
    void update();
    
    // 设置回调函数
    void setConfigCallback(ConfigCallback callback) { onConfigReceived = callback; }
    void setMessageCallback(MessageCallback callback) { onMessageReceived = callback; }
    
    // 发送状态更新
    void sendStatus(const String& status);
    
    // 处理配置更新
    void handleConfig(const String& jsonStr);
    // 处理消息发送
    void handleMessage(const String& jsonStr);
};

#endif