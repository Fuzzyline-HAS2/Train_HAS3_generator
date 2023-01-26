#ifndef _LIBRARY_AND_PIN_
#define _LIBRARY_AND_PIN_

#include <Arduino.h>

#include <Adafruit_PN532.h>
#include <Adafruit_NeoPixel.h>

#include <Nextion.h>
#include <HAS2_Wifi.h>
#include <SimpleTimer.h>

#define NEXTIONHWSERIAL_RX_PIN 39
#define NEXTIONHWSERIAL_TX_PIN 33

#define PN532_SCK 18
#define PN532_MISO 19
#define PN532_MOSI 23
#define PN532_SS1 5

#define PN532_NEOPIXEL_PIN 25
#define ENCODER_NEOPIXEL_PIN 26
#define INNER_NEOPIXEL_PIN 27
#define CIRCUIT_NEOPIXEL_PIN 14

#define MOTOR_INA1_PIN  32
#define MOTOR_INA2_PIN  4
#define MOTOR_PWMA_PIN  22

#define RELAY_PIN  14

#define BOXSWITCH_PIN  36

#define encoderPinA 13
#define encoderPinB 15
#define buttonPin   34

#endif
