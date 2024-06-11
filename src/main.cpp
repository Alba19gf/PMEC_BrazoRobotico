#include "AiEsp32RotaryEncoder.h"
#include "Arduino.h"
#include <LiquidCrystal_I2C.h>
#include <ESP32Servo.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>

//WIFI
const char* ssid = "ESP_3DEYE";
const char* password = "holahola";
const char* host = "192.168.4.3"; // IP servidor MATLAB
const uint16_t port = 55000;

WiFiClient client;


//ENCODER
#define ROTARY_ENCODER_A_PIN 17  //dt
#define ROTARY_ENCODER_B_PIN 14 //clk
#define ROTARY_ENCODER_BUTTON_PIN 21  //sw
#define ROTARY_ENCODER_VCC_PIN -1
#define ROTARY_ENCODER_STEPS 4
//instead of changing here, rather change numbers above
AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_BUTTON_PIN, ROTARY_ENCODER_VCC_PIN, ROTARY_ENCODER_STEPS);
static unsigned long lastTimePressed = 0; // Soft debouncing


//Servomotor
#define MOTOR_PINZA_PIN 10
Servo motorPinza; // Declara un objeto de tipo Servo para controlar el servomotor de la base

// Variables globales para almacenar el último estado del contador y nivel de menú
int contador=0;
int cont=0;
int cont_pre=0;
int last_contador = -1;
int last_level_menu = -1;
bool btnpress = false;
int global_sizemenu;
const int buttonPin = 15; // Pin del botón
int buttonState = 0;
int MatrizEstado[3][3]; // Matriz de estado global

//LCD
#define PIN_SDA 8
#define PIN_SCL 12
#define LCD_COLS 20
#define LCD_ROWS 4

//paro de emergencia
const int EmergencyButton = 16; // Pin del botón
volatile bool EmergencyPressed = false; // Variable para almacenar el estado del botón

LiquidCrystal_I2C lcd(0x27, LCD_COLS, LCD_ROWS);
int level_menu = 0;  //Iniciamos la variable en el menu principal 
byte flecha[] = { B10000, B11000, B11100, B11110, B11100, B11000, B10000, B00000 }; //Array para el símbolo de la flecha personalizada en el menú

//Calibracion
float M_Base = 0, M_Hombro=0, M_Codo=0, M_Pinza=0;

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

//===========================================FUNCIONES===========================================================================================================

void Fn_Emergency() {
  
  EmergencyPressed = digitalRead(EmergencyButton); // Lee el estado del botón
}

void showEmergencyMessage() {
    lcd.setCursor(0, 0);
    lcd.print("Parada de emergencia");
    } 






//COMUNICACION JSON
void Fn_MatrizState() {
  int tries=3, tr;

  if (client.connect(host, port)) {
    Serial.println("Conectado al servidor");
    
    // Enviar solicitud al servidor
    client.println("Solicitando matriz");
      if (!client.connected()) {
      Serial.println("Conexión perdida. Intentando reconectar...");
      if (client.connect(host, port)) {
        Serial.println("Conexión establecida.");
      } else {
        Serial.println("Error al conectar al servidor.");
        delay(1000);
        return;
      }
    }
    
    // Leer respuesta del servidor
    String jsonData = client.readStringUntil('\n');
    client.println("Solicitando matriz");
    
    while (jsonData=="" && tr<=tries){
      Serial.print(tr);
      Serial.println(" jsonEmpty");
      jsonData = client.readStringUntil('\n');
      tr++;
    }

    // Parsear JSON y almacenar la matriz en MatrizEstado
    DynamicJsonDocument doc(256);
    DeserializationError error = deserializeJson(doc, jsonData);
    
    if (error) {
      Serial.print("Error al deserializar JSON: ");
      Serial.println(error.c_str());
      jsonData = client.readStringUntil('\n');
      Serial.println("Matriz recibida2: ");
      Serial.println(jsonData);
      //Parsear JSON y almacenar la matriz en MatrizEstado
      DynamicJsonDocument doc(256);
      DeserializationError error = deserializeJson(doc, jsonData);
      client.stop();
      return;
    }

    JsonArray jsonArray = doc.as<JsonArray>();
    int i = 0;
    for (JsonArray::iterator it = jsonArray.begin(); it != jsonArray.end(); ++it) {
      JsonArray row = *it;
      int j = 0;
      for (JsonArray::iterator it2 = row.begin(); it2 != row.end(); ++it2) {
        MatrizEstado[i][j] = (*it2).as<int>();
        j++;
      }
      i++;
    }
    
    client.stop();
  } else {
    Serial.println("Conexión fallida");
  }
}

