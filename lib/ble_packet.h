/* -*- c++ -*- */

#ifndef INCLUDED_BLE_BLEPACKET_H
#define INCLUDED_BLE_BLEPACKET_H

#include "ble_constants.h"

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
  uint8_t data[BLE_PDU_HEAD_LEN + BLE_MAX_PDU_LEN + BLE_CRC_LEN];
  uint8_t pduHeaderRaw[BLE_PDU_HEAD_LEN];
  uint8_t channel;
  bool valid;

  uint8_t bitswap(uint8_t b);
  void dewhitening(uint8_t *data, int length, uint8_t *out);
  bool checkCRC();

public:
  uint8_t *pduHeader;
  uint8_t *pduData;
  uint8_t *crc;

  BLEPacket(uint8_t *pduHeader, int chan);
  ~BLEPacket();

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
