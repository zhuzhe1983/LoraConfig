#include "KeyboardManager.h"

void KeyboardManager::begin() {
#ifdef HAS_FACE_KB
    Wire.begin(FACE_KB_SDA, FACE_KB_SCL);
    Wire.setClock(400000); // 400kHz
#endif
    bufferIndex = 0;
    modifiers = 0;
}

void KeyboardManager::update() {
#ifdef HAS_FACE_KB
    readFromI2C();
#endif
}

void KeyboardManager::readFromI2C() {
    Wire.requestFrom(FACE_KB_ADDR, 8);
    if (Wire.available()) {
        modifiers = Wire.read(); // 读取修饰键状态
        Wire.read(); // 保留字节
        
        // 读取最多6个按键
        uint8_t keyCodes[6];
        for (int i = 0; i < 6; i++) {
            keyCodes[i] = Wire.read();
        }
        
        // 处理第一个非零按键
        for (int i = 0; i < 6; i++) {
            if (keyCodes[i] != 0) {
                lastKey = decodeKey(keyCodes[i]);
                keyAvailable = true;
                break;
            }
        }
    }
}

char KeyboardManager::decodeKey(uint8_t keycode) {
    // 基本ASCII映射
    if (keycode >= 0x04 && keycode <= 0x1D) {
        char base = isShiftPressed() ? 'A' : 'a';
        return base + (keycode - 0x04);
    }
    // 数字键映射
    else if (keycode >= 0x1E && keycode <= 0x27) {
        if (isShiftPressed()) {
            const char symbols[] = "!@#$%^&*()";
            return symbols[keycode - 0x1E];
        } else {
            return '0' + (keycode - 0x1E);
        }
    }
    // 特殊字符映射
    else if (keycode == 0x2C) return ' ';  // Space
    else if (keycode == 0x28) return '\n'; // Enter
    else if (keycode == 0x2A) return '\b'; // Backspace
    
    return 0;
}

bool KeyboardManager::available() {
    return keyAvailable;
}

char KeyboardManager::read() {
    if (keyAvailable) {
        keyAvailable = false;
        return lastKey;
    }
    return 0;
}

String KeyboardManager::readString() {
    String result;
    while (available()) {
        char c = read();
        if (c) result += c;
    }
    return result;
}

bool KeyboardManager::isShiftPressed() {
    return modifiers & SHIFT_MASK;
}

bool KeyboardManager::isCtrlPressed() {
    return modifiers & CTRL_MASK;
}

bool KeyboardManager::isAltPressed() {
    return modifiers & ALT_MASK;
}

bool KeyboardManager::isFnPressed() {
    return modifiers & FN_MASK;
}