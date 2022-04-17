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
char buffer[513];

void lightLed();
boolean startSDCard();

// File on SD card that is open for writing
File logfile;

// Runs once at start
void setup()
{
  // Hardware serial (pins 0 / 1) - regular Serial only goes to USB
  Serial1.begin(115200);
  // For debugging only
  Serial.begin(115200);
  // while (!Serial);
  // {
  //   // wait till USB port is available - do not use for UART logging
  // }

  pinMode(GREEN_LED, OUTPUT); // Otherwise, LED shines only dimly

  pinMode(cardDetect, INPUT_PULLUP);
  // see if the card is present and can be initialized:
  if (startSDCard() == true)
  {
    Serial.println(F("SD initialized!"));
    lightLed();
  }

  File logfile = SD.open(filename, FILE_WRITE);
  if (logfile)
  {
    logfile.println("> LOG BOOTED");
    lastIntervalWrite = millis();
    logfile.flush();
  }
}

void loop()
{
  // put your main code here, to run repeatedly:
  if (Serial1.available())
  {
    Serial1.readBytesUntil('\n', buffer, sizeof(buffer) - 1);
    if (!logfile.print(buffer))
    {
      Serial.printf(PSTR("Failed to write: ")); // for debugging
    }
    Serial.print(buffer);
    lightLed();
    memset(buffer, 0, sizeof(buffer));
  }
  else if (millis() - lastIntervalWrite > 600e3)
  {
    Serial.println("> 10m PASSED");
    logfile.close(); // Periodically try reopening in case card was removed and put back in.
    logfile = SD.open(filename, FILE_WRITE);
    if (logfile)
    {
      logfile.println("> 10m PASSED");
      lastIntervalWrite = millis();
      lightLed();
    }
  }
  else if (lastLedOn && millis() - lastLedOn > 1e3)
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
    Serial.println(F("Waiting for card..."));
    delay(2000);
  }

  // wait until the card initialized successfully:
  while (!SD.begin(chipSelect))
  {
    Serial.println(F("Card failed"));
    delay(2000);
  }
  return true;
}

void lightLed()
{
  digitalWrite(GREEN_LED, HIGH);
  lastLedOn = millis();
}