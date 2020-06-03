
#include <filesystem>
#include <vector>
#include <string>

namespace fs = std::filesystem;

#ifndef _GLOSS_CSV_PARSER_INCLUDED
#define _GLOSS_CSV_PARSER_INCLUDED

static std::string const DFLT_SEPR = ",";

struct TGlossEntry {
    std::string szTerm;
    std::string szTranslation;
    std::string szComment;
    std::string szFile;
};

class CGlossaryCsvParser
{
    std::vector<TGlossEntry> m_vGlossaryEntries;

public:
    explicit CGlossaryCsvParser(fs::path pCsv, std::string const& spr = DFLT_SEPR);
    std::vector<TGlossEntry> GetGlossaryEntries() const { return m_vGlossaryEntries; }
};

#endif