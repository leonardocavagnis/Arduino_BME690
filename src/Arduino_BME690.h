/*
  This file is part of the Arduino_BME690 library.
  Copyright (c) 2026 Arduino SA. All rights reserved.

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

  ---

  This library drives the Bosch Sensortec BME690 sensor through the
  low-level "bme69x" driver vendored in src/bme69x/ (Copyright (c) Bosch
  Sensortec GmbH, BSD-3-Clause, see src/bme69x/LICENSE). This wrapper class
  and its Arduino-style API are original work provided by Arduino, licensed
  under the GNU General Public License, version 2 (see LICENSE).
*/

#ifndef _ARDUINO_BME690_H_
#define _ARDUINO_BME690_H_

#include <Arduino.h>
#include <Wire.h>

#include "bme69x/bme69x.h"

// I2C addresses (SDO pin low/high)
#define BME690_DEFAULT_ADDRESS    BME69X_I2C_ADDR_LOW   // 0x76
#define BME690_SECONDARY_ADDRESS  BME69X_I2C_ADDR_HIGH  // 0x77

// Oversampling settings, for use with setOversampling().
// Oversampling repeats a measurement N times internally and averages the
// result: higher values trade a longer measurement time for less noise
// and finer resolution. NONE disables that particular measurement entirely
// (its reading becomes invalid).
enum {
  BME690_OS_NONE = BME69X_OS_NONE,  // measurement disabled
  BME690_OS_1X   = BME69X_OS_1X,    // no oversampling (fastest, noisiest)
  BME690_OS_2X   = BME69X_OS_2X,
  BME690_OS_4X   = BME69X_OS_4X,
  BME690_OS_8X   = BME69X_OS_8X,
  BME690_OS_16X  = BME69X_OS_16X    // maximum oversampling (slowest, most stable)
};

// IIR filter settings, for use with setIIRFilter().
// The IIR filter smooths temperature and pressure readings over successive
// measurements, damping fast/short-lived fluctuations (e.g. a door opening,
// a gust of air). Larger sizes filter more aggressively but make the
// reading slower to react to real changes; OFF applies no filtering.
enum {
  BME690_FILTER_OFF      = BME69X_FILTER_OFF,      // no filtering, raw reading
  BME690_FILTER_SIZE_1   = BME69X_FILTER_SIZE_1,
  BME690_FILTER_SIZE_3   = BME69X_FILTER_SIZE_3,
  BME690_FILTER_SIZE_7   = BME69X_FILTER_SIZE_7,
  BME690_FILTER_SIZE_15  = BME69X_FILTER_SIZE_15,
  BME690_FILTER_SIZE_31  = BME69X_FILTER_SIZE_31,
  BME690_FILTER_SIZE_63  = BME69X_FILTER_SIZE_63,
  BME690_FILTER_SIZE_127 = BME69X_FILTER_SIZE_127  // strongest filtering, slowest response
};

class Arduino_BME690 {
public:
  // Construct the sensor object on a given I2C bus. Pass Wire1 (or any other
  // TwoWire instance) here if your board/wiring uses a different I2C bus
  // than the default one.
  Arduino_BME690(TwoWire& wire = Wire);

  // Starts communication with the sensor. Returns 1 on success, 0 on failure.
  int begin(uint8_t address = BME690_DEFAULT_ADDRESS);
  void end();

  // Triggers a single forced-mode measurement and waits for it to complete.
  // Returns 1 if new data was read, 0 otherwise. Call this before the
  // read*() getters below to refresh their values.
  int read();

  float readTemperature();  // degrees Celsius
  float readHumidity();     // % relative humidity
  float readPressure();     // hPa
  float readGas();          // gas resistance, in Ohms

  // --- Optional advanced configuration, call before read() ---

  // Set how many times each measurement is internally repeated and averaged.
  // Higher oversampling reduces noise at the cost of a longer measurement.
  // temperature, pressure, humidity: one of the BME690_OS_* constants above.
  void setOversampling(uint8_t temperature, uint8_t pressure, uint8_t humidity);

  // Set the IIR filter coefficient applied to temperature and pressure, to
  // smooth out short-term fluctuations across successive measurements.
  // filter: one of the BME690_FILTER_* constants above.
  void setIIRFilter(uint8_t filter);

  // Configure the gas sensor's heater plate: it must reach a stable target
  // temperature for the gas resistance reading to be meaningful.
  // temperature: heater plate target temperature, in degrees Celsius
  //              (typically 200-400 degC).
  // duration: how long to keep the plate at that temperature before
  //           sampling, in milliseconds (typically 100-200 ms).
  void setGasHeater(uint16_t temperature, uint16_t duration);

  // Turn the gas heater plate off. Temperature/humidity/pressure readings
  // stay available, but the gas resistance reading is no longer meaningful.
  void setGasHeaterOff();

private:
  static int8_t i2cRead(uint8_t regAddr, uint8_t* regData, uint32_t length, void* intfPtr);
  static int8_t i2cWrite(uint8_t regAddr, const uint8_t* regData, uint32_t length, void* intfPtr);
  static void usDelay(uint32_t period, void* intfPtr);

  TwoWire* _wire;
  uint8_t _address;
  bool _initialized;

  struct bme69x_dev _dev;
  struct bme69x_conf _conf;
  struct bme69x_heatr_conf _heaterConf;
  struct bme69x_data _data;
};

#endif
