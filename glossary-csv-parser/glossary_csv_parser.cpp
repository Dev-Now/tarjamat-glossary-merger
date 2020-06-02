#include "glossary_csv_parser.h"

#include <fstream>
#include <string>

CGlossaryCsvParser::CGlossaryCsvParser(fs::path pCsv, std::string const& spr)
{
    if (!fs::exists(pCsv)) return;
    std::ifstream ifs(pCsv.string().c_str());
    std::string szLine;
    if (!std::getline(ifs, szLine)) return; // first line is just the header, not needed
    while (std::getline(ifs, szLine)) {
        TGlossEntry tGlsTerm;
        tGlsTerm.szFile = pCsv.stem().string();
        tGlsTerm.szTerm = szLine.substr(0u, szLine.find(spr));
        size_t nOffset = tGlsTerm.szTerm.length();
        tGlsTerm.szTranslation = szLine.substr(nOffset, szLine.find(spr, nOffset)-nOffset);
        m_vGlossaryEntries.push_back(tGlsTerm);
    }
}
