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

uint8_t BLEParser::preamble = 0xAA;

// For following values see: BT5.0 Core Spec, page 2562
size_t BLEParser::accessAddrLen = 4;
size_t BLEParser::pduHeadLen = 2;
size_t BLEParser::crcLen = 3;

// Access Address for advertising channel packets; See: BT5.0 Core Spec, page 2563
uint32_t BLEParser::accessAddr = 0x8e89bed6;

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
                        if (d_cbuffer.front() == preamble)
                        {
                              d_state = ADDR;
                        }
                        d_cbuffer.pop_front();
                        if (d_cbuffer.empty())
                              loop = false;
                        break;

                  case ADDR:
                        if (d_cbuffer.size() >= accessAddrLen)
                        {
                              d_cbuffer.linearize();
                              uint8_t *buffArray = d_cbuffer.array_one().first;

                              uint32_t accessAddress = *((uint32_t *)buffArray);
                              if (accessAddress == accessAddr)
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
                        if (d_cbuffer.size() >= pduHeadLen)
                        {
                              d_cbuffer.linearize();
                              uint8_t *buffArray = d_cbuffer.array_one().first;

                              BLEPacket *current = new BLEPacket(buffArray);
                              char buff[32];
                              current->getPDUTypeName(buff, 32);
                              printf("\npduHead: %02x %02x", buffArray[0], buffArray[1]);
                              printf("\nPDUType: %s", buff);
                              printf("\nChannel: %d", d_channel);

                              if (current->isPDUValid())
                              {
                                    d_cbuffer.pop_front();
                                    d_cbuffer.pop_front();
                                    d_packets.push_back(current);
                                    d_state = DATA;
                              }
                              else
                              {
                                    delete current;
                                    d_cbuffer.pop_front();
                                    d_state = IDLE;
                              }
                        }
                        else
                              loop = false;
                        break;
                  }
            }
            // for (int i = 0; i < d_cbuffer.size(); i++)
            // {
            //       switch (d_state)
            //       {
            //       case IDLE:
            //             if (d_cbuffer.front() == preamble)
            //             {
            //                   d_state = ADDR;
            //             }
            //             d_cbuffer.pop_front();
            //             break;
            //       case ADDR:
            //             if (d_cbuffer.size() > (accessAddrLen + pduHeadLen))
            //             {
            //                   uint32_t accessAddress = d_cbuffer.at(0) << 24;
            //                   accessAddress |= d_cbuffer.at(1) << 16;
            //                   accessAddress |= d_cbuffer.at(2) << 8;
            //                   accessAddress |= d_cbuffer.at(3);
            //                   if (accessAddress == accessAddr)
            //                   {
            //                         d_cbuffer.pop_front();
            //                         d_cbuffer.pop_front();
            //                         d_cbuffer.pop_front();
            //                         d_cbuffer.pop_front();
            //                         i += accessAddrLen - 1;
            //                         d_state = PDU;
            //                   }
            //                   else
            //                   {
            //                         d_cbuffer.pop_front();
            //                         d_state = IDLE;
            //                   }
            //             }
            //             break;
            //       case PDU:
            //       {
            //             if (d_cbuffer.size() < pduHeadLen)
            //                   return;

            //             uint8_t pduHead[2];
            //             pduHead[0] = d_cbuffer.at(1);
            //             pduHead[1] = d_cbuffer.at(0);

            //             BLEPacket *current = new BLEPacket(pduHead);
            //             char buff[32];
            //             current->getPDUTypeName(buff, 32);
            //             printf("\npduHead: %02x %02x", pduHead[0], pduHead[1]);

            //             if (current->isPDUValid())
            //             {
            //                   d_state = DATA;
            //                   i--;
            //                   d_packets.push_back(current);
            //             }
            //             else
            //             {
            //                   delete current;
            //                   d_cbuffer.pop_front();
            //                   d_state = IDLE;
            //             }
            //       }
            //       break;
            //       case DATA:
            //       {
            //             BLEPacket *current = d_packets.back();
            //             if (!current->setPDUData(d_cbuffer))
            //                   return;
            //             d_state = RESULT;
            //       }
            //       break;
            //       case CRC:
            //             // crc = packetData[dataLen - 1] << 16;
            //             // crc |= packetData[dataLen - 2] << 8;
            //             // crc |= packetData[dataLen - 3];
            //             // printf("CRC: 0x%06x\n", crc);
            //             // d_state = RESULT;
            //             break;
            //       case RESULT:
            //             BLEPacket *current = d_packets.back();
            //             if (current->getPDUType() == ADV_NONCONN_IND && current->getPDUDataLength() > 0)
            //             {
            //                   printf("\nPacket count: %ld", d_packets.size());
            //                   // dewhitening(39, packetData, dataLen, buf);
            //                   // uint8_t *macAddr = &packetData[blePDUHeadLen];
            //                   // printf("\nMAC: ");
            //                   // printhex(packetData, 6);
            //                   // printf("\nMAC[Dewhitened]: ");
            //                   // printhex(buf, 6);
            //             }
            //             else
            //                   d_packets.pop_back();
            //             d_state = IDLE;
            //             break;
            //       }
            // }
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
}

} /* namespace ble */
} /* namespace gr */
