#include <can.h>
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  if(!can_setup()) {
    Serial.println("CAN init fail!");
    while(1);
  }
  Serial.println("CAN init success");
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(100);
  String toPrint = "";
  toPrint += "Br temp FL: ";
  toPrint += can_read(2);
  toPrint += "\tFR: ";
  toPrint += can_read(3);
  toPrint += "\tRL: ";
  toPrint += can_read(4);
  toPrint += "\tRR: ";
  toPrint += can_read(5);
  toPrint += "\tBr press f: ";
  toPrint += can_read(0);
  toPrint += "\tR: ";
  toPrint += can_read(1);
  toPrint += "\tCool temp: ";
  toPrint += can_read(53);
  toPrint += "\tEngine RPM: ";
  toPrint += can_read(50);
  Serial.println(toPrint);
}
