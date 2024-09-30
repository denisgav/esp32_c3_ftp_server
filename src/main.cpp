#include <SD.h>
#include <SPIFFS.h>
#include <WiFi.h>

#include "ESP-FTP-Server-Lib.h"
#include "FTPFilesystem.h"

#include "credentials.h" // Contains defines: WIFI_SSID, WIFI_PASSWORD, FTP_USER, FTP_PASSWORD. 

//#define PIN_SPI_CLK 4
//#define PIN_SPI_MOSI 6
//#define PIN_SPI_MISO 5
#define PIN_SPI_CS 7

FTPServer ftp;

void setup() {
  Serial.begin(115520);
  Serial.println(); // only for mbed 2.9.x
  Serial.println("Power ON \n ");  // Let's BEGIN!!
  Serial.println("Test program compiled on " __DATE__ " at " __TIME__);
  Serial.println();
  Serial.println("Processor came out of reset.");
  Serial.println();

  pinMode(PIN_SPI_CS, OUTPUT);
  digitalWrite(PIN_SPI_CS, HIGH); // <-- Set CS pin HIGH to deselect

  Serial.println("--------------");
  Serial.println("PIN_SPI_CS = " + String(PIN_SPI_CS));
  Serial.println("MOSI = " + String(MOSI)); // master out, slave in
  Serial.println("MISO = " + String(MISO)); // master in, slave out
  Serial.println("SCK = " + String(SCK));  // clock
  Serial.println("--------------");

  //SPIFFS.begin(true);
  // SPI.begin(14, 2, 15);
  // if (!SD.begin(13)) {
  //   Serial.println("SD Card Mount Failed");
  // }

  if(!SD.begin(PIN_SPI_CS)){
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();

  if(cardType == CARD_NONE){
    Serial.println("No SD card attached");
    return;
  }

  Serial.print("SD Card Type: ");
  if(cardType == CARD_MMC){
    Serial.println("MMC");
  } else if(cardType == CARD_SD){
    Serial.println("SDSC");
  } else if(cardType == CARD_SDHC){
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }

  uint64_t cardSize = SD.cardSize();
  Serial.printf("SD Card Size: %llu B\n", cardSize);

  Serial.printf("Total space: %llu B\n", SD.totalBytes());
  Serial.printf("Used space: %llu B\n", SD.usedBytes());

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  ftp.addUser(FTP_USER, FTP_PASSWORD);
  ftp.addFilesystem("SD", &SD);
  //ftp.addFilesystem("SPIFFS", &SPIFFS);

  ftp.begin();

  Serial.println("...---'''---...---'''---...---'''---...");
}

void loop() {
  ftp.handle();
}
