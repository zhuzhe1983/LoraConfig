#ifndef VOICE_INPUT_H
#define VOICE_INPUT_H

#include <Arduino.h>
#include <driver/i2s.h>
#include "esp_wn_iface.h"
#include "esp_wn_models.h"
#include "esp_mn_iface.h"
#include "esp_mn_models.h"
#include "AudioConfig.h"

class VoiceInput {
private:
    static const int SAMPLE_RATE = 16000;
    static const int SAMPLE_BITS = 16;
    static const i2s_port_t I2S_PORT = I2S_NUM_0;
    
    // 语音识别模型
    esp_mn_iface_t *multinet = NULL;
    model_iface_data_t *model_data = NULL;
    
    // 命令词ID
    static const int CMD_SEND = 100;     // "发送"
    static const int CMD_GROUP = 101;    // "群发"
    static const int CMD_TO = 102;       // "给"
    static const int CMD_SAY = 103;      // "说"
    
    bool isRecording;
    int16_t *audio_buffer;
    size_t buffer_size;
    
    void configureI2S();
    void initializeModel();
    void processAudio();
    
public:
    VoiceInput();
    ~VoiceInput();
    void begin();
    void startRecording();
    void stopRecording();
    String getRecognitionResult();
    bool isActive() { return isRecording; }
};

#endif