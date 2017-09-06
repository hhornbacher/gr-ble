/* -*- c++ -*- */
/* 
 * Copyright 2017 Harry Hornbacher.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef INCLUDED_BLE_BLEPACKET_H
#define INCLUDED_BLE_BLEPACKET_H

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
}

} // namespace ble
} // namespace gr

#endif /* INCLUDED_BLE_BLEPACKET_H */
