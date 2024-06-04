#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//#include <ESP32Servo.h>

//Encoder
#define salida_A 14
#define salida_B  17
#define swPin  21

bool rotated = true; //info of the rotation
bool btnpress = false; //Esta variable de retorno en false asegurando que el boton del Encoder aun no se ha oprimido

int global_sizemenu;

// Variables globales para almacenar el último estado del contador y nivel de menú
int last_contador = -1;
int last_level_menu = -1;
unsigned long lastRotationTime = 0; // Variable para almacenar el tiempo de la última rotación


//Statuses
int CLKNow;
int CLKPrevious;
int DTNow;
int DTPrevious;

int contador = 0;///0  //Esta variable en 0 se posicionara en el primer elemento del Menu principal
int A_estado_actual;
int A_ultimo_estado;

// Timers
float TimeNow1;
float TimeNow2;

//LCD
#define PIN_SDA 8
#define PIN_SCL 12
#define LCD_COLS 20
#define LCD_ROWS 4

LiquidCrystal_I2C lcd(0x27, LCD_COLS, LCD_ROWS);

String linea1, linea2, linea3;  //Lineas del LCD a utilizar 
int level_menu = 0;  //Iniciamos la variable en el menu principal 
byte flecha[] = { B10000, B11000, B11100, B11110, B11100, B11000, B10000, B00000 }; //Array para el símbolo de la flecha personalizada en el menú

//LEDS
#define LED_ROJO 40
#define LED_VERDE 41
bool ledRojoEncendido = false; // Estado del LED rojo

//SERVO
//#define SERVO_PIN 10  // Por ejemplo, puedes usar el pin 10 para el servo
//Servo servoMotor;

int M_Base = 0;
int temp_Mbase = 0;
//int M_Hombro = 0;
//int M_Codo = 0;
//int Pinza = 0;
//int pos_Base = 0;
//int pos_Hombro = 0;
//int pos_Codo = 0;
//int pos_Pinza =0;

//-------------------MENU-------------------//

String menu1[] = { "Calibracion", "M.Automatico", "M.Manual"};  //Inicializamos nuestro Array con los elementos del menu princiapl
int sizemenu1 = sizeof(menu1) / sizeof(menu1[0]); 

String menu2[] = { "Motor Base:","Motor Hombro:","Motor Codo:","Pinza:", "Atras" }; //Inicializamos nuestro Array con los elementos del submenu de calibracion
int sizemenu2 = sizeof(menu2) / sizeof(menu2[0]);

String menu3[] = { "Empezar", "Atras" };
int sizemenu3 = sizeof(menu3) / sizeof(menu3[0]); //Inicializamos nuestro Array con los elementos del submenu de modo automatico

String menu4[] = { "T.Usuario", "T.Robot", "Atras" }; 
int sizemenu4 = sizeof(menu4) / sizeof(menu4[0]);

String menu5[] = { "P1","P2","P3","P4","P5","P6","P7","P8","P9","Atras" }; //Inicializamos nuestro Array con los elementos del submenu de modo manual
int sizemenu5 = sizeof(menu5) / sizeof(menu5[0]);

//..........................................................................................................................
//Funciones adicionales
//..........................................................................................................................

//Función para detectar si el botón del codificador ha sido presionado
void selectOption() {
//This timer is a "software debounce". It is not the most effective solution, but it works
  TimeNow2 = millis();
  if(TimeNow2 - TimeNow1 > 500)
  {    
    btnpress = true;    
  }
  TimeNow1 = millis();  //"reset" timer; the next 500 ms is counted from this moment
}

//Función que actualiza el LCD con el menú actual
void fn_menu(byte pos, String menus[], byte sizemenu, String titulo) {
  lcd.clear();
  linea1 = "";
  linea2 = "";
  linea3 = "";

  // Imprimir el título en la fila 0 columna 0
  lcd.setCursor(0, 0);
  lcd.print(titulo);

  int startIndex = pos / 3 * 3; // Índice de inicio de las líneas a imprimir

  for (int i = 0; i < 3; i++) {
    int index = startIndex + i;
    if (index < sizemenu) {
      String menuItem = menus[index];
      lcd.setCursor(1, i + 1); // Columna 1, filas 1, 2, 3
      lcd.print(menuItem);
      if (index == pos) {
        lcd.setCursor(0, i + 1); // Columna 0, filas 1, 2, 3
        lcd.write(byte(0)); // Flecha
      }
    }
  }
}

