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

RTC_PCF8523 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// RTC End

boolean LEDOn = false; // State of Built-in LED true=on, false=off.
#define LOOPDELAY 100

void setup()
{

  temperatureSetup();
  spiffWifiSetup();
  motorSetup();
  windowBlindSetup();
}

void loop()
{
  builtinLED();
  delay(LOOPDELAY); // To allow time to publish new code.
  readAndDisplayTemperature();
  automaticFan(10.0);
  windowBlinds();
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
      debugPrint("blinds Open");
      myservo.write(0);
    }
    else
    {
      myservo.write(180);
      debugPrint("blinds Closed");
    }
    blindsOpen = !blindsOpen;
  }
  else
  {
    debugPrint("buttons " + buttons);
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
