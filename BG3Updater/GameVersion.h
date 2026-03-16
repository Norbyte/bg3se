#pragma once

#include <string>

BEGIN_SE()

struct VersionNumber
{
    inline VersionNumber()
        : Major(0), Minor(0), Revision(0), Build(0)
    {}

    inline VersionNumber(int32_t major, int32_t minor, int32_t revision, int32_t build)
        : Major(major), Minor(minor), Revision(revision), Build(build)
    {}

    inline static std::optional<VersionNumber> FromString(std::string_view versionNumber)
    {
        VersionNumber ver;
        if (sscanf_s(versionNumber.data(), "%d.%d.%d.%d", &ver.Major, &ver.Minor, &ver.Revision, &ver.Build) == 4) {
            return ver;
        } else {
            return {};
        }
    }

    inline std::string ToString() const
    {
        char ver[100];
        sprintf_s(ver, "%d.%d.%d.%d", Major, Minor, Revision, Build);
        return ver;
    }

    inline bool operator > (VersionNumber const& o) const
    {
        return Major > o.Major ||
            (Major == o.Major && Minor > o.Minor) ||
            (Major == o.Major && Minor == o.Minor && Revision > o.Revision) ||
            (Major == o.Major && Minor == o.Minor && Revision == o.Revision && Build > o.Build);
    }

    inline bool operator < (VersionNumber const& o) const
    {
        return Major < o.Major ||
            (Major == o.Major && Minor < o.Minor) ||
            (Major == o.Major && Minor == o.Minor && Revision < o.Revision) ||
            (Major == o.Major && Minor == o.Minor && Revision == o.Revision && Build < o.Build);
    }

    inline bool operator == (VersionNumber const& o) const
    {
        return Major == o.Major 
            && Minor == o.Minor 
            && Revision == o.Revision 
            && Build == o.Build;
    }

    inline bool operator != (VersionNumber const& o) const
    {
        return Major != o.Major 
            || Minor != o.Minor 
            || Revision != o.Revision 
            || Build != o.Build;
    }

    int32_t Major, Minor, Revision, Build;
};

END_SE()
