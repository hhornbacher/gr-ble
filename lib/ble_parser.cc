/* -*- c++ -*- */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ble_parser.h"

namespace gr
{
namespace ble
{

uint8_t BLEParser::preamble = 0xAA;

// For following values see: BT5.0 Core Spec, page 2562
size_t BLEParser::accessAddrLen = 4;
size_t BLEParser::pduHeadLen = 2;
size_t BLEParser::crcLen = 3;

// Access Address for advertising channel packets (0x8e89bed6) Reversed order! See: BT5.0 Core Spec, page 2563
uint32_t BLEParser::accessAddr = 0xd6be898e;

BLEParser::BLEParser() : d_state(IDLE), d_cbuffer(8 * 1024)
{
      //d_cbuffer = new boost::circular_buffer<uint8_t>(8 * 1024);
}

BLEParser::~BLEParser()
{
}

} /* namespace ble */
} /* namespace gr */
