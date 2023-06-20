void DataChanged()
{
  static StaticJsonDocument<500> cur;   //저장되어 있는 cur과 읽어온 my 값과 비교후 실행
  if((String)(const char*)my["game_state"] != (String)(const char*)cur["game_state"]){  
    if((String)(const char*)my["game_state"] == "setting"){
      SettingFunc();
    }
    else if((String)(const char*)my["game_state"] == "ready"){
      ReadyFunc();
    }
    else if((String)(const char*)my["game_state"] == "activate"){
      ActivateFunc();
      LeftGenerator();
    }
  } 
  if((String)(const char*)my["left_generator"] != (String)(const char*)cur["left_generator"]){  
    LeftGenerator();
  }
  if(receiveMineOn == false){
    if((String)(const char*)my["device_state"] != (String)(const char*)cur["device_state"]){  
      if((String)(const char*)my["device_state"] == "repaired_all"){ 
        ptrRfidMode = WaitFunc;
        ptrCurrentMode = WaitFunc;
        LeftGenerator();
        EngineStop(); 
        SendCmd("page pgEscapeOpen");
        LeftGenerator();
        GameTimer.deleteTimer(gameTimerId);
        LogoutTimer.deleteTimer(logoutTimerId);
        BlinkTimer.deleteTimer(blinkTimerId);
        AllNeoOn(BLUE);
      }
      else if((String)(const char*)my["device_state"] == "repaired"){
        Serial.println("StartFinish PTRFUNC");
        LogoutTimer.deleteTimer(logoutTimerId);
        GameTimer.deleteTimer(gameTimerId);        //게임 타이머 종료
        BlinkTimer.deleteTimer(blinkTimerId);
        Serial.println("Generator Fixed!");
        SendCmd("page pgFixed");
        LeftGenerator();
        AllNeoOn(BLUE);
        ledcWrite(MotorLedChannel, 250);
        ptrCurrentMode = WaitFunc;
      }
      else if((String)(const char*)my["device_state"] == "battery_max"){
        int maxBattery = (int)my["max_battery_pack"] - (int)my["battery_pack"];
        Serial.println((String)maxBattery);
        has2wifi.Send((String)(const char*)my["device_name"], "battery_pack", ((String)maxBattery));
        LogoutTimer.deleteTimer(logoutTimerId);
        GameTimer.deleteTimer(gameTimerId);        //게임 타이머 종료
        ActivateFunc();
      }
      else if((String)(const char*)my["device_state"] == "starter_finish"){

      }
      else if((String)(const char*)my["device_state"] == "player_win"){ 
        ptrRfidMode = WaitFunc;
        ptrCurrentMode = WaitFunc;
        AllNeoOn(BLUE);
        SendCmd("page pgPlayerWin");
      }
      else if((String)(const char*)my["device_state"] == "player_lose"){ 
        ptrRfidMode = WaitFunc;
        ptrCurrentMode = WaitFunc;
        AllNeoOn(RED);
        SendCmd("page pgPlayerLose");
      } 
    }
  }
  else{
    receiveMineOn = false;
  }
  cur = my; // cur 데이터 그룹에 현재 읽어온 데이터 저장
}
void WaitFunc(){

}
void SettingFunc(void){
    Serial.println("SETTING");
    SendCmd("page pgBeforeTagger");
    LeftGenerator();
    AllNeoOn(WHITE);
    EngineStop();
    encoderValue = 100;
    detachInterrupt(encoderPinA);
    detachInterrupt(encoderPinB);
    GameTimer.deleteTimer(gameTimerId);
    LogoutTimer.deleteTimer(logoutTimerId);
    BlinkTimer.deleteTimer(blinkTimerId);
    ptrRfidMode = WaitFunc;
    ptrCurrentMode = WaitFunc;
    receiveMineOn = false;
}
void ActivateFunc(void){
    Serial.println("ACTIVATE");
    AllNeoOn(YELLOW);
    SendCmd("page pgLocked");
    LeftGenerator();
    detachInterrupt(encoderPinA);
    detachInterrupt(encoderPinB);
    GameTimer.deleteTimer(gameTimerId);
    LogoutTimer.deleteTimer(logoutTimerId);
    BlinkTimer.deleteTimer(blinkTimerId);
    ptrRfidMode = LoginGenerator;
    ptrCurrentMode = RfidLoopMain;
}
void ReadyFunc(void){
    Serial.println("READY");
    AllNeoOn(RED);
    SendCmd("page pgPreTagger");
    LeftGenerator();
    detachInterrupt(encoderPinA);
    detachInterrupt(encoderPinB);
    GameTimer.deleteTimer(gameTimerId);
    LogoutTimer.deleteTimer(logoutTimerId);
    BlinkTimer.deleteTimer(blinkTimerId);
    ptrRfidMode = WaitFunc;
    ptrCurrentMode = WaitFunc;

}