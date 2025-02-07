#pragma once

BEGIN_NS(lua)

class Callable {};
// Supports legacy indexing through CppObjectMetadata
class Indexable {};
// Supports TValue indexing through CppObjectOpaque
class OpaqueIndexable {};
class NewIndexable {};
class Lengthable {};
class Iterable {};
class Stringifiable {};
class GarbageCollected {};
class EqualityComparable {};
class LessThanComparable {};
class HasBinaryOps {};

END_NS()
