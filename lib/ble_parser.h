/* -*- c++ -*- */

#ifndef INCLUDED_BLE_BLEPARSER_H
#define INCLUDED_BLE_BLEPARSER_H

#include "ble_packet.h"

#include <boost/circular_buffer.hpp>

namespace gr
{
namespace ble
{

class BLEParser
{
private:
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
