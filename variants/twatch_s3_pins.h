#pragma once

// LoRa pins for T-Watch S3
#define LORA_MISO     13
#define LORA_MOSI     14
#define LORA_SCLK     12
#define LORA_CS       11
#define LORA_RST      21
#define LORA_IRQ      3

// Display configuration
#define TFT_WIDTH     240
#define TFT_HEIGHT    240
#define ROTATION      0  // Portrait

// GPS UART pins
#define GPS_TX        43
#define GPS_RX        44

// Button pins
#define BTN_A         0  // Touch button
#define BTN_B         1  // Crown encoder
#define BTN_C         2  // Crown button

// Features
#define HAS_SD_CARD   0
#define HAS_GPS       0