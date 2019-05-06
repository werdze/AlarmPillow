#include <Wire.h>

#define MAINPAGE 0
#define SETTINGSPAGE 1
#define ALARMPAGE 2
#define SETTIMEPAGE 3

#define LCDDATETIME 0
#define ALARMSTATUSES 1
#define A1LINE 2
#define A2LINE 3
#define A3LINE 4
#define SNOOZE 5
#define OFF 6
#define SETTINGS 7

#define SETTINGSSETTIME 0
#define SETTINGSSETDAYLIGHT 1
#define SETTINGSSETPATTERNS 2
#define SETTINGSBACK 3

#define SETTIMETIMELINE 0
#define SETTIMECONFIRM 1
#define SETTIMECANCEL 2

#define SETA1TIME 0
#define SETA1ACTIVESTATUS 1
#define SETA1REPEATSTATUS 2
#define SETA1CONFIRM 3
#define SETA1CANCEL 4

#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

long long prevMillis;
String currDate;
String currTime;
String currDateShortened;
String currTimeShortened;
String prevCurrTimeShortened;
bool hourOrMinChanged = false;
int secondsVibrationMotorsActive = 0;
int maxVibrationDuration = 10;

int8_t myAlarmTimes[6] = {-1, -1, -1, -1, -1, -1}; // hour, then minute, for 3 alarms
int maxlineSelectionForPage[4];

int8_t currHourFirstDigit = 0;
int8_t currHourSecondDigit = 0;
int8_t currMinFirstDigit = 0;
int8_t currMinSecondDigit = 0;

int currPageIdx = MAINPAGE;
int currLineSelected = 0;
int currLinesShowing[2] = {0, 1};
int currColumnSelected = 0;

String* currPageArr[4];
String mainPage[8];
String settingsPage[4];
String alarmPage[5];
String setTimePage[3];

bool alarm1Active = false;
bool alarm2Active = false;
bool alarm3Active = false;
bool alarm1Repeat = false;
bool alarm2Repeat = false;
bool alarm3Repeat = false;

void setup() {
  Serial.begin(115200);
  powerToLCDAndJoystickAndVibrationMotorsSetup();
  JoystickSetup();
  RTCsetup();
  setCurrTime(23, 59, 50, 12, 12, 2019);  // H, M, S, Month, Day, Year
  RTCloop();
//  setAlarm(1, -1, -1);  // alarm #, H, M
  getAlarmsAndUpdateMyAlarmsArr();
  delay(100);
  alarm1Active = true;    // just for testing purposes
  LCDsetup();
  prevMillis = millis();
  
  maxlineSelectionForPage[MAINPAGE] = sizeof(mainPage) / sizeof(String) - 1;
  maxlineSelectionForPage[SETTINGSPAGE] = sizeof(settingsPage) / sizeof(String) - 1;
  maxlineSelectionForPage[ALARMPAGE] = sizeof(alarmPage) / sizeof(String) - 1;
  maxlineSelectionForPage[SETTIMEPAGE] = sizeof(setTimePage) / sizeof(String) - 1;

  currPageArr[MAINPAGE] = mainPage;
  currPageArr[SETTINGSPAGE] = settingsPage;
  currPageArr[ALARMPAGE] = alarmPage;
  currPageArr[SETTIMEPAGE] = setTimePage;

  Serial.println("myAlarmTimes[0 and 1] Here:");
  Serial.println(myAlarmTimes[0]);
  Serial.println(myAlarmTimes[1]);

  delay(1000);  // allow enough time to read from memory
}

void loop() {
  // Get data from the DS3231
//  lcd.cursor();
  if(millis() - prevMillis > 1000){
    secondsVibrationMotorsActive++;
    Serial.println("secondsVibrationMotorsActive: " + String(secondsVibrationMotorsActive));
    prevCurrTimeShortened = currTimeShortened;
    getTimeStrings(currDate, currTime, currDateShortened, currTimeShortened);
//    Serial.println(currTime);
//    Serial.println(currTime.substring(0,2));
//    Serial.println(currTime.substring(3,5));
//    Serial.println(currDate + " " + currTime);
//    Serial.println(currDateShortened + " " + currTimeShortened);
    LcdSetDateTime();
    prevMillis = millis();
//    LcdSetAlarmLines();
    if(currPageIdx == MAINPAGE && currLinesShowing[0] == 0 && hourOrMinChanged){
      LcdClearLines(0);
      LcdDisplayCurrLines();
      hourOrMinChanged = false;
    }
  }
  JoystickLoop();
  lcd.setCursor(currColumnSelected, 0);
  if(currPageIdx == ALARMPAGE && currLineSelected == 0 && currColumnSelected != 0 && currColumnSelected != 3){
    lcd.blink();
  }
  else{
    lcd.noBlink();
  }
  int currHour = getCurrHourFromTime(currTime);
  int currMin = getCurrMinFromTime(currTime);
//  Serial.println(currHour);
//  Serial.println("My alarm 1 time");
//  Serial.println(myAlarmTimes[0]);
//  Serial.println(myAlarmTimes[1]);
  if(alarm1Active && myAlarmTimes[0] == currHour && myAlarmTimes[1] == currMin){
    secondsVibrationMotorsActive = 0;
    //activate alarm for the day
    Serial.println("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");
    turnOnVibrations();
    alarm1Active = false;
    LcdSetAlarmLines();         // TODO Fix this later so that the S after A1 clears out, unless it is on Repeat
    LcdClearLines(0);
    LcdDisplayCurrLines();
  }
  if(secondsVibrationMotorsActive == maxVibrationDuration){
    Serial.println("Turning Vibrations Off");
    turnOffVibrations();
  }
  
}

void powerToLCDAndJoystickAndVibrationMotorsSetup() {
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(12, OUTPUT);    // sets the digital pin 12 as output
  pinMode(13, OUTPUT);
  digitalWrite(12, HIGH); // sets the digital pin 12 on
  digitalWrite(13, HIGH);
}
