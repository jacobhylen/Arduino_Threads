<img src="https://content.arduino.cc/website/Arduino_logo_teal.svg" height="100" align="right" />

`Arduino_Threads`
=================

[![Compile Examples status](https://github.com/arduino-libraries/Arduino_Threads/actions/workflows/compile-examples.yml/badge.svg)](https://github.com/arduino-libraries/Arduino_Threads/actions/workflows/compile-examples.yml)
[![Check Arduino status](https://github.com/arduino-libraries/Arduino_Threads/actions/workflows/check-arduino.yml/badge.svg)](https://github.com/arduino-libraries/Arduino_Threads/actions/workflows/check-arduino.yml)
[![Spell Check status](https://github.com/arduino-libraries/Arduino_Threads/actions/workflows/spell-check.yml/badge.svg)](https://github.com/arduino-libraries/Arduino_Threads/actions/workflows/spell-check.yml)

This library makes it easy to use the multi-threading capability of [Arduino](https://www.arduino.cc/) boards that use an [Mbed OS](https://os.mbed.com/docs/mbed-os/latest/introduction/index.html)-based core library. Additionally this library provides thread-safe access to `Wire`, `SPI` and `Serial` which is relevant when creating multi-threaded sketches in order to avoid common pitfalls such as race-conditions and invalid state.

## :zap: Features
### :thread: Multi-threaded sketch execution
Instead of one big state-machine-of-doom you can split your application into multiple independent threads, each with it's own `setup()` and `loop()` function. Instead of implementing your application in a single `.ino` file, each independent thread is implemented in a dedicated `.inot` representing a clear separation of concerns on a file level.

### :calling: Easy communication between multiple threads
Easy inter-thread-communication is facilitated via a `Shared` abstraction providing thread-safe sink/source semantics allowing to safely exchange data of any type between threads.

### :electric_plug: Threadsafe, asynchronous and convenient Input/Output API
#### Threadsafe
A key problem of multi-tasking is the **prevention of erroneous state when multiple threads share a single resource**. The following example borrowed from a typical application demonstrates the problems resulting from multiple threads sharing a single resource:

Imagine a embedded system where multiple `Wire` client devices are physically connected to a single `Wire` server. Each `Wire` client device is managed by a separate software thread. Each thread polls its `Wire` client device periodically. Access to the I2C bus is managed via the `Wire` library and typically follows this pattern:

```C++
/* Wire Write Access */
Wire.beginTransmission(addr);
Wire.write(val);
Wire.endTransmission();

/* Wire Read Access */
Wire.beginTransmission(addr);
Wire.write(val);
Wire.endTransmission();
Wire.requestFrom(addr, bytes)
while(Wire.available()) {
  int val = Wire.read();
}
```

Since we are using [ARM Mbed OS](https://os.mbed.com/mbed-os/) which is a [preemptive](https://en.wikipedia.org/wiki/Preemption_(computing)) [RTOS](https://en.wikipedia.org/wiki/Real-time_operating_system) for achieving multi-tasking capability and under the assumption that all threads share the same priority (which leads to a [round-robin](https://en.wikipedia.org/wiki/Round-robin_scheduling) scheduling) it can easily happen that one thread is half-way through its Wire IO access when the scheduler interrupts it and schedules the next thread which in turn starts/continues/ends its own Wire IO access.

As a result this interruption by the scheduler will break Wire IO access for both devices and leave the Wire IO controller in an undefined state. :fire:.

`Arduino_Threads` solves this problem by encapsulating a complete IO access (e.g. reading from a `Wire` client device) within a single function call which generates an IO request to be asynchronously executed by a high-priority IO thread. The high-priority IO thread is the **only** instance which actually directly communicates with physical hardware.

#### Asynchronous
The mechanisms implemented in this library allow any thread to dispatch an IO request asynchronously and either continue operation or [yield](https://en.wikipedia.org/wiki/Yield_(multithreading))-ing control to the next scheduled thread. All IO requests are stored in a queue and are executed within a high-priority IO thread after a context-switch. An example of this can be seen [here](examples/Threadsafe_IO/Threadsafe_SPI/Threadsafe_SPI.ino)).

#### Convenient API
Although you are free to directly manipulate IO requests and responses (e.g. [Threadsafe_Wire](examples/Threadsafe_IO/Threadsafe_Wire/Threadsafe_Wire.ino)) there do exist convenient `read`/`write`/`write_then_read` abstractions inspired by the [Adafruit_BusIO](https://github.com/adafruit/Adafruit_BusIO) library (e.g. [Threadsafe_Wire_BusIO](examples/Threadsafe_IO/Threadsafe_Wire_BusIO/Threadsafe_Wire_BusIO.ino)).

## :mag_right: Resources

* [How to install a library](https://www.arduino.cc/en/guide/libraries)
* [Help Center](https://support.arduino.cc/)
* [Forum](https://forum.arduino.cc)

## :bug: Bugs & Issues

If you want to report an issue with this library, you can submit it to the [issue tracker](issues) of this repository. Remember to include as much detail as you can about your hardware set-up, code and steps for reproducing the issue. Make sure you're using an original Arduino board.

## :technologist: Development

There are many ways to contribute:

* Improve documentation and examples
* Fix a bug
* Test open Pull Requests
* Implement a new feature
* Discuss potential ways to improve this library

You can submit your patches directly to this repository as Pull Requests. Please provide a detailed description of the problem you're trying to solve and make sure you test on real hardware.

## :yellow_heart: Donations

This open-source code is maintained by Arduino with the help of the community. We invest a considerable amount of time in testing code, optimizing it and introducing new features. Please consider [donating](https://www.arduino.cc/en/donate/) or [sponsoring](https://github.com/sponsors/arduino) to support our work, as well as [buying original Arduino boards](https://store.arduino.cc/) which is the best way to make sure our effort can continue in the long term.
