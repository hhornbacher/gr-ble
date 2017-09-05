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

#ifndef INCLUDED_BLE_DECODE_IMPL_H
#define INCLUDED_BLE_DECODE_IMPL_H

#include <ble/decode.h>
#include <boost/circular_buffer.hpp>

namespace gr
{
namespace ble
{

class decode_impl : public decode
{
private:
  // Nothing to declare in this block.
  boost::circular_buffer<uint8_t> *d_cbuffer;

  enum States
  {
    IDLE,
    PREAMBLE,
    ADDR,
    PDU,
    DATA,
    RESULT
  } d_state;

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

public:
  decode_impl();
  ~decode_impl();

  // Where all the action really happens
  int work(int noutput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);
};

} // namespace ble
} // namespace gr

#endif /* INCLUDED_BLE_DECODE_IMPL_H */
