#include "VoiceInput.h"

VoiceInput::VoiceInput() {
    isRecording = false;
    buffer_size = 16000; // 1秒的音频数据
    audio_buffer = (int16_t*)malloc(buffer_size * sizeof(int16_t));
}

VoiceInput::~VoiceInput() {
    if (audio_buffer) {
        free(audio_buffer);
    }
    if (multinet) {
        esp_mn_destroy(multinet);
    }
}

void VoiceInput::begin() {
    configureI2S();
    initializeModel();
}

void VoiceInput::configureI2S() {
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
        .sample_rate = SAMPLE_RATE,
        .bits_per_sample = (i2s_bits_per_sample_t)SAMPLE_BITS,
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
        .communication_format = I2S_COMM_FORMAT_STAND_I2S,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 8,
        .dma_buf_len = 64,
        .use_apll = false,
        .tx_desc_auto_clear = false,
        .fixed_mclk = 0
    };
    
    i2s_pin_config_t pin_config = {
        .bck_io_num = I2S_BCLK_PIN,
        .ws_io_num = I2S_WCLK_PIN,
        .data_out_num = I2S_DOUT_PIN,
        .data_in_num = I2S_DIN_PIN
    };
    
    i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
    i2s_set_pin(I2S_PORT, &pin_config);
}

void VoiceInput::initializeModel() {
    // 初始化中文语音识别模型
    multinet = esp_mn_create_from_flash();
    if (!multinet) {
        Serial.println("创建语音识别模型失败");
        return;
    }
    
    // 添加命令词
    const char *commands[] = {
        "发送", "群发", "给", "说"
    };
    const int command_ids[] = {
        CMD_SEND, CMD_GROUP, CMD_TO, CMD_SAY
    };
    
    for (int i = 0; i < 4; i++) {
        esp_mn_add_cmd(multinet, commands[i], command_ids[i]);
    }
}

void VoiceInput::startRecording() {
    if (!isRecording) {
        isRecording = true;
        i2s_start(I2S_PORT);
        memset(audio_buffer, 0, buffer_size * sizeof(int16_t));
    }
}

void VoiceInput::stopRecording() {
    if (isRecording) {
        i2s_stop(I2S_PORT);
        isRecording = false;
        processAudio();
    }
}

void VoiceInput::processAudio() {
    size_t bytes_read = 0;
    i2s_read(I2S_PORT, audio_buffer, buffer_size * sizeof(int16_t), &bytes_read, portMAX_DELAY);
    
    // 进行语音识别
    esp_mn_state_t state = {0};
    int command_id = esp_mn_process(multinet, audio_buffer, buffer_size, &state);
    
    // 处理识别结果
    if (command_id > 0) {
        Serial.printf("识别到命令: %d\n", command_id);
    }
}

String VoiceInput::getRecognitionResult() {
    // 返回识别结果
    // 这里需要根据实际识别到的命令序列构建完整的命令字符串
    return "";
}