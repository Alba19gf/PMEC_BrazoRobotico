#ifndef JSON
#define JSON

#include <WiFi.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include "LcdMenu.h"
#include "Arduino.h"

//int MatrizEstado[3][3]; // Matriz de estado global
extern int MatrizEstado[3][3];
const char* ssid = "ESP_3DEYE";
const char* password = "holahola";
//const char* ssid = "MIM-UNIOVI";
//const char* password = "mimuniovi2018";
const char* host = "192.168.4.4"; // IP servidor MATLAB
const uint16_t port = 55000;

WiFiClient client;

void Fn_MatrizState();
void imprimirMatrizLCD();
void Inicializar_COMS();
int Fn_readSerial();

#endif
