#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

const uint8_t cardDetect{7};
const uint8_t chipSelect{4};

// last write to log file
uint32_t lastIntervalWrite{0};

// log file path
char *filename PROGMEM = "/serial.log";

// buffer to hold serial data
char buffer[512];

boolean startSDCard();

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial)
  {
    // wait till port is available
  }

  pinMode(cardDetect, INPUT_PULLUP);
  // see if the card is present and can be initialized:
  if (startSDCard() == true)
  {
    Serial.println(F("SD initialized!"));
  }

  File logfile = SD.open(filename, FILE_WRITE);
  if (!logfile)
  {
    logfile.println("--> LOG BOOTED");
    lastIntervalWrite = millis();
    logfile.flush();
  }
}

void loop()
{
  // put your main code here, to run repeatedly:
  if (Serial.available())
  {
    Serial.readBytesUntil('\n', buffer, sizeof(buffer));
    File logfile = SD.open(filename, FILE_WRITE);
    if (logfile)
    {
      logfile.print("> ");
      logfile.println(buffer);
      Serial.println(F("Log file updated!"));
      logfile.flush();
    }
    else
    { // for debugging
      Serial.printf(PSTR("Failed to write: %s\n"), buffer);
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
    }
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