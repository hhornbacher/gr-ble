/* -*- c++ -*- */

#ifndef INCLUDED_BLE_BLEPACKET_H
#define INCLUDED_BLE_BLEPACKET_H

#include <stdint.h>

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

  public:
    BLEPacket(uint16_t pduHeader);
    ~BLEPacket();

    PDUType getPDUType();
    uint8_t getPDUDataLength();
    bool isValid();

    void setPDUData();
};

} // namespace ble
} // namespace gr

#endif /* INCLUDED_BLE_BLEPACKET_H */
