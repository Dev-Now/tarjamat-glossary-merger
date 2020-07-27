#include "glossary_csv_parser.h"
#include "../utils/logs.h"

#include <fstream>
#include <string>

CGlossaryCsvParser::CGlossaryCsvParser(fs::path pCsv, std::string const& spr)
{
    if (!fs::exists(pCsv)) return;
    std::ifstream ifs(pCsv.string().c_str());
    std::string szLine;
    unsigned nLine = 0u;
    if (!std::getline(ifs, szLine)) return; // first line is just the header, not needed
    while (std::getline(ifs, szLine)) {
        try {
            ++nLine;
            TGlossEntry tGlsTerm;
            tGlsTerm.szTerm = szLine.substr(0u, szLine.find(spr));
            size_t nOffset = 1u + tGlsTerm.szTerm.length();
            tGlsTerm.szTranslation = szLine.substr(nOffset, szLine.find(spr, nOffset) - nOffset);
            nOffset += 1u + tGlsTerm.szTranslation.length();
            tGlsTerm.szComment = szLine.substr(nOffset, szLine.find(spr, nOffset) - nOffset);
            nOffset += 1u + tGlsTerm.szComment.length();
            std::string szFileAvail(szLine.substr(nOffset, szLine.find(spr, nOffset) - nOffset));
            tGlsTerm.szFile = szFileAvail == "" ? pCsv.stem().string() : szFileAvail;
            m_vGlossaryEntries.push_back(tGlsTerm);
        }
        catch (std::exception const& e) {
            LOG("csv file "); LOG(pCsv); LOG(" parsing failed around line:\n");
            LOG(nLine); LOG(": "); LOG(szLine); LOG("\n");
            LOG(e.what()); LOG("\n");
            throw e;
        }
    }
}