//Función que actualiza el LCD con el menú actual
void fn_menu(byte pos, String menus[], byte sizemenu, String titulo) {
  lcd.clear();
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
  if (contador>=sizemenu)
  contador=0;
  if (contador<0)
  contador=sizemenu-1;
}

//Funcion para desplazarse dentro del submenu Modo manual
void fn_menu1(byte pos, String menus[], byte sizemenu,  String titulo) {
  lcd.clear(); // Limpia la pantalla LCD
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
    if (contador>=sizemenu)
  contador=0;
  if (contador<0)
  contador=sizemenu-1;
}

//Funcion presionar boton encoder
void rotary_onButtonClick() {
  btnpress = true;

  static unsigned long lastTimePressed = 0; // Soft debouncing
  if (millis() - lastTimePressed < 500)
  {
    return;
  }
  lastTimePressed = millis();
  Serial.print("button pressed ");
  Serial.print(millis());
  Serial.println(" milliseconds after restart");
}

void rotary_loop() {
    
  //dont print anything unless value changed
  if (rotaryEncoder.encoderChanged())
  {
    Serial.print("Value: ");
    Serial.println(rotaryEncoder.readEncoder());
    cont=(rotaryEncoder.readEncoder());
    if (cont>cont_pre)
    {
      contador++;
    }
    if (cont<cont_pre)
    {
      contador--;
    }
    cont_pre=cont;
  }
  if (rotaryEncoder.isEncoderButtonClicked())
  {
    rotary_onButtonClick();
  }
}

void IRAM_ATTR readEncoderISR() {
  rotaryEncoder.readEncoder_ISR();
}

float calibration (float M_var, int vminC, int vmaxC,int SelecMotor) {
  lcd.setCursor(0, 1);
  lcd.print("Valor: ");
  lcd.print(M_var);
  float temp_M_var;
  float step_cal=0.1;     //Paso para incrementar en la calibracion

  temp_M_var = M_var;
  while (true){
    if (rotaryEncoder.encoderChanged()) {

      cont=rotaryEncoder.readEncoder();
      if (cont>cont_pre && M_var < vmaxC) {
      M_var=M_var+step_cal;   

      }

      if (cont<cont_pre && M_var > vminC){

      M_var=M_var-step_cal;

      }
      cont_pre=cont;
      if (SelecMotor == 1) {
        //motorPinza.write(M_var);
      } 
      else if (SelecMotor == 2) {
       // motorPinza.write(M_var);
      } 
      else if (SelecMotor == 3) {
        //motorPinza.write(M_var);
      } 
      else if (SelecMotor == 4) {
        motorPinza.write(M_var*10);
      }

      Serial.println(M_var);
      lcd.setCursor(0, 1);
      lcd.print("Valor: "+String(M_var) +"     ");
      //step_cal = 0.1 + abs(cont - cont_pre) * 10000;

    }  
    if (rotaryEncoder.isEncoderButtonClicked()) {

      //static unsigned long lastTimePressed = 0; // Soft debouncing
      if (millis() - lastTimePressed >= 500) {
        Serial.print("button pressed soft");
        temp_M_var = M_var; // Guarda el valor final al salir del bucle
        lastTimePressed = millis();
        return M_var;
      }
    }
  }
  contador=0;
  level_menu=1;
}

void Modo_Manual_Tusuario(){
  while (true) {
    if (rotaryEncoder.isEncoderButtonClicked()) {

      //static unsigned long lastTimePressed = 0; // Soft debouncing
      if (millis() - lastTimePressed >= 500) {
        Serial.print("button pressed soft");
        
        lastTimePressed = millis();
        return;
      }
    }
  }
}

void Modo_Manual() {
    while (true) {
    if (rotaryEncoder.isEncoderButtonClicked()) {

      //static unsigned long lastTimePressed = 0; // Soft debouncing
      if (millis() - lastTimePressed >= 500) {
        Serial.print("button pressed soft");
        
        lastTimePressed = millis();
        return;
      }
    }
    // Lógica para abrir y cerrar la pinza automáticamente
    for (int angle = 0; angle <= 35; angle++) {
      motorPinza.write(angle);
    }
    
    for (int angle = 35; angle >= 0; angle--) {
      motorPinza.write(angle);
    }
    
  }
  contador=0;
  level_menu=6;
}

void Modo_Automatico() {
  while (true) {
    if (rotaryEncoder.isEncoderButtonClicked()) {

      //static unsigned long lastTimePressed = 0; // Soft debouncing
      if (millis() - lastTimePressed >= 500) {
        Serial.print("button pressed soft");
        
        lastTimePressed = millis();
        return;
      }
    }
  }
}


