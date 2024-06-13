#include "Comunicacion_JSON.h"

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

void Inicializar_COMS(){
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
}

void Fn_printMatriz(int matriz[3][3]){
  Serial.println("Loop:");
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        Serial.print(matriz[i][j]);
         Serial.print(" ");
      }
      Serial.println();
    }
}


int Fn_readSerial() {
  int valor;
  while (!Serial.available());  // Espera hasta que haya datos disponibles en el puerto serial
  valor = Serial.parseInt();  // Lee el valor desde el puerto serial

  // Limpia el buffer del puerto serial
  while (Serial.available()) {
    Serial.read();
  }
  Serial.print("Engresado: ");
  Serial.println(valor);
  
  return valor;  // Devuelve el valor leído
}