#include "utils/utils.h"

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
public:
    CGlossaryMaster();

    // todo: remove this... just for testing
    std::vector<std::string> GetGlossary() const {
        std::vector<std::string> v;
        for (auto& item : m_sMasterGlossary) {
            v.push_back(item);
        }
        return v;
    }
};

#endif // !_GLOSS_MASTER
