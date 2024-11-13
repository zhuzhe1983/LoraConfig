#pragma once

#include <Arduino.h>
#include <Wire.h>

class KeyboardManager {
public:
    static KeyboardManager& getInstance() {
        static KeyboardManager instance;
        return instance;
    }

    void begin();
    void update();
    bool available();
    char read();
    String readString();
    
    // 特殊键状态
    bool isShiftPressed();
    bool isCtrlPressed();
    bool isAltPressed();
    bool isFnPressed();

private:
    KeyboardManager() : lastKey(0), keyAvailable(false) {}
    
    uint8_t buffer[64];
    uint8_t bufferIndex;
    char lastKey;
    bool keyAvailable;
    uint8_t modifiers;
    
    void readFromI2C();
    char decodeKey(uint8_t keycode);
    
    static const uint8_t SHIFT_MASK = 0x22;
    static const uint8_t CTRL_MASK = 0x11;
    static const uint8_t ALT_MASK = 0x44;
    static const uint8_t FN_MASK = 0x88;
};

#define KEYBOARD KeyboardManager::getInstance()