//Funcion para desplazarse dentro del submenu Modo manual
void fn_menu1(byte pos, String menus[], byte sizemenu,  String titulo) {
  lcd.clear(); // Limpia la pantalla LCD
  linea1 = "";
  linea2 = "";
  linea3 = "";

  // Imprimir el título en la fila 0 columna 0
  lcd.setCursor(0, 0);
  lcd.print(titulo);

  // Calcula la columna y fila para la posición actual del cursor
  int col = (pos / 3) * 4;
  int row = pos % 3 + 1;

  // Mueve el cursor a la posición actual y escribe la flecha
  lcd.setCursor(col, row);
  lcd.write(byte(0));

  // Itera sobre todas las opciones del menú
  for (int i = 0; i < sizemenu; i++) {
    // Calcula la columna y fila para la opción actual
    int col = (i / 3) * 4 + 1;
    int row = i % 3 + 1;

    // Mueve el cursor a la posición de la opción actual y escribe el texto del menú
    lcd.setCursor(col, row);
    lcd.print(menus[i]);
  }
}
void fn_encoder() {
  unsigned long currentTime = millis(); // Obtener el tiempo actual en milisegundos

  // Verificar si ha pasado suficiente tiempo desde la última rotación
  if (currentTime - lastRotationTime > 400) { // Ajusta el valor 100 según tus necesidades de retardo
    CLKNow = digitalRead(salida_A); // Leer el estado del pin CLK

    // Si el estado actual de CLK es diferente al anterior, se ha producido un pulso
    if (CLKNow != CLKPrevious && CLKNow == 1) {
      // Si el estado de DT es diferente al estado de CLK, el encoder está girando en sentido antihorario
      if (digitalRead(salida_B) != CLKNow) {
        contador++;
        if (contador >= global_sizemenu) {
          contador = 0;
        }
      } else { // Si no, el encoder está girando en sentido horario
        contador--;
        if (contador < 0) {
          contador = global_sizemenu - 1;
        }
      }
      // Actualizar el tiempo de la última rotación
      lastRotationTime = currentTime;
    }
  }

  CLKPrevious = CLKNow; // Almacenar el estado anterior de CLK
  rotated = true; // Indicar que se ha producido una rotación
}
void Robot_Menu() {
  // Muestra el menú correspondiente según el nivel actual si ha cambiado
  if (contador != last_contador || level_menu != last_level_menu) {
    last_contador = contador;
    last_level_menu = level_menu;

    switch (level_menu) {
      case 0: // Menu Principal
        global_sizemenu = sizemenu1;
        fn_menu(contador, menu1, sizemenu1, "MENU PRINCIPAL");
        break;

      case 1: // SubMenu Calibracion
        digitalWrite(LED_VERDE, LOW);
        digitalWrite(LED_ROJO, LOW);
        global_sizemenu = sizemenu2;
        fn_menu(contador, menu2, sizemenu2, "MENU CALIBRACION");
        break;

      case 2: // SubMenu Modo Automatico
        digitalWrite(LED_VERDE, LOW);
        digitalWrite(LED_ROJO, LOW);
        global_sizemenu = sizemenu3;
        fn_menu(contador, menu3, sizemenu3, "MENU M.AUTOMATICO");
        break;

      case 3: // SubMenu Empezar del Modo Automatico
        global_sizemenu = sizemenu4;
        fn_menu(contador, menu4, sizemenu4, "MENU M.AUTOMATICO");
        break;

      case 4: // SubMenu Modo Manual
        digitalWrite(LED_VERDE, LOW);
        digitalWrite(LED_ROJO, LOW);
        global_sizemenu = sizemenu3;
        fn_menu(contador, menu3, sizemenu3, "MENU M.MANUAL");
        break;

      case 5: // SubMenu Empezar del Modo Manual
        global_sizemenu = sizemenu4;
        fn_menu(contador, menu4, sizemenu4, "MENU M.MANUAL");
        break;

      case 6: // SubMenu Empezar del Modo Manual (detallado)
        digitalWrite(LED_VERDE, LOW);
        digitalWrite(LED_ROJO, LOW);
        global_sizemenu = sizemenu5;
        fn_menu1(contador, menu5, sizemenu5, "MENU M.MANUAL");
        break;
    }
  }

  // Maneja la lógica de los botones cuando se presiona el botón
  if (btnpress) {
    switch (level_menu) {
      case 0: // Menu Principal
        switch (contador) {
          case 0: // Calibracion
            contador = 0;
            level_menu = 1;
            break;
          case 1: // Modo Automatico
            contador = 0;
            level_menu = 2;
            break;
          case 2: // Modo Manual
            contador = 0;
            level_menu = 4;
            break;
        }
        break;

      case 1: // SubMenu Calibracion
        switch (contador) {
          case 0: // Motor Base
            contador = M_Base;
            lcd.clear();
            lcd.setCursor(0, 1);
            lcd.print("Ajuste la posicion");
            while (digitalRead(swPin) == HIGH) {
              //fn_programar_tiempo();
            }
            
            temp_Mbase = M_Base;
            break;
          case 1: // Motor Hombro
          case 2: // Motor Codo
          case 3: // Pinza
            lcd.clear();
            lcd.setCursor(0, 1);
            lcd.print("Ajuste la posicion");
            break;
          case 4: // Atras
            contador = 0;
            level_menu = 0;
            digitalWrite(LED_ROJO, HIGH);
            digitalWrite(LED_VERDE, HIGH);
            break;
        }
        break;

      case 2: // SubMenu Modo Automatico
        switch (contador) {
          case 0: // Empezar
            contador = 0;
            level_menu = 3;
            break;
          case 1: // Atras
            contador = 1;
            level_menu = 0;
            digitalWrite(LED_ROJO, HIGH);
            digitalWrite(LED_VERDE, HIGH);
            break;
        }
        break;

      case 3: // SubMenu Empezar del Modo Automatico
        switch (contador) {
          case 0: // T. Usuario
            lcd.clear();
            lcd.setCursor(0, 1);
            lcd.print("MUEVE UNA FICHA");
            ledRojoEncendido = !ledRojoEncendido;
            digitalWrite(LED_ROJO, ledRojoEncendido ? HIGH : LOW);
            digitalWrite(LED_VERDE, LOW);
            break;
          case 1: // T. Robot
            lcd.clear();
            lcd.setCursor(0, 1);
            lcd.print("Posicion 2");
            digitalWrite(LED_VERDE, HIGH);
            digitalWrite(LED_ROJO, LOW);
            break;
          case 2: // Atras
            contador = 0;
            level_menu = 2;
            digitalWrite(LED_VERDE, LOW);
            digitalWrite(LED_ROJO, LOW);
            break;
        }
        break;

      case 4: // SubMenu Modo Manual
        switch (contador) {
          case 0: // Empezar
            contador = 0;
            level_menu = 5;
            break;
          case 1: // Atras
            contador = 2;
            level_menu = 0;
            digitalWrite(LED_ROJO, HIGH);
            digitalWrite(LED_VERDE, HIGH);
            break;
        }
        break;

      case 5: // SubMenu Empezar del Modo Manual
        switch (contador) {
          case 0: // T. Usuario
            lcd.clear();
            lcd.setCursor(0, 1);
            lcd.print("Posicion 1");
            digitalWrite(LED_ROJO, HIGH);
            digitalWrite(LED_VERDE, LOW);
            break;
          case 1: // T. Robot
            contador = 0;
            level_menu = 6;
            digitalWrite(LED_VERDE, HIGH);
            digitalWrite(LED_ROJO, LOW);
            break;
          case 2: // Atras
            contador = 0;
            level_menu = 4;
            digitalWrite(LED_VERDE, LOW);
            digitalWrite(LED_ROJO, LOW);
            break;
        }
        break;

      case 6: // SubMenu Empezar del Modo Manual (detallado)
        switch (contador) {
          case 0: lcd.clear(); lcd.setCursor(0, 1); lcd.print("Posicion 1"); break;
          case 1: lcd.clear(); lcd.setCursor(0, 1); lcd.print("Posicion 2"); break;
          case 2: lcd.clear(); lcd.setCursor(0, 1); lcd.print("Posicion 3"); break;
          case 3: lcd.clear(); lcd.setCursor(0, 1); lcd.print("Posicion 4"); break;
          case 4: lcd.clear(); lcd.setCursor(0, 1); lcd.print("Posicion 5"); break;
          case 5: lcd.clear(); lcd.setCursor(0, 1); lcd.print("Posicion 6"); break;
          case 6: lcd.clear(); lcd.setCursor(0, 1); lcd.print("Posicion 7"); break;
          case 7: lcd.clear(); lcd.setCursor(0, 1); lcd.print("Posicion 8"); break;
          case 8: lcd.clear(); lcd.setCursor(0, 1); lcd.print("Posicion 9"); break;
          case 9:
            contador = 1;
            level_menu = 5;
            digitalWrite(LED_VERDE, LOW);
            digitalWrite(LED_ROJO, LOW);
            break;
        }
        break;
    }
    btnpress = false;
  }
}

