#ifndef LCD_MENU_H
#define LCD_MENU_H

#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "Arduino.h"
#include "BotonEmergencia.h"
#include "RotaryEncoder.h"
#include "Comunicacion_JSON.h"
#include <ESP32Servo.h>
#include "../lib/TresEnRayaTest.h"
#include "../lib/TresEnRaya.h"

//LCD
#define PIN_SDA 8
#define PIN_SCL 12
#define LCD_COLS 20
#define LCD_ROWS 4
LiquidCrystal_I2C lcd(0x27, LCD_COLS, LCD_ROWS);
int level_menu = 0;  //Iniciamos la variable en el menu principal 
byte flecha[] = { B10000, B11000, B11100, B11110, B11100, B11000, B10000, B00000 }; //Array para el símbolo de la flecha personalizada en el menú

extern LiquidCrystal_I2C lcd;

//Servomotor
#define MOTOR_PINZA_PIN 10
extern Servo motorPinza; // Declara un objeto de tipo Servo para controlar el servomotor de la base

// Variables globales para almacenar el último estado del contador y nivel de menú
int contador=0;
int cont=0;
int cont_pre=0;
int last_contador = -1;
int last_level_menu = -1;
bool btnpress = false;
int global_sizemenu;


extern int MatrizEstado[3][3];
extern bool Pieces[10];

//Calibracion
extern float M_Base, M_Hombro, M_Codo, M_Pinza;

extern int global_position; //Variable para recorrer la posicion de 1 a 9 en el modo manual Turno robot
extern int final_position; //Variable global para guardar almacenar la posición seleccionada en el modo manual Turno robot

//-------------------MENU-------------------//
String menu1[] = { "Calibracion", "M.Automatico", "M.Manual", "Comunicacion"};  //Inicializamos nuestro Array con los elementos del menu princiapl
int sizemenu1 = sizeof(menu1) / sizeof(menu1[0]); 

String menu2[] = { "Motor Base:","Motor Hombro:","Motor Codo:","Pinza:", "Atras" }; //Inicializamos nuestro Array con los elementos del submenu de calibracion
int sizemenu2 = sizeof(menu2) / sizeof(menu2[0]);

String menu3[] = { "Empezar", "Atras" };
int sizemenu3 = sizeof(menu3) / sizeof(menu3[0]); //Inicializamos nuestro Array con los elementos del submenu de modo automatico

String menu4[] = { "T.Usuario", "T.Robot", "Atras" }; 
int sizemenu4 = sizeof(menu4) / sizeof(menu4[0]);

String menu5[] = { "P1","P2","P3","P4","P5","P6","P7","P8","P9","Atras" }; //Inicializamos nuestro Array con los elementos del submenu de modo manual
int sizemenu5 = sizeof(menu5) / sizeof(menu5[0]);

String menu6[] = { "Matriz", "Atras" }; //Inicializamos nuestro Array con los elementos del submenu de modo manual
int sizemenu6 = sizeof(menu6) / sizeof(menu6[0]);

String menu7[] = { "Facil", "Dificil", "Reset", "Atras" }; //Inicializamos nuestro Array con los elementos del submenu de modo manual
int sizemenu7 = sizeof(menu7) / sizeof(menu7[0]);

void fn_menu(byte pos, String menus[], byte sizemenu, String titulo);
void fn_menu1(byte pos, String menus[], byte sizemenu, String titulo);

void Inicializar_LCD();

#endif
