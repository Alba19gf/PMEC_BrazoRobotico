#include <Arduino.h>

#define timoutLimit 100

const int motor1Pin1 = 15;
const int motor1Pin2 = 16;
const int enable = 17;

const int ledChannel = 0;
const int frequency = 50000;
const int resolution = 8;
int dutyCycle = 125;

String cmd;

void setup() {

  
  Serial.begin(115200);
  Serial.print("Testing DC Motor...");
}

void loop() {

  // CMD
  while(Serial.available()) {

    cmd = Serial.readString();// read the incoming data as string
    cmd.trim();

    Serial.printf("\nCMD: %s\n", cmd);
    
    if(cmd.equals("H"))
    {
      digitalWrite(SPI_SS, HIGH);
      Serial.println("SS HIGH");
    }

    else if(cmd.equals("L"))
    {
      digitalWrite(SPI_SS, LOW);
      Serial.println("SS LOW");
    }

    else if(cmd.toInt() >= 0 && cmd.toInt() <= 255)
    {
      dutyCycle = cmd.toInt();
      Serial.printf("New duty cycle: %i \n", dutyCycle);
    }
  }
  
  digitalWrite(motor1Pin1, LOW);
  ledcWrite(ledChannel, dutyCycle);

}