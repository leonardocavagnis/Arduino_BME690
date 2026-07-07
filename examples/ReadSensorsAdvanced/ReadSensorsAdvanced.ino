/*
 * Arduino_BME690 - ReadSensorsAdvanced
 *
 * This example shows how to customize the sensor's oversampling, IIR
 * filter and gas heater settings before taking a reading.
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

  // Higher oversampling, some smoothing on temperature/pressure/humidity
  bme690.setOversampling(BME690_OS_8X, BME690_OS_8X, BME690_OS_2X);
  bme690.setIIRFilter(BME690_FILTER_SIZE_3);

  // Heat the gas sensor plate to 320 deg C for 150 ms before each reading
  bme690.setGasHeater(320, 150);
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
