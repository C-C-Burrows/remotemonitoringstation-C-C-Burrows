#include <Arduino.h>
#line 1 "c:\\Users\\CharlotteBurrows\\OneDrive - Friends and Family Dogfood ozburrows.com\\Documents\\GitHub\\remotemonitoringstation-C-C-Burrows\\RMS\\RMS.ino"
#include "sensitiveInformation.h"

#define FORMAT_SPIFFS_IF_FAILED true

// Wifi & Webserver
#include "WiFi.h"
#include "SPIFFS.h"
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);

// Temperature Includes and setup
#include <Wire.h>
#include "Adafruit_ADT7410.h"
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include "Adafruit_miniTFTWing.h"

Adafruit_miniTFTWing ss;
#define TFT_RST -1 // we use the seesaw for resetting to save a pin
#define TFT_CS 14
#define TFT_DC 32

// Create the ADT7410 temperature sensor object
Adafruit_ADT7410 tempsensor = Adafruit_ADT7410();

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// Motor Shield START
#include <Adafruit_MotorShield.h>
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *myMotor = AFMS.getMotor(3);
// Motor Shield END

// ESP32Servo Start
#include <ESP32Servo.h>
Servo myservo; // create servo object to control a servo
int servoPin = 12;
boolean blindsOpen = false;
// ESP32Servo End

// RTC Start - Remove if unnecessary
#include "RTClib.h"

// safe security subsytem

#define LEDRed 27
#define LEDGreen 33
// RFID Start

#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 21  // ES32 Feather
#define RST_PIN 17 // esp32 Feather - SCL pin. Could be others.

MFRC522 rfid(SS_PIN, RST_PIN);
bool safeLocked = true;

// RFID End

RTC_PCF8523 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// RTC End

boolean LEDOn = false; // State of Built-in LED true=on, false=off.
#define LOOPDELAY 100

