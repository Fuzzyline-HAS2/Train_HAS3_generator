void StarterActivate(){
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