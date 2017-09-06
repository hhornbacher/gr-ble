/* -*- c++ -*- */

#ifndef INCLUDED_BLE_DECODE_IMPL_H
#define INCLUDED_BLE_DECODE_IMPL_H

#include <ble/decode.h>
#include <boost/circular_buffer.hpp>

#include "ble_parser.h"

namespace gr
{
namespace ble
{

class decode_impl : public decode
{
private:
  BLEParser d_parser;

public:
  decode_impl(int channel);
  ~decode_impl();

  // Where all the action really happens
  int work(int noutput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);

  void set_channel(int channel);
};

} // namespace ble
} // namespace gr

#endif /* INCLUDED_BLE_DECODE_IMPL_H */
