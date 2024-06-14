#ifndef BOTON_EMERGENCIA
#define BOTON_EMERGENCIA

#include "LcdMenu.h"
#include "Arduino.h"



//ENCODER
//paro de emergencia
const int EmergencyButton = 16; // Pin del botón
volatile bool EmergencyPressed = false; // Variable para almacenar el estado del botón

void Fn_Emergency();
void showEmergencyMessage();
void Inicializar_Boton_Emergencia();

#endif
