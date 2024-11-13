# ESP32 LoRa Mesh 聊天系统

基于 ESP32 和 LoRa 模块的分布式mesh网络通信系统。

## 功能特点

- 群组聊天
- 私人聊天
- GPS位置共享
- 离线地图显示（支持的设备）
- 本地语音识别
- Mesh网络自动组网
- 微信小程序配置和控制
- 蓝牙LE连接
- IP地理位置备用方案

## 支持设备

### M5Stack CoreS3/CoreS3 SE
- 完整功能支持
- 内置显示屏
- SD卡支持
- GPS模块支持
- 兼容M5Stack LoRa模块（433MHz）

### LilyGO T-Watch S3
- 有限功能（无SD卡）
- 内置显示屏
- 不支持地图功能
- 蓝牙LE支持
- 触摸屏界面

### LilyGO T-Deck Plus
- 完整功能支持
- 内置显示屏
- 内置键盘
- SD卡支持
- GPS模块支持

## 硬件要求

### 核心组件
- ESP32-S3基础板（支持的型号之一）
- LoRa模块（433MHz）
- 显示屏（内置于支持设备）

### 可选组件
- GPS模块（NEO-6M或兼容）
- TF卡模块（用于地图功能）
- INMP441 MEMS麦克风模块

## 引脚配置

### M5Stack CoreS3 LoRa连接
```
LORA_MISO -> GPIO19
LORA_MOSI -> GPIO23
LORA_SCLK -> GPIO18
LORA_CS   -> GPIO5
LORA_RST  -> GPIO13
LORA_IRQ  -> GPIO34
```

### T-Watch S3 LoRa连接
```
LORA_MISO -> GPIO13
LORA_MOSI -> GPIO14
LORA_SCLK -> GPIO12
LORA_CS   -> GPIO11
LORA_RST  -> GPIO21
LORA_IRQ  -> GPIO3
```

### T-Deck Plus LoRa连接
```
LORA_MISO -> GPIO9
LORA_MOSI -> GPIO10
LORA_SCLK -> GPIO8
LORA_CS   -> GPIO7
LORA_RST  -> GPIO5
LORA_IRQ  -> GPIO6
```

## 开发设置

### 前置要求
- PlatformIO
- ESP-IDF
- 微信开发者工具（用于小程序开发）

### 项目构建

1. 克隆仓库
2. 在PlatformIO中打开
3. 选择目标设备型号
4. 编译并烧录

```bash
# 构建M5Stack CoreS3版本
pio run -e m5stack-cores3

# 构建T-Watch S3版本
pio run -e t-watch-s3

# 构建T-Deck Plus版本
pio run -e t-deck-plus
```

## 微信小程序功能

- 蓝牙LE设备配置
- 聊天界面
- 设备位置地图视图
- 设备设置管理
- GPS位置共享
- 自动使用微信位置服务作为备选

## 语音命令

系统使用ESP-SR进行本地中文语音识别，支持以下命令：

1. 发送私聊消息：
```
"发送给 [接收者] 说 [消息内容]"
```

2. 发送群组消息：
```
"群发 [消息内容]"
```

## 位置服务

系统实现分层位置服务：

1. GPS模块（如果可用）
2. 微信位置服务（通过小程序）
3. IP地理位置（备选方案）

## 注意事项

1. 每个节点需要唯一的nodeId
2. LoRa频率设置需符合当地法规
3. GPS模块首次定位可能需要几分钟
4. 地图功能需要SD卡支持
5. 语音识别需要ESP32-S3带PSRAM
6. 建议在较安静的环境下使用语音识别
7. T-Watch S3因无SD卡而不支持地图功能

## 开发指南

- 使用PlatformIO进行构建和部署
- 遵循设备变体模式
- 使用条件编译支持设备特定功能
- GitHub Actions自动化构建
- 通过platformio.ini进行依赖管理

## 贡献

1. Fork本仓库
2. 创建特性分支
3. 提交更改
4. 推送到分支
5. 创建Pull Request

---

# ESP32 LoRa Mesh Chat System

[English version continues with the same structure and content as above, but in English...]</content>