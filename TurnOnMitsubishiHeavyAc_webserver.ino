#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ir_MitsubishiHeavy.h>
#include <ESP8266WiFi.h>
#include "IRtext.h"

const char* ssid     = "IODATA-762f6c-2G";
const char* password = "4733919286686";
const uint16_t kIrLed = 4;
IRMitsubishiHeavy152Ac ac(kIrLed);
bool power_state;
int present_temp;
int present_mode;
int present_fan;
int vertical;
int horizontal;
String header;
unsigned long currentTime = millis();
unsigned long previousTime = 0; 
const long timeoutTime = 2000;

String tempStr = String(present_temp);
String powerStr(int power_state) {
  if(power_state == 1){
    return String("ON");
  } else if(power_state == 0) {
    return String("OFF");
  }
}
String modeStr(int present_mode) {
  if(present_mode == 0) {
    return String("AUTO");
  } else if(present_mode == 1) {
    return String("COOL");
  } else if(present_mode == 2) {
    return String("DRY");
  } else if(present_mode == 3) {
    return String("FAN");
  } else if(present_mode == 4) {
    return String("HEAT");
  }
}
String fanStr(int present_fan) {
  if(present_fan == 0) {
    return String("AUTO");
  } else if(present_fan == 1) {
    return String("LOW");
  } else if(present_fan == 2) {
    return String("MEDIUM");
  } else if(present_fan == 3) {
    return String("HIGH");
  } else if(present_fan == 4) {
    return String("MAX");
  } else if(present_fan == 6) {
    return String("ECONO");
  } else if(present_fan == 8) {
    return String("TURBO");
  }
}
String verticalStr(int vertical) {
  if(vertical == 0) {
    return String("AUTO");
  } else if(vertical == 1) {
    return String("HIGHEST");
  } else if(vertical == 2) {
    return String("HIGH");
  } else if(vertical == 3) {
    return String("MIDDLE");
  } else if(vertical == 4) {
    return String("LOW");
  } else if(vertical == 5) {
    return String("LOWEST");
  } else if(vertical == 6) {
    return String("OFF");
  }
}
String horizontalStr(int horizontal) {
  if(horizontal == 0) {
    return String("AUTO");
  } else if(horizontal == 1) {
    return String("LEFT MAX");
  } else if(horizontal == 2) {
    return String("LEFT");
  } else if(horizontal == 3) {
    return String("MIDDLE");
  } else if(horizontal == 4) {
    return String("RIGHT");
  } else if(horizontal == 5) {
    return String("RIGHT MAX");
  } else if(horizontal == 6) {
    return String("RIGHT LEFT");
  } else if(horizontal == 7) {
    return String("LEFT RIGHT");
  } else if(horizontal == 8) {
    return String("OFF");
  }
}
WiFiServer server(80);
void printState() {
  Serial.println("Mitsubishi Heavy A/C remote is in the following state:");
  Serial.printf("%s\n", ac.toString().c_str());
  unsigned char* ir_code = ac.getRaw();
  Serial.print("IR Code: 0x");
  for (uint8_t i = 0; i < kMitsubishiHeavy152StateLength; i++)
    Serial.printf("%02X", ir_code[i]);
  Serial.println();
}
void get_temp() {
  present_temp = ac.getTemp();
}
void get_mode() {
  present_mode = ac.getMode();
}
void get_power() {
  power_state = ac.getPower();
}
void get_fan() {
  present_fan = ac.getFan();
}
void get_vertical() {
  vertical = ac.getSwingVertical();
}
void get_horizontal() {
  horizontal = ac.getSwingHorizontal();
}
void set_default() {
  ac.setPower(false);
  ac.setFan(0);
  ac.setMode(0);
  ac.setTemp(24);
  ac.setSwingVertical(0);
  ac.setSwingHorizontal(0);
}
void power_onoff() {
  get_power();
  if(power_state == false) {
    power_state = true;
    ac.setPower(power_state);
    ac.send();
    printState();
  } else if(power_state == true) {
    power_state = false;
    ac.setPower(power_state);
    ac.send();
    printState();
  }
}
void temp_up() {
  get_temp();
  if(present_temp >= 17 && present_temp <31) {
    present_temp ++;
    ac.setTemp(present_temp);
    ac.send();
    printState();
  } else if(present_temp == 31) {
    present_temp = 31;
    ac.setTemp(present_temp);
    ac.send();
    printState();
  }
}
void temp_down() {
  get_temp();
  if(present_temp > 17 && present_temp <= 31) {
    present_temp --;
    ac.setTemp(present_temp);
    ac.send();
    printState();
  } else if(present_temp == 17) {
    present_temp = 17;
    ac.setTemp(present_temp);
    ac.send();
    printState();
  }
}
void change_mode() {
  get_mode();
  if(present_mode >= 0 && present_mode < 4) {
    present_mode ++;
    ac.setMode(present_mode);
    ac.send();
    printState();
  } else if(present_mode == 4) {
    present_mode = 0;
    ac.setMode(present_mode);
    ac.send();
    printState();
  }
}
void change_fan() {
  get_fan();
  if(present_fan >= 0 && present_fan < 4) {
    present_fan ++;
    ac.setFan(present_fan);
    ac.send();
    printState();
  } else if(present_fan == 4) {
    present_fan = 6;
    ac.setFan(present_fan);
    ac.send();
    printState();
  } else if(present_fan == 6) {
    present_fan = 8;
    ac.setFan(present_fan);
    ac.send();
    printState();
  } else if(present_fan == 8) {
    present_fan = 0;
    ac.setFan(present_fan);
    ac.send();
    printState();
  }
}
void change_vertical() {
  get_vertical();
  if(vertical >= 0 && vertical < 6) {
    vertical ++;
    ac.setSwingVertical(vertical);
    ac.send();
    printState();
  } else if(vertical == 6) {
    vertical = 0;
    ac.setSwingVertical(vertical);
    ac.send();
    printState();
  }
}
void change_horizontal() {
    get_horizontal();
  if(horizontal >= 0 && horizontal < 8) {
    horizontal ++;
    ac.setSwingHorizontal(horizontal);
    ac.send();
    printState();
  } else if(horizontal == 8) {
    horizontal = 0;
    ac.setSwingHorizontal(horizontal);
    ac.send();
    printState();
  }
}
void setup() {
  ac.begin();
  Serial.begin(115200);
  delay(200);
  set_default();
  Serial.println("Mitsubishi Heavy A/C remote is in the following state:");
  printState();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}