#line 71 "c:\\Users\\CharlotteBurrows\\OneDrive - Friends and Family Dogfood ozburrows.com\\Documents\\GitHub\\remotemonitoringstation-C-C-Burrows\\RMS\\RMS.ino"
void setup();
#line 81 "c:\\Users\\CharlotteBurrows\\OneDrive - Friends and Family Dogfood ozburrows.com\\Documents\\GitHub\\remotemonitoringstation-C-C-Burrows\\RMS\\RMS.ino"
void loop();
#line 92 "c:\\Users\\CharlotteBurrows\\OneDrive - Friends and Family Dogfood ozburrows.com\\Documents\\GitHub\\remotemonitoringstation-C-C-Burrows\\RMS\\RMS.ino"
void builtinLED();
#line 110 "c:\\Users\\CharlotteBurrows\\OneDrive - Friends and Family Dogfood ozburrows.com\\Documents\\GitHub\\remotemonitoringstation-C-C-Burrows\\RMS\\RMS.ino"
void debugPrint(String debugString);
#line 121 "c:\\Users\\CharlotteBurrows\\OneDrive - Friends and Family Dogfood ozburrows.com\\Documents\\GitHub\\remotemonitoringstation-C-C-Burrows\\RMS\\RMS.ino"
void debugPrint(String debugString, bool newline);
#line 137 "c:\\Users\\CharlotteBurrows\\OneDrive - Friends and Family Dogfood ozburrows.com\\Documents\\GitHub\\remotemonitoringstation-C-C-Burrows\\RMS\\RMS.ino"
void logEvent(String dataToLog);
#line 163 "c:\\Users\\CharlotteBurrows\\OneDrive - Friends and Family Dogfood ozburrows.com\\Documents\\GitHub\\remotemonitoringstation-C-C-Burrows\\RMS\\RMS.ino"
void readAndDisplayTemperature();
#line 178 "c:\\Users\\CharlotteBurrows\\OneDrive - Friends and Family Dogfood ozburrows.com\\Documents\\GitHub\\remotemonitoringstation-C-C-Burrows\\RMS\\RMS.ino"
void tftDrawText(String text, uint16_t color);
#line 198 "c:\\Users\\CharlotteBurrows\\OneDrive - Friends and Family Dogfood ozburrows.com\\Documents\\GitHub\\remotemonitoringstation-C-C-Burrows\\RMS\\RMS.ino"
void automaticFan(float temperatureThreshold);
#line 221 "c:\\Users\\CharlotteBurrows\\OneDrive - Friends and Family Dogfood ozburrows.com\\Documents\\GitHub\\remotemonitoringstation-C-C-Burrows\\RMS\\RMS.ino"
void windowBlinds();
#line 254 "c:\\Users\\CharlotteBurrows\\OneDrive - Friends and Family Dogfood ozburrows.com\\Documents\\GitHub\\remotemonitoringstation-C-C-Burrows\\RMS\\RMS.ino"
void readRFID();
#line 290 "c:\\Users\\CharlotteBurrows\\OneDrive - Friends and Family Dogfood ozburrows.com\\Documents\\GitHub\\remotemonitoringstation-C-C-Burrows\\RMS\\RMS.ino"
void temperatureSetup();
#line 332 "c:\\Users\\CharlotteBurrows\\OneDrive - Friends and Family Dogfood ozburrows.com\\Documents\\GitHub\\remotemonitoringstation-C-C-Burrows\\RMS\\RMS.ino"
void spiffWifiSetup();
#line 384 "c:\\Users\\CharlotteBurrows\\OneDrive - Friends and Family Dogfood ozburrows.com\\Documents\\GitHub\\remotemonitoringstation-C-C-Burrows\\RMS\\RMS.ino"
void motorSetup();
#line 395 "c:\\Users\\CharlotteBurrows\\OneDrive - Friends and Family Dogfood ozburrows.com\\Documents\\GitHub\\remotemonitoringstation-C-C-Burrows\\RMS\\RMS.ino"
void windowBlindSetup();
#line 410 "c:\\Users\\CharlotteBurrows\\OneDrive - Friends and Family Dogfood ozburrows.com\\Documents\\GitHub\\remotemonitoringstation-C-C-Burrows\\RMS\\RMS.ino"
void safeSubSytem();
#line 2 "c:\\Users\\CharlotteBurrows\\OneDrive - Friends and Family Dogfood ozburrows.com\\Documents\\GitHub\\remotemonitoringstation-C-C-Burrows\\RMS\\spiffsFunctionality.ino"
void readFile(fs::FS &fs, const char * path);
#line 18 "c:\\Users\\CharlotteBurrows\\OneDrive - Friends and Family Dogfood ozburrows.com\\Documents\\GitHub\\remotemonitoringstation-C-C-Burrows\\RMS\\spiffsFunctionality.ino"
void writeFile(fs::FS &fs, const char * path, const char * message);
#line 35 "c:\\Users\\CharlotteBurrows\\OneDrive - Friends and Family Dogfood ozburrows.com\\Documents\\GitHub\\remotemonitoringstation-C-C-Burrows\\RMS\\spiffsFunctionality.ino"
void appendFile(fs::FS &fs, const char * path, const char * message);
#line 52 "c:\\Users\\CharlotteBurrows\\OneDrive - Friends and Family Dogfood ozburrows.com\\Documents\\GitHub\\remotemonitoringstation-C-C-Burrows\\RMS\\spiffsFunctionality.ino"
void renameFile(fs::FS &fs, const char * path1, const char * path2);
#line 62 "c:\\Users\\CharlotteBurrows\\OneDrive - Friends and Family Dogfood ozburrows.com\\Documents\\GitHub\\remotemonitoringstation-C-C-Burrows\\RMS\\spiffsFunctionality.ino"
void deleteFile(fs::FS &fs, const char * path);
#line 1 "c:\\Users\\CharlotteBurrows\\OneDrive - Friends and Family Dogfood ozburrows.com\\Documents\\GitHub\\remotemonitoringstation-C-C-Burrows\\RMS\\websiteFunctionality.ino"
void routesConfiguration();
#line 60 "c:\\Users\\CharlotteBurrows\\OneDrive - Friends and Family Dogfood ozburrows.com\\Documents\\GitHub\\remotemonitoringstation-C-C-Burrows\\RMS\\websiteFunctionality.ino"
String getDateTime();
#line 67 "c:\\Users\\CharlotteBurrows\\OneDrive - Friends and Family Dogfood ozburrows.com\\Documents\\GitHub\\remotemonitoringstation-C-C-Burrows\\RMS\\websiteFunctionality.ino"
String processor(const String& var);
#line 71 "c:\\Users\\CharlotteBurrows\\OneDrive - Friends and Family Dogfood ozburrows.com\\Documents\\GitHub\\remotemonitoringstation-C-C-Burrows\\RMS\\RMS.ino"
void setup()
{

  temperatureSetup();
  spiffWifiSetup();
  motorSetup();
  windowBlindSetup();
  safeSubSytem();
}

