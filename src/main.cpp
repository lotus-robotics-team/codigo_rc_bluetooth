#include <Arduino.h>
#include <ps5Controller.h>

const char *ENDERECO_BLUETOOTH = "4c:b9:9b:1c:44:d8";


void setup() {

  ps5.begin(ENDERECO_BLUETOOTH);
  
}

void loop() {

  if (ps5.isConnected()) {
    // Loop do robô
  }

  else {
    // Robô parado
  }

}