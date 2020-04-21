
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

#ifndef _GLOSS_CSV_PARSER_INCLUDED
#define _GLOSS_CSV_PARSER_INCLUDED

class CGlossaryCsvParser
{
    std::vector<std::string> m_vGlossaryEntries;

public:
    explicit CGlossaryCsvParser(fs::path pCsv);
    std::vector<std::string> GetGlossaryEntries() const { return m_vGlossaryEntries; }
};

#endif