#pragma once
#include "../compression_interface.h"

#include <algorithm>

struct deflate_encoder_properties
  : compression_encoder_properties_interface
{
  deflate_encoder_properties()
    : BufferCapacity(1 << 15)
    , CompressionLevel(6)
  {

  }

  void normalize() override
  {
    CompressionLevel = clamp(0, 9, CompressionLevel);
  }

  size_t BufferCapacity;
  int    CompressionLevel;
};
