void TimerInit(){
    wifiTimerId = WifiTimer.setInterval(wifiTime,WifiIntervalFunc);
    gameTimerId = GameTimer.setInterval(gameTime,GameTimerFunc);

    blinkTimerId = BlinkTimer.setInterval(blinkTime,BlinkTimerFunc);

    GameTimer.deleteTimer(gameTimerId);

    BlinkTimer.deleteTimer(blinkTimerId);
}

void WifiIntervalFunc(){
    has2wifi.Loop(DataChanged);
}

void GameTimerFunc(){
    gameTimerCnt++;
    // Serial.println("gameTimerCnt:" + (String)gameTimerCnt);
    if(gameTimerCnt == 5){ // 0.5s x 6 =3sec
        encoderValue = encoderValue - (starterNeoDivider*0.01); //초마다 전체량에서 1프로씩 감소
        gameTimerCnt = 3;
        if(encoderValue < 0){
            encoderValue = 0;
            gameTimerCnt = 0;
        }
    }
}

void BlinkTimerFunc(){
    // Serial.println("Blink!");
    if(blinkOn == true){
        NeoLightColor(blinkNeo, color[blinkColor]);
        blinkOn = false;
    }
    else{
        NeoLightColor(blinkNeo, color[BLACK]);
        blinkOn = true;
    }
}
void BlinkTimerStart(int Neo, int NeoColor){
    blinkNeo = Neo;
    blinkColor = NeoColor;
    blinkTimerId = BlinkTimer.setInterval(blinkTime, BlinkTimerFunc);
}

void TimerRun(){
    WifiTimer.run();
    GameTimer.run();

    BlinkTimer.run();
}
