#include "MeshtasticProtocol.h"

void MeshtasticProtocol::begin(const RadioConfig& cfg) {
    config = cfg;
    
    // Configure LoRa with Meshtastic compatible settings
    LoRa.setFrequency(config.frequency);
    LoRa.setTxPower(config.txPower);
    LoRa.setSpreadingFactor(config.spreadingFactor);
    LoRa.setSignalBandwidth(config.signalBandwidth);
    LoRa.setCodingRate4(config.codingRate);
    LoRa.enableCrc();
    
    if (config.implicitHeader) {
        LoRa.enableImplicitHeaderMode();
    } else {
        LoRa.disableImplicitHeaderMode();
    }
}

bool MeshtasticProtocol::sendTextMessage(const String& text, uint32_t toNode) {
    PacketHeader header;
    header.from = nodeId;
    header.to = toNode;
    header.id = generatePacketId();
    header.flags = {0};
    header.channel = config.channelNum;
    header.type = PACKET_TYPE_DATA;
    
    // Create JSON payload
    StaticJsonDocument<512> doc;
    doc["type"] = "text";
    doc["text"] = text;
    
    String jsonStr;
    serializeJson(doc, jsonStr);
    
    sendPacket(header, (uint8_t*)jsonStr.c_str(), jsonStr.length());
    return true;
}

bool MeshtasticProtocol::sendPosition(float lat, float lon, float alt) {
    PacketHeader header;
    header.from = nodeId;
    header.to = 0xFFFFFFFF; // Broadcast
    header.id = generatePacketId();
    header.flags = {0};
    header.channel = config.channelNum;
    header.type = PACKET_TYPE_POSITION;
    
    // Create position payload
    struct {
        float latitude;
        float longitude;
        float altitude;
        uint32_t timestamp;
    } posPayload = {
        lat, lon, alt,
        (uint32_t)(millis() / 1000)
    };
    
    sendPacket(header, (uint8_t*)&posPayload, sizeof(posPayload));
    return true;
}

void MeshtasticProtocol::sendPacket(const PacketHeader& header, const uint8_t* payload, size_t len) {
    // Start LoRa packet
    LoRa.beginPacket();
    
    // Write header
    LoRa.write((uint8_t*)&header, sizeof(header));
    
    // Write payload
    LoRa.write(payload, len);
    
    // End packet
    LoRa.endPacket();
}

void MeshtasticProtocol::update() {
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
        processReceivedPacket();
    }
}

void MeshtasticProtocol::processReceivedPacket() {
    // Read header
    PacketHeader header;
    LoRa.readBytes((uint8_t*)&header, sizeof(header));
    
    // Skip if not for us and not broadcast
    if (header.to != nodeId && header.to != 0xFFFFFFFF) {
        return;
    }
    
    // Process by type
    switch (header.type) {
        case PACKET_TYPE_POSITION: {
            struct {
                float latitude;
                float longitude;
                float altitude;
                uint32_t timestamp;
            } posData;
            
            LoRa.readBytes((uint8_t*)&posData, sizeof(posData));
            
            if (onPosition) {
                onPosition(header.from, posData.latitude, 
                         posData.longitude, posData.altitude);
            }
            break;
        }
        
        case PACKET_TYPE_DATA: {
            String jsonStr;
            while (LoRa.available()) {
                jsonStr += (char)LoRa.read();
            }
            
            StaticJsonDocument<512> doc;
            DeserializationError error = deserializeJson(doc, jsonStr);
            
            if (!error && doc["type"] == "text" && onMessage) {
                onMessage(header.from, doc["text"].as<String>());
            }
            break;
        }
    }
}

uint32_t MeshtasticProtocol::generatePacketId() {
    return ++packetId;
}