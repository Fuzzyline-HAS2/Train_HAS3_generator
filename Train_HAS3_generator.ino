#include <HAS2_Wifi.h>

#include <HAS2_Wifi.h>

#include <SecureOTA.h>

/**
 * @file Train_HAS3_generator.ino
 * @author 김병준 (you@domain.com)
 * @brief
 * @version 1.0
 * @date 2022-11-29
 *
 * @copyright Copyright (c) 2022
 *
 */

#define FIRMWARE_VER 22
#define PARTITION_VER 1
#include "generator.h"

void setup() {
    Serial.begin(115200);
    NeopixelInit();
    RfidInit();
    MotorInit();
    EncoderInit();
    NextionInit();
    TimerInit();
// has2wifi.Setup("KT_GiGA_6C64","ed46zx1198");
 has2wifi.Setup("badland_shoot", "Code3824@");
//    has2wifi.Setup("city");
    ota.setLogStream(Serial);
    ota.setOnSuccess([]() {
        Serial.println("[OTA] ✅ 업데이트 성공! 재부팅합니다...");
        has2wifi.Send((String)(const char*)my["device_name"], "device_state", "setting");
    });
    ota.setOnSkip([]() {
        Serial.println("[OTA] 이미 최신 버전입니다.");
        has2wifi.Send((String)(const char*)my["device_name"], "device_state", "setting");
    });
    ota.setPartitionUpdate(
        "https://raw.githubusercontent.com/Fuzzyline-HAS2/Train_HAS3_generator/third_store/partitions.bin",
        "https://raw.githubusercontent.com/Fuzzyline-HAS2/Train_HAS3_generator/third_store/partitions.sig",
        "https://raw.githubusercontent.com/Fuzzyline-HAS2/Train_HAS3_generator/third_store/partition_version.txt",
        PARTITION_VER
    );
    ptrCurrentMode = WaitFunc;
    ptrRfidMode = WaitFunc;
    DataChanged();
}
void loop() {
    EncoderLoop();
    ptrCurrentMode();
    TimerRun();
}
