# 🌡️ Arduino BME690 Library

The Arduino BME690 Library allows you to read temperature, humidity, barometric pressure and gas resistance from a [Bosch Sensortec BME690](https://www.bosch-sensortec.com/products/environmental-sensors/gas-sensors/bme690/) sensor over Wire/I2C.

It is built on top of Bosch Sensortec's low-level [BME690_SensorAPI](https://github.com/boschsensortec/BME690_SensorAPI) driver.

---

## License

This library as a whole is licensed under the terms of the [GNU General Public License, version 2](./LICENSE) (GPL-2.0).

It internally uses Bosch Sensortec's `bme69x` low-level sensor driver, vendored unmodified in `src/bme69x/`, which is licensed separately under the [BSD-3-Clause license](./src/bme69x/LICENSE). See that folder for the original Bosch Sensortec copyright notice and license terms.

---

📚 For more information about this library, please refer to the documentation [here](./docs/).
