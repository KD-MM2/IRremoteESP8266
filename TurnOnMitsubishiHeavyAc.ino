/* Copyright 2019 David Conran
*
* An IR LED circuit *MUST* be connected to the ESP8266 on a pin
* as specified by kIrLed below.
*
* TL;DR: The IR LED needs to be driven by a transistor for a good result.
*
* Suggested circuit:
*     https://github.com/crankyoldgit/IRremoteESP8266/wiki#ir-sending
*
* Common mistakes & tips:
*   * Don't just connect the IR LED directly to the pin, it won't
*     have enough current to drive the IR LED effectively.
*   * Make sure you have the IR LED polarity correct.
*     See: https://learn.sparkfun.com/tutorials/polarity/diode-and-led-polarity
*   * Typical digital camera/phones can be used to see if the IR LED is flashed.
*     Replace the IR LED with a normal LED if you don't have a digital camera
*     when debugging.
*   * Avoid using the following pins unless you really know what you are doing:
*     * Pin 0/D3: Can interfere with the boot/program mode & support circuits.
*     * Pin 1/TX/TXD0: Any serial transmissions from the ESP8266 will interfere.
*     * Pin 3/RX/RXD0: Any serial transmissions to the ESP8266 will interfere.
*   * ESP-01 modules are tricky. We suggest you use a module with more GPIOs
*     for your first time. e.g. ESP-12 etc.
*/
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ir_MitsubishiHeavy.h>

const uint16_t kIrLed = 4;  // ESP8266 GPIO pin to use. Recommended: 4 (D2).
IRMitsubishiHeavy152Ac ac(kIrLed);  // Set the GPIO used for sending messages.

int nhietdo = 24;
int chedomacdinh = 0;
bool pwr = false;

// Thiet lap nut gpio pin
int nguon = 5;
int nguonms = 0;
int chedo = 13;
int chedoms = 0;
int tang = 12;
int tangms = 0;
int giam = 14;
int giamms = 0;

void printState() {
  // Display the settings.
  Serial.println("Mitsubishi Heavy A/C remote is in the following state:");
  Serial.printf("  %s\n", ac.toString().c_str());
  // Display the encoded IR sequence.
  unsigned char* ir_code = ac.getRaw();
  Serial.print("IR Code: 0x");
  for (uint8_t i = 0; i < kMitsubishiHeavy152StateLength; i++)
    Serial.printf("%02X", ir_code[i]);
  Serial.println();
}

void setup() {
  ac.begin();
  Serial.begin(115200);
  delay(200);

  // Set up what we want to send. See ir_MitsubishiHeavy.(cpp|h) for all the options.
  Serial.println("Default state of the remote.");
  printState();
  Serial.println("Setting desired state for A/C.");
  ac.setPower(pwr);  // Turn it on.
  ac.setFan(chedomacdinh);  // Auto Fan
  ac.setMode(chedomacdinh);  // Auto mode
  ac.setTemp(nhietdo);  // Celsius
  ac.setSwingVertical(chedomacdinh);  // Swing vertically
  ac.setSwingHorizontal(chedomacdinh);  // Swing Horizontally
  pinMode(nguon, INPUT_PULLUP);
  pinMode(chedo, INPUT_PULLUP);
  pinMode(tang, INPUT_PULLUP);
  pinMode(giam, INPUT_PULLUP);
}

void loop() {
  // nguon
  while (digitalRead(nguon) == LOW){
    nguonms = nguonms + 100;
  }
  if(nguonms >= 100){
    if(pwr == false){
      ac.setPower(true);
      pwr = true;
      Serial.println("Turning on A/C...");
      ac.send();
      printState();
    } else if(pwr == true) {
      ac.setPower(false);
      pwr = false;
      Serial.println("Turning off A/C ...");
      ac.send();
      printState();
    }
  }
  nguonms = 0;

// che do
  while (digitalRead(chedo) == LOW){
    chedoms = chedoms + 100;
  }
  if(chedoms >= 100){
    if(chedomacdinh >= 0 && chedomacdinh < 4){
      chedomacdinh ++;
      ac.setMode(chedomacdinh);
      ac.send();
      printState();
    } else if(chedomacdinh == 4){
      chedomacdinh = 0;
      ac.setMode(chedomacdinh);
      ac.send();
      printState();
    }
  }
  chedoms = 0;

  // tang
  while (digitalRead(tang) == LOW){
    tangms = tangms + 100;
  }
  if(tangms >= 100){
    if(nhietdo >= 17 && nhietdo < 31){
      nhietdo ++;
      ac.setTemp(nhietdo);
      ac.send();
      printState();
    } else if(nhietdo = 31) {
      nhietdo = 31;
      ac.setTemp(nhietdo);
      ac.send();
      printState();
    }
  }
  tangms = 0;

// giam
  while (digitalRead(giam) == LOW){
    giamms = giamms + 100;
  }
  if(giamms >= 100){
    if(nhietdo > 17 && nhietdo <= 31){
      nhietdo --;
      ac.setTemp(nhietdo);
      ac.send();
      printState();
    } else if(nhietdo = 17) {
      nhietdo = 17;
      ac.setTemp(nhietdo);
      ac.send();
      printState();
    }
  }
  giamms = 0;
}
