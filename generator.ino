#include <HAS2_Wifi.h>

#include <HAS2_Wifi.h>

#include <SecureOTA.h>

/**
 * @file Done_Generator_code.ino
 * @author 김병준 (you@domain.com)
 * @brief
 * @version 1.0
 * @date 2022-11-29
 *
 * @copyright Copyright (c) 2022
 *
 */

#define FIRMWARE_VER 14
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
 has2wifi.Setup("badland_ruins", "Code3824@");
//    has2wifi.Setup("city");
    ota.setLogStream(Serial);
    ota.setOnSuccess([]() {
        Serial.println("[OTA] ✅ 업데이트 성공! 재부팅합니다...");
    });
    ota.setOnSkip([]() {
        Serial.println("[OTA] 이미 최신 버전입니다.");
    });
    ptrCurrentMode = WaitFunc;
    ptrRfidMode = WaitFunc;
    DataChanged();
}
void loop() {
    ptrCurrentMode();
    TimerRun();
}
