/*
 created 24 Nov 2010
 modified 9 Apr 2012
 by Tom Igoe
 
 modified 18 Sep 2014
 by Bobby Chan @ SparkFun Electronics Inc.
 
 Modified by Toni Klopfenstein @ SparkFun Electronics
  September 2015
  https://github.com/sparkfun/CAN-Bus_Shield
 
 SD Card Datalogger
 
 This example is based off an example code from Arduino's site
 http://arduino.cc/en/Tutorial/Datalogger and it shows how to 
 log data from three analog sensors with a timestamp based on when
 the Arduino began running the current program to an SD card using
 the SD library https://github.com/greiman/SdFat by William 
 Greiman. This example code also includes an output to the 
 Serial Monitor for debugging.
  
 The circuit:
 * analog sensors on analog pins 0, 1, and 2
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 9
 
 This example code is in the public domain.
 */
 
#include <SPI.h>
#include <SD.h>
#include <can.h>
#include <global.h>
#include <Canbus.h>
#include <mcp2515_defs.h>
#include <mcp2515.h>
#include <defaults.h>
#include <Arduino.h>


// On the Ethernet Shield, CS is pin 4. Note that even if it's not
// used as the CS pin, the hardware CS pin (10 on most Arduino boards,
// 53 on the Mega) must be left as an output or the SD library
// functions will not work.

// Chip Select pin is tied to pin 9 on the SparkFun CAN-Bus Shield (pin 49 on Mega)
const int chipSelect = 49;  
unsigned int brake_temp_fl;
unsigned int brake_temp_fr;
unsigned int brake_temp_rl;
unsigned int brake_temp_rr;
unsigned int brake_pressure_front;
unsigned int brake_pressure_rear;
unsigned int wheel_speed_fl;
unsigned int wheel_speed_fr;
unsigned int wheel_speed_rl;
unsigned int wheel_speed_rr;
unsigned int oil_pressure;
unsigned int oil_temp;
unsigned int neutral_gear_switch;
unsigned int engine_speed;
unsigned int manifold_air_pressure;
unsigned int manifold_air_temp;
unsigned int coolant_temp;
int count = 0;

void setup()
{
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(chipSelect, OUTPUT);

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    //Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  //Serial.println("card initialized.");
  File dataFile = SD.open("can_read_test.txt", FILE_WRITE);
  if(!can_setup()){
    dataFile.print("CAN init unsuccessful");
    dataFile.close();
    return;
  } else {
    dataFile.print("CAN init successful");
    dataFile.close();
    return;
  } 
}

void loop()
{ 
  // make a string for assembling the data to log:
  String dataString = "";

  brake_temp_fl = can_read(2);
  dataString += "brake temp front left:";
  dataString += String(brake_temp_fl);
  dataString += "\n";
  
  brake_temp_fr = can_read(3);
  dataString += "brake temp front right:";
  dataString += String(brake_temp_fr);
  dataString += "\n";

  brake_temp_rl = can_read(4);
  dataString += "brake temp rear left:";
  dataString +=  String(brake_temp_rl);
  dataString += "\n";
  
  brake_temp_rr = can_read(5);
  dataString += "brake temp rear right:";
  dataString +=  String(brake_temp_rr);
  dataString += "v";
  
  brake_pressure_front = can_read(0);
  dataString += "brake pressure front:";
  dataString +=  String(brake_pressure_front);
  dataString += "\n";
  
  brake_pressure_rear = can_read(1);
  dataString += "brake pressure rear:";
  dataString +=  String(brake_pressure_rear);
  dataString += "\n";
  
  wheel_speed_fl = can_read(10);
  dataString += "wheel speed front left:";
  dataString +=  String(wheel_speed_fl);
  dataString += "\n";
  
  wheel_speed_fr = can_read(11);
  dataString += "wheel speed front right:";
  dataString +=  String(wheel_speed_fr);
  dataString += "\n";
  
  wheel_speed_rl = can_read(12);
  dataString += "wheel speed rear left:";
  dataString +=  String(wheel_speed_rl);
  dataString += "\n";
  
  wheel_speed_rr = can_read(13);
  dataString += "wheel speed rear right: ";
  dataString +=  String(wheel_speed_rr);
  dataString += "\n";
  
  oil_pressure = can_read(20);
  dataString += "oil pressure: ";
  dataString +=  String(oil_pressure);
  dataString += "\n";
  
  oil_temp = can_read(21);
  dataString += "oil temp: ";
  dataString +=  String(oil_temp);
  dataString += "\n";
  
  neutral_gear_switch = can_read(30);
  dataString += "neutral gear switch: ";
  dataString +=  String(neutral_gear_switch);
  dataString += "\n";
  
  engine_speed = can_read(50);
  dataString += "engine speed: ";
  dataString +=  String(engine_speed);
  dataString += "\n";
  
  manifold_air_pressure = can_read(51);
  dataString += "manifold air pressure: ";
  dataString +=  String(manifold_air_pressure);
  dataString += "\n";
  
  manifold_air_temp = can_read(52);
  dataString += "manifold air temp: ";
  dataString +=  String(manifold_air_temp);
  dataString += "\n";
  
  coolant_temp = can_read(53);
  dataString += "coolant temp: ";
  dataString +=  String(coolant_temp);
  dataString += "\n";
  
  count++;

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  // this opens the file and appends to the end of file
  // if the file does not exist, this will create a new file.

  // if the file is available, write to it:
  if (count == 50) {
    if (dataFile)   { 
      File dataFile = SD.open("can_read_test.txt", FILE_WRITE);
      dataFile.print(dataString); //create a new row to read data more clearly
      dataFile.close();   //close file
      pinMode(14, OUTPUT);
      digitalWrite(14, HIGH);
      return;
  } else {
      pinMode(14, OUTPUT);
      digitalWrite(14, HIGH);
      return;
  }

}