void setup() {
  Serial.begin(9600);  //Iniciamos conexion Serial Usb

  //Enconder
  pinMode(salida_A, INPUT);
  pinMode(salida_B, INPUT);
  pinMode(swPin, INPUT_PULLUP);

  //Store states
  //CLKPrevious = digitalRead(salida_A);
  //CLKPrevious = digitalRead(salida_B);
    
  attachInterrupt(digitalPinToInterrupt(salida_A), fn_encoder, RISING);
  attachInterrupt(digitalPinToInterrupt(swPin), selectOption, FALLING); //either falling or rising but never "change".

  TimeNow1 = millis(); //Start timer 1
  
  //LCD
  Wire.begin(PIN_SDA, PIN_SCL);
  lcd.init();
  lcd.backlight(); //Encender la luz de fondo.
  lcd.createChar(0, flecha); 
  
  //Iniciar en menu principal
  fn_menu(contador, menu1, sizemenu1, "MENU PRINCIPAL"); //Iniciamos presentando el menu principal 
  CLKPrevious = digitalRead(salida_A); //Leemos el estado de la salida del Encoder usando el pin CLK

  // Inicializar pines de los LEDs como salida
  pinMode(LED_ROJO, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);

  // Encender ambos LEDs en el menú principal
  digitalWrite(LED_ROJO, HIGH);
  digitalWrite(LED_VERDE, HIGH);

  //Servomotor
  //servoMotor.attach(SERVO_PIN);
}

void loop() {
  Robot_Menu();
}