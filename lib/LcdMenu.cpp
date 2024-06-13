#include "LcdMenu.h"

LiquidCrystal_I2C lcD(0x27, LCD_COLS, LCD_ROWS);

Servo motorPinza;

void Modo_Automatico_Trobot();
void Modo_Automatico_Tusuario();
void Modo_Manual_Trobot();
void Modo_Manual_Tusuario();
void Modo_Automatico_Trobot_Dificl();
void Modo_Automatico_Tusuario_Dificil();
void PosicionManual();


int ejecutarJuegoSerial() {
    int valorLeido = 0;
    while (Serial.available() == 0) {
        // Espera hasta que haya datos disponibles en la entrada serial
    }
    valorLeido = Serial.parseInt(); // Leer un entero desde la entrada serial
    return valorLeido;
}

//Función que actualiza el LCD con el menú actual
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

//Funcion para desplazarse dentro del submenu Modo manual, Turno Robot
void fn_menu1(byte pos, String menus[], byte sizemenu, String titulo) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(titulo);
    int seleccion;

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

    //global_position = pos + 1; // Actualiza la posición global
    if (pos==0) global_position=1;
    else if (pos==1) global_position=4;
    else if (pos==2) global_position=7;
    else if (pos==3) global_position=2;
    else if (pos==4) global_position=5;
    else if (pos==5) global_position=8;
    else if (pos==6) global_position=3;
    else if (pos==7) global_position=6;
    else if (pos==8) global_position=9;
    //Serial.println("Manual robot: "+String(seleccion));

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

//Funcion para jugar el modo facil dentro del modo automatico
void Modo_Automatico_Trobot() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TURNO ROBOT");
  lcd.setCursor(0, 1);
  lcd.print("Robot jugando");
  digitalWrite(led_red, HIGH);
  digitalWrite(led_green, LOW);

  while (true) {
    Serial.println("Modo automatico funcion"); //modo facil
    if (EmergencyPressed) {
      return; // Detener todas las acciones si el botón de emergencia está presionado
    }

    //int valor = ejecutarJuegoSerial();
    int valor = ComputadoraMovimientoEasy();  // funcion de 3 en raya basico

    //int valor = PosicionOptima();
    
    Serial.println(valor);
    

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
      Serial.println("JUEGO EMPATADO");
      return;
    }
    else if (valor==11){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("PERDISTE!");
      Serial.println("GANO ROBOT");
      return;
    }
    else if (valor==12){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("GANASTE!");
      Serial.println("GANO USUARIO");
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

//Funcion para jugar el modo dificil dentro del modo automatico
void Modo_Automatico_Trobot_Dificl() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TURNO ROBOT");
  lcd.setCursor(0, 1);
  lcd.print("Robot jugando");
  digitalWrite(led_red, HIGH);
  digitalWrite(led_green, LOW);
  while (true) {
    Serial.println("Modo automatico funcion");
    if (EmergencyPressed) {
      return; // Detener todas las acciones si el botón de emergencia está presionado
    }

    //int valor = ejecutarJuegoSerial();
    int valor = ComputadoraMovimiento();  // funcion de 3 en raya basico

    //int valor = PosicionOptima();
    
    Serial.println(valor);
    

    if (valor<=9)
    {
      Modo_Automatico_Tusuario_Dificil();
      return;
    }

    else if (valor==10)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("JUEGO EMPATADO");
      Serial.println("JUEGO EMPATADO");
      return;
    }
    else if (valor==11){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("PERDISTE!");
      Serial.println("GANO ROBOT");
      return;
    }
    else if (valor==12){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("GANASTE!");
      Serial.println("GANO USUARIO");
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
  level_menu = 8;
}

//Funcion para ejecutar el movimiento del turno usurio dentro del modo automatico, modo facil
void Modo_Automatico_Tusuario() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TURNO USUARIO");
  lcd.setCursor(0, 1);
  lcd.print("Mueve una ficha");
  digitalWrite(led_red, LOW);
  digitalWrite(led_green, HIGH);

  while (true) {
    //Serial.println("Modo automatico turno usuario");

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

//Funcion para ejecutar el movimiento del turno usurio dentro del modo automatico, modo dificil
void Modo_Automatico_Tusuario_Dificil() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TURNO USUARIO");
  lcd.setCursor(0, 1);
  lcd.print("Mueve una ficha");
  digitalWrite(led_red, LOW);
  digitalWrite(led_green, HIGH);

  while (true) {
    //Serial.println("Modo automatico turno usuario");

    if (EmergencyPressed) {
      return; // Detener todas las acciones si el botón de emergencia está presionado
    }

    if (rotaryEncoder.isEncoderButtonClicked()) {

      //static unsigned long lastTimePressed = 0; // Soft debouncing
      if (millis() - lastTimePressed >= 50) {
        Serial.print("button pressed soft en Fnmodo automatico usuario");
        
        lastTimePressed = millis();
        Modo_Automatico_Trobot_Dificl();
        return;
      }
    }
  }
}

// //Funcion para ejecutar el movimiento del turno usurio dentro del modo manual
// void Modo_Manual_Tusuario(){  //FICHA X
//   while (true) {

//     if (EmergencyPressed) {
//       return; // Detener todas las acciones si el botón de emergencia está presionado
//     }

//     if (rotaryEncoder.isEncoderButtonClicked()) {

//       //static unsigned long lastTimePressed = 0; // Soft debouncing
//       if (millis() - lastTimePressed >= 500) {
//         Serial.print("button pressed soft");
//         lastTimePressed = millis();
//         return;
//       }
//     }
//   }
// }

void ComandoMovimiento(int position,int ficha)     //ficha 0 para azul         1 para rosa
{
  if (position < 10 && position != final_position)
  {
    //Serial.println("Move to position: " + String(position));
    final_position = position;
  }

  Moving=1;
  Fn_MatrizState();
  int result = checkWinsf(MatrizEstado);
  if (result==10)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("JUEGO EMPATADO");
      Serial.println("JUEGO EMPATADO");
      return;
    }
    else if (result==11){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("GANO 0!");
      Serial.println("GANO 0");
      return;
    }
    else if (result==12){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("GANO X!");
      Serial.println("GANO X");
      return;
    }

  int from=Movefrom(ficha);
  //Serial.println(from);
  //Serial.print("Mover a la posicion:");
  //Serial.println(global_position);

  int valor=Mover_pieza(from,global_position);


  if (valor==1){
        return;           
  }
  

}  




