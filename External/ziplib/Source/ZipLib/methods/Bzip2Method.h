#pragma once
#include "ICompressionMethod.h"
#include "../compression/bzip2/bzip2_encoder.h"
#include "../compression/bzip2/bzip2_decoder.h"

#include <memory>

class Bzip2Method :
  public ICompressionMethod
{
  public:
    ZIP_METHOD_CLASS_PROLOGUE(
      Bzip2Method,
      bzip2_encoder, bzip2_decoder,
      _encoderProps, _decoderProps,
      /* CompressionMethod */ 12,
      /* VersionNeededToExtract */ 46
    );

    enum class BlockSize : int
    {
      B100 = 1,
      B200 = 2,
      B300 = 3,
      B400 = 4,
      B500 = 5,
      B600 = 6,
      B700 = 7,
      B800 = 8,
      B900 = 9,

      Fastest = B100,
      Default = B600,
      Best = B900
    };

    size_t GetBufferCapacity() const { return _encoderProps.BufferCapacity; }
    void SetBufferCapacity(size_t bufferCapacity) { _encoderProps.BufferCapacity = bufferCapacity; }

    BlockSize GetBlockSize() const { return static_cast<BlockSize>(_encoderProps.BlockSize); }
    void SetBlockSize(BlockSize compressionLevel) { _encoderProps.BlockSize = static_cast<int>(compressionLevel); }

  private:
    bzip2_encoder_properties _encoderProps;
    bzip2_decoder_properties _decoderProps;
};
