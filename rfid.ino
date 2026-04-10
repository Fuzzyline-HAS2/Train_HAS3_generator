void RfidInit()
{
  RestartPn532:
  nfc[MAINPN532].begin();
  if (!(nfc[MAINPN532].getFirmwareVersion()))
  {
    Serial.println("PN532 FAIL : MAINPN532");
    AllNeoOn(RED);
    //goto RestartPn532;
  }
  else
  {
    nfc[MAINPN532].SAMConfig();
    Serial.println("PN532 SUCC : MAINPN532");
    rfid_init_complete[MAINPN532] = true;
    
  }
  delay(100);
}

void RfidLoopMain()
{
  uint8_t uid[3][7] = {{0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0}}; // Buffer to store the returned UID
  uint8_t uidLength[] = {0};                   // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  uint8_t data[32];
  byte pn532_packetbuffer11[64];
  pn532_packetbuffer11[0] = 0x00;

  for (int i = 0; i < rfid_num; ++i)
  {
    if (nfc[MAINPN532].sendCommandCheckAck(pn532_packetbuffer11, 1)){ // rfid 통신 가능한 상태인지 확인
      if (nfc[MAINPN532].startPassiveTargetIDDetection(PN532_MIFARE_ISO14443A)){                                       // rfid에 tag 찍혔는지 확인용 //데이터 들어오면 uid정보 가져오기
        if (nfc[MAINPN532].ntag2xx_ReadPage(7, data)){ // ntag 데이터에 접근해서 불러와서 data행열에 저장
          Serial.println("TAGGGED");
          CheckingPlayers(data);
        }
      }
    }
  }
}
void CheckingPlayers(uint8_t rfidData[32]) //어떤 카드가 들어왔는지 확인용
{ 
  String tagUser = "";
  for(int i = 0; i < 4; i++)    //GxPx 데이터만 배열에서 추출해서 string으로 저장
    tagUser += (char)rfidData[i];
  Serial.println("tag_user_data : " + tagUser);     // 1. 태그한 플레이어의 역할과 생명칩갯수, 최대생명칩갯수 등 읽어오기
  if(tagUser == "MMMM"){  //스태프카드 초기화
    ESP.restart();
  }
  has2wifi.Receive(tagUser);                        // 2. 술래인지, 플레이어인지 구분
  if((String)(const char*)tag["role"] == "player"){ // 3. 태그한 사용자가 플레이어고
    Serial.println("Player Tagged");
    ptrRfidMode();
  }
  else if((String)(const char*)tag["role"] == "tagger"){ // 3. 태그한 사용자가 플레이어고
    Serial.println("Tagger Tagged");
  }
  else if((String)(const char*)tag["role"] == "ghost"){ // 3. 태그한 사용자가 플레이어고
    Serial.println("Ghost Tagged");
  }
  else{
    Serial.println("Wrong TAG");
  }
}

void LoginGenerator()
{ 
  Serial.println("LoginGenerator PTRFUNC");
  BatteryPackSend();                                //현재 배터리팩 개수 NExTION으로 전송
  PageSend();                                       //로그인 페이지 다음에 나올 페이지 지정
  SendCmd("page pgLogin");
  LeftGenerator();
  delay(3500);
  BatteryPackSend();                                //현재 배터리팩 개수 NExTION으로 전송
  if((String)(const char*)my["device_state"] == "starter_finish"){
    AllNeoOn(GREEN);
    LeftGenerator(); 
    detachInterrupt(encoderPinA);
    detachInterrupt(encoderPinB);
    EngineSpeeed(250);
    ptrRfidMode = StartFinish;
    ptrCurrentMode = RfidLoopMain;
    NeoLightColor(GAUGE, color[BLUE]);
    BlinkTimer.deleteTimer(blinkTimerId);
    BlinkTimerStart(CIRCUIT, YELLOW);                                  //스타터종료 인식 pn532 함수로 변경
  }
  else if((String)(const char*)my["device_state"] == "battery_max"){
    Serial.println("Battery_Max Login");
    BatteryFinish();                                        //배터리팩 충전와료 pn532 함수로 변경
  }
  else{
    Serial.println("Battery_Charge Login");
    ptrRfidMode = BatteryPackCharge;                                    //배터리팩 인식 pn532 함수로 변경
    if((int)my["battery_pack"] == (int)my["max_battery_pack"]){
      Serial.println("Battery Full on Login! → BatteryFinish");
      BatteryFinish();
    }
  }
}

