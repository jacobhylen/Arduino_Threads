/*
 * This file is part of the Arduino_ThreadsafeIO library.
 * Copyright (c) 2021 Arduino SA.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef ARDUINO_THREADS_SINK_HPP_
#define ARDUINO_THREADS_SINK_HPP_

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include <mbed.h>

/**************************************************************************************
 * CLASS DECLARATION
 **************************************************************************************/

template<typename T>
class SinkBase
{
public:

  virtual ~SinkBase() { }

  virtual T read() = 0;
  virtual void inject(T const & value) = 0;
};

template<typename T>
class SinkNonBlocking : public SinkBase<T>
{
public:

           SinkNonBlocking();
  virtual ~SinkNonBlocking() { }

  virtual T read() override
  {
    _mutex.lock();
    return _data;
    _mutex.unlock();
  }
  virtual void inject(T const & value) override
  {
    _mutex.lock();
    _data = value;
    _mutex.unlock();
  }

private:

  T _data;
  rtos::Mutex _mutex;

};

template<typename T>
class SinkBlocking : public SinkBase<T>
{
public:

           SinkBlocking();
  virtual ~SinkBlocking() { }

  virtual T read() override;
  virtual void inject(T const & value) override;


private:

  T _data;
  bool _is_data_available;
  rtos::Mutex _mutex;
  rtos::ConditionVariable _cond_data_available;
  rtos::ConditionVariable _cond_slot_available;

};

/**************************************************************************************
 * PUBLIC MEMBER FUNCTIONS - SinkBlocking
 **************************************************************************************/

template<typename T>
SinkBlocking<T>::SinkBlocking()
: _is_data_available{false}
, _cond_data_available(_mutex)
, _cond_slot_available(_mutex)
{ }

template<typename T>
T SinkBlocking<T>::read()
{
  _mutex.lock();
  while (!_is_data_available)
    _cond_data_available.wait();
  T const d = _data;
  _is_data_available = false;
  _cond_slot_available.notify_all();
  _mutex.unlock();
  return d;
}

template<typename T>
void SinkBlocking<T>::inject(T const & value)
{
  _mutex.lock();
  while (_is_data_available)
    _cond_slot_available.wait();
  _data = value;
  _is_data_available = true;
  _cond_data_available.notify_all();
  _mutex.unlock();
}

#endif /* ARDUINO_THREADS_SINK_HPP_ */
