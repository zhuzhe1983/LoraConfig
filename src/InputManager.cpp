#include "InputManager.h"

InputManager::InputManager() : chineseMode(false) {
    inputBuffer.reserve(256);
}

void InputManager::begin() {
    CHINESE_IME.begin();
    clear();
}

void InputManager::update() {
    if (KEYBOARD.available()) {
        char key = KEYBOARD.read();
        
        // 处理输入模式切换
        if (KEYBOARD.isCtrlPressed() && key == ' ') {
            toggleInputMode();
            return;
        }
        
        // 根据当前模式处理输入
        if (chineseMode) {
            CHINESE_IME.handleKey(key);
        } else {
            if (key == '\b') {
                if (inputBuffer.length() > 0) {
                    inputBuffer.remove(inputBuffer.length() - 1);
                }
            }
            else if (key) {
                inputBuffer += key;
            }
        }
        
        updateDisplay();
    }
}

void InputManager::updateDisplay() {
    TFT_eSPI& tft = BOARD.getDisplay();
    
    // 清除输入区域
    tft.fillRect(0, BOARD.getDisplayHeight() - INPUT_AREA_HEIGHT - CANDIDATE_AREA_HEIGHT,
                 BOARD.getDisplayWidth(), INPUT_AREA_HEIGHT + CANDIDATE_AREA_HEIGHT, TFT_BLACK);
    
    // 绘制输入区域
    drawInputArea();
    
    // 在中文模式下绘制候选字区域
    if (chineseMode && CHINESE_IME.isActive()) {
        drawCandidates();
    }
}

void InputManager::drawInputArea() {
    TFT_eSPI& tft = BOARD.getDisplay();
    int y = BOARD.getDisplayHeight() - INPUT_AREA_HEIGHT;
    
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    
    if (chineseMode && CHINESE_IME.isActive()) {
        String displayText = "> " + CHINESE_IME.getPinyin();
        tft.drawString(displayText, 5, y + 5, 2);
    } else {
        String displayText = "> " + inputBuffer;
        tft.drawString(displayText, 5, y + 5, 2);
    }
}

void InputManager::drawCandidates() {
    TFT_eSPI& tft = BOARD.getDisplay();
    int y = BOARD.getDisplayHeight() - INPUT_AREA_HEIGHT - CANDIDATE_AREA_HEIGHT;
    
    const auto& candidates = CHINESE_IME.getCandidates();
    int startIdx = CHINESE_IME.getCurrentPage() * 5;
    
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    
    for (int i = 0; i < 5 && (startIdx + i) < candidates.size(); i++) {
        String displayText = String(i + 1) + "." + candidates[startIdx + i];
        tft.drawString(displayText, 5 + i * 50, y + 5, 2);
    }
    
    // 显示页码信息
    if (CHINESE_IME.getTotalPages() > 1) {
        String pageInfo = String(CHINESE_IME.getCurrentPage() + 1) + "/" + 
                         String(CHINESE_IME.getTotalPages());
        tft.drawString(pageInfo, BOARD.getDisplayWidth() - 50, y + 5, 2);
    }
}

void InputManager::toggleInputMode() {
    chineseMode = !chineseMode;
    if (chineseMode) {
        CHINESE_IME.clearInput();
    }
    updateDisplay();
}

bool InputManager::hasInput() const {
    return !inputBuffer.isEmpty() || CHINESE_IME.isActive();
}

String InputManager::getInput() const {
    return inputBuffer;
}

void InputManager::clear() {
    inputBuffer = "";
    CHINESE_IME.clearInput();
    updateDisplay();
}