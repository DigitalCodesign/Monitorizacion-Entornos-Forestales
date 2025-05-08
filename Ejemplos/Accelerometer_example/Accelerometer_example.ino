#include <Wire.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>

Adafruit_LIS3DH lis = Adafruit_LIS3DH();

void setup(void) {
  Serial.begin(115200);
  if (!lis.begin(0x18)) { //Cambiar 0x18 por 0x19 si es necesario, direcciones mas comunes.
    Serial.println("Could not find LIS3DH accelerometer!");
    while (1);
  }
  Serial.println("LIS3DH found!");

  lis.setRange(LIS3DH_RANGE_2_G);
  Serial.print("Range = "); Serial.print(2); Serial.println("G");
}

void loop(void) {
  sensors_event_t event;
  lis.getEvent(&event);

  Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.println(" m/s^2");

  delay(100);
}