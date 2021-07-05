/*
 * A deeply magical library providing threadsafe IO via pipes.
 */

#ifndef IO_REQUEST_H_
#define IO_REQUEST_H_

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include <stdint.h>

/**************************************************************************************
 * CLASS DECLARATION
 **************************************************************************************/

class IoRequest
{
public:

  IoRequest(uint8_t const * const write_buf_, size_t const bytes_to_write_, uint8_t * read_buf_, size_t const bytes_to_read_)
  : write_buf{write_buf_}
  , bytes_to_write{bytes_to_write_}
  , read_buf{read_buf_}
  , bytes_to_read{bytes_to_read_}
  { }


  uint8_t const * const write_buf{nullptr};
  size_t const bytes_to_write{0};
  uint8_t * read_buf{nullptr};
  size_t const bytes_to_read{0};

};

#endif /* IO_REQUEST_H_ */