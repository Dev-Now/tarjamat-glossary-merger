#include "glossary_master.h"
#include "glossary-csv-parser/glossary_csv_parser.h"
#include "utils/logs.h"

#include <filesystem>
#include <future>

namespace fs = std::filesystem;

void CGlossaryMaster::Merge(std::vector<std::string> const& vGlossEntries)
{
    for (auto const& szEntry : vGlossEntries) {
        if(szEntry!="") m_sMasterGlossary.insert(szEntry);
    }
}

void CGlossaryMaster::FindVariaties(std::vector<std::string>& vVariaties, std::string const& szCmpWord, std::vector<TSubtitle> const& vAllSubtitles)
{
    for (auto const& subt : vAllSubtitles) {
        auto pos = subt.m_szCmpText.find(szCmpWord);
        while (pos != std::string::npos) {
            vVariaties.push_back( subt.ExtractOriginal(pos, WORD_COUNT(szCmpWord), szCmpWord) );
            pos = subt.m_szCmpText.find(szCmpWord, pos + szCmpWord.length());
        }
    }
}

CGlossaryMaster::CGlossaryMaster()
{
    // first, find all csv files
    // next, parse them one by one
    // then, merge found glossary enteries into the master set
    LOG("Merging all glossary csv files. . .\n");
    for (auto& p : fs::directory_iterator(".")) {
        auto pExt = p.path().extension();
        if (pExt == ".csv" || pExt == ".CSV") {
            Merge(CGlossaryCsvParser(p.path()).GetGlossaryEntries());
        }
    }
    LOG("Merge complete!\n");
}

void CGlossaryMaster::LookupVariaties(std::vector<TSubtitle>&& vAllSubtitles)
{
    LOG("Lookup glossary words variaties. . .\n");
    std::vector<std::string> vFoundVariaties, vFoundVariatiesNoAl;
    for (auto const& glsRec : m_sMasterGlossary) {
        std::string szCmp = glsRec, szCmpNoAl;
        UNIFORMIZE(szCmp);
        szCmpNoAl = szCmp;
        REMOVE_LEADING_AL(szCmpNoAl);
        std::future<void> fNoAl = std::async(&CGlossaryMaster::FindVariaties, this, std::ref(vFoundVariatiesNoAl), std::ref(szCmpNoAl), std::ref(vAllSubtitles));
        FindVariaties(vFoundVariaties, szCmp, vAllSubtitles);
        fNoAl.get();
    }
    Merge(vFoundVariaties);
    Merge(vFoundVariatiesNoAl);
    LOG("Lookup complete!\n");
}
