#pragma once

#include <GameDefinitions/Base/Base.h>

BEGIN_SE()

class SavegameSerializer
{
public:
	void SavegameVisit(ObjectVisitor* visitor);

private:
	void Serialize(ObjectVisitor* visitor, uint32_t version);
	void SerializePersistentVariables(ObjectVisitor* visitor, uint32_t version);
	void RestorePersistentVariables(std::unordered_map<FixedString, STDString> const&);
	void SerializeStatObjects(ObjectVisitor* visitor, uint32_t version);
	void RestoreStatObject(FixedString const& statId, FixedString const& statType, ScratchBuffer const& blob);
	bool SerializeStatObject(FixedString const& statId, FixedString& statType, ScratchBuffer& blob);
};

END_SE()
