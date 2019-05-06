// Used LCD library by YWROBOT
// Compatible with the Arduino IDE 1.0
// Library version:1.1
//#include <LiquidCrystal_I2C.h>

// SCL to A5 on the Arduino.
// SDA to A4 on the Arduino.

// LCD has 15 char display

//LiquidCrystal_I2C lcd(0x3F,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

#define ALARM1HOUR 0    // TODO Use these more below
#define ALARM1MIN 1
#define ALARM2HOUR 2
#define ALARM2MIN 3
#define ALARM3HOUR 4
#define ALARM3MIN 5

void LCDsetup() {
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  
  LcdMainSetOtherInfo();
  LcdSetAlarmLines();
  
  LcdSettingsSetTimeLine();
  LcdSettingsDaylightLine();
  LcdSettingsAlarmPatternsLine();
  LcdSettingsAlarmPatternsLine();
  LcdSettingsBackLine();

  LcdSetTimeLine();
  LcdSetTimeOtherInfo();

  LcdSetA1TimeLine();
  LcdA1OtherInfo();
  LcdSetA1ActiveAndRepeatStatus();
}

void LCDloop() {
//  lcd.setCursor(0,0);
//  lcd.print(currDateShortened + " " + currTimeShortened);
//  lcd.setCursor(0,0);
//  lcd.clear();
//  lcd.print(currTime);
}

void LcdClearLines(int where) {   // for testing purposes - replace with lcd.clear() later
  lcd.clear();
//  Serial.println("Cleared Lines" + String(where));
}

void LcdDisplayCurrLines() {
//  updateTimeAndAlarmLines();
//  if(currAlarmTimesChanged()){
    
//  }
  lcd.setCursor(0,0); // column, then line
  if(currLineSelected == currLinesShowing[0]){  // if we should show the * on the first line vs the second line of the screen
    lcd.print('*' + currPageArr[currPageIdx][currLinesShowing[0]]);
    lcd.setCursor(0,1);
    lcd.print(currPageArr[currPageIdx][currLinesShowing[1]]);
  }
  else{
    lcd.print(currPageArr[currPageIdx][currLinesShowing[0]]);
    lcd.setCursor(0,1);
    lcd.print('*' + currPageArr[currPageIdx][currLinesShowing[1]]);
  }
  lcd.setCursor(currColumnSelected, 0);
//  Serial.println("Called LcdDisplayCurrLines");
//  Serial.println(currColumnSelected);
}

bool currAlarmTimesChanged() {
  int8_t currHour = currHourFirstDigit * 10 + currHourSecondDigit;
  int8_t currMin = currMinFirstDigit * 10 + currMinSecondDigit;
  if(myAlarmTimes[0] != currHour || myAlarmTimes[1] != currMin){
    return true;
  }
  return false;
}

//void updateTimeAndAlarmLines() {
//  LcdSetA1TimeLine();
//}

//void LcdRefreshSetTime(char c) {
//  lcd.setCursor(currColumnSelected, 0);
//  lcd.print(c);
//}

void LcdSetDateTime() {
  mainPage[LCDDATETIME] = currDateShortened + " " + currTimeShortened;
}

void LcdSetAlarmLines() {
//  alarm1Active = true;
//  alarm2Active = true;
//  alarm1Repeat = true;
//  alarm2Repeat = false;

  LcdSetA1TimeLine();
  String StringA1TimeToDisplay = alarmPage[SETA1TIME];
  
  // Three options: Not Set, Set but not on, or Set and on 
  if(myAlarmTimes[ALARM1HOUR] == -1 || myAlarmTimes[ALARM1MIN] == -1){
    mainPage[A1LINE] = "A1 Time Not Set";
  }
  else{
    mainPage[A1LINE] = String("A1 ") + (alarm1Active ? "On " : "Off ") + (alarm1Repeat ? "R " : "") + StringA1TimeToDisplay;
  }
  if(myAlarmTimes[ALARM2HOUR] == -1 || myAlarmTimes[ALARM2MIN] == -1){
    mainPage[A2LINE] = "A2 Time Not Set";
  }
  else{
    mainPage[A2LINE] = String("A2 ") + (alarm2Active ? "On " : "Off ") + (alarm2Repeat ? "R " : "") + myAlarmTimes[ALARM2HOUR] + ":" + myAlarmTimes[ALARM2MIN];
  }
  if(myAlarmTimes[ALARM3HOUR] == -1 || myAlarmTimes[ALARM3MIN] == -1){
    mainPage[A3LINE] = "A3 Time Not Set";
  }
  else{
    mainPage[A3LINE] = String("A3 ") + (alarm3Active ? "On " : "Off ") + (alarm3Repeat ? "R " : "") + myAlarmTimes[ALARM3HOUR] + ":" + myAlarmTimes[ALARM3MIN];
  }
}

