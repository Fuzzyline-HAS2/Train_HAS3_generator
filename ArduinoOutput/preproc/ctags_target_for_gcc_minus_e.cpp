# 1 "c:\\Users\\HAS1\\Desktop\\BBangJun\\HAS2_Code\\HAS2_Final_Code\\generator\\generator.ino"
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
# 12 "c:\\Users\\HAS1\\Desktop\\BBangJun\\HAS2_Code\\HAS2_Final_Code\\generator\\generator.ino"
# 13 "c:\\Users\\HAS1\\Desktop\\BBangJun\\HAS2_Code\\HAS2_Final_Code\\generator\\generator.ino" 2

void setup() {
    Serial.begin(115200);
    NeopixelInit();
    RfidInit();
    MotorInit();
    EncoderInit();
    NextionInit();
    TimerInit();
// has2wifi.Setup("KT_GiGA_6C64","ed46zx1198");
 has2wifi.Setup("badland");
//    has2wifi.Setup("city");
    DataChanged();
}
void loop() {
    ptrCurrentMode();
    TimerRun();
}
# 1 "c:\\Users\\HAS1\\Desktop\\BBangJun\\HAS2_Code\\HAS2_Final_Code\\generator\\Game_system.ino"
void StarterActivate(){
    // int gaugeNeoCnt = map(encoderValue,0,(starterNeoDivider),0,NumPixels[GAUGE]);
    // int motorSpeed = map(encoderValue,0,(starterNeoDivider),0,255);
    // Serial.println(String(encoderValue) + "___"+ String(gaugeNeoCnt) + "___" + String(motorSpeed));
    int gaugeNeoCnt = encoderValue / 4000;
    Serial.println(gaugeNeoCnt);
    EncoderNeopixelOn(gaugeNeoCnt);
    EngineSpeeed(gaugeNeoCnt*8);
    if(gaugeNeoCnt >= NumPixels[GAUGE]){
        detachInterrupt(13);
        detachInterrupt(15);
        SendCmd("page pgStarterDone");
        has2wifi.Send((String)(const char*)my["device_name"], "device_state", "starter_finish");
        ptrRfidMode = StartFinish;
        ptrCurrentMode = RfidLoopMain;
        BlinkTimer.deleteTimer(blinkTimerId);
        pixels[STARTER].lightColor(color[GREEN]);
        GameTimer.deleteTimer(gameTimerId); //게임 타이머 종료
        BlinkTimer.deleteTimer(blinkTimerId);
        BlinkTimerStart(CIRCUIT, YELLOW);
    }
}
# 1 "c:\\Users\\HAS1\\Desktop\\BBangJun\\HAS2_Code\\HAS2_Final_Code\\generator\\Wifi.ino"
void DataChanged()
{
  static StaticJsonDocument<500> cur; //저장되어 있는 cur과 읽어온 my 값과 비교후 실행
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
        GameTimer.deleteTimer(gameTimerId); //게임 타이머 종료
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
        GameTimer.deleteTimer(gameTimerId); //게임 타이머 종료
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
    detachInterrupt(13);
    detachInterrupt(15);
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
    detachInterrupt(13);
    detachInterrupt(15);
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
    detachInterrupt(13);
    detachInterrupt(15);
    GameTimer.deleteTimer(gameTimerId);
    LogoutTimer.deleteTimer(logoutTimerId);
    BlinkTimer.deleteTimer(blinkTimerId);
    ptrRfidMode = WaitFunc;
    ptrCurrentMode = WaitFunc;

}
# 1 "c:\\Users\\HAS1\\Desktop\\BBangJun\\HAS2_Code\\HAS2_Final_Code\\generator\\encoder.ino"
void EncoderInit()
{
    Serial.println("ENCODER INIT");
    pinMode(13, 0x01);
    pinMode(15, 0x01);
    // pinMode(buttonPin, INPUT_PULLUP);

    digitalWrite(13, 0x1); // turn pullup resistor on
    digitalWrite(15, 0x1); // turn pullup resistor on

    // call updateEncoder() when any high/low changed seen
    // on interrupt 0 (pin 2), or interrupt 1 (pin 3)
    // attachInterrupt(encoderPinA, updateEncoder, CHANGE);
    // attachInterrupt(encoderPinB, updateEncoder, CHANGE);
}
void updateEncoder()
{
    logoutTimerCnt = 0;
    gameTimerCnt = 0;

    int MSB = digitalRead(13); // MSB = most significant bit
    int LSB = digitalRead(15); // LSB = least significant bit

    int encoded = (MSB << 1) | LSB; // converting the 2 pin value to single number
    int sum = (lastEncoded << 2) | encoded; // adding it to the previous encoded value

    if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValue++;
    if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderValue++;
    lastEncoded = encoded; // store this value for next time

}
# 1 "c:\\Users\\HAS1\\Desktop\\BBangJun\\HAS2_Code\\HAS2_Final_Code\\generator\\motor.ino"
void MotorInit()
{
    //Linear Motor Init
    pinMode(36,0x05);
    pinMode(32, 0x03);
    pinMode(4, 0x03);
    ledcSetup(MotorLedChannel, MotorFreq, MotorResolution);
    ledcAttachPin(22, MotorLedChannel);
    ledcWrite(MotorLedChannel, 0);
}
void EngineSpeeed(int enginePwm)
{
    // Serial.println("Engine On");
    ledcWrite(MotorLedChannel, enginePwm);
    digitalWrite(32, 0x0);
    digitalWrite(4, 0x1);
}

