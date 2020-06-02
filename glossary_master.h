#include "utils/utils.h"
#include "srt-parser/srt_parser.h"
#include "glossary-csv-parser/glossary_csv_parser.h"

#include <vector>
#include <string>
#include <set>

#ifndef _GLOSS_MASTER
#define _GLOSS_MASTER

struct GlossRecCmp {
    bool operator()(const TGlossEntry& lhs, const TGlossEntry& rhs) const {
        std::string lhsc(lhs.szTerm), rhsc(rhs.szTerm);
        UNIFORMIZE(lhsc); UNIFORMIZE(rhsc);
        REMOVE_LEADING_AL(lhsc); REMOVE_LEADING_AL(rhsc);
        size_t lhsScore = (std::hash<std::string>()(lhsc) << 8) | 
            ( std::hash<std::string>()(lhs.szTerm) >> (sizeof(size_t)*8 - 8) );
        size_t rhsScore = (std::hash<std::string>()(rhsc) << 8) |
            ( std::hash<std::string>()(rhs.szTerm) >> (sizeof(size_t) * 8 - 8) );
        return lhsScore < rhsScore;
    }
};

class CGlossaryMaster
{
    std::set<TGlossEntry, GlossRecCmp> m_sMasterGlossary;

    void Merge(std::vector<TGlossEntry> const&);
    void FindVariaties(std::vector<TGlossEntry>& vVariaties, std::string const& szCmpWord, std::vector<TSubtitle> const& vAllSubtitles,
        std::string const& szFile, std::string const& szTranslation);
public:
    CGlossaryMaster(std::string const& spr = DFLT_SEPR);

    void LookupVariaties(std::vector<TSubtitle>&& vAllSubtitles);
    std::set<TGlossEntry, GlossRecCmp> GetGlossary() const { return m_sMasterGlossary; }
};

#endif // !_GLOSS_MASTER
