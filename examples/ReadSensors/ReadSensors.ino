/*
 * Arduino_BME690 - ReadSensors
 *
 * This example shows how to read temperature, humidity, barometric
 * pressure and gas resistance from a BME690 sensor over I2C.
 *
 * The gas resistance is a raw signal: higher values generally mean
 * cleaner air, lower values mean the presence of Volatile Organic
 * Compounds (VOCs) or other gases. It is not calibrated into an
 * air-quality index by this library.
 *
 * Circuit:
 * - Arduino board with I2C (SDA/SCL), connected to a BME690 breakout
 *
 * This example code is in the public domain.
 */

#include <Arduino_BME690.h>

// Create the sensor object on the Wire (I2C) bus. Change this to Wire1,
// or any other TwoWire instance, if your board/wiring needs it.
Arduino_BME690 bme690(Wire);

void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!bme690.begin()) {
    Serial.println("Failed to find the BME690 sensor. Check your wiring!");
    while (1);
  }
}

void loop() {
  if (bme690.read()) {
    Serial.print("Temperature (°C): ");
    Serial.println(bme690.readTemperature(), 2);

    Serial.print("Humidity (rH%): ");
    Serial.println(bme690.readHumidity(), 2);

    Serial.print("Pressure (hPa): ");
    Serial.println(bme690.readPressure(), 2);

    Serial.print("Gas Resistance (Ohms): ");
    Serial.println(bme690.readGas(), 2);

    Serial.println("------------------------------------");
  }

  delay(2000);
}
