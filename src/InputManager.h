#pragma once

#include "KeyboardManager.h"
#include "ChineseIME.h"
#include "BoardManager.h"

class InputManager {
public:
    static InputManager& getInstance() {
        static InputManager instance;
        return instance;
    }
    
    void begin();
    void update();
    
    // 输入状态
    bool hasInput() const;
    String getInput() const;
    void clear();
    
    // 输入法状态
    bool isChineseMode() const { return chineseMode; }
    void toggleInputMode();
    
private:
    InputManager();
    
    String inputBuffer;
    bool chineseMode;
    
    // 显示相关
    void updateDisplay();
    void drawInputArea();
    void drawCandidates();
    
    // 常量定义
    static const int INPUT_AREA_HEIGHT = 20;
    static const int CANDIDATE_AREA_HEIGHT = 30;
};

#define INPUT_MGR InputManager::getInstance()