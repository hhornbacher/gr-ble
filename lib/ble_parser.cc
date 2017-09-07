/* -*- c++ -*- */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ble_parser.h"
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

BLEParser::BLEParser(int channel) : d_state(IDLE), d_cbuffer(32 * 1024), d_packets(), d_channel(channel)
{
}

BLEParser::~BLEParser()
{
      for (std::list<BLEPacket *>::iterator it = d_packets.begin(); it != d_packets.end(); ++it)
            delete *it;

      d_packets.clear();
}

void BLEParser::work()
{
      if (!d_cbuffer.empty())
      {
            bool loop = true;
            while (loop)
            {
                  switch (d_state)
                  {
                  case IDLE:
                        if (d_cbuffer.front() == BLE_PREAMBLE)
                        {
                              d_state = ADDR;
                        }
                        d_cbuffer.pop_front();
                        if (d_cbuffer.empty())
                              loop = false;
                        break;

                  case ADDR:
                        if (d_cbuffer.size() >= BLE_ACCESS_ADDR_LEN)
                        {
                              d_cbuffer.linearize();
                              uint8_t *buffArray = d_cbuffer.array_one().first;

                              uint32_t accessAddress = *((uint32_t *)buffArray);
                              if (accessAddress == BLE_ACCESS_ADDRESS)
                              {
                                    d_cbuffer.pop_front();
                                    d_cbuffer.pop_front();
                                    d_cbuffer.pop_front();
                                    d_cbuffer.pop_front();
                                    d_state = PDU;
                              }
                              else
                              {
                                    d_cbuffer.pop_front();
                                    d_state = IDLE;
                              }
                        }
                        else
                              loop = false;
                        break;
                  case PDU:
                        if (d_cbuffer.size() >= BLE_PDU_HEAD_LEN)
                        {
                              d_cbuffer.linearize();
                              uint8_t *buffArray = d_cbuffer.array_one().first;

                              BLEPacket *current = new BLEPacket(buffArray, d_channel);
                              char buff[32];
                              current->getPDUTypeName(buff, 32);
                              printf("Detected PDU: %s on channel %d\n", buff, d_channel);

                              if (current->isPDUValid())
                              {
                                    d_cbuffer.pop_front();
                                    d_cbuffer.pop_front();
                                    d_packets.push_back(current);
                                    d_state = DATA;
                              }
                              else
                              {
                                   // delete current;
                                    d_cbuffer.pop_front();
                                    d_state = IDLE;
                              }
                        }
                        else
                              loop = false;
                        break;
                  case DATA:
                        if (d_cbuffer.size() >= d_packets.back()->getPDUDataLength())
                        {
                              d_packets.back()->setPDUData(d_cbuffer);
                              for (int i = 0; i < d_packets.back()->getPDUDataLength(); i++)
                                    d_cbuffer.pop_front();
                              d_state = FINISH;
                        }
                        else
                              loop = false;
                        break;

                  case FINISH:
                        if (d_packets.back()->isCRCValid())
                        {
                              printf("\nPacket #%ld\n", d_packets.size());
                              printf("\nPDU Data length: %d\n", d_packets.back()->getPDUDataLength());
                              printf("PDU Header: \n");
                              printhex(d_packets.back()->pduHeader, 2);
                              printf("PDU Data: \n");
                              printhex(d_packets.back()->pduData, d_packets.back()->getPDUDataLength());
                        }
                        else
                        {
                              //delete d_packets.back();
                              d_packets.pop_back();
                        }
                        d_state = IDLE;
                        break;
                  }
            }
      }
}

void BLEParser::writeBuffer(const uint8_t *src, size_t length)
{
      for (int i = 0; i < length; i++)
      {
            d_cbuffer.push_back(src[i]);
      }
}

void BLEParser::setChannel(int channel)
{
      d_channel = channel;
      d_state = IDLE;
      d_cbuffer.clear();
}

} /* namespace ble */
} /* namespace gr */
