// DS3231_Serial_Hard
// Copyright (C)2015 Rinky-Dink Electronics, Henning Karlsen. All right reserved
// web: http://www.rinkydinkelectronics.com/library.php?id=73
//
// A quick demo of how to use my DS3231-library to 
// retrieve time- and date-data for you to manipulate.
//
// To use the hardware I2C (TWI) interface of the Arduino you must connect
// the pins as follows:
//
// Arduino Uno/2009:
// ----------------------
// DS3231:  SDA pin   -> Arduino Analog 4 or the dedicated SDA pin
//          SCL pin   -> Arduino Analog 5 or the dedicated SCL pin
//
// The internal pull-up resistors will be activated when using the 
// hardware I2C interfaces.
//
// You can connect the DS3231 to any available pin but if you use any
// other than what is described above the library will fall back to
// a software-based, TWI-like protocol which will require exclusive access 
// to the pins used, and you will also have to use appropriate, external
// pull-up resistors on the data and clock signals.
//

#include <DS3231.h>


// Init the DS3231 using the hardware interface
DS3231 rtc(SDA, SCL);

// Init a Time-data structure
Time t;

void RTCsetup() {
  // Initialize the rtc object
  rtc.begin();

//  int8_t myAlarmTimes[6] = {-1, -1, -1, -1, -1, -1};
//  myAlarmTimes[0] = 127;

  Wire.begin(); // initialise the connection


}

void RTCloop() {
  // nothing here anymore

}

// Get data from the DS3231
void getTimeStrings(String& currDate, String& currTime, String& currDateShortened, String& currTimeShortened) {
  t = rtc.getTime();
  currDate = rtc.getMonthStr() + String(' ') + t.date + ", " + t.year;
//  currDate = t.mon + String('-') + t.date + ", " + t.year;
  currDateShortened = String(t.mon) + '-' + t.date + "-" + String(t.year).substring(2, 4);
  currTime = String(t.hour) + ':' + t.min + ':' + t.sec;
  currTimeShortened = String(t.hour) + ':' + t.min;
  if(currTimeShortened != prevCurrTimeShortened){
    hourOrMinChanged = true;
  }
  
}

void setCurrTime(int Hour, int minute, int second, int month, int day, int Year) {
// The following lines can be uncommented to set the date and time
//  rtc.setDOW(THURSDAY);     // Set Day-of-Week to SUNDAY
  rtc.setTime(Hour, minute, second);     // Set the time to 12:00:00 (24hr format)
  rtc.setDate(day, month, Year);   // Set the date to January 1st, 2014
}


// alarmNum is either 1, 2, or 3
void setAlarm(int alarmNum, int Hour, int minute) {
  int startIdx = (alarmNum == 1) ? 0 : ((alarmNum == 2) ? 2 : 4);
  myAlarmTimes[startIdx] = Hour;
  myAlarmTimes[startIdx + 1] = minute;

  // write to EEPROM
  i2c_eeprom_write_page(0x57, 0, (byte *)myAlarmTimes, sizeof(int8_t) * 6);
  delay(25);
}


void getAlarmsAndUpdateMyAlarmsArr() {
  int addr = 0; //first address
  int count = 0;
  Serial.println("getAlarmsAndUpdateMyAlarmsArr(): ");
  
  while (count < 6){
    // access the first address from the memory
    byte b = i2c_eeprom_read_byte(0x57, addr);
    Serial.println(b);
    myAlarmTimes[count] = (int8_t) b;
    addr++;
    count++;
  }
}


void i2c_eeprom_write_byte( int deviceaddress, unsigned int eeaddress, byte data ) {
    int rdata = data;
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress >> 8)); // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.write(rdata);
    Wire.endTransmission();
}

// WARNING: address is a page address, 6-bit end will wrap around
// also, data can be maximum of about 30 bytes, because the Wire library has a buffer of 32 bytes
void i2c_eeprom_write_page( int deviceaddress, unsigned int eeaddresspage, byte* data, byte length ) {
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddresspage >> 8)); // MSB
    Wire.write((int)(eeaddresspage & 0xFF)); // LSB
    byte c;
    for ( c = 0; c < length; c++)
        Wire.write(data[c]);
    Wire.endTransmission();
}

byte i2c_eeprom_read_byte( int deviceaddress, unsigned int eeaddress ) {
    byte rdata = 0xFF;
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress >> 8)); // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.endTransmission();
    Wire.requestFrom(deviceaddress,1);
    if (Wire.available()) rdata = Wire.read();
    return rdata;
}

// maybe let's not read more than 30 or 32 bytes at a time!
void i2c_eeprom_read_buffer( int deviceaddress, unsigned int eeaddress, byte *buffer, int length ) {
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress >> 8)); // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.endTransmission();
    Wire.requestFrom(deviceaddress,length);
    int c = 0;
    for ( c = 0; c < length; c++ )
        if (Wire.available()) buffer[c] = Wire.read();
}