//Funcion para ejecutar el movimiento del turno robot dentro del modo manual
// void Modo_Manual_Trobot() {         //FICHA O
//   while (true) {

//     if (EmergencyPressed) {
//       return; // Detener todas las acciones si el botón de emergencia está presionado
//     }
//     if (rotaryEncoder.isEncoderButtonClicked()) {

//       //static unsigned long lastTimePressed = 0; // Soft debouncing
//       if (millis() - lastTimePressed >= 500) {
//         Serial.print("button pressed soft");
        
//         lastTimePressed = millis();
//         return;
//       }
//     }
    
//   }
// }

// Funcion para imprimir la posicion dentro del modo manual, turno robot
// Falta Poner codigo del modo de juego manual
void PosicionManual_Ficha_X()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("FICHA X");                 
  lcd.setCursor(0, 1); 
  lcd.print("Posicion " + String(global_position));
  ComandoMovimiento(global_position,1);
  //Modo_Manual_Trobot();
  contador = 0;
  level_menu = 9; 
}

// Funcion para imprimir la posicion dentro del modo manual, turno robot
// Falta Poner codigo del modo de juego manual
void PosicionManual_Ficha_O()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("FICHA O");                 
  lcd.setCursor(0, 1); 
  lcd.print("Posicion " + String(global_position));
  ComandoMovimiento(global_position,0);
  //Modo_Manual_Tusuario();
  contador = 0;
  level_menu = 6; 
}

