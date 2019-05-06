
/* The following section is just for navigation */

void resetCurrLines() {
  currLineSelected = 0;
  currLinesShowing[0] = 0;
  currLinesShowing[1] = 1;
}

bool checkIfButtonIsSettings() {
  if(currPageIdx == MAINPAGE && currLineSelected == SETTINGS) {
    currPageIdx = SETTINGSPAGE;
    return true;
  }
  return false;
}

bool checkIfButtonIsSettingsPageBack() {
  if(currPageIdx == SETTINGSPAGE && currLineSelected == SETTINGSBACK) {
    currPageIdx = MAINPAGE;
    return true;
  }
  return false;
}

bool checkIfButtonIsSettingsSetTime() {
  if(currPageIdx == SETTINGSPAGE && currLineSelected == SETTINGSSETTIME) {
    currPageIdx = SETTIMEPAGE;
    return true;
  }
  return false;
}

bool checkIfButtonIsSetTimeCancel() {
  if(currPageIdx == SETTIMEPAGE && currLineSelected == SETTIMECANCEL) {
    currPageIdx = SETTINGSPAGE;
    return true;
  }
  return false;
}

bool checkIfButtonIsA1Line() {
  if(currPageIdx == MAINPAGE && currLineSelected == A1LINE) {
    currPageIdx = ALARMPAGE;
    setCurrHourAndMinsForAlarm(1);
    return true;
  }
  return false;
}

void setCurrHourAndMinsForAlarm(int alarmNum) {
  int8_t currHour = myAlarmTimes[0];
  int8_t currMin = myAlarmTimes[1];
  
  currHourFirstDigit = currHour / 10;
  currHourSecondDigit = currHour % 10;
  currMinFirstDigit = currMin / 10;
  currMinSecondDigit = currMin % 10;
}

bool checkIfButtonIsA1Activate() {
  if(currPageIdx == ALARMPAGE && currLineSelected == SETA1ACTIVESTATUS) {
    alarm1Active = alarm1Active ? false : true;
    currPageIdx = MAINPAGE;
    LcdMainSetOtherInfo();
    LcdSetAlarmLines();
    LcdSetA1ActiveAndRepeatStatus();
    return true;
  }
  return false;
}

bool checkIfButtonIsA1Repeat() {
  if(currPageIdx == ALARMPAGE && currLineSelected == SETA1REPEATSTATUS) {
    alarm1Repeat = alarm1Repeat ? false : true;
    currPageIdx = MAINPAGE;
    LcdMainSetOtherInfo();
    LcdSetAlarmLines();
    LcdSetA1ActiveAndRepeatStatus();
    return true;
  }
  return false;
}


bool checkIfButtonIsA1Confirm() {
  if(currPageIdx == ALARMPAGE && currLineSelected == SETA1CONFIRM) {
    currPageIdx = MAINPAGE;
    tempSaveHoursAndMinsTomyAlarmTimes();
//    String hours = String(currHourFirstDigit) + String(currHourSecondDigit);
//    String mins = String(currMinFirstDigit) + String(currMinSecondDigit);
//    myAlarmTimes[0] = (int8_t) hours.toInt();
//    myAlarmTimes[1] = (int8_t) mins.toInt();
    Serial.println("About to set alarm times in memory to:");
    Serial.println(myAlarmTimes[0]);
    Serial.println(myAlarmTimes[1]);
    setAlarm(1, myAlarmTimes[0], myAlarmTimes[1]);  // alarm #, H, M
    LcdSetAlarmLines();
    resetCurrTimes();
    return true;
  }
  return false;
}

void tempSaveHoursAndMinsTomyAlarmTimes(){
  String hours = String(currHourFirstDigit) + String(currHourSecondDigit);
  String mins = String(currMinFirstDigit) + String(currMinSecondDigit);
  Serial.println("tempSaveHoursAndMinsTomyAlarmTimes");
  Serial.println(hours);
  Serial.println(mins);
  myAlarmTimes[0] = (int8_t) hours.toInt();
  myAlarmTimes[1] = (int8_t) mins.toInt();
}

bool checkIfButtonIsA1Cancel() {
  if(currPageIdx == ALARMPAGE && currLineSelected == SETA1CANCEL) {
    currPageIdx = MAINPAGE;
    getAlarmsAndUpdateMyAlarmsArr();
    LcdSetA1TimeLine();
    delay(50);
    return true;
  }
  return false;
}

/* The following section is just for ??? */
void resetCurrTimes() {
  currHourFirstDigit = 0;
  currHourSecondDigit = 0;
  currMinFirstDigit = 0;
  currMinSecondDigit = 0;
}
