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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "decode_impl.h"

namespace gr
{
namespace ble
{

static uint8_t blePreamble = 0xaa;
// For following values see: BT5.0 Core Spec, page 2562
static int bleAddrLen = 4;
static int blePDUHeaderLen = 2;
static int bleCRCLen = 3;
static uint32_t bleAccessAddr = 0xd6be898e; // Access Address for advertising channel packets (0x8e89bed6) Reversed order! See: BT5.0 Core Spec, page 2563

void printhex(uint8_t *ptr, int count)
{
  for (int i = 0; i < (count / 8); i++)
  {
    for (int j = 0; j < 8 && (i * 8) + j < count; j++)
    {
      printf("%02x ", ptr[(i * 8) + j]);
    }
    printf("\n");
  }
  printf("\n");
}

decode::sptr
decode::make()
{
  return gnuradio::get_initial_sptr(new decode_impl());
}

/*
     * The private constructor
     */
decode_impl::decode_impl()
    : gr::sync_block("decode",
                     gr::io_signature::make(1, 1, sizeof(uint8_t)),
                     gr::io_signature::make(0, 0, 0)),
      d_state(IDLE)
{
  d_cbuffer = new boost::circular_buffer<uint8_t>(8 * 1024);
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

  for (int i = 0; i < noutput_items; i++)
  {
    d_cbuffer->push_back(in[i]);
  }

  if (!d_cbuffer->empty())
  {
    uint8_t dataLen = 0;
    uint8_t packetData[64];
    for (int i = 0; i < d_cbuffer->size(); i++)
    {
      switch (d_state)
      {
      case IDLE:
        if (d_cbuffer->front() == blePreamble)
        {
          d_state = ADDR;
        }
        d_cbuffer->pop_front();
        break;
      case ADDR:
        if (d_cbuffer->size() > (bleAddrLen + blePDUHeaderLen))
        {
          uint32_t accessAddress = d_cbuffer->at(0) << 24;
          accessAddress |= d_cbuffer->at(1) << 16;
          accessAddress |= d_cbuffer->at(2) << 8;
          accessAddress |= d_cbuffer->at(3);
          if (accessAddress == bleAccessAddr)
          {
            d_cbuffer->pop_front();
            d_cbuffer->pop_front();
            d_cbuffer->pop_front();
            d_cbuffer->pop_front();
            i += 3;
            d_state = PDU;
          }
          else
          {
            d_cbuffer->pop_front();
            d_state = IDLE;
          }
        }
        break;
      case PDU:
      {
        uint8_t pduHeader = d_cbuffer->at(1) & 0x0f;
        dataLen = d_cbuffer->at(0) & 0x3f;
        if (pduHeader == ADV_IND)
        {
          printf("ADV_IND!\n");
          d_state = DATA;
          i++;
        }
        else if (pduHeader == ADV_DIRECT_IND)
        {
          printf("ADV_DIRECT_IND!\n");
          d_state = DATA;
          i++;
        }
        else if (pduHeader == ADV_NONCONN_IND)
        {
          printf("ADV_NONCONN_IND!\n");
          d_state = DATA;
          i++;
        }
        else if (pduHeader == SCAN_REQ)
        {
          printf("SCAN_REQ!\n");
          d_state = DATA;
          i++;
        }
        else if (pduHeader == SCAN_RSP)
        {
          printf("SCAN_RES!\n");
          d_state = DATA;
          i++;
        }
        else if (pduHeader == CONNECT_REQ)
        {
          printf("CONNECT_REQ!\n");
          d_state = DATA;
          i++;
        }
        else if (pduHeader == ADV_SCAN_IND)
        {
          printf("ADV_SCAN_IND!\n");
          d_state = DATA;
          i++;
        }
        else
        {
          d_cbuffer->pop_front();
          d_state = IDLE;
        }
      }
      break;
      case DATA:
        printf("Data Len = %d\n", dataLen);
        if (d_cbuffer->size() < dataLen)
        {
          return noutput_items;
        }
        for (int j = 0; j < dataLen; j++)
        {
          packetData[j] = d_cbuffer->front();
          d_cbuffer->pop_front();
          i++;
        }
        d_state = RESULT;
        break;
      case RESULT:
        printhex(packetData, dataLen);
        d_state = IDLE;
        break;
      }
    }
  }

  // Tell runtime system how many output items we produced.
  return noutput_items;
}

} /* namespace ble */
} /* namespace gr */
