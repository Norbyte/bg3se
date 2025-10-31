#pragma once
#include <iostream>
#include <vector>

namespace utils { namespace stream {

static void copy(std::istream& from, std::ostream& to, size_t bufferSize = 1024 * 1024)
{
  std::vector<char> buff(bufferSize);

  do
  {
    from.read(buff.data(), buff.size());
    to.write(buff.data(), from.gcount());
  } while (static_cast<size_t>(from.gcount()) == buff.size());
}

} }
