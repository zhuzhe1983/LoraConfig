#pragma once

// Meshtastic Protocol Constants
#define MESHTASTIC_PORTNUM 4403
#define MESHTASTIC_CURRENT_PROTOCOL_VERSION 2
#define MESHTASTIC_CHANNEL_DEFAULT 0
#define MESHTASTIC_REGION_DEFAULT LORA_REGION_CN

// Default radio config (matches Meshtastic defaults)
struct RadioConfig {
    // LoRa parameters
    long frequency = 433000000;     // 433MHz for CN region
    uint8_t txPower = 20;          // dBm
    uint8_t spreadingFactor = 9;   // Matches Meshtastic default
    long signalBandwidth = 125000; // 125kHz
    uint8_t codingRate = 5;        // 4/5 coding rate
    bool implicitHeader = false;
    
    // Meshtastic specific
    uint32_t channelNum = MESHTASTIC_CHANNEL_DEFAULT;
    uint8_t modemPreset = 0;       // LONG_FAST
};

// Meshtastic packet types
enum PacketType {
    PACKET_TYPE_POSITION = 0,
    PACKET_TYPE_DATA = 1,
    PACKET_TYPE_ADMIN = 2
};

// Meshtastic header flags
struct PacketFlags {
    bool wantAck : 1;
    bool wantResponse : 1;
    bool reserved1 : 1;
    bool reserved2 : 1;
    bool reserved3 : 1;
    bool reserved4 : 1;
    bool reserved5 : 1;
    bool reserved6 : 1;
};

// Meshtastic packet header
struct PacketHeader {
    uint32_t from;
    uint32_t to;
    uint32_t id;
    PacketFlags flags;
    uint8_t channel;
    PacketType type;
};