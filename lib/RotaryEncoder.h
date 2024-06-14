#ifndef ROTARY_ENCODER_H
#define ROTARY_ENCODER_H

#include "AiEsp32RotaryEncoder.h"
#include "Arduino.h"
#include "BotonEmergencia.h"
#include "LcdMenu.h"


//ENCODER
#define ROTARY_ENCODER_A_PIN 17  //dt
#define ROTARY_ENCODER_B_PIN 14 //clk
#define ROTARY_ENCODER_BUTTON_PIN 21  //sw
#define ROTARY_ENCODER_VCC_PIN -1
#define ROTARY_ENCODER_STEPS 4

static unsigned long lastTimePressed = 0;

extern AiEsp32RotaryEncoder rotaryEncoder;

void rotary_onButtonClick();
void rotary_loop();
void IRAM_ATTR readEncoderISR();
void Inicializar_Encoder();

#endif
