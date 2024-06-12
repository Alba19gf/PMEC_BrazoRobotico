#include "LcdMenu.h"

LiquidCrystal_I2C lcD(0x27, LCD_COLS, LCD_ROWS);

Servo motorPinza;

void Modo_Automatico_Trobot();
void Modo_Automatico_Tusuario();


int ejecutarJuegoSerial() {
    int valorLeido = 0;
    while (Serial.available() == 0) {
        // Espera hasta que haya datos disponibles en la entrada serial
    }
    valorLeido = Serial.parseInt(); // Leer un entero desde la entrada serial
    return valorLeido;
}


void fn_menu(byte pos, String menus[], byte sizemenu, String titulo) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(titulo);

    int startIndex = pos / 3 * 3;

    for (int i = 0; i < 3; i++) {
        int index = startIndex + i;
        if (index < sizemenu) {
            String menuItem = menus[index];
            lcd.setCursor(1, i + 1);
            lcd.print(menuItem);
            if (index == pos) {
                lcd.setCursor(0, i + 1);
                lcd.write(byte(0));
            }
        }
    }
    if (contador >= sizemenu) contador = 0;
    if (contador < 0) contador = sizemenu - 1;
}

void fn_menu1(byte pos, String menus[], byte sizemenu, String titulo) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(titulo);

    int col = (pos / 3) * 4;
    int row = pos % 3 + 1;
    lcd.setCursor(col, row);
    lcd.write(byte(0));

    for (int i = 0; i < sizemenu; i++) {
        col = (i / 3) * 4 + 1;
        row = i % 3 + 1;
        lcd.setCursor(col, row);
        lcd.print(menus[i]);
    }

    global_position = pos + 1; // Actualiza la posición global
    Serial.println("Manual robot: "+String(global_position ));

    if (contador >= sizemenu) contador = 0;
    if (contador < 0) contador = sizemenu - 1;
}



//Funcion para la calibracion de los motores
float calibration (float M_var, int vminC, int vmaxC,int SelecMotor) {
  lcd.setCursor(0, 1);
  lcd.print("Valor: ");
  lcd.print(M_var);
  float temp_M_var;
  float step_cal=0.1;     //Paso para incrementar en la calibracion

  temp_M_var = M_var;
  while (true){

    if (EmergencyPressed) {
      return M_var; // Detener todas las acciones si el botón de emergencia está presionado
    }

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



//Funcion para salir presionando el boton del modo manual turno usuario y robot
void Modo_Automatico_Trobot() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TURNO ROBOT");
  lcd.setCursor(0, 1);
  lcd.print("Robot jugando");

  while (true) {
    Serial.println("Modo automatico funcion");
    if (EmergencyPressed) {
      return; // Detener todas las acciones si el botón de emergencia está presionado
    }

    int valor = ejecutarJuegoSerial();
    //int valor=mov_optimo();

    if (valor<=9)
    {
      Modo_Automatico_Tusuario();
      return;
    }

    else if (valor==10)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("JUEGO EMPATADO");
      return;
    }
    else if (valor==11){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("PERDISTE!");
      return;
    }
    else if (valor==12){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("GANASTE!");
      return;
    }

    if (rotaryEncoder.isEncoderButtonClicked()) {

      static unsigned long lastTimePressed = 0; // Soft debouncing
      if (millis() - lastTimePressed >= 500) {
        Serial.print("button pressed soft");
        
        lastTimePressed = millis();

        return;
      }
    }
  }
  contador = 0;
  level_menu = 3;
}