void loop() {
  WiFiClient client = server.available();
   if (client) {
    Serial.println("Found new client.");
    String currentLine = "";
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) {
      currentTime = millis();         
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        header += c;
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            if (header.indexOf("GET /power_onoff") >= 0) {
              Serial.println("Power On/Off");
              power_onoff();
            } else if (header.indexOf("GET /change_mode") >= 0) {
              Serial.println("Change mode");
              change_mode();
            } else if (header.indexOf("GET /temp_up") >= 0) {
              Serial.println("Temp up");
              temp_up();
            } else if (header.indexOf("GET /temp_down") >= 0) {
              Serial.println("Temp down");
              temp_down();
            }else if (header.indexOf("GET /change_fan") >= 0) {
              Serial.println("Change fan mode");
              change_fan();
            }else if (header.indexOf("GET /change_vertical") >= 0) {
              Serial.println("Swing vertical");
              change_vertical();
            }else if (header.indexOf("GET /change_horizontal") >= 0) {
              Serial.println("Swing horizontal");
              change_horizontal();
            }
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");
            client.println("<body><h1>ESP8266 Web Server</h1>");
            client.println("<h2>Air conditioner controller by KaoTD</h2>");
            get_power();
            client.println("<p>Power: " + powerStr(power_state) + "</p>");
            get_temp();
            client.println("<p>Temprature: " + String(present_temp) + "</p>");
            get_mode();
            client.println("<p>Mode: " + modeStr(present_mode) + "</p>");
            get_fan();
            client.println("<p>Fan Mode: " + fanStr(present_fan) + "</p>");
            get_vertical();
            client.println("<p>Swing Vertical: " + verticalStr(vertical) + "</p>");
            get_horizontal();
            client.println("<p>Swing Horizontal: " + horizontalStr(horizontal) + "</p>");
            client.println("<p><a href=\"/power_onoff\"><button class=\"button\">POWER ON/OFF</button></a></p>");
            client.println("<p><a href=\"/change_mode\"><button class=\"button\">CHANGE MODE</button></a></p>");
            client.println("<p><a href=\"/change_fan\"><button class=\"button\">CHANGE FAN MODE</button></a></p>");
            client.println("<p><a href=\"/temp_up\"><button class=\"button\">TEMP UP</button></a></p>");
            client.println("<p><a href=\"/temp_down\"><button class=\"button\">TEMP DOWN</button></a></p>");
            client.println("<p><a href=\"/change_vertical\"><button class=\"button\">SWING VERTICAL</button></a></p>");
            client.println("<p><a href=\"/change_horizontal\"><button class=\"button\">SWING HORIZONTAL</button></a></p>");
            client.println("</body></html>");
            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    header = "";
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
