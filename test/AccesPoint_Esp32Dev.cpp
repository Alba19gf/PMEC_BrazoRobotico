#include <WiFi.h>

const char *ssid = "ESP_3D";
const char *password = "123400000";
const int ledPin = 38; // Pin del LED

void setup() {
  Serial.begin(115200);
  
  // Inicializar el pin del LED como salida
  pinMode(ledPin, OUTPUT);
  
  // Iniciar el modo de Access Point
  WiFi.mode(WIFI_AP);
  
  // Configurar el nombre y la contraseña del Access Point
  WiFi.softAP(ssid, password);
  
  Serial.println("Punto de Acceso iniciado");
  Serial.print("Dirección IP del Punto de Acceso: ");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  // Parpadear el LED
  digitalWrite(ledPin, HIGH); // Encender el LED
  delay(500); // Esperar 0.5 segundos
  digitalWrite(ledPin, LOW); // Apagar el LED
  delay(500); // Esperar 0.5 segundos
  
  // Tu código de loop, si es necesario
}