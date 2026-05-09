#pragma once

BEGIN_NS(lua)

struct EntityTracingOptions
{
    bool TrackECB{ true };
    bool TrackImmediateWorldCache{ true };
    bool TrackReplication{ true };
    bool TrackModifications{ false };
    Array<STDString> ExcludeModificationComponents;
};

END_NS()
