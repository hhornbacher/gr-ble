/* -*- c++ -*- */

#define BLE_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "ble_swig_doc.i"

%{
#include "ble/decode.h"
%}
%include "ble/decode.h"
GR_SWIG_BLOCK_MAGIC2(ble, decode);
