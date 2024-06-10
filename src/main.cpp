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

int MatrizEstado[3][3]; // Matriz de estado global
const int buttonPin = 15; // Pin del botón
int buttonState = 0;
float M_Base = 0, M_Hombro=0, M_Codo=0, M_Pinza=0;

void setup()
{
  Inicializar_COMS();
  Inicializar_Boton_Emergencia();
  Inicializar_Encoder();
  Inicializar_LCD();

  pinMode(buttonPin, INPUT_PULLUP); // Configurar el pin del botón
}

void loop()
{
  //in loop call your custom function which will process rotary encoder values
  rotary_loop();
  Robot_Menu();

  buttonState = digitalRead(buttonPin);

  if (buttonState == LOW) { // Si el botón es presionado
     Fn_MatrizState(); // Llamamos a la función MatrizState y almacenamos el resultado en MatrizEstado
     Serial.println("Loop:");
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        Serial.print(MatrizEstado[i][j]);
         Serial.print(" ");
      }
      Serial.println();
    }
  }
 // Serial.println(EmergencyPressed);

 if(EmergencyPressed)
 {
  Serial.print("Emergency stop");
 }

}