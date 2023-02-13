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
    has2wifi.Setup("badland");
    NeopixelInit();
    RfidInit();
    MotorInit();
    EncoderInit();
    NextionInit();
    TimerInit();
    // has2wifi.Setup();
    // has2wifi.Setup("tp_link_badland","Code3824@");
    DataChanged();
}
void loop() {
    ptrCurrentMode();
    TimerRun();
}
