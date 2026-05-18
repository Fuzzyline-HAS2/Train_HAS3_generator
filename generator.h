#ifndef _DONE_ITEMBOX_CODE_
#define _DONE_ITEMBOX_CODE_

#include "Library_and_pin.h"
const int rfid_num = 1; // 설치된 pn532의 개수

//****************************************WIFI****************************************************************
HAS2_Wifi has2wifi("http://172.30.1.1");
SecureOTA ota(
    "https://raw.githubusercontent.com/Fuzzyline-HAS2/generator/third_store/update.bin",
    "https://raw.githubusercontent.com/Fuzzyline-HAS2/generator/third_store/version.txt",
    "https://raw.githubusercontent.com/Fuzzyline-HAS2/generator/third_store/update.sig",
    HMAC_SECRET,
    FIRMWARE_VER
);
void DataChanged();
void SettingFunc(void);
void ActivateFunc(void);
void ReadyFunc(void);

bool receiveMineOn = false;
//****************************************Game System****************************************************************
void (*ptrCurrentMode)();   //현재모드 저장용 포인터 함수
void (*ptrRfidMode)();      //rfid모드 저장용 포인터 함수

void WaitFunc();
void StarterActivate();
const unsigned long starterNeoDivider = 15000*7.5;
int starterEncoderUnit = 4000;        // 게이지 1칸당 필요한 엔코더 값
int starterDecreaseAmount = 1125;     // 2초마다 감소하는 엔코더 양
bool blinkOn = false;
//****************************************Timer System****************************************************************
SimpleTimer GameTimer;
SimpleTimer WifiTimer;

SimpleTimer BlinkTimer;

void TimerInit();
void WifiIntervalFunc();
void GameTimerFunc();


void BlinkTimerFunc();
void BlinkTimerStart(int Neo, int NeoColor);
int blinkNeo = 0;
int blinkColor = 0;

int wifiTimerId;
int gameTimerId;

int blinkTimerId;

unsigned long wifiTime = 2000;    // 1sec

unsigned long gameTime = 400;    // 3sec
unsigned long blinkTime = 1800;   // 1sec


volatile unsigned int gameTimerCnt;
//****************************************Nextion SETUP****************************************************************
HardwareSerial nexHwSerial(2);
String nextion_str = "";
int currner_page_num;
void NextionInit();
void DisplayCheck();
void NextionReceived();
void LeftGenerator();
void BatteryPackSend();
enum { PG_UNLOCKED = 0, PG_STARTER, PG_FIXED, PG_LOCKED, PG_BEFORE_TAGGER, PG_PRE_TAGGER, PG_PLAYER_WIN, PG_PLAYER_LOSE, PG_ESCAPE_OPEN };
const String NEXTION_PAGES[] = {
    "page pgUnlocked",
    "page pgStarter",
    "page pgFixed",
    "page pgLocked",
    "page pgBeforeTagger",
    "page pgPreTagger",
    "page pgPlayerWin",
    "page pgPlayerLose",
    "page pgEscapeOpen"
};
//****************************************Neopixel SETUP****************************************************************
#define DEFAULT_BRIGHTNESS 50
int ledBrightness = DEFAULT_BRIGHTNESS;
void NeopixelInit();
void UpdateBrightness();
void EncoderNeopixelOn();
void NeoBlink(int neo, int neoColor, int cnt, int blinkTime);
const int NumPixels[4] = {28,4,16,10};
const int NeopixelNum = 4;
enum {GAUGE = 0, STARTER, DEVICESTATE, CIRCUIT};
enum {WHITE = 0, RED, YELLOW, GREEN, BLUE, PURPLE, BLACK, BLUE0, BLUE1, BLUE2, BLUE3};
// Neopixel 색상정보
int color[11][3] = {    {255, 255, 255}, //WHITE
                        {255, 0,   0},   //RED
                        {255, 255, 0},   //YELLOW
                        {0,   255, 0},   //GREEN
                        {0,   0,   255}, //BLUE
                        {255, 0,   255}, //PURPLE
                        {0,   0,   0},   //BLACK
                        {0,   0,   64},  //ENCODERBLUE0
                        {0,   0,   128}, //ENCODERBLUE1
                        {0,   0,   192}, //ENCODERBLUE2
                        {0,   0,   255}}; //ENCODERBLUE3

const int neopixel_num = 4; // 설치된 네오픽셀의 개수

Adafruit_NeoPixel pixels[NeopixelNum] = {Adafruit_NeoPixel(NumPixels[GAUGE], PN532_NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800),
                                         Adafruit_NeoPixel(NumPixels[STARTER], ENCODER_NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800),
                                         Adafruit_NeoPixel(NumPixels[DEVICESTATE], INNER_NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800),
                                         Adafruit_NeoPixel(NumPixels[CIRCUIT], CIRCUIT_NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800)
                                         };
                                         
//****************************************RFID SETUP****************************************************************
enum {MAINPN532 = 0};

Adafruit_PN532 nfc[rfid_num] = {Adafruit_PN532(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS1)};

bool rfid_tag;
bool rfid_timer_assess;

bool rfid_init_complete[rfid_num];
void RfidInit(void);
void RfidLoop(void);
void CheckingPlayers(uint8_t user, uint8_t user_num, uint8_t rfid_num);


//****************************************MOTOR SETUP****************************************************************
const int MotorFreq = 5000;
const int MotorResolution = 8;
const int MotorLedChannel = 3;
const int MotorMAX_DUTY_CYCLE = (int)(pow(2, MotorResolution) - 1);
void MotorInit();
void EngineSpeeed(int enginePwm);
void EngineStop();

//****************************************ENCODER SETUP****************************************************************
void EncoderInit();
long readEncoderValue(void);
void updateEncoder();

volatile int lastEncoded = 0;       //직전 엔코더 값
volatile long encoderValue = 0;     //현재 엔코더 값

long lastencoderValue = 0;

int lastMSB = 0;
int lastLSB = 0;

#endif
