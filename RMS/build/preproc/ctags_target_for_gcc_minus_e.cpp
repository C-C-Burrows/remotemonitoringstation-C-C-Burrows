# 1 "c:\\Users\\CharlotteBurrows\\OneDrive - Friends and Family Dogfood ozburrows.com\\Documents\\GitHub\\remotemonitoringstation-C-C-Burrows\\RMS\\RMS.ino"
# 2 "c:\\Users\\CharlotteBurrows\\OneDrive - Friends and Family Dogfood ozburrows.com\\Documents\\GitHub\\remotemonitoringstation-C-C-Burrows\\RMS\\RMS.ino" 2



// Wifi & Webserver
# 7 "c:\\Users\\CharlotteBurrows\\OneDrive - Friends and Family Dogfood ozburrows.com\\Documents\\GitHub\\remotemonitoringstation-C-C-Burrows\\RMS\\RMS.ino" 2
# 8 "c:\\Users\\CharlotteBurrows\\OneDrive - Friends and Family Dogfood ozburrows.com\\Documents\\GitHub\\remotemonitoringstation-C-C-Burrows\\RMS\\RMS.ino" 2
# 9 "c:\\Users\\CharlotteBurrows\\OneDrive - Friends and Family Dogfood ozburrows.com\\Documents\\GitHub\\remotemonitoringstation-C-C-Burrows\\RMS\\RMS.ino" 2
# 10 "c:\\Users\\CharlotteBurrows\\OneDrive - Friends and Family Dogfood ozburrows.com\\Documents\\GitHub\\remotemonitoringstation-C-C-Burrows\\RMS\\RMS.ino" 2

AsyncWebServer server(80);

// Temperature Includes and setup
# 15 "c:\\Users\\CharlotteBurrows\\OneDrive - Friends and Family Dogfood ozburrows.com\\Documents\\GitHub\\remotemonitoringstation-C-C-Burrows\\RMS\\RMS.ino" 2
# 16 "c:\\Users\\CharlotteBurrows\\OneDrive - Friends and Family Dogfood ozburrows.com\\Documents\\GitHub\\remotemonitoringstation-C-C-Burrows\\RMS\\RMS.ino" 2
# 17 "c:\\Users\\CharlotteBurrows\\OneDrive - Friends and Family Dogfood ozburrows.com\\Documents\\GitHub\\remotemonitoringstation-C-C-Burrows\\RMS\\RMS.ino" 2
# 18 "c:\\Users\\CharlotteBurrows\\OneDrive - Friends and Family Dogfood ozburrows.com\\Documents\\GitHub\\remotemonitoringstation-C-C-Burrows\\RMS\\RMS.ino" 2
# 19 "c:\\Users\\CharlotteBurrows\\OneDrive - Friends and Family Dogfood ozburrows.com\\Documents\\GitHub\\remotemonitoringstation-C-C-Burrows\\RMS\\RMS.ino" 2

Adafruit_miniTFTWing ss;




// Create the ADT7410 temperature sensor object
Adafruit_ADT7410 tempsensor = Adafruit_ADT7410();

Adafruit_ST7735 tft = Adafruit_ST7735(14, 32, -1 /* we use the seesaw for resetting to save a pin*/);

// Motor Shield START
# 32 "c:\\Users\\CharlotteBurrows\\OneDrive - Friends and Family Dogfood ozburrows.com\\Documents\\GitHub\\remotemonitoringstation-C-C-Burrows\\RMS\\RMS.ino" 2
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *myMotor = AFMS.getMotor(4);
// Motor Shield END

// ESP32Servo Start
# 38 "c:\\Users\\CharlotteBurrows\\OneDrive - Friends and Family Dogfood ozburrows.com\\Documents\\GitHub\\remotemonitoringstation-C-C-Burrows\\RMS\\RMS.ino" 2
Servo myservo; // create servo object to control a servo
int servoPin = 12;
boolean blindsOpen = false;
// ESP32Servo End

// RTC Start - Remove if unnecessary
# 45 "c:\\Users\\CharlotteBurrows\\OneDrive - Friends and Family Dogfood ozburrows.com\\Documents\\GitHub\\remotemonitoringstation-C-C-Burrows\\RMS\\RMS.ino" 2

