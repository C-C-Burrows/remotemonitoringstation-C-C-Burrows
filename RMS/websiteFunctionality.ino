const char *PARAM_INPUT_1 = "tempThreshold";

void routesConfiguration()
{
  server.onNotFound([](AsyncWebServerRequest *request)
  { request->send(SPIFFS, "/404.html", "text/html"); });

  // Example of a 'standard' route
  // No Authentication
  server.on("/index.html", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    logEvent("route: /");
    request->send(SPIFFS, "/index.html", "text/html"); });

  // Duplicated serving of index.html route, so the IP can be entered directly to browser.
  // No Authentication
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    logEvent("route: /");
    request->send(SPIFFS, "/index.html", "text/html"); });

  // Example of linking to an external file
  server.on("/arduino.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/arduino.css", "text/css"); });

  // Example of linking to an external file
  server.on("/welcomepage.png", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/welcomepage.png", "image/png"); });

  // Example of a route with additional authentication (popup in browser)
  // And uses the processor function.
  server.on("/dashboard.html", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    if (!request->authenticate(usernameGuest, passwordGuest))
      return request->requestAuthentication();
    debugPrint("In Dashbord Click");
    logEvent("Dashboard");
    request->send(SPIFFS, "/dashboard.html", "text/html", false, processor); });

  // Example of a route with additional authentication (popup in browser)
  // And uses the processor function.
  server.on("/admin", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    if (!request->authenticate(usernameAdmin,passwordAdmin))
      return request->requestAuthentication();
    
    logEvent("Admin");
    request->send(SPIFFS, "/admin.html", "text/html", false, processor); });

  // Example of route with authentication, and use of processor
  // Also demonstrates how to have arduino functionality included (turn LED on)
  server.on("/LEDOn", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    if (!request->authenticate(usernameGuest, passwordGuest))
      return request->requestAuthentication();
    LEDOn = true;
    request->send(SPIFFS, "/dashboard.html", "text/html", false, processor); });

  server.on("/LEDOff", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    if (!request->authenticate(usernameGuest, passwordGuest))
      return request->requestAuthentication();
    LEDOn = false;
    request->send(SPIFFS, "/dashboard.html", "text/html", false, processor); });

  // Example of route which sets file to download - 'true' in send() command.
  server.on("/logOutput", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    if (!request->authenticate(usernameGuest, passwordGuest))
      return request->requestAuthentication();
    logEvent("Log Event Download");
    request->send(SPIFFS, "/logEvents.csv", "text/html", true); });

  server.on("/SafeLock", HTTP_GET, [](AsyncWebServerRequest *request)
            {
  if (!request->authenticate(usernameGuest, passwordGuest))
    return request->requestAuthentication();
    safeLocked = true;
  logEvent("Safe Locked via Website");
  request->send(SPIFFS, "/dashboard.html", "text/html", false, processor); });

  server.on("/SafeUnlock", HTTP_GET, [](AsyncWebServerRequest *request)
            {
  if (!request->authenticate(usernameGuest, passwordGuest))
    return request->requestAuthentication();
    safeLocked = false;
  logEvent("Safe Unlocked via Website");
  request->send(SPIFFS, "/dashboard.html", "text/html", false, processor); });

  server.on("/FanControl", HTTP_GET, [](AsyncWebServerRequest *request)
            {
  if (!request->authenticate(usernameGuest, passwordGuest))
    return request->requestAuthentication();
  automaticFanControl = !automaticFanControl;
  logEvent("Fan Manual Control: On");
  request->send(SPIFFS, "/dashboard.html", "text/html", false, processor); });

  server.on("/FanManualOn", HTTP_GET, [](AsyncWebServerRequest *request)
            {
  if (!request->authenticate(usernameGuest, passwordGuest))
    return request->requestAuthentication();
  fanEnabled = true;
  logEvent("Fan Manual Control: On");
  request->send(SPIFFS, "/dashboard.html", "text/html", false, processor); });

  server.on("/FanManualOff", HTTP_GET, [](AsyncWebServerRequest *request)
            {
  if (!request->authenticate(usernameGuest, passwordGuest))
    return request->requestAuthentication();
  fanEnabled = false;
  logEvent("Fan Manual Control: Off");
  request->send(SPIFFS, "/dashboard.html", "text/html", false, processor); });

  // Example of route with authentication, and use of processor
  // Also demonstrates how to have arduino functionality included (turn LED on)
  server.on("/LEDOnAdmin", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    if (!request->authenticate(usernameAdmin, passwordAdmin))
      return request->requestAuthentication();
    LEDOn = true;
    request->send(SPIFFS, "/admin.html", "text/html", false, processor); });

  server.on("/LEDOffAdmin", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    if (!request->authenticate(usernameAdmin, passwordAdmin))
      return request->requestAuthentication();
    LEDOn = false;
    request->send(SPIFFS, "/admin.html", "text/html", false, processor); });

  // Example of route which sets file to download - 'true' in send() command.
  server.on("/logOutputAdmin", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    if (!request->authenticate(usernameAdmin, passwordAdmin))
      return request->requestAuthentication();
    logEvent("Log Event Download Admin");
    request->send(SPIFFS, "/logEvents.csv", "text/html", true); });

  server.on("/SafeLockAdmin", HTTP_GET, [](AsyncWebServerRequest *request)
            {
  if (!request->authenticate(usernameAdmin, passwordAdmin))
    return request->requestAuthentication();
    safeLocked = true;
  logEvent("Safe Locked via Website - Admin");
  request->send(SPIFFS, "/admin.html", "text/html", false, processor); });

  server.on("/SafeUnlockAdmin", HTTP_GET, [](AsyncWebServerRequest *request)
            {
  if (!request->authenticate(usernameAdmin, passwordAdmin))
    return request->requestAuthentication();
    safeLocked = false;
  logEvent("Safe Unlocked via Website - Admin");
  request->send(SPIFFS, "/admin.html", "text/html", false, processor); });

  server.on("/FanControlAdmin", HTTP_GET, [](AsyncWebServerRequest *request)
            {
  if (!request->authenticate(usernameAdmin, passwordAdmin))
    return request->requestAuthentication();
  automaticFanControl = !automaticFanControl;
  logEvent("Fan Manual Control: On - Admin");
  request->send(SPIFFS, "/admin.html", "text/html", false, processor); });

  server.on("/FanManualOnAdmin", HTTP_GET, [](AsyncWebServerRequest *request)
            {
  if (!request->authenticate(usernameAdmin, passwordAdmin))
    return request->requestAuthentication();
  fanEnabled = true;
  logEvent("Fan Manual Control: On - Admin");
  request->send(SPIFFS, "/admin.html", "text/html", false, processor); });

  server.on("/FanManualOffAdmin", HTTP_GET, [](AsyncWebServerRequest *request)
            {
  if (!request->authenticate(usernameAdmin, passwordAdmin))
    return request->requestAuthentication();
  fanEnabled = false;
  logEvent("Fan Manual Control: Off - Admin");
  request->send(SPIFFS, "/admin.html", "text/html", false, processor); });

  server.onNotFound([](AsyncWebServerRequest *request)
                    {
    if (request->url().endsWith(F(".jpg")))
    {
      // Extract the filename that was attempted
      int fnsstart = request->url().lastIndexOf('/');
      String fn = request->url().substring(fnsstart);
      // Load the image from SPIFFS and send to the browser.
      request->send(SPIFFS, fn, "image/jpeg", true);
    }
    if (request->url().endsWith(F(".png")))
    {
      // Extract the filename that was attempted
      int fnsstart = request->url().lastIndexOf('/');
      String fn = request->url().substring(fnsstart);
      // Load the image from SPIFFS and send to the browser.
      request->send(SPIFFS, fn, "image/png", true);
    }
    else
    {
      request->send(SPIFFS, "/404.html");
    } });

  server.on("/setTemperatureThreshold", HTTP_GET, [](AsyncWebServerRequest *request)
            {
  int newThreshold; 
  if (request->hasParam(PARAM_INPUT_1)) {
    fanTemperatureThreshold = request->getParam(PARAM_INPUT_1)->value().toFloat();
    String logMessage = "Administrator set Automatic Fan Theshold to" + String(fanTemperatureThreshold);
    logEvent(logMessage);
  }
  request->send(SPIFFS, "/admin.html", "text/html", false, processor); });
}