void EngineStop()
{
    Serial.println("Engine Stop");
    digitalWrite(32, 0x0);
    digitalWrite(4, 0x0);
}
# 1 "c:\\Users\\HAS1\\Desktop\\BBangJun\\HAS2_Code\\HAS2_Final_Code\\generator\\neopixel.ino"
void NeopixelInit()
{
  for (int i = 0; i < NeopixelNum; ++i)
  {
    pixels[i].begin();
  }
  for (int i = 0; i < NeopixelNum; ++i)
  {
    pixels[i].lightColor(color[WHITE]);
  }
}

void EncoderNeopixelOn(int neoNum){
  for(int i = 0; i < neoNum; i++)
    pixels[GAUGE].setPixelColor(i,pixels[GAUGE].Color(color[BLUE][0], color[BLUE][1], color[BLUE][2]));
  for(int i = neoNum; i < NumPixels[GAUGE]; i++)
    pixels[GAUGE].setPixelColor(i,pixels[GAUGE].Color(color[GREEN][0],color[GREEN][1],color[GREEN][2]));
  pixels[GAUGE].show();
}
void NeoBlink(int neo, int neoColor, int cnt, int blinkTime){
  for(int i = 0; i < cnt; i++){ //0.5*10=5초동안 점멸
    pixels[neo].lightColor(color[BLACK]); //전체 off
    delay(blinkTime);
    pixels[neo].lightColor(color[neoColor]); //전체 적색on
    delay(blinkTime); //전체 적색on
  }
}

void AllNeoOn(int neoColor){
  for (int i = 0; i < NeopixelNum; ++i)
    pixels[i].lightColor(color[neoColor]);
}
# 1 "c:\\Users\\HAS1\\Desktop\\BBangJun\\HAS2_Code\\HAS2_Final_Code\\generator\\nextion.ino"
void NextionInit()
{
   nexInit();
   nexHwSerial.begin(9600, 0x800001c, 39, 33);
}

void DisplayCheck()
{
 while (nexHwSerial.available() > 0)
 {
   String nextion_string = nexHwSerial.readStringUntil(' ');
   NextionReceived(&nextion_string);
 }
}

void NextionReceived(String *nextion_string)
{
 if (*nextion_string == "test")
 {
   sendCommand("page pgItemTaken");
   sendCommand("wQuizSolved.en=1");
 }
}

