#pragma once

#include <Arduino.h>
#include <vector>
#include <map>

class ChineseIME {
public:
    static ChineseIME& getInstance() {
        static ChineseIME instance;
        return instance;
    }

    void begin();
    void update();
    
    // 输入处理
    void handleKey(char key);
    void nextPage();
    void prevPage();
    void selectCandidate(int index);
    void clearInput();
    
    // 状态查询
    bool isActive() const { return !pinyin.empty(); }
    const String& getPinyin() const { return pinyin; }
    const std::vector<String>& getCandidates() const { return candidates; }
    int getCurrentPage() const { return currentPage; }
    int getTotalPages() const { return (candidates.size() + CANDIDATES_PER_PAGE - 1) / CANDIDATES_PER_PAGE; }
    
private:
    ChineseIME();
    
    // 拼音和候选字管理
    String pinyin;
    std::vector<String> candidates;
    int currentPage;
    
    // 拼音到汉字的映射表
    std::map<String, std::vector<String>> pinyinMap;
    
    // 常量定义
    static const int CANDIDATES_PER_PAGE = 5;
    
    // 内部方法
    void updateCandidates();
    void loadPinyinData();
    bool isValidPinyinChar(char c);
};

#define CHINESE_IME ChineseIME::getInstance()