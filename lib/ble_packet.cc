/* -*- c++ -*- */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ble_packet.h"

#include <string.h>
#include <stdio.h>

namespace gr
{
namespace ble
{

/*
       * The private constructor
       */
BLEPacket::BLEPacket(uint8_t *header)
{
    memcpy(pduHeader, header, 2);
    pduData = new uint8_t[getPDUDataLength()];
}

/*
       * Our virtual destructor.
       */
BLEPacket::~BLEPacket()
{
    delete pduData;
}

//Taken from: https://stackoverflow.com/a/2602885
uint8_t BLEPacket::swapbits(uint8_t b)
{
    b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
    b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
    b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
    return b;
}

void BLEPacket::dewhitening(uint8_t channel, uint8_t *data, int length, uint8_t *out)
{
    uint16_t lfsr = swapbits(channel) | 2;

    for (int i = 0; i < length; i++)
    {
        uint8_t d = swapbits(data[i]);
        for (int bit = 0; bit < 8; bit++)
        {
            uint8_t b = 1 << bit;
            if (lfsr & 0x80)
            {
                lfsr ^= 0x11;
                d ^= b;
            }
            lfsr = lfsr << 1;
        }
        out[i] = d;
    }
}

bool BLEPacket::crc()
{
}

PDUType BLEPacket::getPDUType()
{
    return (PDUType)(pduHeader[0] & 0x0f);
}

void BLEPacket::getPDUTypeName(char *dst, int max)
{
    PDUType pduType = getPDUType();
    if (pduType == ADV_IND)
        snprintf(dst, max, "ADV_IND");
    else if (pduType == ADV_DIRECT_IND)
        snprintf(dst, max, "ADV_IND");
    else if (pduType == SCAN_REQ)
        snprintf(dst, max, "ADV_IND");
    else if (pduType == SCAN_RSP)
        snprintf(dst, max, "ADV_IND");
    else if (pduType == CONNECT_REQ)
        snprintf(dst, max, "ADV_IND");
    else if (pduType == ADV_SCAN_IND)
        snprintf(dst, max, "ADV_IND");
    else if (pduType == ADV_NONCONN_IND)
        snprintf(dst, max, "ADV_IND");
    else
        snprintf(dst, max, "UNKNOWN: 0x%02x", pduType);
}

uint8_t BLEPacket::getPDUDataLength()
{
    return pduHeader[1];
}

bool BLEPacket::setPDUData(boost::circular_buffer<uint8_t> &rbuff)
{
    if (rbuff.size() < getPDUDataLength())
        return false;
    for (int i = 0; i < getPDUDataLength(); i++)
    {
        pduData[i] = rbuff.front();
        rbuff.pop_front();
    }
    return true;
}

bool BLEPacket::isPDUValid()
{
    PDUType pduType = getPDUType();
    if (pduType == ADV_IND || pduType == ADV_DIRECT_IND || pduType == SCAN_REQ || pduType == SCAN_RSP || pduType == CONNECT_REQ || pduType == ADV_SCAN_IND || pduType == ADV_NONCONN_IND)
    {
        return true;
    }
    return false;
}

bool BLEPacket::isCRCValid()
{
}

} /* namespace ble */
} /* namespace gr */
