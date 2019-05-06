// Arduino pin numbers
const int SW_pin = 2; // digital pin connected to switch output
const int X_pin = 0; // analog pin connected to X output
const int Y_pin = 1; // analog pin connected to Y output

void JoystickSetup() {
  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);
}

void JoystickLoop() {
  int x_val = analogRead(X_pin);
  int y_val = analogRead(Y_pin);
  bool button_pressed = digitalRead(SW_pin);

  if(!button_pressed){
    Serial.println("Button Pressed");
    if(checkIfButtonIsSettings() || checkIfButtonIsSettingsPageBack() || checkIfButtonIsSettingsSetTime() || checkIfButtonIsSetTimeCancel() || checkIfButtonIsA1Line() || 
      checkIfButtonIsA1Activate() || checkIfButtonIsA1Repeat() || checkIfButtonIsA1Confirm() || checkIfButtonIsA1Cancel()){
      resetCurrLines();
      LcdClearLines(1);
      LcdDisplayCurrLines();
    }
  }
  else if(x_val > 1000 && y_val > 50 && y_val < 1000){
    Serial.println("Right");
    if(currPageIdx == ALARMPAGE && currLineSelected == SETA1TIME && currColumnSelected < 5){
      if(currColumnSelected == 2){
        currColumnSelected++;
      }
      currColumnSelected++;
//      LcdRefreshSetTime('X');
    }
    Serial.println("currColumnSelected: " + String(currColumnSelected));
  }
  else if(x_val < 50 && y_val > 50 && y_val < 1000){
    Serial.println("Left");
    if(currPageIdx == ALARMPAGE && currLineSelected == SETA1TIME && currColumnSelected > 0){
      if(currColumnSelected == 4){
        currColumnSelected--;
      }
      currColumnSelected--;
//      LcdRefreshSetTime('Y');
    }
    Serial.println("currColumnSelected: " + String(currColumnSelected));
  }
  else if(y_val > 1000 && x_val > 50 && x_val < 1000){
    Serial.println("Down");
    if(currPageIdx == ALARMPAGE && currColumnSelected != 0){
      if(currColumnSelected == 1){
        if(currHourSecondDigit < 4){
          currHourFirstDigit = (currHourFirstDigit + 1) % 3;
        }
        else{
          currHourFirstDigit = (currHourFirstDigit + 1) % 2;
        }
        LcdRefreshA1TimeDigit(currHourFirstDigit);
      }
      // TODO Finish the other columns here      
    }
    else if(currLineSelected < maxlineSelectionForPage[currPageIdx] && currColumnSelected == 0){
      if(currPageIdx == ALARMPAGE){
        tempSaveHoursAndMinsTomyAlarmTimes();
        LcdSetA1TimeLine();
      }
      currLineSelected++;
      if(currLineSelected != currLinesShowing[0] && currLineSelected != currLinesShowing[1]){
        currLinesShowing[0]++;
        currLinesShowing[1]++;
        Serial.println("Here000");
      }
      LcdClearLines(2);
      LcdDisplayCurrLines();
    }
//    LcdDisplayCurrLines();
    Serial.println("currLineSelected: " + String(currLineSelected));
  }
  else if(y_val < 50 && x_val > 50 && x_val < 1000){
    Serial.println("Up");
    if(currPageIdx == ALARMPAGE && currColumnSelected != 0){ // change to (currPageIdx == ALARMPAGE || currPageIdx == SETTIMEPAGE)
      if(currColumnSelected == 1){
        if(currHourSecondDigit < 4){
          currHourFirstDigit = (currHourFirstDigit + 1) % 3;
        }
        else{
          currHourFirstDigit = (currHourFirstDigit + 1) % 2;
        }
        LcdRefreshA1TimeDigit(currHourFirstDigit);
      }
      else if(currColumnSelected == 2){
        if(currHourFirstDigit < 2){
          currHourSecondDigit = (currHourSecondDigit + 1) % 10;
        }
        else{
          currHourSecondDigit = (currHourSecondDigit + 1) % 4;
        }
        LcdRefreshA1TimeDigit(currHourSecondDigit);
      }
      else if(currColumnSelected == 4){
        currMinFirstDigit = (currMinFirstDigit + 1) % 6;
        LcdRefreshA1TimeDigit(currMinFirstDigit);
      }
      else if(currColumnSelected == 5){
        currMinSecondDigit = (currMinSecondDigit + 1) % 10;
        LcdRefreshA1TimeDigit(currMinSecondDigit);
      }
      
    }
    else if(currLineSelected > 0 && currColumnSelected == 0){
      currLineSelected--;
      if(currLineSelected != currLinesShowing[0] && currLineSelected != currLinesShowing[1]){
        currLinesShowing[0]--;
        currLinesShowing[1]--;
      }
      LcdClearLines(3);
      LcdDisplayCurrLines();
    }
//    LcdDisplayCurrLines();
    Serial.println("currLineSelected: " + String(currLineSelected));
  }
  delay(250);
}
