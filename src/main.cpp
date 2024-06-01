#include <Arduino.h>
#include <SPI.h>
#include "../include/Almar.h"

hw_timer_t *timer = NULL;

int ctrl = false;
void IRAM_ATTR timerCtrl() {
 ctrl = true;
}

void setup() {
  // Inicializamos el puerto serial
  Serial.begin(115200);
  // Inicializamos el puerto serial para los comandos
  // Comando por defecto para comandos no reconocidos
  cmd.SetDefaultHandler(cmd_unrecognized);
  // Comandos
  cmd.AddCommand(&cmd_set_debug_);
  cmd.AddCommand(&cmd_set_led1_);
  cmd.AddCommand(&cmd_set_led2_);
  cmd.AddCommand(&cmd_set_motor_pos_);
  cmd.AddCommand(&cmd_set_encoder_zero_);
  cmd.AddCommand(&cmd_set_motor_kp_);
  cmd.AddCommand(&cmd_set_motor_ki_);
  cmd.AddCommand(&cmd_set_motor_kd_);

  // Inicializamos el timer
  timer = timerBegin(0, 80, true);                // Timer 0, clock divider 80
  timerAttachInterrupt(timer, &timerCtrl, true);  // Attach the interrupt handling function
  timerAlarmWrite(timer, 1000000, true);          // Interrupt every 1 second
  timerAlarmEnable(timer);                        // Enable the alarm


  if(DEBUG & DEBUG_INFO != 0)
    Serial.printf("Serial ready!\n");

  if(DEBUG & DEBUG_EXTRA != 0)
    Serial.printf("DEBUG MODE: %i\n", DEBUG);

  // LEDS
  pinMode(led_1, OUTPUT);
  pinMode(led_2, OUTPUT);

  analogWrite(led_1, 0);
  analogWrite(led_2, 0);
  
  // Recorremos todos los motores existe en el array motor
  for(int i = 0; i < N_MOTORS; i++)
  {
    // Inicializamos el motor
    _m[i] = new AlMar::Esp32::Driver_L298n(motor[i][0], motor[i][1], motor[i][2], motor[i][3]);
    _m[i]->begin();
    // Inicializamos el control
    _pid[i] = new PID(motor[i][4], motor[i][5], motor[i][6]);
  }
  // Inicializamos los encoders
  _enc = new AlMar::Esp32::EncoderATM203_Spi2(cs_pins,N_MOTORS,PIN_MOSI,PIN_MISO,PIN_SCLK);
}

void loop() {
  cmd.ReadSerial();

  if(ctrl)
  {
     // Tasks to perform when the Timer interrupt is triggered
    //digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); // Toggle the state of the LED
    int pos=_enc->Read(0); // lee encoder 0 (M1)
    int pos2=_enc->Read(1); // lee encoder 1 (M2)
    int pos3=_enc->Read(2); // lee encoder 2 (M3)

    if(pos!=0x80000000) { 
      posOld = abs(posD);
      posD = (float) (pos*360.0/4096.0);
      
      dutyCycle = _pid[0]->calc(posD, desPos);
      
      _m[0]->SetDuty(dutyCycle);
    }

    ctrl = false;
  }
}