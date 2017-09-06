/* -*- c++ -*- */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "decode_impl.h"

namespace gr
{
namespace ble
{

decode::sptr
decode::make(int channel)
{
  return gnuradio::get_initial_sptr(new decode_impl(channel));
}

/*
     * The private constructor
     */
decode_impl::decode_impl(int channel)
    : gr::sync_block("decode",
                     gr::io_signature::make(1, 1, sizeof(uint8_t)),
                     gr::io_signature::make(0, 0, 0)),
      d_parser(channel)
{
}

/*
     * Our virtual destructor.
     */
decode_impl::~decode_impl()
{
}

int decode_impl::work(int noutput_items,
                      gr_vector_const_void_star &input_items,
                      gr_vector_void_star &output_items)
{
  const uint8_t *in = (const uint8_t *)input_items[0];

  d_parser.writeBuffer(in, noutput_items);
  d_parser.work();

  // Tell runtime system how many output items we produced.
  return noutput_items;
}
void decode_impl::set_channel(int channel)
{
  d_parser.setChannel(channel);
}

} /* namespace ble */
} /* namespace gr */
