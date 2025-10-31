#pragma once
#include <cstdint>

#define MARK_AS_TYPED_ENUMFLAGS_BASE(EnumType, type, friend)                                                        \
  inline friend bool     operator !  (EnumType  lhs)                { return bool( ! type (lhs) ); }                \
  inline friend EnumType operator ~  (EnumType  lhs)                { return EnumType( ~ type (lhs) ); }            \
  inline friend EnumType operator |  (EnumType  lhs, EnumType rhs)  { return EnumType( type (lhs) | type (rhs) ); } \
  inline friend EnumType operator &  (EnumType  lhs, EnumType rhs)  { return EnumType( type (lhs) & type (rhs) ); } \
  inline friend EnumType operator ^  (EnumType  lhs, EnumType rhs)  { return EnumType( type (lhs) ^ type (rhs) ); } \
  inline friend EnumType operator |= (EnumType& lhs, EnumType rhs)  { return lhs = lhs | rhs; }                     \
  inline friend EnumType operator &= (EnumType& lhs, EnumType rhs)  { return lhs = lhs & rhs; }                     \
  inline friend EnumType operator ^= (EnumType& lhs, EnumType rhs)  { return lhs = lhs ^ rhs; }

#define MARK_AS_TYPED_ENUMFLAGS(EnumType)         MARK_AS_TYPED_ENUMFLAGS_BASE(EnumType, uint32_t, )
#define MARK_AS_TYPED_ENUMFLAGS_FRIEND(EnumType)  MARK_AS_TYPED_ENUMFLAGS_BASE(EnumType, uint32_t, friend)
