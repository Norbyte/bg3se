#pragma once
#include "../compression_interface.h"

#include <algorithm>

struct store_decoder_properties
  : compression_decoder_properties_interface
{
  store_decoder_properties()
    : BufferCapacity(1 << 15)
  {

  }

  void normalize() override
  {

  }

  size_t BufferCapacity;
};
