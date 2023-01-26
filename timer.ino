void TimerInit(){
    wifiTimerId = WifiTimer.setInterval(wifiTime,WifiIntervalFunc);
    gameTimerId = GameTimer.setInterval(gameTime,GameTimerFunc);
    logoutTimerId = LogoutTimer.setInterval(logoutTime,LogoutTimerFunc);
    blinkTimerId = BlinkTimer.setInterval(blinkTime,BlinkTimerFunc);

    GameTimer.deleteTimer(gameTimerId);
    LogoutTimer.deleteTimer(logoutTimerId);
    BlinkTimer.deleteTimer(blinkTimerId);
}

void WifiIntervalFunc(){
    has2wifi.Loop(DataChanged);
}

void GameTimerFunc(){
    gameTimerCnt++;
    Serial.println("gameTimerCnt:" + (String)gameTimerCnt);
    if(gameTimerCnt == 5){ // 0.5s x 6 =3sec
        encoderValue = encoderValue - (starterNeoDivider*0.01); //초마다 전체량에서 1프로씩 감소
        gameTimerCnt = 3;
        if(encoderValue < 0){
            encoderValue = 0;
            gameTimerCnt = 0;
        }
    }
}
void LogoutTimerFunc(){
    logoutTimerCnt++;
    Serial.println("LogoutTimerCnt:" + (String)logoutTimerCnt);
    if(logoutTimerCnt >= 12){
        Serial.println("LogOutTimer TimeOUT");
        LogoutTimer.deleteTimer(logoutTimerId);        //로그아웃 타이머 종료
        GameTimer.deleteTimer(gameTimerId);
        BlinkTimer.deleteTimer(blinkTimerId);
        EngineStop();
        ActivateFunc();
        logoutTimerCnt = 0;
    }
}

void BlinkTimerFunc(){
    Serial.println("Blink!");
    if(blinkOn == true){
        pixels[blinkNeo].lightColor(color[blinkColor]);
        blinkOn = false;
    }
    else{
        pixels[blinkNeo].lightColor(color[BLACK]);
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
    LogoutTimer.run();
    BlinkTimer.run();
}
