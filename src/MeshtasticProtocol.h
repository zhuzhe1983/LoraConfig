#pragma once

#include <Arduino.h>
#include <LoRa.h>
#include <ArduinoJson.h>
#include "MeshtasticConfig.h"

class MeshtasticProtocol {
public:
    static MeshtasticProtocol& getInstance() {
        static MeshtasticProtocol instance;
        return instance;
    }

    void begin(const RadioConfig& config);
    void setNodeId(uint32_t id) { nodeId = id; }
    uint32_t getNodeId() const { return nodeId; }
    
    // Send messages
    bool sendTextMessage(const String& text, uint32_t toNode = 0xFFFFFFFF);
    bool sendPosition(float lat, float lon, float alt = 0);
    
    // Receive messages
    void update();
    
    // Callbacks
    typedef std::function<void(uint32_t from, const String& text)> MessageCallback;
    typedef std::function<void(uint32_t from, float lat, float lon, float alt)> PositionCallback;
    
    void setMessageCallback(MessageCallback cb) { onMessage = cb; }
    void setPositionCallback(PositionCallback cb) { onPosition = cb; }

private:
    MeshtasticProtocol() : nodeId(0), packetId(0) {}
    
    uint32_t nodeId;
    uint32_t packetId;
    RadioConfig config;
    
    MessageCallback onMessage;
    PositionCallback onPosition;
    
    // Internal methods
    void sendPacket(const PacketHeader& header, const uint8_t* payload, size_t len);
    void processReceivedPacket();
    uint32_t generatePacketId();
};

#define MESHTASTIC MeshtasticProtocol::getInstance()