// Función para imprimir la matriz en la pantalla LCD
void imprimirMatrizLCD() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Estado de juego:");

  for (int i = 0; i < 3; i++) {
    lcd.setCursor(0, i + 1); // Cambia de fila
    for (int j = 0; j < 3; j++) {
      char characterToPrint;
      switch (MatrizEstado[i][j]) {
        case 0:
          characterToPrint = 'O';
          break;
        case 1:
          characterToPrint = 'X';
          break;
        case 2:
          characterToPrint = '-';
          break;
      }
      lcd.print(characterToPrint);
      lcd.print(" "); // Espacio entre caracteres
    }
  }
}
//Funcion para manejar los menus y submenus de la interfaz
void Robot_Menu() {
  if(EmergencyPressed){
    showEmergencyMessage();
    return;
  }
  // Muestra el menú correspondiente según el nivel actual si ha cambiado
  if (contador != last_contador || level_menu != last_level_menu) {
    last_contador = contador;
    last_level_menu = level_menu;

    switch (level_menu) {
      case 0: // Menu Principal
        global_sizemenu = sizemenu1;
        fn_menu(contador, menu1, sizemenu1, "MENU PRINCIPAL");
        Serial.println(sizemenu1);
        break;

      case 1: // SubMenu Calibracion
        //digitalWrite(LED_VERDE, LOW);
        //digitalWrite(LED_ROJO, LOW);
        global_sizemenu = sizemenu2;
        fn_menu(contador, menu2, sizemenu2, "MENU CALIBRACION");
        break;

      case 2: // SubMenu Modo Automatico
        //digitalWrite(LED_VERDE, LOW);
        //digitalWrite(LED_ROJO, LOW);
        global_sizemenu = sizemenu3;
        fn_menu(contador, menu3, sizemenu3, "MENU M.AUTOMATICO");
        break;

      case 3: // SubMenu Empezar del Modo Automatico
        global_sizemenu = sizemenu4;
        fn_menu(contador, menu4, sizemenu4, "MENU M.AUTOMATICO");
        break;

      case 4: // SubMenu Modo Manual
        //digitalWrite(LED_VERDE, LOW);
        //digitalWrite(LED_ROJO, LOW);
        global_sizemenu = sizemenu3;
        fn_menu(contador, menu3, sizemenu3, "MENU M.MANUAL");
        break;

      case 5: // SubMenu Empezar del Modo Manual
        global_sizemenu = sizemenu4;
        fn_menu(contador, menu4, sizemenu4, "MENU M.MANUAL");
        break;

      case 6: // SubMenu Empezar del Modo Manual (detallado)
       // digitalWrite(LED_VERDE, LOW);
        //digitalWrite(LED_ROJO, LOW);
        global_sizemenu = sizemenu5;
        fn_menu1(contador, menu5, sizemenu5, "MENU M.MANUAL");
        break;
      
      case 7: // SubMenu Conexion
        global_sizemenu = sizemenu6;
        fn_menu(contador, menu6, sizemenu6, "MENU CONEXION");
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
          case 3: // Comunicacion 
            contador = 0;
            level_menu = 7;
            break;
        }
        break;

      case 1: // SubMenu Calibracion
        switch (contador) {
          case 0: // Motor Base
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("CALIBRAR BASE       ");
            M_Base=calibration(M_Base,-20,20,1);
            break;
          case 1: // Motor Hombro
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("CALIBRAR HOMBRO     ");
            M_Hombro=calibration(M_Hombro,0,27,2);
            break;
          case 2: // Motor Codo
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("CALIBRAR M.CODO      ");
            M_Codo=calibration(M_Codo,65,100,3);
            break;
          case 3: // Pinza
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("CALIBRAR PINZA       ");
            M_Pinza=calibration(M_Pinza,0,180,4);
            break;
          case 4: // Atras
            contador = 0;
            level_menu = 0;
            //digitalWrite(LED_ROJO, HIGH);
            //digitalWrite(LED_VERDE, HIGH);
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
            //digitalWrite(LED_ROJO, HIGH);
            //digitalWrite(LED_VERDE, HIGH);
            break;
        }
        break;

      case 3: // SubMenu Empezar del Modo Automatico
        switch (contador) {
          case 0: // T. Usuario
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("TURNO USUARIO");
            lcd.setCursor(0, 1);
            lcd.print("Mueve una ficha");
            
            //digitalWrite(LED_ROJO, ledRojoEncendido ? HIGH : LOW);
            //digitalWrite(LED_VERDE, LOW);
            break;
          case 1: // T. Robot
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("TURNO ROBOT");
            lcd.setCursor(0, 1);
            lcd.print("Robot jugando");
            //digitalWrite(LED_VERDE, HIGH);
            //digitalWrite(LED_ROJO, LOW);
            break;
          case 2: // Atras
            contador = 0;
            level_menu = 2;
            //digitalWrite(LED_VERDE, LOW);
            //digitalWrite(LED_ROJO, LOW);
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
            //digitalWrite(LED_ROJO, HIGH);
            //digitalWrite(LED_VERDE, HIGH);
            break;
        }
        break;

      case 5: // SubMenu Empezar del Modo Manual
        switch (contador) {
          case 0: // T. Usuario
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("TURNO USUARIO");
            lcd.setCursor(0, 1);
            lcd.print("Mueve una ficha");
            Modo_Manual_Tusuario();
            contador = 0;
            level_menu = 6;
            //digitalWrite(LED_ROJO, HIGH);
            //digitalWrite(LED_VERDE, LOW);
            break;
          case 1: // T. Robot
            contador = 0;
            level_menu = 6;
            //digitalWrite(LED_VERDE, HIGH);
            //digitalWrite(LED_ROJO, LOW);
            break;
          case 2: // Atras
            contador = 0;
            level_menu = 4;
            //digitalWrite(LED_VERDE, LOW);
            //digitalWrite(LED_ROJO, LOW);
            break;
        }
        break;
      
      case 6: // SubMenu T.ROBOT Modo Manual
        switch (contador) {
          case 0: 
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("TURNO ROBOT"); 
            lcd.setCursor(0, 1); 
            lcd.print("Posicion 1");
            Modo_Manual(); 
            break;
          case 1: 
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("TURNO ROBOT"); 
            lcd.setCursor(0, 1); 
            lcd.print("Posicion 2");
            Modo_Manual(); 
            break;
          case 2:
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("TURNO ROBOT"); 
            lcd.setCursor(0, 1); 
            lcd.print("Posicion 3");
            Modo_Manual(); 
            break;
          case 3: 
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("TURNO ROBOT"); 
            lcd.setCursor(0, 1); 
            lcd.print("Posicion 4");
            Modo_Manual(); 
            break;
          case 4: 
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("TURNO ROBOT"); 
            lcd.setCursor(0, 1); 
            lcd.print("Posicion 5");
            Modo_Manual(); 
            break;
          case 5: 
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("TURNO ROBOT"); 
            lcd.setCursor(0, 1); 
            lcd.print("Posicion 6");
            Modo_Manual(); 
            break;
          case 6: 
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("TURNO ROBOT"); 
            lcd.setCursor(0, 1); 
            lcd.print("Posicion 7");
            Modo_Manual(); 
            break;
          case 7: 
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("TURNO ROBOT"); 
            lcd.setCursor(0, 1); 
            lcd.print("Posicion 8");
            Modo_Manual(); 
            break;
          case 8: 
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("TURNO ROBOT"); 
            lcd.setCursor(0, 1); 
            lcd.print("Posicion 9");
            Modo_Manual(); 
            break;
          case 9:
            contador = 1;
            level_menu = 5;
            //digitalWrite(LED_VERDE, LOW);
            //digitalWrite(LED_ROJO, LOW);
            break;
        }
        break;

      case 7: //===============================================================================menu etsado
        switch (contador) {
          case 0: // Conexion
          Fn_MatrizState();
          imprimirMatrizLCD();



            break;
          case 1: // T. Robot
            contador = 0;
            level_menu = 0;
            //digitalWrite(LED_VERDE, HIGH);
            //digitalWrite(LED_ROJO, LOW);
            break;
        }
        break;


    }
    btnpress = false;
  }
}



