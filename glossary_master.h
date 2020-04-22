#include "utils/utils.h"
#include "srt-parser/srt_parser.h"

#include <vector>
#include <string>
#include <set>

#ifndef _GLOSS_MASTER
#define _GLOSS_MASTER

struct GlossRecCmp {
    bool operator()(const std::string& lhs, const std::string& rhs) const {
        std::string lhsc(lhs), rhsc(rhs);
        TRIM(lhsc, SPACES); TRIM(rhsc, SPACES);
        REMOVE_VOCALS(lhsc); REMOVE_VOCALS(rhsc);
        UNIFORMIZE_TAAS(lhsc); UNIFORMIZE_TAAS(rhsc);
        UNIFORMIZE_HMZAS(lhsc); UNIFORMIZE_HMZAS(rhsc);
        REMOVE_LEADING_AL(lhsc); REMOVE_LEADING_AL(rhsc);
        size_t lhsScore = (std::hash<std::string>()(lhsc) << 8) | 
            ( std::hash<std::string>()(lhs) >> (sizeof(size_t)*8 - 8) );
        size_t rhsScore = (std::hash<std::string>()(rhsc) << 8) |
            ( std::hash<std::string>()(rhs) >> (sizeof(size_t) * 8 - 8) );
        return lhsScore < rhsScore;
    }
};

class CGlossaryMaster
{
    std::set<std::string, GlossRecCmp> m_sMasterGlossary;

    void Merge(std::vector<std::string> const&);
    void FindVariaties(std::vector<std::string>& vVariaties, std::string const& szCmpWord, std::vector<TSubtitle> vAllSubtitles, bool bNoAl);
public:
    CGlossaryMaster();

    void LookupVariaties(std::vector<TSubtitle>&& vAllSubtitles);
    std::set<std::string, GlossRecCmp> GetGlossary() const { return m_sMasterGlossary; }
};

#endif // !_GLOSS_MASTER
