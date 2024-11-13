#include "ChineseIME.h"

ChineseIME::ChineseIME() : currentPage(0) {
    pinyin.reserve(32);
}

void ChineseIME::begin() {
    loadPinyinData();
}

void ChineseIME::loadPinyinData() {
    // 加载基本拼音映射数据
    // 这里只添加一些示例数据，实际应用中需要更完整的数据
    pinyinMap["ni"] = {"你", "呢", "尼", "泥"};
    pinyinMap["hao"] = {"好", "号", "耗", "浩"};
    pinyinMap["ma"] = {"吗", "马", "妈", "码"};
    pinyinMap["wo"] = {"我", "窝", "握", "卧"};
    // ... 添加更多拼音映射
}

void ChineseIME::handleKey(char key) {
    if (key == '\b') {
        if (!pinyin.isEmpty()) {
            pinyin.remove(pinyin.length() - 1);
            updateCandidates();
        }
    }
    else if (key >= 'a' && key <= 'z') {
        if (pinyin.length() < 6) {
            pinyin += key;
            updateCandidates();
        }
    }
    else if (key >= '1' && key <= '5') {
        int index = key - '1';
        selectCandidate(index);
    }
}

void ChineseIME::updateCandidates() {
    candidates.clear();
    currentPage = 0;
    
    if (!pinyin.isEmpty()) {
        auto it = pinyinMap.find(pinyin.c_str());
        if (it != pinyinMap.end()) {
            candidates = it->second;
        }
    }
}

void ChineseIME::selectCandidate(int index) {
    int realIndex = currentPage * CANDIDATES_PER_PAGE + index;
    if (realIndex < candidates.size()) {
        // 选中候选字后的处理
        // 这里需要与输入系统集成
        clearInput();
    }
}

void ChineseIME::clearInput() {
    pinyin = "";
    candidates.clear();
    currentPage = 0;
}

void ChineseIME::nextPage() {
    if (currentPage < getTotalPages() - 1) {
        currentPage++;
    }
}

void ChineseIME::prevPage() {
    if (currentPage > 0) {
        currentPage--;
    }
}

bool ChineseIME::isValidPinyinChar(char c) {
    return (c >= 'a' && c <= 'z') || c == '\'';
}