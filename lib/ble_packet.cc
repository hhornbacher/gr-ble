/* -*- c++ -*- */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ble_packet.h"

namespace gr
{
namespace ble
{

/*
       * The private constructor
       */
BLEPacket::BLEPacket(uint16_t header)
{
    memcpy(pduHeader, (uint8_t *)&header, 2);
}

/*
       * Our virtual destructor.
       */
BLEPacket::~BLEPacket()
{
}

PDUType BLEPacket::getPDUType()
{
    return (PDUType)(pduHeader[0] & 0x0f);
}

uint8_t BLEPacket::getPDUDataLength()
{
    return pduHeader[1];
}

} /* namespace ble */
} /* namespace gr */
