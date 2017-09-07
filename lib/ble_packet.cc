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
static void printhex(uint8_t *ptr, int count)
{
    for (int i = 0; i < (count / 16) + 1; i++)
    {
        for (int j = 0; j < 16 && (i * 16) + j < count; j++)
        {
            printf("%02x ", ptr[(i * 16) + j]);
        }
        printf("\n");
    }
    printf("\n");
}

/*
       * The private constructor
       */
BLEPacket::BLEPacket(uint8_t *header, int chan) : channel((uint8_t)chan)
{
    pduHeader = data;
    pduData = &data[2];
    dewhitening(header, 2, pduHeader);
    memccpy(pduHeaderRaw, header, 2, 2);
}

/*
       * Our virtual destructor.
       */
BLEPacket::~BLEPacket()
{
    delete pduData;
}

// Taken from: https://stackoverflow.com/a/2602885
uint8_t BLEPacket::bitswap(uint8_t b)
{
    b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
    b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
    b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
    return b;
}

// Inspired by: https://github.com/drtyhlpr/ble_dump/blob/master/proto.py
void BLEPacket::dewhitening(uint8_t *data, int length, uint8_t *out)
{
    uint32_t lfsr = bitswap(channel) | 2;

    for (int i = 0; i < length; i++)
    {
        uint8_t d = bitswap(data[i]);
        for (int bit = 0; bit < 8; bit++)
        {
            uint8_t b = 1 << (7 - bit);
            if (lfsr & 0x80)
            {
                lfsr ^= 0x11;
                d ^= b;
            }
            lfsr = lfsr << 1;
        }
        out[i] = bitswap(d);
    }
}

// Taken from: https://dmitry.gr/index.php?r=05.Projects&proj=11.%20Bluetooth%20LE%20fakery
bool BLEPacket::crc()
{
    uint8_t dst[] = {0x55, 0x55, 0x55};
    uint8_t len = getPDUDataLength() - 3 + 2;

    uint8_t v, t, d;

    uint8_t *dataIt = data;
    while (len--)
    {
        d = *dataIt++;
        for (v = 0; v < 8; v++, d >>= 1)
        {

            t = dst[0] >> 7;
            dst[0] <<= 1;
            if (dst[1] & 0x80)
                dst[0] |= 1;
            dst[1] <<= 1;
            if (dst[2] & 0x80)
                dst[1] |= 1;
            dst[2] <<= 1;

            if (t != (d & 1))
            {

                dst[2] ^= 0x5B;
                dst[1] ^= 0x06;
            }
        }
    }
    dst[0] = bitswap(dst[0]);
    dst[1] = bitswap(dst[1]);
    dst[2] = bitswap(dst[2]);

    if (memcmp(dst, &data[getPDUDataLength() - 1], 3) == 0)
        return true;
    return false;
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
        snprintf(dst, max, "ADV_DIRECT_IND");
    else if (pduType == SCAN_REQ)
        snprintf(dst, max, "SCAN_REQ");
    else if (pduType == SCAN_RSP)
        snprintf(dst, max, "SCAN_RSP");
    else if (pduType == CONNECT_REQ)
        snprintf(dst, max, "CONNECT_REQ");
    else if (pduType == ADV_SCAN_IND)
        snprintf(dst, max, "ADV_SCAN_IND");
    else if (pduType == ADV_NONCONN_IND)
        snprintf(dst, max, "ADV_NONCONN_IND");
    else
        snprintf(dst, max, "UNKNOWN: 0x%02x", pduType);
}

uint8_t BLEPacket::getPDUDataLength()
{
    return pduHeader[1] + 3;
}

void BLEPacket::setPDUData(boost::circular_buffer<uint8_t> &rbuff)
{
    uint8_t tmpBuffer[260];
    uint8_t outBuffer[260];
    uint8_t len = getPDUDataLength();
    rbuff.linearize();
    memccpy(tmpBuffer, pduHeaderRaw, 2, 260);
    memccpy(&tmpBuffer[2], rbuff.array_one().first, len, 258);
    dewhitening(tmpBuffer, len + 2, outBuffer);
    memccpy(pduData, &outBuffer[2], len, len);
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
    return crc();
}

} /* namespace ble */
} /* namespace gr */
