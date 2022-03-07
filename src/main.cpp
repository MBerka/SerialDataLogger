#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

const uint8_t cardDetect{7};
const uint8_t chipSelect{4};

#define GREEN_LED 8

// last write to log file
uint32_t lastIntervalWrite{0};
// last turned on light
uint32_t lastLedOn{0};

// log file path
char *filename PROGMEM = "/serial.log";

// buffer to hold serial data
char buffer[512];

void lightLed();
boolean startSDCard();

// Runs once at start
void setup()
{
  // Hardware serial (pins 0 / 1) - regular Serial only goes to USB
  Serial1.begin(115200);
  // while (!Serial);
  // {
  //   // wait till USB port is available - do not use for UART logging
  // }

  pinMode(cardDetect, INPUT_PULLUP);
  // see if the card is present and can be initialized:
  if (startSDCard() == true)
  {
    Serial1.println(F("SD initialized!"));
    lightLed();
  }

  File logfile = SD.open(filename, FILE_WRITE);
  if (logfile)
  {
    logfile.println("--> LOG BOOTED");
    lastIntervalWrite = millis();
    logfile.flush();
  }
}

void loop()
{
  // put your main code here, to run repeatedly:
  if (Serial1.available())
  {
    Serial1.readBytesUntil('\n', buffer, sizeof(buffer));
    File logfile = SD.open(filename, FILE_WRITE);
    if (logfile)
    {
      logfile.print("> ");
      logfile.println(buffer);
      Serial1.println(F("Log file updated!"));
      logfile.flush();
      lightLed();
    }
    else
    { // for debugging
      Serial1.printf(PSTR("Failed to write: %s\n"), buffer);
    }
    memset(buffer, 0, sizeof(buffer));
  }
  if (millis() - lastIntervalWrite > 600e3)
  {
    File logfile = SD.open(filename, FILE_WRITE);
    if (logfile)
    {
      logfile.println("--> 10m PASSED");
      lastIntervalWrite = millis();
      logfile.flush();
      lightLed();
    }
  }
  if (lastLedOn && millis() - lastLedOn > 1e3)
  {
    digitalWrite(GREEN_LED, LOW);
    lastLedOn = 0;
  }
}

/**
 * detects presence of sd card before init
 */
boolean startSDCard()
{
  // Wait until the card is inserted:
  while (digitalRead(cardDetect) == LOW)
  {
    Serial1.println(F("Waiting for card..."));
    delay(2000);
  }

  // wait until the card initialized successfully:
  while (!SD.begin(chipSelect))
  {
    Serial1.println(F("Card failed"));
    delay(2000);
  }
  return true;
}

void lightLed()
{
  digitalWrite(GREEN_LED, HIGH);
  lastLedOn = millis();
}