void RfidInit()
{
  RestartPn532:
  nfc[MAINPN532].begin();
  if (!(nfc[MAINPN532].getFirmwareVersion()))
  {
    Serial.println("PN532 FAIL : MAINPN532");
    rfid_init_complete[MAINPN532] = false;
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
  if (!rfid_init_complete[MAINPN532]) {
    return;
  }

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
  Serial.println("tag_user_data : " + tagUser);
  if(tagUser == "MMMM"){  //스태프카드 초기화
    ESP.restart();
  }
  // G9PX 값으로 로컬에서 역할 고정 판단: G9P1=술래, G9P2=유령, G9P3~G9P8=생존자
  // 임시 테스트: G2P2도 생존자로 처리
  if(tagUser == "G9P1"){
    Serial.println("Tagger Tagged");
  }
  else if(tagUser == "G9P2"){
    Serial.println("Ghost Tagged");
  }
  else if((tagUser.substring(0,3) == "G9P" && tagUser[3] >= '3' && tagUser[3] <= '8') || tagUser == "G2P2"){
    Serial.println("Player Tagged");
    ptrRfidMode();
  }
  else{
    Serial.println("Wrong TAG");
  }
}


void BatteryPackCharge()
{
  Serial.println("BatteryPackCharge PTRFUNC");
  if((int)my["battery_pack"] < (int)my["max_battery_pack"]){
    Serial.println("BatteyPack Charge");
    has2wifi.Send((String)(const char*)my["device_name"], "battery_pack", ("+1"));
    receiveMineOn = true;
    has2wifi.ReceiveMine();
    delay(10);
    BatteryPackSend();
    delay(2000);
    if((int)my["battery_pack"] == (int)my["max_battery_pack"]){
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
  SendCmd(NEXTION_PAGES[PG_STARTER]);
  delay(10);
  SendCmd("wStaterOn.en=1");
  delay(10);
  AllNeoOn(GREEN);
  Serial.println("Battery Finish Func!");
  encoderValue = 1;
  GameTimer.deleteTimer(gameTimerId);
  gameTimerCnt = 0;
  gameTimerId = GameTimer.setInterval(gameTime,GameTimerFunc);
  AllNeoOn(BLUE);
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
  has2wifi.Send((String)(const char*)my["device_name"], "device_state", "repaired");
  receiveMineOn = true;
  has2wifi.ReceiveMine();
  if ((String)(const char*)my["device_state"] == "repaired_all") {
    ptrRfidMode = WaitFunc;
    ptrCurrentMode = WaitFunc;
    EngineStop();
    SendCmd(NEXTION_PAGES[PG_ESCAPE_OPEN]);
    BlinkTimer.deleteTimer(blinkTimerId);
    AllNeoOn(BLUE);
    return;
  }
  SendCmd(NEXTION_PAGES[PG_FIXED]);
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
