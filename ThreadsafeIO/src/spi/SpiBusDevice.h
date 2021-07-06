/*
 * A deeply magical library providing threadsafe IO via pipes.
 */

#ifndef SPI_BUS_DEVICE_H_
#define SPI_BUS_DEVICE_H_

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include <Arduino.h>
#include <mbed.h>

#include "../BusDevice.h"

#include "SpiDispatcher.h"
#include "SpiBusDeviceConfig.h"

/**************************************************************************************
 * CLASS DECLARATION
 **************************************************************************************/

class SpiBusDevice : public BusDevice
{
public:

  SpiBusDevice(std::string const & spi_bus, SpiBusDeviceConfig const & config)
  : _config{config}
  { /* TODO: Select SPI bus based in string. */ }


  virtual IoResponse transfer(IoRequest & req) override
  {
    return SpiDispatcher::instance().dispatch(&req, &_config);
  }


private:

  SpiBusDeviceConfig _config;

};

#endif /* SPI_BUS_DEVICE_H_ */
