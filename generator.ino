 ;/**
 * @file Done_Generator_code.ino
 * @author 김병준 (you@domain.com)
 * @brief
 * @version 1.0
 * @date 2022-11-29
 *
 * @copyright Copyright (c) 2022
 *
 */

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
 has2wifi.Setup("badland");
//    has2wifi.Setup("city");
    DataChanged();
}
void loop() {
    ptrCurrentMode();
    TimerRun();
}
