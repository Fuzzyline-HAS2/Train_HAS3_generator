void DataChanged()
{
  static StaticJsonDocument<1000> cur;  //저장되어 있는 cur과 읽어온 my 값과 비교후 실행
  // 서버에서 받은 스타터 설정값 동기화
  if((int)my["starter_encoder_unit"] > 0)  starterEncoderUnit  = (int)my["starter_encoder_unit"];
  if((int)my["starter_decrease_amount"] > 0) starterDecreaseAmount = (int)my["starter_decrease_amount"];
  if(my["brightness"].as<int>() != cur["brightness"].as<int>()) {
    UpdateBrightness();
  }
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
        SendCmd(NEXTION_PAGES[PG_ESCAPE_OPEN]);
        LeftGenerator();
        GameTimer.deleteTimer(gameTimerId);

        BlinkTimer.deleteTimer(blinkTimerId);
        AllNeoOn(BLUE);
      }
      else if((String)(const char*)my["device_state"] == "repaired"){
        Serial.println("StartFinish PTRFUNC");

        GameTimer.deleteTimer(gameTimerId);        //게임 타이머 종료
        BlinkTimer.deleteTimer(blinkTimerId);
        Serial.println("Generator Fixed!");
        SendCmd(NEXTION_PAGES[PG_FIXED]);
        LeftGenerator();
        AllNeoOn(BLUE);
        ledcWrite(MOTOR_PWMA_PIN, 250);
        ptrCurrentMode = WaitFunc;
      }
      else if((String)(const char*)my["device_state"] == "battery_max"){
        int maxBattery = (int)my["max_battery_pack"] - (int)my["battery_pack"];
        Serial.println((String)maxBattery);
        has2wifi.Send((String)(const char*)my["device_name"], "battery_pack", ((String)maxBattery));

        GameTimer.deleteTimer(gameTimerId);        //게임 타이머 종료
        ActivateFunc();
      }
      else if((String)(const char*)my["device_state"] == "starter_finish"){

      }
      else if((String)(const char*)my["device_state"] == "player_win"){ 
        ptrRfidMode = WaitFunc;
        ptrCurrentMode = WaitFunc;
        AllNeoOn(BLUE);
        SendCmd(NEXTION_PAGES[PG_PLAYER_WIN]);
      }
      else if((String)(const char*)my["device_state"] == "player_lose"){
        ptrRfidMode = WaitFunc;
        ptrCurrentMode = WaitFunc;
        AllNeoOn(RED);
        SendCmd(NEXTION_PAGES[PG_PLAYER_LOSE]);
      }
      else if((String)(const char*)my["device_state"] == "github"){
        Serial.println("[OTA] OTA 업데이트 요청 수신");
        ota.check();
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
    SendCmd(NEXTION_PAGES[PG_BEFORE_TAGGER]);
    LeftGenerator();
    AllNeoOn(WHITE);
    EngineStop();
    encoderValue = 100;
    detachInterrupt(encoderPinA);
    detachInterrupt(encoderPinB);
    GameTimer.deleteTimer(gameTimerId);

    BlinkTimer.deleteTimer(blinkTimerId);
    ptrRfidMode = WaitFunc;
    ptrCurrentMode = WaitFunc;
    receiveMineOn = false;
}
void ActivateFunc(void){
    Serial.println("ACTIVATE");
    AllNeoOn(YELLOW);
    SendCmd(NEXTION_PAGES[PG_LOCKED]);
    LeftGenerator();
    detachInterrupt(encoderPinA);
    detachInterrupt(encoderPinB);
    GameTimer.deleteTimer(gameTimerId);

    BlinkTimer.deleteTimer(blinkTimerId);
    nfc[MAINPN532].SAMConfig();   // PN532 재활성화 (idle 후 응답 복구)
    ptrRfidMode = LoginGenerator;
    ptrCurrentMode = RfidLoopMain;
}
void ReadyFunc(void){
    Serial.println("READY");
    AllNeoOn(RED);
    SendCmd(NEXTION_PAGES[PG_PRE_TAGGER]);
    LeftGenerator();
    detachInterrupt(encoderPinA);
    detachInterrupt(encoderPinB);
    GameTimer.deleteTimer(gameTimerId);

    BlinkTimer.deleteTimer(blinkTimerId);
    ptrRfidMode = WaitFunc;
    ptrCurrentMode = WaitFunc;

}