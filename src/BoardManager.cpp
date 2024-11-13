#include "BoardManager.h"

void BoardManager::begin() {
    // Initialize display
    initDisplay();
    initButtons();
    initPower();
    
#ifdef HAS_FACE_KB
    KEYBOARD.begin();
#endif

#ifdef TWATCH_S3
    initTouchScreen();
#endif

#ifdef TDECK_PLUS
    initKeyboard();
#endif
}

void BoardManager::update() {
#ifdef HAS_FACE_KB
    KEYBOARD.update();
#endif

#ifdef TWATCH_S3
    updateTouchScreen();
#endif

#ifdef TDECK_PLUS
    updateKeyboard();
#endif
}

// ... (其余代码保持不变)