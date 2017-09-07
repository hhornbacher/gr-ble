/* -*- c++ -*- */

#ifndef INCLUDED_BLE_CONSTANTS_H
#define INCLUDED_BLE_CONSTANTS_H

#include "ble_packet.h"

#include <stdint.h>
#include <list>
#include <boost/circular_buffer.hpp>

namespace gr
{
namespace ble
{

#define BLE_PREAMBLE 0xaa

// For following values see: BT5.0 Core Spec, page 2562
#define BLE_ACCESS_ADDR_LEN 4
#define BLE_PDU_HEAD_LEN 2
#define BLE_CRC_LEN 3
#define BLE_MAX_PDU_LEN 255

// Access Address for advertising channel packets; See: BT5.0 Core Spec, page 2563
#define BLE_ACCESS_ADDRESS 0x8e89bed6

} // namespace ble
} // namespace gr

#endif /* INCLUDED_BLE_CONSTANTS_H */
