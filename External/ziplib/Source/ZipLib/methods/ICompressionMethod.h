#pragma once
#include "../compression/compression_interface.h"

#include "../compression/store/store_encoder.h"
#include "../compression/store/store_decoder.h"

#include <memory>

#define ZIP_METHOD_CLASS_PROLOGUE(                                              \
    method_class,                                                               \
    encoder_class, decoder_class,                                               \
    encoder_props_member, decoder_props_member,                                 \
    compression_method, version_needed_to_extract)                              \
                                                                                \
  typedef std::shared_ptr<method_class> Ptr;                                    \
                                                                                \
  static const uint16_t CompressionMethod = compression_method;                 \
  static const uint16_t VersionNeededToExtract = version_needed_to_extract;     \
                                                                                \
  method_class()                                                                \
  {                                                                             \
    this->SetEncoder(std::make_shared<encoder_class>());                        \
    this->SetDecoder(std::make_shared<decoder_class>());                        \
  }                                                                             \
                                                                                \
  static Ptr Create()                                                           \
  {                                                                             \
    return std::make_shared<method_class>();                                    \
  }                                                                             \
                                                                                \
  compression_encoder_properties_interface& GetEncoderProperties() override     \
  {                                                                             \
    encoder_props_member.normalize();                                           \
    return encoder_props_member;                                                \
  }                                                                             \
                                                                                \
  compression_decoder_properties_interface& GetDecoderProperties() override     \
  {                                                                             \
    decoder_props_member.normalize();                                           \
    return decoder_props_member;                                                \
  }                                                                             \
                                                                                \
  const ZipMethodDescriptor& GetZipMethodDescriptor() const override            \
  {                                                                             \
    return GetZipMethodDescriptorStatic();                                      \
  }                                                                             \
                                                                                \
  static const ZipMethodDescriptor& GetZipMethodDescriptorStatic()              \
  {                                                                             \
    static ZipMethodDescriptor zmd(CompressionMethod, VersionNeededToExtract);  \
    return zmd;                                                                 \
  }

struct ZipMethodDescriptor
{
  ZipMethodDescriptor(uint16_t compressionMethod, uint16_t versionNeededToExtract)
    : CompressionMethod(compressionMethod)
    , VersionNeededToExtract(versionNeededToExtract)
  {

  }

  uint16_t GetCompressionMethod() const { return CompressionMethod; }
  uint16_t GetVersionNeededToExtract() const { return VersionNeededToExtract; }

  private:
    const uint16_t CompressionMethod;
    const uint16_t VersionNeededToExtract;
};

class ICompressionMethod
{
  public:
    static const uint16_t StoredCompressionMethod = 0;
    static const uint16_t StoredVersionNeededToExtract = 10;

    typedef std::shared_ptr<ICompressionMethod> Ptr;

    typedef std::shared_ptr<compression_encoder_interface> encoder_t;
    typedef std::shared_ptr<compression_decoder_interface> decoder_t;

    encoder_t GetEncoder() const { return _encoder; }
    decoder_t GetDecoder() const { return _decoder; }

    virtual compression_encoder_properties_interface& GetEncoderProperties() = 0;
    virtual compression_decoder_properties_interface& GetDecoderProperties() = 0;

    virtual const ZipMethodDescriptor& GetZipMethodDescriptor() const = 0;
    static const ZipMethodDescriptor& GetZipMethodDescriptorStatic()
    {
      // Default "Stored method" descriptor.
      static ZipMethodDescriptor zmd(StoredCompressionMethod, StoredVersionNeededToExtract);
      return zmd;
    }

  protected:
    void SetEncoder(encoder_t encoder) { _encoder = encoder; }
    void SetDecoder(decoder_t decoder) { _decoder = decoder; }

  private:
    encoder_t _encoder;
    decoder_t _decoder;
};
