#pragma once

// LoRa pins for M5Stack CoreS3
#define LORA_MISO     19
#define LORA_MOSI     23
#define LORA_SCLK     18
#define LORA_CS       5
#define LORA_RST      13
#define LORA_IRQ      34

// Display configuration
#define TFT_WIDTH     320
#define TFT_HEIGHT    240
#define ROTATION      1  // Landscape

// GPS UART pins
#define GPS_TX        16
#define GPS_RX        17

// Button pins
#define BTN_A         0
#define BTN_B         1
#define BTN_C         2

// Face KB pins (I2C)
#define FACE_KB_SDA   21
#define FACE_KB_SCL   22
#define FACE_KB_ADDR  0x08

// Features
#define HAS_SD_CARD   1
#define HAS_GPS       1
#define HAS_FACE_KB   1