void LcdMainSetOtherInfo() {
  mainPage[ALARMSTATUSES] = String("A1") + (alarm1Active ? "S" : "") + (alarm1Repeat ? "R " : " ") + "A2" + (alarm2Active ? "S" : "") + (alarm2Repeat ? "R " : " ") + "A3" + (alarm3Active ? "S" : "") + (alarm3Repeat ? "R " : " ");
  mainPage[SNOOZE] = "Snooze";
  mainPage[OFF] = "Off";
  mainPage[SETTINGS] = "Settings";
}

void LcdSettingsSetTimeLine() {
  settingsPage[SETTINGSSETTIME] = "Set Time";
}

void LcdSettingsDaylightLine() {
  settingsPage[SETTINGSSETDAYLIGHT] = "Daylight Savings";
}

void LcdSettingsAlarmPatternsLine() {
  settingsPage[SETTINGSSETPATTERNS] = "Alarm Patterns";
}

void LcdSettingsBackLine() {
  settingsPage[SETTINGSBACK] = "Back";
}

void LcdSetTimeLine() {
  setTimePage[SETTIMETIMELINE] = "setTimeOnLine";
}

void LcdSetTimeOtherInfo() {
  setTimePage[SETTIMECONFIRM] = "Confirm";
  setTimePage[SETTIMECANCEL] = "Cancel";
}

void LcdSetA1TimeLine() { //TODO Fix this so it will show the stored alarm time, if not currently setting it
//  alarmPage[SETA1TIME] = String(currHourFirstDigit) + String(currHourSecondDigit) + ':' + String(currMinFirstDigit) + String(currMinSecondDigit);
  String hourToDisplay = String(myAlarmTimes[ALARM1HOUR]);
  String minToDisplay = String(myAlarmTimes[ALARM1MIN]);
  if(myAlarmTimes[ALARM1HOUR] < 10){
    hourToDisplay = String('0') + hourToDisplay;
  }
  if(myAlarmTimes[ALARM1MIN] < 10){
    minToDisplay = String('0') + minToDisplay;
  }
  alarmPage[SETA1TIME] = hourToDisplay + ':' + minToDisplay;

}

void LcdRefreshA1TimeDigit(int8_t digitToRefresh) {
  lcd.setCursor(currColumnSelected, 0);
  lcd.print(digitToRefresh);
  lcd.setCursor(currColumnSelected, 0);
}

void LcdSetA1ActiveAndRepeatStatus() {
  if(myAlarmTimes[0] == -1 || myAlarmTimes[1] == -1){
    alarmPage[SETA1ACTIVESTATUS] = "Set A1 First";
    alarmPage[SETA1REPEATSTATUS] = "Set A1 First"; 
  }
  else{
    alarmPage[SETA1ACTIVESTATUS] = alarm1Active ? "Deactivate A1?" : "Activate A1?";
    alarmPage[SETA1REPEATSTATUS] = alarm1Repeat ? "Unset Repeat?" : "Set Repeat?";
  }
}

void LcdA1OtherInfo() {
  alarmPage[SETA1CONFIRM] = "Confirm";
  alarmPage[SETA1CANCEL] = "Cancel";
}
