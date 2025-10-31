#pragma once
#include "../compression_interface.h"

#include <algorithm>

struct bzip2_encoder_properties
  : compression_encoder_properties_interface
{
  bzip2_encoder_properties()
    : BufferCapacity(1 << 15)
    , BlockSize(6)
    , WorkFactor(30)
  {

  }

  void normalize() override
  {
    BlockSize = clamp(1, 9, BlockSize);
    WorkFactor = clamp(0, 4, WorkFactor);
  }

  size_t  BufferCapacity;
  int     BlockSize;
  int     WorkFactor;
};
