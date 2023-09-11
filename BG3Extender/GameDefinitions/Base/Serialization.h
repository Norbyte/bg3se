#pragma once

BEGIN_SE()

struct ObjectVisitor : public ProtectedGameObject<ObjectVisitor>
{
	virtual void Destroy() = 0;
	virtual void SetFileName(Path*) = 0;
	virtual void SetFileVersion(uint32_t*) = 0;
	virtual void SetCodeVersion(uint32_t*) = 0;
	virtual Path* GetFileName() = 0;
	virtual uint32_t GetCodeVersion() = 0;
	virtual uint32_t GetFileVersion() = 0;
	virtual void SetIO() = 0;
	virtual bool IsReading() = 0;
	virtual void Save(Path*) = 0;
	virtual void Load(Path*) = 0;
	virtual void Print(const char*) = 0;
	virtual bool TryEnterRegion(FixedString const&) = 0;
	virtual bool EnterRegion(FixedString const&) = 0;
	virtual void HasRegion(FixedString const&) = 0;
	virtual void ExitRegion(FixedString const&) = 0;
	virtual bool EnterNode(FixedString const&, FixedString const&) = 0;
	virtual void HasNode(FixedString const&) = 0;
	virtual void ExitNode(FixedString const&) = 0;
	virtual void VisitCount(FixedString const&, uint32_t*) = 0;
	virtual void FindDataType(FixedString const&) = 0;
	virtual void VisitRaw(FixedString const&, void*, uint64_t, uint32_t) = 0;
	virtual void VisitTransatedFSString() = 0;
	virtual void VisitGuid(FixedString const&, void*, void const*) = 0;
	virtual void VisitTranslatedString(FixedString const&, TranslatedString&, TranslatedString const&) = 0;
	virtual void VisitSTDString(FixedString const&, STDString&, STDString const&) = 0;
	virtual void VisitFixedString(FixedString const&, FixedString&, FixedString const&) = 0;
	virtual void VisitPath(FixedString const&, Path*, Path*) = 0;
	virtual void VisitWCharT() = 0;
	virtual void VisitWCharT2() = 0;
	virtual void VisitCharArray() = 0;
	virtual void VisitCharArray2() = 0;
	virtual void VisitComponentHandle(FixedString const&, void*, void const*) = 0;
	virtual void VisitQuaternion(FixedString const&, float&, float const&) = 0;
	virtual void VisitMatrix4x3(FixedString const&, float&, float const&) = 0;
	virtual void VisitMatrix3x4(FixedString const&, float&, float const&) = 0;
	virtual void VisitMatrix4x4(FixedString const&, float&, float const&) = 0;
	virtual void VisitMatrix3x3(FixedString const&, float&, float const&) = 0;
	virtual void VisitMatrix2x2(FixedString const&, float&, float const&) = 0;
	virtual void VisitVector4Unaligned(FixedString const&, float&, float const&) = 0;
	virtual void VisitVector4(FixedString const&, float&, float const&) = 0;
	virtual void VisitVector3(FixedString const&, float&, float const&) = 0;
	virtual void VisitVector2(FixedString const&, float&, float const&) = 0;
	virtual void VisitVector3i(FixedString const&, int*, int*) = 0;
	virtual void VisitVector2i(FixedString const&, int*, int*) = 0;
	virtual void VisitUInt64(FixedString const&, uint64_t&, uint64_t) = 0;
	virtual void VisitInt64(FixedString const&, int64_t&, int64_t) = 0;
	virtual void VisitFloat(FixedString const&, float&, float) = 0;
	virtual void VisitDouble(FixedString const&, double&, double) = 0;
	virtual void VisitInt32(FixedString const&, int32_t&, int32_t) = 0;
	virtual void VisitUInt32(FixedString const&, uint32_t&, uint32_t) = 0;
	virtual void VisitUInt16(FixedString const&, uint16_t&, uint16_t) = 0;
	virtual void VisitInt16(FixedString const&, int16_t&, int16_t) = 0;
	virtual void VisitBool(FixedString const&, bool&, bool) = 0;
	virtual void VisitUInt8(FixedString const&, uint8_t&, uint8_t) = 0;
	virtual void VisitInt8(FixedString const&, int8_t&, int8_t) = 0;
	virtual void VisitBuffer(FixedString const&, ScratchBuffer&) = 0;
	virtual void ResetRegions() = 0;

	int field_8;
	STDString field_10;
	Version FileVersion;
	Version CodeVersion;
	Array<ScratchBuffer> Array_ScratchBuffer;
};

END_SE()