RTC_PCF8523 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// RTC End

boolean LEDOn = false; // State of Built-in LED true=on, false=off.


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
  delay(100); // To allow time to publish new code.
  readAndDisplayTemperature();
  automaticFan(20.0);
  windowBlinds();
  debugPrint("We are                  herrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrre!");
}

void builtinLED()
{
  if (LEDOn)
  {
    digitalWrite(LED_BUILTIN, 0x1);
  }
  else
  {
    digitalWrite(LED_BUILTIN, 0x0);
  }
}

void debugPrint(String debugString)
{
  debugPrint(debugString, true);
}

void debugPrint(String debugString, bool newline)
{
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
# 103 "c:\\Users\\CharlotteBurrows\\OneDrive - Friends and Family Dogfood ozburrows.com\\Documents\\GitHub\\remotemonitoringstation-C-C-Burrows\\RMS\\RMS.ino"
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
  // Read and print out the temperature, then convert to *F
  float c = tempsensor.readTempC();
  float f = c * 9.0 / 5.0 + 32;
  debugPrint("Temp: " + (String)c + "*C \t" + (String)f + "*F");
  String tempInC = String(c);
  tftDrawText(tempInC, 0xFFFF);
  delay(100);
}

void tftDrawText(String text, uint16_t color)
{
  debugPrint("Start DrawText");
  tft.fillScreen(0x0000);
  tft.setCursor(0, 0);
  tft.setTextSize(3);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.print(text);
  debugPrint("End DrawText");
}

void automaticFan(float temperatureThreshold)
{
  float c = tempsensor.readTempC();
  myMotor->setSpeed(100);
  if (c < temperatureThreshold)
  {
    myMotor->run(4);
    Serial.println("stop");
  }
  else
  {
    myMotor->run(1);
    Serial.println("forward");
  }
}

void windowBlinds()
{
  uint32_t buttons = ss.readButtons();
  debugPrint("blinds " + buttons);
  if (!(buttons & (1UL << 10)))
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
  tft.initR(0x04); // initialize a ST7735S chip, mini display

  tft.setRotation(3);
  tft.fillScreen(0x0000);

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
  Serial.begin(9600);
  while (!Serial)
  {
    delay(10);
  }
  delay(1000);

  if (!SPIFFS.begin(true))
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
  rtc.adjust(DateTime(((reinterpret_cast<const __FlashStringHelper *>(("Oct  8 2022")))), ((reinterpret_cast<const __FlashStringHelper *>(("14:42:11"))))));
  rtc.start();
  pinMode(LED_BUILTIN, 0x03);
}

// Motor Shield Start
void motorSetup()
{
  AFMS.begin(); // Motor Shield Start
}

// window Blind Control subsytem

void windowBlindSetup()
{
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(50); // standard 50 hz servo
  myservo.attach(servoPin, 1000, 2000); // attaches the servo on pin 18 to the servo object
}
# 1 "c:\\Users\\CharlotteBurrows\\OneDrive - Friends and Family Dogfood ozburrows.com\\Documents\\GitHub\\remotemonitoringstation-C-C-Burrows\\RMS\\spiffsFunctionality.ino"
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

  File file = fs.open(path, "w");
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

  File file = fs.open(path, "a");
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
# 1 "c:\\Users\\CharlotteBurrows\\OneDrive - Friends and Family Dogfood ozburrows.com\\Documents\\GitHub\\remotemonitoringstation-C-C-Burrows\\RMS\\websiteFunctionality.ino"
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
  sprintf(csvReadableDate, "%02d:%02d:%02d %02d/%02d/%02d", rightNow.hour(), rightNow.minute(), rightNow.second(), rightNow.day(), rightNow.month(), rightNow.year());
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
# 76 "c:\\Users\\CharlotteBurrows\\OneDrive - Friends and Family Dogfood ozburrows.com\\Documents\\GitHub\\remotemonitoringstation-C-C-Burrows\\RMS\\websiteFunctionality.ino"
  if (var == "DATETIME") {
    String datetime = getDateTime();
    return datetime;
  }

  // Default "catch" which will return nothing in case the HTML has no variable to replace.
  return String();
}
