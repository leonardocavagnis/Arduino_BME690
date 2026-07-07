# Summary

 Members                        | Descriptions
--------------------------------|---------------------------------------------
`class` [`Arduino_BME690`](#class-arduino_bme690) | The main class for reading a Bosch Sensortec BME690 sensor.

# class [`Arduino_BME690`](#class-arduino_bme690)
The main class for reading a Bosch Sensortec BME690 sensor over I2C. Construct your own instance on the I2C bus your sensor is wired to.

## Summary

| Members                                                     | Descriptions                                |
|-------------------------------------------------------------|---------------------------------------------|
| `public ` [`Arduino_BME690`](#public-arduino_bme690towire-wire) | Construct a new Arduino_BME690 object on a given I2C bus. |
| `public int` [`begin`](#public-int-beginuint8_t-address) | Initialize the sensor. |
| `public void` [`end`](#public-void-end) | De-initialize the sensor. |
| `public int` [`read`](#public-int-read) | Trigger a single forced-mode measurement and cache the results. |
| `public float` [`readTemperature`](#public-float-readtemperature) | Get the last measured temperature. |
| `public float` [`readHumidity`](#public-float-readhumidity) | Get the last measured relative humidity. |
| `public float` [`readPressure`](#public-float-readpressure) | Get the last measured barometric pressure. |
| `public float` [`readGas`](#public-float-readgas) | Get the last measured gas resistance. |
| `public void` [`setOversampling`](#public-void-setoversamplinguint8_t-temperature-uint8_t-pressure-uint8_t-humidity) | Set the oversampling factor for temperature, pressure and humidity. |
| `public void` [`setIIRFilter`](#public-void-setiirfilteruint8_t-filter) | Set the IIR filter coefficient applied to temperature and pressure. |
| `public void` [`setGasHeater`](#public-void-setgasheateruint16_t-temperature-uint16_t-duration) | Configure the gas sensor's heater plate temperature and duration. |
| `public void` [`setGasHeaterOff`](#public-void-setgasheateroff) | Disable the gas sensor's heater plate. |

## Members

### `public ` [`Arduino_BME690`](#)`(TwoWire &wire = Wire)`

Construct a new Arduino_BME690 object on the given I2C bus. Pass `Wire1` (or any other `TwoWire` instance) here if your board/wiring uses a different I2C bus than the default one.

```cpp
Arduino_BME690 bme690(Wire);   // default I2C bus
Arduino_BME690 bme690(Wire1);  // e.g. Nano 33 BLE's internal I2C bus
```

#### Parameters
- `wire`: The `TwoWire` (I2C) interface the sensor is connected to. Defaults to `Wire`.

---

### `public int` [`begin`](#)`(uint8_t address = BME690_DEFAULT_ADDRESS)`

Initialize communication with the sensor and apply default oversampling, filter and gas heater settings (300 °C for 100 ms).

#### Parameters
- `address`: The I2C address of the sensor. Use `BME690_DEFAULT_ADDRESS` (0x76, SDO low) or `BME690_SECONDARY_ADDRESS` (0x77, SDO high).

#### Returns
`int`: `1` on success, `0` if the sensor could not be found or initialized.

#### Example

```cpp
if (!bme690.begin()) {
  Serial.println("Failed to find the BME690 sensor. Check your wiring!");
  while (1);
}
```

#### See also

* [end()](#public-void-end)
* [read()](#public-int-read)

---

### `public void` [`end`](#)`()`

De-initialize the sensor.

#### Returns
None.

#### See also

* [begin()](#public-int-beginuint8_t-address)

---

### `public int` [`read`](#)`()`

Triggers a single forced-mode measurement (temperature, humidity, pressure and gas resistance) and blocks until it completes, before caching the results internally. Call this once per reading cycle, before using the `read*()` getters below.

#### Returns
`int`: `1` if new data was read, `0` otherwise (for example if `begin()` was not called successfully, or the sensor did not respond in time).

#### Example

```cpp
if (bme690.read()) {
  Serial.println(bme690.readTemperature());
}
```

#### See also

* [readTemperature()](#public-float-readtemperature)
* [readHumidity()](#public-float-readhumidity)
* [readPressure()](#public-float-readpressure)
* [readGas()](#public-float-readgas)

---

### `public float` [`readTemperature`](#)`()`

Get the temperature measured during the last successful `read()`.

#### Returns
`float`: Temperature in degrees Celsius, or `NAN` if the sensor has not been initialized.

---

### `public float` [`readHumidity`](#)`()`

Get the relative humidity measured during the last successful `read()`.

#### Returns
`float`: Relative humidity in %, or `NAN` if the sensor has not been initialized.

---

### `public float` [`readPressure`](#)`()`

Get the barometric pressure measured during the last successful `read()`.

#### Returns
`float`: Pressure in hectopascals (hPa), or `NAN` if the sensor has not been initialized.

---

### `public float` [`readGas`](#)`()`

Get the gas resistance measured during the last successful `read()`. This is a raw signal, not a calibrated air-quality index: higher values generally mean cleaner air, lower values mean the presence of VOCs or other gases.

#### Returns
`float`: Gas resistance in Ohms, or `NAN` if the sensor has not been initialized.

---

### `public void` [`setOversampling`](#)`(uint8_t temperature, uint8_t pressure, uint8_t humidity)`

Set the oversampling factor used for temperature, pressure and humidity measurements. Higher oversampling reduces noise at the cost of a longer measurement time. Call before `read()`.

#### Parameters
- `temperature`, `pressure`, `humidity`: One of `BME690_OS_NONE`, `BME690_OS_1X`, `BME690_OS_2X`, `BME690_OS_4X`, `BME690_OS_8X`, `BME690_OS_16X`.

#### Example

```cpp
bme690.setOversampling(BME690_OS_8X, BME690_OS_8X, BME690_OS_2X);
```

---

### `public void` [`setIIRFilter`](#)`(uint8_t filter)`

Set the IIR filter coefficient applied to temperature and pressure readings, to smooth out short-term fluctuations (for example due to air conditioning or opening a door). Call before `read()`.

#### Parameters
- `filter`: One of `BME690_FILTER_OFF`, `BME690_FILTER_SIZE_1`, `BME690_FILTER_SIZE_3`, `BME690_FILTER_SIZE_7`, `BME690_FILTER_SIZE_15`, `BME690_FILTER_SIZE_31`, `BME690_FILTER_SIZE_63`, `BME690_FILTER_SIZE_127`.

---

### `public void` [`setGasHeater`](#)`(uint16_t temperature, uint16_t duration)`

Configure the gas sensor's heater plate temperature and heating duration used for each forced-mode measurement. `begin()` already sets a default of 300 °C for 100 ms; call this afterwards to override it.

#### Parameters
- `temperature`: Heater plate target temperature, in degrees Celsius (typically 200-400 °C).
- `duration`: Heating duration, in milliseconds.

#### See also

* [setGasHeaterOff()](#public-void-setgasheateroff)

---

### `public void` [`setGasHeaterOff`](#)`()`

Disable the gas sensor's heater plate. Temperature, humidity and pressure readings remain available, but the gas resistance reading will no longer be meaningful.

#### See also

* [setGasHeater()](#public-void-setgasheateruint16_t-temperature-uint16_t-duration)
