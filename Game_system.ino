void StarterActivate(){
    // RFID 태그 감지 확인 - 태그가 있어야만 레버 효과 발생
    byte pn532_buf[64];
    pn532_buf[0] = 0x00;
    bool tagOnReader = false;
    if (nfc[MAINPN532].sendCommandCheckAck(pn532_buf, 1)){
        if (nfc[MAINPN532].startPassiveTargetIDDetection(PN532_MIFARE_ISO14443A)){
            tagOnReader = true;
        }
    }
    if (!tagOnReader){
        detachInterrupt(encoderPinA);
        detachInterrupt(encoderPinB);
        EngineSpeeed(0);
        return;
    }

    attachInterrupt(encoderPinA, updateEncoder, CHANGE);
    attachInterrupt(encoderPinB, updateEncoder, CHANGE);

    // int gaugeNeoCnt = map(encoderValue,0,(starterNeoDivider),0,NumPixels[GAUGE]);
    // int motorSpeed = map(encoderValue,0,(starterNeoDivider),0,255);
    // Serial.println(String(encoderValue) + "___"+ String(gaugeNeoCnt) + "___" + String(motorSpeed));
    int gaugeNeoCnt = encoderValue / 4000;
    Serial.println("raw: " + String(encoderValue));
    EncoderNeopixelOn(gaugeNeoCnt);
    EngineSpeeed(gaugeNeoCnt*8);
    if(gaugeNeoCnt >= NumPixels[GAUGE]){
        detachInterrupt(encoderPinA);
        detachInterrupt(encoderPinB);
        SendCmd("page pgStarterDone");
        has2wifi.Send((String)(const char*)my["device_name"], "device_state", "starter_finish");
        ptrRfidMode = StartFinish;
        ptrCurrentMode = RfidLoopMain;
        BlinkTimer.deleteTimer(blinkTimerId);
        NeoLightColor(STARTER, color[GREEN]);
        GameTimer.deleteTimer(gameTimerId);        //게임 타이머 종료
        BlinkTimer.deleteTimer(blinkTimerId);
        BlinkTimerStart(CIRCUIT, YELLOW);
    }
}