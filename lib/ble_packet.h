/* -*- c++ -*- */

#ifndef INCLUDED_BLE_BLEPACKET_H
#define INCLUDED_BLE_BLEPACKET_H

#include <stdint.h>
#include <boost/circular_buffer.hpp>

namespace gr
{
namespace ble
{

enum PDUType
{
  ADV_IND = 0b0000,
  ADV_DIRECT_IND = 0b0001,
  ADV_NONCONN_IND = 0b0010,
  SCAN_REQ = 0b0011,
  SCAN_RSP = 0b0100,
  CONNECT_REQ = 0b0101,
  ADV_SCAN_IND = 0b0110
};

class BLEPacket
{
private:
  uint8_t pduHeader[2];
  uint8_t *pduData;
  uint32_t crc24;

  uint8_t swapbits(uint8_t b);
  void dewhitening(uint8_t channel, uint8_t *data, int length, uint8_t *out);
  bool crc();

public:
  BLEPacket(uint8_t *pduHeader);
  ~BLEPacket();

  PDUType getPDUType();
  void getPDUTypeName(char *dst, int max);
  uint8_t getPDUDataLength();

  bool setPDUData(boost::circular_buffer<uint8_t> &rbuff);

  bool isPDUValid();
  bool isCRCValid();
};

} // namespace ble
} // namespace gr

#endif /* INCLUDED_BLE_BLEPACKET_H */
