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
  ADV_IND = 0x0,
  ADV_DIRECT_IND = 0x1,
  ADV_NONCONN_IND = 0x2,
  SCAN_REQ = 0x3,
  SCAN_RSP = 0x4,
  CONNECT_REQ = 0x5,
  ADV_SCAN_IND = 0x6
};

class BLEPacket
{
private:
  uint8_t data[260];
  uint8_t pduHeaderRaw[2];
  uint32_t crc24;
  uint8_t channel;

  uint8_t bitswap(uint8_t b);
  void dewhitening(uint8_t *data, int length, uint8_t *out);
  bool crc();

public:
  BLEPacket(uint8_t *pduHeader, int chan);
  ~BLEPacket();

  uint8_t *pduHeader;
  uint8_t *pduData;


  PDUType getPDUType();
  void getPDUTypeName(char *dst, int max);
  uint8_t getPDUDataLength();

  void setPDUData(boost::circular_buffer<uint8_t> &rbuff);

  bool isPDUValid();
  bool isCRCValid();
};

} // namespace ble
} // namespace gr

#endif /* INCLUDED_BLE_BLEPACKET_H */
