# Arduino BME690 Library

The Arduino BME690 library is a C++ library that lets you read temperature, humidity, barometric pressure and gas resistance from a [Bosch Sensortec BME690](https://www.bosch-sensortec.com/products/environmental-sensors/gas-sensors/bme690/) sensor connected over I2C.

The BME690 combines temperature, humidity and pressure sensing with a MOX gas sensor that reacts to Volatile Organic Compounds (VOCs) and other gases.

Internally, the library talks to the sensor through Bosch Sensortec's own low-level `bme69x` driver (vendored in [`src/bme69x/`](../src/bme69x/)), which handles calibration, compensation and register-level communication. The `Arduino_BME690` class on top of it takes care of I2C wiring, sensible default configuration and forced-mode measurement timing, so most sketches only need `begin()` and `read()`.

## Features

- Reads temperature (°C), humidity (%rH), barometric pressure (hPa) and gas resistance (Ω)
- Simple, synchronous forced-mode measurements: no interrupts or background tasks to manage
- Works with either I2C address of the sensor (SDO pin low or high)
- Optional advanced configuration: oversampling, IIR filter and gas heater profile

## Usage

To use this library:

```cpp
#include <Arduino_BME690.h>
```

Construct an `Arduino_BME690` object on the I2C bus your sensor is wired to (`Wire`, `Wire1`, or any other `TwoWire` instance), call `begin()` once in `setup()`, then `read()` in `loop()` before reading the individual values:

```cpp
#include <Arduino_BME690.h>

// Change Wire to Wire1, or any other TwoWire instance, if your board/wiring needs it
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
    Serial.println(bme690.readTemperature());

    Serial.print("Humidity (%): ");
    Serial.println(bme690.readHumidity());

    Serial.print("Pressure (hPa): ");
    Serial.println(bme690.readPressure());

    Serial.print("Gas Resistance (Ohms): ");
    Serial.println(bme690.readGas());
  }

  delay(2000);
}
```

The gas resistance is a raw signal, not a calibrated air-quality index: higher values generally indicate cleaner air, lower values indicate the presence of VOCs or other gases. If you need a fused Air Quality Index (IAQ), CO2-equivalent or VOC-equivalent estimate instead, look for a library built on top of [Bosch's BSEC algorithm](https://www.bosch-sensortec.com/en/software-tools/software/bme680-software-bsec).

## Examples

- **[ReadSensors](../examples/ReadSensors):** reads temperature, humidity, pressure and gas resistance at the sensor's default I2C address.
- **[ReadSensorsAdvanced](../examples/ReadSensorsAdvanced):** shows how to use the secondary I2C address and customize oversampling, IIR filtering and the gas heater profile.

## API

The API documentation can be found [here](./api.md).
