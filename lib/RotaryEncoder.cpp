#include "RotaryEncoder.h"

AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_BUTTON_PIN, ROTARY_ENCODER_VCC_PIN, ROTARY_ENCODER_STEPS);

void rotary_onButtonClick() {
    btnpress = true;
    if (millis() - lastTimePressed < 500) return;
    lastTimePressed = millis();
    //Serial.print("button pressed ");
    //Serial.print(millis());
    //Serial.println(" milliseconds after restart");
}

void rotary_loop() {
    if (EmergencyPressed) return;

    if (rotaryEncoder.encoderChanged()) {
        //Serial.print("Value: ");
        //Serial.println(rotaryEncoder.readEncoder());
        cont = (rotaryEncoder.readEncoder());
        if (cont > cont_pre) contador++;
        if (cont < cont_pre) contador--;
        cont_pre = cont;
    }
    if (rotaryEncoder.isEncoderButtonClicked()) {
        rotary_onButtonClick();
    }
}

void IRAM_ATTR readEncoderISR() {
    rotaryEncoder.readEncoder_ISR();
}

void Inicializar_Encoder(){
  //we must initialize rotary encoder
  rotaryEncoder.begin();
  rotaryEncoder.setup(readEncoderISR);
  //bool circleValues = false;
  rotaryEncoder.setBoundaries(0, 1000, true); //minValue, maxValue, circleValues true|false (when max go to min and vice versa)
  rotaryEncoder.setAcceleration(100); //or set the value - larger number = more accelearation; 0 or 1 means disabled acceleration
  Serial.println("Initialized");
}