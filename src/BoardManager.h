#pragma once

#include <Arduino.h>
#include <TFT_eSPI.h>

class BoardManager {
public:
    static BoardManager& getInstance() {
        static BoardManager instance;
        return instance;
    }

    void begin();
    void update();
    
    // Display functions
    TFT_eSPI& getDisplay() { return tft; }
    uint16_t getDisplayWidth() const { return TFT_WIDTH; }
    uint16_t getDisplayHeight() const { return TFT_HEIGHT; }
    
    // Button states
    bool isButtonPressed(uint8_t btn);
    
    // Power management
    float getBatteryVoltage();
    int getBatteryPercentage();
    
private:
    BoardManager() {}
    TFT_eSPI tft;
    
    void initDisplay();
    void initButtons();
    void initPower();
    
#ifdef TWATCH_S3
    void initTouchScreen();
    void updateTouchScreen();
#endif

#ifdef TDECK_PLUS
    void initKeyboard();
    void updateKeyboard();
#endif
};

#define BOARD BoardManager::getInstance()