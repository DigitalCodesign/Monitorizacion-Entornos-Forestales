#include <Wire.h>
#include <Adafruit_ADS7830.h>

Adafruit_ADS7830 ads;

void setup() {
  Serial.begin(115200);

  if (!ads.begin(0x4B)) { // Verifica la dirección I2C (0x48 por defecto)
    Serial.println("Error al inicializar el ADS7830. ¡Revisa las conexiones!");
    while (1);
  }

  Serial.println("ADS7830 inicializado correctamente.");
}

void loop() {
  // Barrido de los 8 canales
  for (int canal = 0; canal < 8; canal++) {
    int valor = ads.readADCsingle(canal);
    Serial.print("Canal ");
    Serial.print(canal);
    Serial.print(": ");
    Serial.println(valor);
    delay(100);
  }

  Serial.println("------------------");
  delay(1000);
}
