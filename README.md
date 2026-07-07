# 🌡️ Arduino BME690 Library

The Arduino BME690 Library allows you to read temperature, humidity, barometric pressure and gas resistance from a [Bosch Sensortec BME690](https://www.bosch-sensortec.com/products/environmental-sensors/gas-sensors/bme690/) sensor over I2C, using a simple, classic Arduino-style API.

It is built on top of Bosch Sensortec's low-level `bme69x` sensor driver, vendored in `src/bme69x/`, wrapped in an idiomatic `BME690` object so you don't have to deal with the underlying register-level driver directly.

---

## Usage

```cpp
#include <Arduino_BME690.h>

// Construct the sensor on the Wire (I2C) bus. Change this to Wire1, or any
// other TwoWire instance, if your board/wiring needs it.
Arduino_BME690 bme690(Wire);

void setup() {
  Serial.begin(9600);

  if (!bme690.begin()) {
    Serial.println("Failed to find the BME690 sensor. Check your wiring!");
    while (1);
  }
}

void loop() {
  if (bme690.read()) {
    Serial.print("Temperature (°C): ");
    Serial.println(bme690.readTemperature());

    Serial.print("Gas Resistance (Ohms): ");
    Serial.println(bme690.readGas());
  }

  delay(2000);
}
```

---

## License

This library as a whole is licensed under the terms of the [GNU General Public License, version 2](./LICENSE) (GPL-2.0).

It internally uses Bosch Sensortec's `bme69x` low-level sensor driver, vendored unmodified in `src/bme69x/`, which is licensed separately under the [BSD-3-Clause license](./src/bme69x/LICENSE). See that folder for the original Bosch Sensortec copyright notice and license terms.

---

📚 For more information about this library, please refer to the documentation [here](./docs/).
