#pragma once

#include <GameDefinitions/Base/Base.h>

BEGIN_SE()

struct Version
{
	inline Version() : Ver(0) {}
	inline Version(uint64_t ver) : Ver(ver) {}
	inline Version(uint64_t minor, uint64_t major, uint64_t revision, uint64_t build)
		: Ver(((major & 0x1ffull) << 55) | ((minor & 0x1ffull) << 47) | ((revision & 0x7ffffull) << 31) | (build & 0x7fffffffull))
	{}

	inline uint64_t Major() const
	{
		return Ver >> 55;
	}

	inline uint64_t Minor() const
	{
		return (Ver >> 47) & 0xff;
	}

	inline uint64_t Revision() const
	{
		return (Ver >> 31) & 0x7ffff;
	}

	inline uint64_t Build() const
	{
		return Ver & 0x7fffffffull;
	}

	uint64_t Ver;
};

struct GameRandom
{
	int* FixedRollList;
	int CurFixedRollIndex;
	int Steps;
	int RerolledLCG_X;
	int BucketCount;
	int LCG_X;
	int MinRoll;
	int StepSize;
	uint8_t field_24;
};

struct CriticalSection : public CRITICAL_SECTION
{
	__int64 field_28;
	__int64 field_30;
};

END_SE()
