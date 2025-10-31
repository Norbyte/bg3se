#pragma once
#include "ICompressionMethod.h"
#include "../compression/store/store_encoder.h"
#include "../compression/store/store_decoder.h"

#include <memory>

class StoreMethod :
  public ICompressionMethod
{
  public:
    ZIP_METHOD_CLASS_PROLOGUE(
      StoreMethod,
      store_encoder, store_decoder,
      _encoderProps, _decoderProps,
      /* CompressionMethod */ 0,
      /* VersionNeededToExtract */ 10
    );

  private:
    store_encoder_properties _encoderProps;
    store_decoder_properties _decoderProps;
};
