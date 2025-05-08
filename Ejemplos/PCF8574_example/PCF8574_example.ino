#include <Wire.h>
#include <Adafruit_PCF8574.h>

// Dirección I2C del PCF8574 (por defecto 0x20, verifica tu módulo)
#define PCF8574_ADDR 0x20

Adafruit_PCF8574 pcf8574;

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22); // Inicializa la comunicación I2C en los pines correctos del ESP32.

  if (!pcf8574.begin(PCF8574_ADDR)) {
    Serial.println("Error al encontrar el PCF8574. ¡Revisa las conexiones y la dirección I2C!");
    while (1);
  }

  Serial.println("PCF8574 encontrado!");

  for (uint8_t i = 0; i < 8; i++) {
    pcf8574.pinMode(i, OUTPUT);
  }
}

void loop() {
  // Enciende los LEDs uno por uno
  for (uint8_t i = 0; i < 8; i++) {
    pcf8574.digitalWrite(i, HIGH);
    delay(500); // Espera 500 ms
    pcf8574.digitalWrite(i, LOW);
    delay(500); // Espera 500 ms
  }

}