//Funcion para salir presionando el boton del modo manual turno usuario y robot
void Modo_Automatico_Tusuario() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TURNO USUARIO");
  lcd.setCursor(0, 1);
  lcd.print("Mueve una ficha");

  while (true) {
    Serial.println("Modo automatico turno usuario");

    if (EmergencyPressed) {
      return; // Detener todas las acciones si el botón de emergencia está presionado
    }

    if (rotaryEncoder.isEncoderButtonClicked()) {

      //static unsigned long lastTimePressed = 0; // Soft debouncing
      if (millis() - lastTimePressed >= 50) {
        Serial.print("button pressed soft en Fnmodo automatico usuario");
        
        lastTimePressed = millis();
        Modo_Automatico_Trobot();
        return;
      }
    }
  }
}

//Funcion para salir presionando el boton del modo manual turno usuario
void Modo_Manual_Tusuario(){
  while (true) {

    if (EmergencyPressed) {
      return; // Detener todas las acciones si el botón de emergencia está presionado
    }

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

//Funcion para salir presionando el boton del modo manual turno robot
void Modo_Manual_Trobot() {
  while (true) {

    if (EmergencyPressed) {
      return; // Detener todas las acciones si el botón de emergencia está presionado
    }

    cont=rotaryEncoder.readEncoder() + 1;


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

//Funcion para manejar los menus y submenus de la interfaz
void Robot_Menu() {

  if(EmergencyPressed){
    showEmergencyMessage();
    contador = 0;
    level_menu = 0;
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
            Modo_Automatico_Tusuario();
            contador = 0;
            level_menu = 3;
            //digitalWrite(LED_ROJO, ledRojoEncendido ? HIGH : LOW);
            //digitalWrite(LED_VERDE, LOW);
            break;
          case 1: // T. Robot
            Modo_Automatico_Trobot();
            contador = 0;
            level_menu = 3;
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
            lcd.print("Posicion " + String(global_position));
            Modo_Manual_Trobot();
            contador = 0;
            level_menu = 5; 
            break;
          case 1: 
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("TURNO ROBOT"); 
            lcd.setCursor(0, 1); 
            lcd.print("Posicion " + String(global_position));
            Modo_Manual_Trobot();
            contador = 0;
            level_menu = 5;  
            break;
          case 2:
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("TURNO ROBOT"); 
            lcd.setCursor(0, 1); 
            lcd.print("Posicion " + String(global_position));
            Modo_Manual_Trobot();
            contador = 0;
            level_menu = 5; 
            break;
          case 3: 
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("TURNO ROBOT"); 
            lcd.setCursor(0, 1); 
            lcd.print("Posicion " + String(global_position));
            Modo_Manual_Trobot();
            contador = 0;
            level_menu = 5;  
            break;
          case 4: 
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("TURNO ROBOT"); 
            lcd.setCursor(0, 1); 
            lcd.print("Posicion " + String(global_position));
            Modo_Manual_Trobot(); 
            contador = 0;
            level_menu = 5; 
            break;
          case 5: 
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("TURNO ROBOT"); 
            lcd.setCursor(0, 1); 
            lcd.print("Posicion " + String(global_position));
            Modo_Manual_Trobot();
            contador = 0;
            level_menu = 5; 
            break;
          case 6: 
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("TURNO ROBOT"); 
            lcd.setCursor(0, 1); 
            lcd.print("Posicion " + String(global_position));
            Modo_Manual_Trobot(); 
            contador = 0;
            level_menu = 5; 
            break;
          case 7: 
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("TURNO ROBOT"); 
            lcd.setCursor(0, 1); 
            lcd.print("Posicion " + String(global_position));
            Modo_Manual_Trobot();
            contador = 0;
            level_menu = 5; 
            break;
          case 8: 
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("TURNO ROBOT"); 
            lcd.setCursor(0, 1); 
            lcd.print("Posicion " + String(global_position));
            Modo_Manual_Trobot();
            contador = 0;
            level_menu = 5; 
            break;
          case 9:
            contador = 1;
            level_menu = 5;
            //digitalWrite(LED_VERDE, LOW);
            //digitalWrite(LED_ROJO, LOW);
            break;
        }
        
        final_position = global_position;
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

void Inicializar_LCD() {
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