#include "EndOfCentralDirectoryBlock.h"
#include "../streams/serialization.h"
#include <cstring>

namespace detail {

EndOfCentralDirectoryBlock::EndOfCentralDirectoryBlock()
{
  memset(this, 0, sizeof(EndOfCentralDirectoryBlockBase));
  Signature = SignatureConstant;
}

bool EndOfCentralDirectoryBlock::Deserialize(std::istream& stream)
{
  // this condition should be optimized out :)
  if (sizeof(EndOfCentralDirectoryBlockBase) == EndOfCentralDirectoryBlockBase::SIZE_IN_BYTES)
  {
    deserialize<EndOfCentralDirectoryBlockBase>(stream, *this);
  }
  else
  {
    deserialize(stream, Signature);
    deserialize(stream, NumberOfThisDisk);
    deserialize(stream, NumberOfTheDiskWithTheStartOfTheCentralDirectory);
    deserialize(stream, NumberOfEntriesInTheCentralDirectoryOnThisDisk);
    deserialize(stream, NumberOfEntriesInTheCentralDirectory);
    deserialize(stream, SizeOfCentralDirectory);
    deserialize(stream, OffsetOfStartOfCentralDirectoryWithRespectToTheStartingDiskNumber);
    deserialize(stream, CommentLength);
  }

  deserialize(stream, Comment, CommentLength);

  return true;
}

void EndOfCentralDirectoryBlock::Serialize(std::ostream& stream)
{
  CommentLength = static_cast<uint16_t>(Comment.length());
 
  if (sizeof(EndOfCentralDirectoryBlockBase) == EndOfCentralDirectoryBlockBase::SIZE_IN_BYTES)
  {
    serialize<EndOfCentralDirectoryBlockBase>(stream, *this);
  }
  else
  {
    serialize(stream, Signature);
    serialize(stream, NumberOfThisDisk);
    serialize(stream, NumberOfTheDiskWithTheStartOfTheCentralDirectory);
    serialize(stream, NumberOfEntriesInTheCentralDirectoryOnThisDisk);
    serialize(stream, NumberOfEntriesInTheCentralDirectory);
    serialize(stream, SizeOfCentralDirectory);
    serialize(stream, OffsetOfStartOfCentralDirectoryWithRespectToTheStartingDiskNumber);
    serialize(stream, CommentLength);
  }

  serialize(stream, Comment);
}

}
