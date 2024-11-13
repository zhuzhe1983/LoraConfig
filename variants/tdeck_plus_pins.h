#pragma once

// LoRa pins for T-Deck Plus
#define LORA_MISO     9
#define LORA_MOSI     10
#define LORA_SCLK     8
#define LORA_CS       7
#define LORA_RST      5
#define LORA_IRQ      6

// Display configuration
#define TFT_WIDTH     320
#define TFT_HEIGHT    240
#define ROTATION      1  // Landscape

// GPS UART pins
#define GPS_TX        43
#define GPS_RX        44

// Button pins
#define BTN_A         0  // Keyboard Fn key
#define BTN_B         1  // Navigation button
#define BTN_C         2  // Power button

// Features
#define HAS_SD_CARD   1
#define HAS_GPS       1