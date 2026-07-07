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
*/

#include <string.h>
#include <math.h>

#include "Arduino_BME690.h"

Arduino_BME690::Arduino_BME690(TwoWire& wire) :
  _wire(&wire),
  _address(BME690_DEFAULT_ADDRESS),
  _initialized(false)
{
  memset(&_dev, 0, sizeof(_dev));
  memset(&_conf, 0, sizeof(_conf));
  memset(&_heaterConf, 0, sizeof(_heaterConf));
  memset(&_data, 0, sizeof(_data));
}

int Arduino_BME690::begin(uint8_t address)
{
  _address = address;
  _wire->begin();

  _dev.intf = BME69X_I2C_INTF;
  _dev.read = i2cRead;
  _dev.write = i2cWrite;
  _dev.delay_us = usDelay;
  _dev.intf_ptr = this;
  _dev.amb_temp = 25;  // typical room temperature, in degrees Celsius

  if (bme69x_init(&_dev) != BME69X_OK) {
    return 0;
  }

  // Default oversampling and filter settings
  _conf.os_hum = BME690_OS_1X;
  _conf.os_temp = BME690_OS_2X;
  _conf.os_pres = BME690_OS_4X;
  _conf.filter = BME690_FILTER_OFF;
  _conf.odr = BME69X_ODR_NONE;

  if (bme69x_set_conf(&_conf, &_dev) != BME69X_OK) {
    return 0;
  }

  // Default gas heater profile: 300 deg C for 100 ms
  setGasHeater(300, 100);

  _initialized = true;
  return 1;
}

void Arduino_BME690::end()
{
  _initialized = false;
}

int Arduino_BME690::read()
{
  if (!_initialized) {
    return 0;
  }

  bme69x_set_op_mode(BME69X_FORCED_MODE, &_dev);

  uint32_t measurementDuration = bme69x_get_meas_dur(BME69X_FORCED_MODE, &_conf, &_dev);
  delayMicroseconds(measurementDuration);

  uint8_t newFields = 0;
  int8_t status = bme69x_get_data(BME69X_FORCED_MODE, &_data, &newFields, &_dev);

  return (status == BME69X_OK && newFields > 0) ? 1 : 0;
}

float Arduino_BME690::readTemperature()
{
  return _initialized ? _data.temperature : NAN;
}

float Arduino_BME690::readHumidity()
{
  return _initialized ? _data.humidity : NAN;
}

float Arduino_BME690::readPressure()
{
  // The driver reports pressure in Pascal, convert to hPa
  return _initialized ? (_data.pressure * 0.01f) : NAN;
}

float Arduino_BME690::readGas()
{
  return _initialized ? _data.gas_resistance : NAN;
}

void Arduino_BME690::setOversampling(uint8_t temperature, uint8_t pressure, uint8_t humidity)
{
  _conf.os_temp = temperature;
  _conf.os_pres = pressure;
  _conf.os_hum = humidity;
  bme69x_set_conf(&_conf, &_dev);
}

void Arduino_BME690::setIIRFilter(uint8_t filter)
{
  _conf.filter = filter;
  bme69x_set_conf(&_conf, &_dev);
}

void Arduino_BME690::setGasHeater(uint16_t temperature, uint16_t duration)
{
  _heaterConf.enable = BME69X_ENABLE;
  _heaterConf.heatr_temp = temperature;
  _heaterConf.heatr_dur = duration;
  bme69x_set_heatr_conf(BME69X_FORCED_MODE, &_heaterConf, &_dev);
}

void Arduino_BME690::setGasHeaterOff()
{
  _heaterConf.enable = BME69X_DISABLE;
  bme69x_set_heatr_conf(BME69X_FORCED_MODE, &_heaterConf, &_dev);
}

int8_t Arduino_BME690::i2cRead(uint8_t regAddr, uint8_t* regData, uint32_t length, void* intfPtr)
{
  Arduino_BME690* sensor = static_cast<Arduino_BME690*>(intfPtr);
  TwoWire* wire = sensor->_wire;

  wire->beginTransmission(sensor->_address);
  wire->write(regAddr);
  if (wire->endTransmission() != 0) {
    return BME69X_E_COM_FAIL;
  }

  if (wire->requestFrom((int)sensor->_address, (int)length) != (int)length) {
    return BME69X_E_COM_FAIL;
  }

  for (uint32_t i = 0; i < length; i++) {
    regData[i] = wire->read();
  }

  return BME69X_OK;
}

int8_t Arduino_BME690::i2cWrite(uint8_t regAddr, const uint8_t* regData, uint32_t length, void* intfPtr)
{
  Arduino_BME690* sensor = static_cast<Arduino_BME690*>(intfPtr);
  TwoWire* wire = sensor->_wire;

  wire->beginTransmission(sensor->_address);
  wire->write(regAddr);
  wire->write(regData, length);
  if (wire->endTransmission() != 0) {
    return BME69X_E_COM_FAIL;
  }

  return BME69X_OK;
}

void Arduino_BME690::usDelay(uint32_t period, void* intfPtr)
{
  (void)intfPtr;
  delayMicroseconds(period);
}