void SendCmd(String command)
{
    String cmd = "";
    if (command.startsWith("page") && (String)(const char *)shift_machine["selected_language"] == "EN")
    {
        cmd = "page E"+ command.substring(5);
    }
    else
    {
        cmd = command;
    }
    sendCommand(cmd.c_str());
}
void LeftGenerator(){

    if((int)my["left_generator"] == 5)
    {
        sendCommand("pgLocked.vLeftGen.val=5");
        sendCommand("pleftDevice.pic=42");
        Serial.println("left Generator 5");
    }
    else if((int)my["left_generator"] == 4)
    {
        sendCommand("pgLocked.vLeftGen.val=4");
        sendCommand("pleftDevice.pic=41");
        Serial.println("left Generator 4");
    }
    else if((int)my["left_generator"] == 3)
    {
        sendCommand("pgLocked.vLeftGen.val=3");
        sendCommand("pleftDevice.pic=40");
        Serial.println("left Generator 3");
    }
    else if((int)my["left_generator"] == 2)
    {
        sendCommand("pgLocked.vLeftGen.val=2");
        sendCommand("pleftDevice.pic=39");
        Serial.println("left Generator 2");
    }
    else if((int)my["left_generator"] == 1)
    {
        sendCommand("pgLocked.vLeftGen.val=1");
        sendCommand("pleftDevice.pic=38");
        Serial.println("left Generator 1");
    }
}
void BatteryPackSend(){
    if((String)(const char*)my["battery_pack"] == "0")
    {
        sendCommand("pgUnlocked.vBatteryPack.val=0");
        sendCommand("pBattery.pic=22");
    }
    else if((String)(const char*)my["battery_pack"] == "1")
    {
        sendCommand("pgUnlocked.vBatteryPack.val=1");
        sendCommand("pBattery.pic=23");
    }
    else if((String)(const char*)my["battery_pack"] == "2")
    {
        sendCommand("pgUnlocked.vBatteryPack.val=2");
        sendCommand("pBattery.pic=24");
    }
    else if((String)(const char*)my["battery_pack"] == "3")
    {
        sendCommand("pgUnlocked.vBatteryPack.val=3");
        sendCommand("pBattery.pic=25");
    }
    else if((String)(const char*)my["battery_pack"] == "4")
    {
        sendCommand("pgUnlocked.vBatteryPack.val=4");
        sendCommand("pBattery.pic=26");
    }
    else if((String)(const char*)my["battery_pack"] == "5")
    {
        sendCommand("pgUnlocked.vBatteryPack.val=5");
        sendCommand("pBattery.pic=27");
    }
}
void PageSend(){
    if((String)(const char*)my["device_state"] == "activate"){
        sendCommand("pgLogin.device_state.val=0");
        if((int)my["battery_pack"] == (int)my["max_battery_pack"])
            sendCommand("pgLogin.device_state.val=3");
    }
    else if((String)(const char*)my["device_state"] == "battery_max")
        sendCommand("pgLogin.device_state.val=1");
    else if((String)(const char*)my["device_state"] == "starter_finish")
        sendCommand("pgLogin.device_state.val=2");
}
# 1 "c:\\Users\\HAS1\\Desktop\\BBangJun\\HAS2_Code\\HAS2_Final_Code\\generator\\rfid.ino"
void RfidInit()
{
  RestartPn532:
  nfc[MAINPN532].begin();
  if (!(nfc[MAINPN532].getFirmwareVersion()))
  {
    Serial.println("PN532 FAIL : MAINPN532");
    AllNeoOn(RED);
    goto RestartPn532;
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
  uint8_t uidLength[] = {0}; // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  uint8_t data[32];
  byte pn532_packetbuffer11[64];
  pn532_packetbuffer11[0] = 0x00;

  for (int i = 0; i < rfid_num; ++i)
  {
    if (nfc[MAINPN532].sendCommandCheckAck(pn532_packetbuffer11, 1)){ // rfid 통신 가능한 상태인지 확인
      if (nfc[MAINPN532].startPassiveTargetIDDetection((0x00))){ // rfid에 tag 찍혔는지 확인용 //데이터 들어오면 uid정보 가져오기
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
  logoutTimerCnt = 0; //로그아웃 타이머 카운트 리셋
  for(int i = 0; i < 4; i++) //GxPx 데이터만 배열에서 추출해서 string으로 저장
    tagUser += (char)rfidData[i];
  Serial.println("tag_user_data : " + tagUser); // 1. 태그한 플레이어의 역할과 생명칩갯수, 최대생명칩갯수 등 읽어오기
  if(tagUser == "MMMM"){ //스태프카드 초기화
    ESP.restart();
  }
  has2wifi.Receive(tagUser); // 2. 술래인지, 플레이어인지 구분
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
  BatteryPackSend(); //현재 배터리팩 개수 NExTION으로 전송
  PageSend(); //로그인 페이지 다음에 나올 페이지 지정
  SendCmd("page pgLogin");
  LeftGenerator();
  delay(3500);
  BatteryPackSend(); //현재 배터리팩 개수 NExTION으로 전송
  LogoutTimer.deleteTimer(logoutTimerId);
  logoutTimerCnt = 0; //로그아웃 타이머 카운트 리셋
  logoutTimerId = LogoutTimer.setInterval(logoutTime,LogoutTimerFunc); //로그아웃 타이머 시작
  if((String)(const char*)my["device_state"] == "starter_finish"){
    AllNeoOn(GREEN);
    LeftGenerator();
    detachInterrupt(13);
    detachInterrupt(15);
    EngineSpeeed(250);
    ptrRfidMode = StartFinish;
    ptrCurrentMode = RfidLoopMain;
    pixels[GAUGE].lightColor(color[BLUE]);
    BlinkTimer.deleteTimer(blinkTimerId);
    BlinkTimerStart(CIRCUIT, YELLOW); //스타터종료 인식 pn532 함수로 변경
  }
  else if((String)(const char*)my["device_state"] == "battery_max"){
    Serial.println("Battery_Max Login");
    BatteryFinish(); //배터리팩 충전와료 pn532 함수로 변경
  }
  else{
    Serial.println("Battery_Charge Login");
    ptrRfidMode = BatteryPackCharge; //배터리팩 인식 pn532 함수로 변경
    if((int)my["battery_pack"] == (int)my["max_battery_pack"]){
      AllNeoOn(GREEN);
      // has2wifi.Send((String)(const char*)my["device_name"], "device_state", "battery_max"); //메인으로 전송
      Serial.println("Battery Full!");
      SendCmd("page pgBatteryMax");
      delay(100);
      SendCmd("wBatteryFull.en=1");
      BlinkTimer.deleteTimer(blinkTimerId);
      BlinkTimerStart(CIRCUIT, YELLOW);
    }
  }
}

void BatteryPackCharge()
{
  Serial.println("BatteryPackCharge PTRFUNC");
  // LogoutTimer.restartTimer(logoutTimerId);
  logoutTimerCnt = 0; //로그아웃 타이머 카운트 리셋
  if((int)tag["battery_pack"] != 0 && (int)my["battery_pack"] < (int)my["max_battery_pack"]){ //발전기에 필요한 배터리팩 개수 > 플레이어가 소지한 배터리팩 개수
    SendCmd("wBatteryCharge.en=1");
    Serial.println("BatteyPack Charge");
    has2wifi.Send((String)(const char*)tag["device_name"], "battery_pack", ("-1"));
    has2wifi.Send((String)(const char*)my["device_name"], "battery_pack", ("+1"));
    receiveMineOn = true; //ReceieveMine() 하기전에 true로 만들어서 기존데이터바뀌어도 DataChanged()실행 안되게 막음
    has2wifi.ReceiveMine();
    delay(10);
    BatteryPackSend();
    delay(2000);
    if((int)my["battery_pack"] == (int)my["max_battery_pack"]){ //배터리 충전완료되면 '태그하여 충전완료'페이지로 이동
      AllNeoOn(GREEN);
      // has2wifi.Send((String)(const char*)my["device_name"], "device_state", "battery_max"); //메인으로 전송
      Serial.println("Battery Full!");
      SendCmd("page pgBatteryMax");
      delay(100);
      SendCmd("wBatteryFull.en=1");
      BlinkTimer.deleteTimer(blinkTimerId);
      BlinkTimerStart(CIRCUIT, YELLOW);
    }
  }
  else if((int)my["battery_pack"] == (int)my["max_battery_pack"]){ //'태그하여 충전완료'페이지 에서 태그하면 스타터로 넘기는 부분
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
  logoutTimerCnt = 0; //로그아웃 타이머 카운트 리셋
  AllNeoOn(GREEN);
  Serial.println("Battery Finish Func!");
  encoderValue = 1;
  GameTimer.deleteTimer(gameTimerId);
  gameTimerCnt = 0;
  gameTimerId = GameTimer.setInterval(gameTime,GameTimerFunc);
  BlinkTimer.deleteTimer(blinkTimerId);
  BlinkTimerStart(STARTER, YELLOW);
  LeftGenerator();
  ptrCurrentMode = StarterActivate;
  attachInterrupt(13, updateEncoder, 0x03);
  attachInterrupt(15, updateEncoder, 0x03);
}

void StartFinish()
{
  Serial.println("StartFinish PTRFUNC");
  LogoutTimer.deleteTimer(logoutTimerId);
  GameTimer.deleteTimer(gameTimerId); //게임 타이머 종료3
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
# 1 "c:\\Users\\HAS1\\Desktop\\BBangJun\\HAS2_Code\\HAS2_Final_Code\\generator\\timer.ino"
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
void LogoutTimerFunc(){
    logoutTimerCnt++;
    // Serial.println("LogoutTimerCnt:" + (String)logoutTimerCnt);
    if(logoutTimerCnt >= 12){
        Serial.println("LogOutTimer TimeOUT");
        LogoutTimer.deleteTimer(logoutTimerId); //로그아웃 타이머 종료
        GameTimer.deleteTimer(gameTimerId);
        BlinkTimer.deleteTimer(blinkTimerId);
        EngineStop();
        ActivateFunc();
        logoutTimerCnt = 0;
    }
}

void BlinkTimerFunc(){
    // Serial.println("Blink!");
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