void loop()
{
  builtinLED();
  delay(LOOPDELAY); // To allow time to publish new code.
  readAndDisplayTemperature();
  automaticFan(10.0);
  windowBlinds();
  readRFID();
  logEvent("We are here!");
}

void builtinLED()
{
  /*
      this is the LED it function is to turn on the LED when LEDon is True
      and off if it is faulse.
      @return: void
   */

  if (LEDOn)
  {
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else
  {
    digitalWrite(LED_BUILTIN, LOW);
  }
}

void debugPrint(String debugString)
{
  /*
     this is debugprint it is used to print debug imfomation.
     @return: void
     @param debugString - String is to print debug
  */

  debugPrint(debugString, true);
}

void debugPrint(String debugString, bool newline)
{

  /*
     this is debugprint it is used to print debug imfomation.
     @return: void
     @param debugString - String is to print debug
     @param newline - bool if true print new line else not
  */

  Serial.print(debugString);
  if (newline)
  {
    Serial.println();
  }
}
void logEvent(String dataToLog)
{
  /*
     Log entries to a file stored in SPIFFS partition on the ESP32.
  */
  // Get the updated/current time
  DateTime rightNow = rtc.now();
  char csvReadableDate[25];
  sprintf(csvReadableDate, "%02d,%02d,%02d,%02d,%02d,%02d,", rightNow.year(), rightNow.month(), rightNow.day(), rightNow.hour(), rightNow.minute(), rightNow.second());

  String logTemp = csvReadableDate + dataToLog + "\n"; // Add the data to log onto the end of the date/time

  const char *logEntry = logTemp.c_str(); // convert the logtemp to a char * variable

  // Add the log entry to the end of logevents.csv
  appendFile(SPIFFS, "/logEvents.csv", logEntry);

  // Output the logEvents - FOR DEBUG ONLY. Comment out to avoid spamming the serial monitor.
  //  readFile(SPIFFS, "/logEvents.csv");

  Serial.print("\nEvent Logged: ");
  Serial.println(logEntry);
}

// Temp Code

void readAndDisplayTemperature()
{

  /*
    Read and print out the temperature, then convert to *F
     @return: void
  */
  float c = tempsensor.readTempC();
  float f = c * 9.0 / 5.0 + 32;
  debugPrint("Temp: " + (String)c + "*C \t" + (String)f + "*F");
  String tempInC = String(c);
  tftDrawText(tempInC, ST77XX_WHITE);
  delay(100);
}

void tftDrawText(String text, uint16_t color)
{

  /*
    tftDrawingText if retures true then it prints the test if
    it returns faules theb dose not print
     @return: void
     @param text
     @param color
  */
  debugPrint("Start DrawText");
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 0);
  tft.setTextSize(3);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.print(text);
  debugPrint("End DrawText");
}

void automaticFan(float temperatureThreshold)
{

  /*
    automaticFan is use to read the tempretue if it is higher then c
    then it is forward if retuned less then c it reutns stop
     @return: void
     @param temperatureThreshold
  */
  float c = tempsensor.readTempC();
  myMotor->setSpeed(100);
  if (c < temperatureThreshold)
  {
    myMotor->run(RELEASE);
    Serial.println("stop");
  }
  else
  {
    myMotor->run(FORWARD);
    Serial.println("forward");
  }
}

void windowBlinds()
{

  /*
    when the button is pressed it  opens the blinds when pressed again it
    closes the blinds
     @return: void
  */
  uint32_t buttons = ss.readButtons();
  debugPrint("blinds " + buttons);
  if (!(buttons & TFTWING_BUTTON_A))
  {
    debugPrint("blinds button");
    if (blindsOpen)
    {
      logEvent("Closing Blinds");
      debugPrint("blinds Closed");
      myservo.write(0);
    }
    else
    {
      myservo.write(180);
      debugPrint("blinds Openned");
      logEvent("Opening Blinds");
    }
    blindsOpen = !blindsOpen;
  }
  else
  {
    debugPrint("buttons " + buttons);
  }
}

