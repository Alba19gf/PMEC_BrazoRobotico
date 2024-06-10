#include "BotonEmergencia.h"
int stp=0;

//Funcion para detectar el boton
void Fn_Emergency() {
  EmergencyPressed = digitalRead(EmergencyButton); // Lee el estado del botón
  
}

void showEmergencyMessage() {
  if (stp=0){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Parada de emergencia");
    stp=1;
  }

  if (!EmergencyPressed){
    stp=0;
  }

}

//Funcion para imprimir mensaje de parado de emergencia
void Inicializar_Boton_Emergencia() {
  pinMode(EmergencyButton, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(EmergencyButton), Fn_Emergency, CHANGE);
}
