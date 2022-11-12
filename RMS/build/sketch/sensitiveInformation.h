#line 1 "c:\\Users\\CharlotteBurrows\\OneDrive - Friends and Family Dogfood ozburrows.com\\Documents\\GitHub\\remotemonitoringstation-C-C-Burrows\\RMS\\sensitiveInformation.h"
/*
 * Contains any sensitive Infomration that you do not want published to Github.
 * 
 * The SSID and Password variables will need to be changed if you’re connecting to another Wireless Access Point (such as at home).
 * The `http_username` and `http_password` variables are used to authenticate when users are attempting to access secured pages.
 * 
 * Make sure this file is included in the .gitignore!
 * 
 */

const char* host            = "RMS";
const char* ssid            = "TheClan-Home"; //"RoboRange";           // Wifi Network Name
const char* password        = "BurrowsClanHome3";   //"Password01";        // Wifi Password


const char* usernameGuest   = "guest";
const char* passwordGuest   = "123";

const char* usernameAdmin  = "root";
const char* passwordAdmin   = "password";