void BatteryPackCharge()
{ 
  Serial.println("BatteryPackCharge PTRFUNC");
  if((int)tag["battery_pack"] != 0 && (int)my["battery_pack"] < (int)my["max_battery_pack"]){    //발전기에 필요한 배터리팩 개수 > 플레이어가 소지한 배터리팩 개수
    SendCmd("wBatteryCharge.en=1");
    Serial.println("BatteyPack Charge");
    has2wifi.Send((String)(const char*)tag["device_name"], "battery_pack", ("-1"));
    has2wifi.Send((String)(const char*)my["device_name"], "battery_pack", ("+1"));
    receiveMineOn = true; //ReceieveMine() 하기전에 true로 만들어서 기존데이터바뀌어도 DataChanged()실행 안되게 막음
    has2wifi.ReceiveMine();
    delay(10);
    BatteryPackSend();
    delay(2000);
    if((int)my["battery_pack"] == (int)my["max_battery_pack"]){ //배터리 충전완료 → 바로 스타터로
      Serial.println("Battery Full! → BatteryFinish");
      BatteryFinish();
    }
  }
  else if((int)my["battery_pack"] == (int)my["max_battery_pack"]){
    BatteryFinish();
  }
  else if((int)my["battery_pack"] > (int)my["max_battery_pack"]){
    Serial.println("ERROR: Battery OverCharged!");
  }
  else{
    Serial.println("Not Charged Yet");
  }
}
void BatteryFinish()
{
  has2wifi.Send((String)(const char*)my["device_name"], "device_state", "battery_max"); //메인으로 전송
  SendCmd("page pgStarter");
  delay(10);
  SendCmd("wStaterOn.en=1");
  LeftGenerator();
  AllNeoOn(GREEN);
  Serial.println("Battery Finish Func!");
  encoderValue = 1;
  GameTimer.deleteTimer(gameTimerId);
  gameTimerCnt = 0;
  gameTimerId = GameTimer.setInterval(gameTime,GameTimerFunc);
  AllNeoOn(BLUE);
  LeftGenerator();
  attachInterrupt(encoderPinA, updateEncoder, CHANGE);
  attachInterrupt(encoderPinB, updateEncoder, CHANGE);
  delay(100);
  ptrCurrentMode = StarterActivate;
}

void StartFinish()
{
  Serial.println("StartFinish PTRFUNC");
  GameTimer.deleteTimer(gameTimerId);        //게임 타이머 종료3
  BlinkTimer.deleteTimer(blinkTimerId);
  Serial.println("Generator Fixed!");
  SendCmd("page pgFixed");
  has2wifi.Send((String)(const char*)my["device_name"], "device_state", "repaired");
  receiveMineOn = true;
  has2wifi.ReceiveMine();
  LeftGenerator();
  AllNeoOn(BLUE);
  ptrCurrentMode = WaitFunc;
}

// void BatteryPackCharge()
// { 
//   Serial.println("BatteryPackCharge PTRFUNC");
//   LogoutTimer.deleteTimer(logoutTimerId);
//   logoutTimerId = LogoutTimer.setInterval(wifiTime,LogoutTimerFunc);
//   if(((int)tag["battery_pack"] + (int)my["battery_pack"]) <= (int)my["max_battery_pack"]){    //발전기에 필요한 배터리팩 개수 > 플레이어가 소지한 배터리팩 개수
//     Serial.println("All Charged  " + ("+-" + (String)(const char*)tag["battery_pack"]));
//     has2wifi.Send((String)(const char*)tag["device_name"], "battery_pack", ("-" + (String)(const char*)tag["battery_pack"]));
//     has2wifi.Send((String)(const char*)my["device_name"], "battery_pack", ("+" + (String)(const char*)tag["battery_pack"]));
//   }
//   else if(((int)tag["battery_pack"] + (int)my["battery_pack"]) > (int)my["max_battery_pack"]){ //발전기에 필요한 배터리팩 개수 < 플레이어가 소지한 배터리팩 개수
//     String numberString = String((int)my["max_battery_pack"] - (int)my["battery_pack"] );       //필요한 배터리팩만 충전하는 계산식
//     Serial.println("Leftover Charged  " + ("+-" + numberString));
//     has2wifi.Send((String)(const char*)tag["device_name"], "battery_pack", ("-" + numberString));
//     has2wifi.Send((String)(const char*)my["device_name"], "battery_pack", ("+" + numberString));
//   }
//   else{
//     Serial.println("Already Fully Charged");
//   }
//   delay(500);
//   has2wifi.ReceiveMine();
//   BatteryPackSend();
//   delay(1000);
//   if((int)my["battery_pack"] == (int)my["max_battery_pack"]){
//     Serial.println("Battery Full!");              
//     has2wifi.Send((String)(const char*)my["device_name"], "device_state", "battery_max"); //메인으로 전송
//     AllNeoOn(GREEN);
//     attachInterrupt(encoderPinA, updateEncoder, CHANGE);                                  
//     attachInterrupt(encoderPinB, updateEncoder, CHANGE);
//     encoderValue = 1;
//     gameTimerId = GameTimer.setInterval(1000,GameTimerFunc);
//     AllNeoOn(GREEN);
//     SendCmd("page pgStarter");
//     LeftGenerator();
//     ptrCurrentMode = StarterActivate;
//   }
//   else if((int)my["battery_pack"] > (int)my["max_battery_pack"]){
//     Serial.println("ERROR: Battery OverCharged!");
//   }
//   else{
//     Serial.println("Not Charged Yet");
//   }
// }
