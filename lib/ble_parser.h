/* -*- c++ -*- */

#ifndef INCLUDED_BLE_BLEPARSER_H
#define INCLUDED_BLE_BLEPARSER_H

#include "ble_packet.h"
#include "ble_constants.h"

#include <stdint.h>
#include <list>
#include <boost/circular_buffer.hpp>

namespace gr
{
namespace ble
{

class BLEParser
{
private:
  int d_channel;
  std::list<BLEPacket *> d_packets;
  boost::circular_buffer<uint8_t> d_cbuffer;
  enum States
  {
    IDLE,
    PREAMBLE,
    ADDR,
    PDU,
    DATA,
    FINISH
  } d_state;

public:
  BLEParser(int channel);
  ~BLEParser();

  void work();
  void writeBuffer(const uint8_t *src, size_t length);
  void setChannel(int channel);
  int getChannel() const
  {
    return d_channel;
  }
};

} // namespace ble
} // namespace gr

#endif /* INCLUDED_BLE_BLEPARSER_H */
