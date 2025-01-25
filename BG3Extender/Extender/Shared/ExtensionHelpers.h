#pragma once

#include <Osiris/Shared/CustomFunctions.h>
#include <GameDefinitions/EntitySystem.h>
#include <GameDefinitions/Character.h>
#include <GameDefinitions/Item.h>

BEGIN_SE()

ecl::EoCClient * GetEoCClient();
esv::EoCServer * GetEoCServer();

FixedString NameGuidToFixedString(char const * nameGuid);
bool IsValidGuidString(const char * s);

END_SE()
