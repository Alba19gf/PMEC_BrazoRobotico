#include <Arduino.h>
#include <iostream>

// put function declarations here:
int myFunction(int, int);

void setup() {
  // put your setup code here, to run once:
  int result = myFunction(2, 3);

  std::cout << "Prueba compilación" << std::endl;

}

void loop() {
  // put your main code here, to run repeatedly:
  std::cout << "MAIN: Prueba compilación" << std::endl;
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}