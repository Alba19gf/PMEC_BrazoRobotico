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
const char* host = "192.168.4.2"; // Cambia esto por la IP de tu servidor MATLAB
const uint16_t port = 55000;

WiFiClient client;

void Fn_MatrizState();
void imprimirMatrizLCD();
void Inicializar_COMS();

#endif
