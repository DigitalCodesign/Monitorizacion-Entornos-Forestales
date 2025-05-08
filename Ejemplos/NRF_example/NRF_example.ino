#include <SPI.h>
#include <RF24.h>

RF24 radio(26, 14); // CE, CSN
const byte direcciones[][6] = {"00001", "00002"};

void setup() {
  Serial.begin(115200);

  radio.begin(); // Inicializa el radio primero

  if (!radio.isChipConnected()) {
    Serial.println("NRF24L01 no detectado. ¡Verifica las conexiones!");
    while (1); // Detiene la ejecución si hay un error
  }

  Serial.println("NRF24L01 detectado y inicializado correctamente.");

  radio.setPALevel(RF24_PA_MIN);
  radio.openWritingPipe(direcciones[0]);
  radio.openReadingPipe(1, direcciones[1]);
  radio.stopListening();
}

void loop() {}
