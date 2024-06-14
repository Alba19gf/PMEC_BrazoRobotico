#include <Wire.h>
#include <WiFi.h>
#include "../lib/LcdMenu.cpp"
#include "../lib/LcdMenu.h"
#include "../lib/RotaryEncoder.cpp"
#include "../lib/RotaryEncoder.h"
#include "../lib/BotonEmergencia.cpp"
#include "../lib/BotonEmergencia.h"
#include "../lib/Comunicacion_JSON.cpp"
#include "../lib/Comunicacion_JSON.h"
#include "../lib/TresEnRayaTest.cpp"
#include "../lib/TresEnRayaTest.h"


int MatrizEstado[3][3]; // Matriz de estado global
const int buttonPin = 15; // Pin del botón
int buttonState = 0;
int global_position = 0; 
int final_position = 0;
int last_final_position = final_position;
int Moving=0;
int led_red=40;
int led_green=41;

float M_Base = 0, M_Hombro=0, M_Codo=0, M_Pinza=0;

void setup()
{
  Inicializar_COMS();
  Inicializar_Boton_Emergencia();
  Inicializar_Encoder();
  Inicializar_LCD();

  pinMode(buttonPin, INPUT_PULLUP); // Configurar el pin del botón

  pinMode(led_red, OUTPUT);
  pinMode(led_green, OUTPUT);

  digitalWrite(led_red, LOW);
  digitalWrite(led_green, LOW);
  

}

void loop()
{
  //in loop call your custom function which will process rotary encoder values
  rotary_loop();
  Robot_Menu();

  // Verifica si se presionó el botón de emergencia fuera del menú
  if (EmergencyPressed) {
    showEmergencyMessage();
    return;
  }

  buttonState = digitalRead(buttonPin);

  if (buttonState == LOW) { // Si el botón es presionado
     Fn_MatrizState(); // Llamamos a la función MatrizState y almacenamos el resultado en MatrizEstado
     
  }
 // Serial.println(EmergencyPressed);

 if(EmergencyPressed)
 {
  Serial.print("Emergency stop");
 }

}