void readRFID()
{

  String uidOfCardRead = "";
  String validCardUID = "00 232 81 25";

  if (rfid.PICC_IsNewCardPresent())
  { // new tag is available
    if (rfid.PICC_ReadCardSerial())
    { // NUID has been readed
      MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
      for (int i = 0; i < rfid.uid.size; i++)
      {
        uidOfCardRead += rfid.uid.uidByte[i] < 0x10 ? " 0" : " ";
        uidOfCardRead += rfid.uid.uidByte[i];
      }
      Serial.println(uidOfCardRead);

      rfid.PICC_HaltA();      // halt PICC
      rfid.PCD_StopCrypto1(); // stop encryption on PCD
      uidOfCardRead.trim();
      if (uidOfCardRead == validCardUID)
      {
        safeLocked = false;
        logEvent("Safe Unlocked");
      }
      else
      {
        safeLocked = true;
        logEvent("Safe Locked");
      }
    }
  }
}
// All Sensor Setup Code

void temperatureSetup()
{

  /*
    this is used to read the ss. and is desplayed on the screen
     @return: void
  */
  Serial.begin(9600);

  if (!ss.begin())
  {
    debugPrint("seesaw init error!");
    while (1)
      ;
  }
  else
    debugPrint("seesaw started");

  ss.tftReset();
  ss.setBacklight(0x0); // set the backlight fully on

  // Use this initializer (uncomment) if you're using a 0.96" 180x60 TFT
  tft.initR(INITR_MINI160x80); // initialize a ST7735S chip, mini display

  tft.setRotation(3);
  tft.fillScreen(ST77XX_BLACK);

  debugPrint("ADT7410 demo");

  // Make sure the sensor is found, you can also pass in a different i2c
  // address with tempsensor.begin(0x49) for example
  if (!tempsensor.begin())
  {
    debugPrint("Couldn't find ADT7410!");
    while (1)
      ;
  }

  // sensor takes 250 ms to get first readings
  delay(250);
}

void spiffWifiSetup()
{

  /*
    this is a spiffWifiSetup
     @return: void
  */
  Serial.begin(9600);
  while (!Serial)
  {
    delay(10);
  }
  delay(1000);

  if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED))
  {
    // Follow instructions in README and install
    // https://github.com/me-no-dev/arduino-esp32fs-plugin
    debugPrint("SPIFFS Mount Failed");
    return;
  }

  // Wifi Configuration
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    debugPrint("Connecting to WiFi..");
  }
  debugPrint("");
  debugPrint("Connected to the Internet");
  debugPrint("IP address: ");
  Serial.println(WiFi.localIP());

  routesConfiguration(); // Reads routes from routesManagementln
  server.begin();

  // RTC
  if (!rtc.begin())
  {
    debugPrint("Couldn't find RTC");
    Serial.flush();
    //    abort();
  }

  // The following line can be uncommented if the time needs to be reset.
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  rtc.start();
  pinMode(LED_BUILTIN, OUTPUT);
}

// Motor Shield Start
void motorSetup()
{
  /*
    this is the setup code for the motor Shield
     @return: void
  */
  AFMS.begin(); // Motor Shield Start
}

// window Blind Control subsytem

void windowBlindSetup()
{

  /*
  this is used to set up the windowblinds
     @return: void
  */
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(50);           // standard 50 hz servo
  myservo.attach(servoPin, 1000, 2000); // attaches the servo on pin 18 to the servo object
}

void safeSubSytem()
{
  // RFID Start
  SPI.begin();     // init SPI bus
  rfid.PCD_Init(); // init MFRC522
  // RFID End
  pinMode(LEDRed, OUTPUT);
  pinMode(LEDGreen, OUTPUT);
  digitalWrite(LEDRed, LOW);
  digitalWrite(LEDGreen, LOW);
}
#line 1 "c:\\Users\\CharlotteBurrows\\OneDrive - Friends and Family Dogfood ozburrows.com\\Documents\\GitHub\\remotemonitoringstation-C-C-Burrows\\RMS\\spiffsFunctionality.ino"
//SPIFFS File Functions
void readFile(fs::FS &fs, const char * path) {
  Serial.printf("Reading file: %s\r\n", path);

  File file = fs.open(path);
  if (!file || file.isDirectory()) {
    Serial.println("- failed to open file for reading");
    return;
  }

  //  Serial.println("- read from file:");
  while (file.available()) {
    Serial.write(file.read());
  }
  file.close();
}

void writeFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Writing file: %s\r\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("- failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    delay(1);
    //    Serial.println("- file written");
  } else {
    Serial.println("- write failed");
  }
  file.close();
}

void appendFile(fs::FS &fs, const char * path, const char * message) {
  //  Serial.printf("Appending to file: %s\r\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file) {
    Serial.println("- failed to open file for appending");
    return;
  }
  if (file.print(message)) {
    delay(1);
    //Serial.println("- message appended");
  } else {
    Serial.println("- append failed");
  }
  file.close();
}

void renameFile(fs::FS &fs, const char * path1, const char * path2) {
  Serial.printf("Renaming file %s to %s\r\n", path1, path2);
  if (fs.rename(path1, path2)) {
    delay(1);
    //Serial.println("- file renamed");
  } else {
    Serial.println("- rename failed");
  }
}

void deleteFile(fs::FS &fs, const char * path) {
  Serial.printf("Deleting file: %s\r\n", path);
  if (fs.remove(path)) {
    Serial.println("- file deleted");
  } else {
    Serial.println("- delete failed");
  }
}

#line 1 "c:\\Users\\CharlotteBurrows\\OneDrive - Friends and Family Dogfood ozburrows.com\\Documents\\GitHub\\remotemonitoringstation-C-C-Burrows\\RMS\\websiteFunctionality.ino"
void routesConfiguration() {

  // Example of a 'standard' route
  // No Authentication
  server.on("/index.html", HTTP_GET, [](AsyncWebServerRequest * request) {
    logEvent("route: /");
    request->send(SPIFFS, "/index.html", "text/html");
  });

  // Duplicated serving of index.html route, so the IP can be entered directly to browser.
  // No Authentication
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    logEvent("route: /");
    request->send(SPIFFS, "/index.html", "text/html");
  });

  // Example of linking to an external file
  server.on("/arduino.css", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/arduino.css", "text/css");
  });


  // Example of a route with additional authentication (popup in browser)
  // And uses the processor function.
  server.on("/dashboard.html", HTTP_GET, [](AsyncWebServerRequest * request) {
    if (!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    logEvent("Dashboard");
    request->send(SPIFFS, "/dashboard.html", "text/html", false, processor);
  });


  // Example of route with authentication, and use of processor
  // Also demonstrates how to have arduino functionality included (turn LED on)
  server.on("/LEDOn", HTTP_GET, [](AsyncWebServerRequest * request) {
    if (!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    LEDOn = true;
    request->send(SPIFFS, "/dashboard.html", "text/html", false, processor);
  });


  server.on("/LEDOff", HTTP_GET, [](AsyncWebServerRequest * request) {
    if (!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    LEDOn = false;
    request->send(SPIFFS, "/dashboard.html", "text/html", false, processor);
  });


  // Example of route which sets file to download - 'true' in send() command.
  server.on("/logOutput", HTTP_GET, [](AsyncWebServerRequest * request) {
    if (!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    logEvent("Log Event Download");
    request->send(SPIFFS, "/logEvents.csv", "text/html", true);
  });
}

String getDateTime() {
  DateTime rightNow = rtc.now();
  char csvReadableDate[25];
  sprintf(csvReadableDate, "%02d:%02d:%02d %02d/%02d/%02d",  rightNow.hour(), rightNow.minute(), rightNow.second(), rightNow.day(), rightNow.month(), rightNow.year());
  return csvReadableDate;
}

String processor(const String& var) {
  /*
     Updates the HTML by replacing set variables with return value from here.
     For example:
     in HTML file include %VARIABLEVALUE%.
     In this function, have:
      if (var=="VARIABLEVALUE") { return "5";}
  */

  if (var == "DATETIME") {
    String datetime = getDateTime();
    return datetime;
  }

  // Default "catch" which will return nothing in case the HTML has no variable to replace.
  return String();
}

