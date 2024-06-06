#include <Arduino.h>
#include <Wire.h>

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  
  byte error, address;
  int deviceCount = 0;

  Serial.println("Escaneando direcciones I2C...");

  for (address = 1; address < 127; address++)
  {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("Dispositivo encontrado en la dirección 0x");
      if (address < 16)
      {
        Serial.print("0");
      }
      Serial.println(address, HEX);
      deviceCount++;
    }
  }

  if (deviceCount == 0)
  {
    Serial.println("No se encontraron dispositivos I2C.");
  }

//   // Configurar la dirección del MPU6050 si es necesario
//   byte newAddress = 0x68;     // Nueva dirección deseada
//   byte currentAddress = 0x68; // Dirección actual del MPU6050

//   if (currentAddress != newAddress)
//   {
//     Wire.beginTransmission(currentAddress);
//     Wire.write(0x6B); // Registro de configuración del MPU6050
//     Wire.write(0x00); // Valor para desactivar el modo de suspensión
//     Wire.endTransmission();

//     Wire.beginTransmission(currentAddress);
//     Wire.write(0x68);       // Registro de dirección I2C del MPU6050
//     Wire.write(newAddress); // Nueva dirección deseada
//     Wire.endTransmission();
//     Serial.print("Dirección del MPU6050 cambiada a 0x");
//     if (newAddress < 16)
//     {
//       Serial.print("0");
//     }
//     Serial.println(newAddress, HEX);
//   }
}

void loop()
{
 
}