String getDateTime()
{
  DateTime rightNow = rtc.now();
  char csvReadableDate[25];
  sprintf(csvReadableDate, "%02d:%02d:%02d %02d/%02d/%02d", rightNow.hour(), rightNow.minute(), rightNow.second(), rightNow.day(), rightNow.month(), rightNow.year());
  return csvReadableDate;
}

String processor(const String &var)
{
  /*
     Updates the HTML by replacing set variables with return value from here.
     For example:
     in HTML file include %VARIABLEVALUE%.
     In this function, have:
      if (var=="VARIABLEVALUE") { return "5";}
  */

  if (var == "DATETIME")
  {
    String datetime = getDateTime();
    return datetime;
  }

  // Default "catch" which will return nothing in case the HTML has no variable to replace.
  if (var == "TEMPERATURE")
  {
    float currentTemp = tempsensor.readTempC();
    int currentTempWholeNubmber = currentTemp;
    String currentTempString = String(currentTempWholeNubmber) + "??C.";
    return String(currentTempString); // currentTempString;
  }

  // Default "catch" which will return nothing in case the HTML has no variable to replace.
  if (var == "SETTEMP")
  {

    String currentTempString = String(fanTemperatureThreshold) + "??C.";
    return String(currentTempString); // currentTempString;
  }
  if (var == "TEMPTHRES")
  {

    String currentTempString = String(fanTemperatureThreshold);
    return String(currentTempString); // currentTempString;
  }

  if (var == "SAFESTATUS")
  {
    if (safeLocked)
    {
      return "Safe LOCKED";
    }
    else
    {
      return "Sade UNLOCKED";
    }
  }

  if (var == "FANCONTROL")
  {
    if (automaticFanControl)
    {
      return "Automatic";
    }
    else
    {
      return "Manual";
    }
  }
}
