void StarterActivate(){
    // RFID 체크는 200ms마다만 수행 (블로킹으로 인한 루프 지연 방지)
    static bool tagOnReader = false;
    static unsigned long lastRfidCheck = 0;
    if (millis() - lastRfidCheck >= 200){
        lastRfidCheck = millis();
        byte pn532_buf[64];
        pn532_buf[0] = 0x00;
        tagOnReader = false;
        if (nfc[MAINPN532].sendCommandCheckAck(pn532_buf, 1)){
            if (nfc[MAINPN532].startPassiveTargetIDDetection(PN532_MIFARE_ISO14443A)){
                tagOnReader = true;
            }
        }
    }
    // 태그 여부 관계없이 항상 게이지 표시 업데이트
    // int gaugeNeoCnt = map(encoderValue,0,(starterNeoDivider),0,NumPixels[GAUGE]);
    // int motorSpeed = map(encoderValue,0,(starterNeoDivider),0,255);
    // Serial.println(String(encoderValue) + "___"+ String(gaugeNeoCnt) + "___" + String(motorSpeed));
    int gaugeNeoCnt = encoderValue / starterEncoderUnit;
    EncoderNeopixelOn(gaugeNeoCnt);

    if (!tagOnReader){
        detachInterrupt(encoderPinA);
        detachInterrupt(encoderPinB);
        EngineSpeeed(0);
        return;
    }

    attachInterrupt(encoderPinA, updateEncoder, CHANGE);
    attachInterrupt(encoderPinB, updateEncoder, CHANGE);

    Serial.println("raw: " + String(encoderValue));
    EngineSpeeed(gaugeNeoCnt*8);
    if(gaugeNeoCnt >= NumPixels[GAUGE]){
        detachInterrupt(encoderPinA);
        detachInterrupt(encoderPinB);
        // SendCmd("page pgFixed");
        StartFinish();
        BlinkTimer.deleteTimer(blinkTimerId);
        NeoLightColor(STARTER, color[GREEN]);
        GameTimer.deleteTimer(gameTimerId);        //게임 타이머 종료
        BlinkTimer.deleteTimer(blinkTimerId);
        BlinkTimerStart(CIRCUIT, YELLOW);
    }
}