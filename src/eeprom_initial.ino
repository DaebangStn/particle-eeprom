/*
 * Project eeprom_initial
 * Description:
 * Author:
 * Date:
 */
#include "kickgoing.h"

SYSTEM_MODE(MANUAL);

uint16_t init;
char foo[6] = {0xff, 0xff};
String null = String("");
// setup() runs once, when the device is first turned on.
void setup() {
  // Put initialization like pinMode and begin functions here.
  Serial.begin(9600);
  EEPROM.put(0, foo);
  EEPROM.get(0, init);
  null.toCharArray(foo, 6);
  EEPROM.put(0, foo);
  EEPROM.get(0, foo);
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  // The core of your code will likely live here.
  Serial.print("init: ");
  Serial.print(init, HEX);
  Serial.println();
  Serial.print("foo: ");
  for(int i=0; i<6; i++){
    Serial.print(foo[i], HEX);
    Serial.println();
  }
  Serial.print("length: ");
  Serial.print(null.length());
  Serial.println();

  delay(1000);
}