void setup()
{
  // WIFI CONECTION
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  pinMode(buttonPin, INPUT_PULLUP); // Configurar el pin del botón
  pinMode(EmergencyButton, INPUT_PULLUP);
   // Configura la interrupción para detectar cambios en el pin del botón
  attachInterrupt(digitalPinToInterrupt(EmergencyButton), Fn_Emergency, CHANGE);

  //we must initialize rotary encoder
  rotaryEncoder.begin();
  rotaryEncoder.setup(readEncoderISR);
  bool circleValues = false;
  rotaryEncoder.setBoundaries(0, 1000, true); //minValue, maxValue, circleValues true|false (when max go to min and vice versa)
  rotaryEncoder.setAcceleration(100); //or set the value - larger number = more accelearation; 0 or 1 means disabled acceleration
  Serial.println("Initialized");

  //LCD
  Wire.begin(PIN_SDA, PIN_SCL);
  lcd.init();
  lcd.backlight(); //Encender la luz de fondo.
  lcd.createChar(0, flecha); 

  //Iniciar en menu principal
  fn_menu(contador, menu1, sizemenu1, "MENU PRINCIPAL"); //Iniciamos presentando el menu principal 
  //Servomotor
  motorPinza.attach(MOTOR_PINZA_PIN);
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
  Serial.print("Emergency stop");
  }


}