#include <Arduino.h>
#include "BoardManager.h"
#include "MapDisplay.h"
#include "LocationManager.h"
#include "BluetoothManager.h"
#include "InputManager.h"
#include "MeshtasticProtocol.h"

MapDisplay* mapDisplay = nullptr;
LocationManager* locationManager = nullptr;

void onMeshtasticMessage(uint32_t from, const String& text) {
    // Display received message
    String displayText = "Node " + String(from, HEX) + ": " + text;
    BOARD.getDisplay().drawString(displayText, 10, 10, 2);
}

void onMeshtasticPosition(uint32_t from, float lat, float lon, float alt) {
    if (mapDisplay) {
        mapDisplay->drawNode(lat, lon, String(from, HEX).c_str());
    }
}

void setup() {
    Serial.begin(115200);
    BOARD.begin();
    INPUT_MGR.begin();
    
    // Initialize LoRa with Meshtastic config
    RadioConfig config;
    MESHTASTIC.begin(config);
    MESHTASTIC.setNodeId(0x12345678); // Set unique node ID
    
    // Set callbacks
    MESHTASTIC.setMessageCallback(onMeshtasticMessage);
    MESHTASTIC.setPositionCallback(onMeshtasticPosition);
    
    // Initialize map display if supported
    if (HAS_SD_CARD) {
        mapDisplay = new MapDisplay();
        mapDisplay->begin();
    }
}

void loop() {
    BOARD.update();
    INPUT_MGR.update();
    MESHTASTIC.update();
    
    if (mapDisplay) {
        mapDisplay->update();
    }
    
    // Handle input
    if (INPUT_MGR.hasInput() && KEYBOARD.read() == '\n') {
        String message = INPUT_MGR.getInput();
        if (message.startsWith("/")) {
            // Handle commands
            if (message.startsWith("/to ")) {
                // Parse target node and message
                int spacePos = message.indexOf(' ', 4);
                if (spacePos > 0) {
                    String target = message.substring(4, spacePos);
                    String content = message.substring(spacePos + 1);
                    uint32_t targetNode = strtoul(target.c_str(), NULL, 16);
                    MESHTASTIC.sendTextMessage(content, targetNode);
                }
            }
        } else {
            // Send broadcast message
            MESHTASTIC.sendTextMessage(message);
        }
        INPUT_MGR.clear();
    }
    
    // Update and send position periodically
    static unsigned long lastPositionUpdate = 0;
    if (millis() - lastPositionUpdate > 30000) { // Every 30 seconds
        if (locationManager && locationManager->hasValidLocation()) {
            MESHTASTIC.sendPosition(
                locationManager->getLatitude(),
                locationManager->getLongitude()
            );
        }
        lastPositionUpdate = millis();
    }
}