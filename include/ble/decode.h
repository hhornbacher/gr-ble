/* -*- c++ -*- */


#ifndef INCLUDED_BLE_DECODE_H
#define INCLUDED_BLE_DECODE_H

#include <ble/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace ble {

    /*!
     * \brief <+description of block+>
     * \ingroup ble
     *
     */
    class BLE_API decode : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<decode> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of ble::decode.
       *
       * To avoid accidental use of raw pointers, ble::decode's
       * constructor is in a private implementation
       * class. ble::decode::make is the public interface for
       * creating new instances.
       */
      static sptr make();
    };

  } // namespace ble
} // namespace gr

#endif /* INCLUDED_BLE_DECODE_H */

