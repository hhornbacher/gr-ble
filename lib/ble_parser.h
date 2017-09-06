/* -*- c++ -*- */

#ifndef INCLUDED_BLE_BLEPARSER_H
#define INCLUDED_BLE_BLEPARSER_H

#include "ble_packet.h"

#include <stdint.h>
#include <boost/circular_buffer.hpp>

namespace gr
{
namespace ble
{
  

class BLEParser
{
private:
  static size_t accessAddrLen;
  static size_t pduHeadLen;
  static size_t crcLen;

  static uint8_t preamble;
  static uint32_t accessAddr; 

  boost::circular_buffer<uint8_t> d_cbuffer;
  enum States
  {
    IDLE,
    PREAMBLE,
    ADDR,
    PDU,
    DATA,
    CRC,
    RESULT
  } d_state;

public:
  BLEParser();
  ~BLEParser();
};

} // namespace ble
} // namespace gr

#endif /* INCLUDED_BLE_BLEPARSER_H */