//Funcion para manejar los menus y submenus de la interfaz
void Robot_Menu() {
  
  // Parada de emergencia.
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
      case 0: // Menu Principal. Calibracion, M.Automatico, M.Manual.
        global_sizemenu = sizemenu1;
        fn_menu(contador, menu1, sizemenu1, "MENU PRINCIPAL");
        Serial.println(sizemenu1);
        break;

      case 1: // SubMenu Calibracion. Motor base, Motor hombro, Motor codo, Pinza.
        global_sizemenu = sizemenu2;
        fn_menu(contador, menu2, sizemenu2, "MENU CALIBRACION");
        break;

      case 2: // SubMenu Modo automatico. Modos Facil, Dificil, atras.
        global_sizemenu = sizemenu7;
        fn_menu(contador, menu7, sizemenu7, "MENU M.AUTOMATICO");
        break;

      case 3: // SubMenu Modo automatico modo fácil. Turnos usuario, robot, atras.
        global_sizemenu = sizemenu4;
        fn_menu(contador, menu4, sizemenu4, "M.AUTOMATICO FACIL");
        break;

      case 4: // SubMenu Modo Manual. Empezar, atras.
        global_sizemenu = sizemenu3;
        fn_menu(contador, menu3, sizemenu3, "MENU M.MANUAL");
        break;

      case 5: // SubMenu Empezar del Modo Manual. Turnos usuario, robot, atras.
        global_sizemenu = sizemenu8;
        fn_menu(contador, menu8, sizemenu8, "MENU M.MANUAL");
        break;

      case 6: // SubMenu Empezar del Modo manual turno robot. P1, P2, P3, ..., atras.
        global_sizemenu = sizemenu5;
        fn_menu1(contador, menu5, sizemenu5, "FICHA X");
        break;
      
      case 7: // SubMenu Conexion, Matriz, atras.
        global_sizemenu = sizemenu6;
        fn_menu(contador, menu6, sizemenu6, "MENU CONEXION");
        break;

      case 8: // SubMenu Modo automatico modo Difícl. Turnos usuario, robot, atras.
        global_sizemenu = sizemenu4;
        fn_menu(contador, menu4, sizemenu4, "M.AUTOMATICO DIFICIL");
        break;

      case 9: // SubMenu Empezar del Modo manual turno robot. P1, P2, P3, ..., atras.
        global_sizemenu = sizemenu5;
        fn_menu1(contador, menu5, sizemenu5, "FICHA O");
        break;
      
    }
    
  }

  // Maneja la lógica de los botones cuando se presiona el botón.
  if (btnpress) {
    switch (level_menu) {
      
      case 0: // Menu Principal.
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

      case 1: // SubMenu Calibracion.
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
            break;
        }
        break;

      case 2: // SubMenu Modo Automatico, escoge la dificultad de juego.
        switch (contador) {
          case 0: // Facil
            contador = 0;
            level_menu = 3;
            break;
          case 1: // Dificil
            contador = 1;
            level_menu = 8;
            break;
          case 2: // Atras
            contador = 1;
            level_menu = 0;
            break;
        }
        break;

      case 3: // SubMenu Modo Fácil del Modo Automatico.
        switch (contador) {
          case 0: // T. Usuario
            Modo_Automatico_Tusuario();
            contador = 0;
            level_menu = 3;
            break;
          case 1: // T. Robot
            Modo_Automatico_Trobot();
            contador = 0;
            level_menu = 3;
            break;
          case 2: // Atras
            contador = 0;
            level_menu = 2;
            break;
        }
        break;

      case 4: // SubMenu Modo Manual.
        switch (contador) {
          case 0: // Empezar
            contador = 0;
            level_menu = 5;
            break;
          case 1: // Atras
            contador = 2;
            level_menu = 0;
            break;
        }
        break;

      case 5: // SubMenu Empezar del Modo Manual.
        switch (contador) {
          case 0: // Ficha X
            contador = 0;
            level_menu = 6;
            break;
          case 1: // Ficha O
            contador = 0;
            level_menu =9;
            break;
          case 2: // Atras
            contador = 0;
            level_menu = 4;
            break;
        }
        break;
      
      case 6: // SubMenu Turno Robot Modo Manual.
        switch (contador) {
          case 0: 
            PosicionManual_Ficha_X();
            break;
          case 1: 
            PosicionManual_Ficha_X();
            break;
          case 2:
            PosicionManual_Ficha_X();
            break;
          case 3: 
            PosicionManual_Ficha_X();
            break;
          case 4: 
            PosicionManual_Ficha_X();
            break;
          case 5: 
            PosicionManual_Ficha_X();
            break;
          case 6: 
            PosicionManual_Ficha_X();
            break;
          case 7: 
            PosicionManual_Ficha_X();
            break;
          case 8: 
            PosicionManual_Ficha_X();
            break;
          case 9:
            contador = 1;
            level_menu = 5;
            break;
        }

        final_position = global_position;
        break;

      case 7: //SubMenu Comunicación.
        switch (contador) {
          case 0: // Conexion
            Fn_MatrizState();
            imprimirMatrizLCD();
            break;
          case 1: // T. Robot
            contador = 0;
            level_menu = 0;
            break;
        }
        break;

      case 8: // SubMenu Modo Difícl del Modo Automatico.
        switch (contador) {
          case 0: // T. Usuario
            Modo_Automatico_Tusuario_Dificil() ;
            contador = 0;
            level_menu = 8;
            break;
          case 1: // T. Robot
            Modo_Automatico_Trobot_Dificl();
            contador = 0;
            level_menu = 8;
            break;
          case 2: // Atras
            contador = 0;
            level_menu = 2;
            break;
        }
        break;

      case 9: // SubMenu Turno Robot Modo Manual.
        switch (contador) {
          case 0: 
            PosicionManual_Ficha_O();
            break;
          case 1: 
            PosicionManual_Ficha_O();
            break;
          case 2:
            PosicionManual_Ficha_O();
            break;
          case 3: 
            PosicionManual_Ficha_O();
            break;
          case 4: 
            PosicionManual_Ficha_O();
            break;
          case 5: 
            PosicionManual_Ficha_O();
            break;
          case 6: 
            PosicionManual_Ficha_O();
            break;
          case 7: 
            PosicionManual_Ficha_O();
            break;
          case 8: 
            PosicionManual_Ficha_O();
            break;
          case 9:
            contador = 1;
            level_menu = 5;
            break;
        }
        final_position = global_position;
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
  fn_menu(contador, menu1, sizemenu1, "MENU PRINCIPAL"); 

  //Servomotor
  motorPinza.attach(MOTOR_PINZA_PIN);
}