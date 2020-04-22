#include "glossary_master.h"
#include "glossary-csv-parser/glossary_csv_parser.h"

#include <filesystem>
#include <future>

extern std::ofstream g_logger;

namespace fs = std::filesystem;

void CGlossaryMaster::Merge(std::vector<std::string> const& vGlossEntries)
{
    for (auto const& szEntry : vGlossEntries) {
        m_sMasterGlossary.insert(szEntry);
    }
}

void CGlossaryMaster::FindVariaties(std::vector<std::string>& vVariaties, std::string const& szCmpWord, std::vector<TSubtitle> vAllSubtitles, bool bNoAl = false)
{
    for (auto const& subt : vAllSubtitles) {
        auto pos = subt.m_szCmpText.find(szCmpWord);
        while (pos != std::string::npos) {
            vVariaties.push_back( subt.ExtractOriginal(pos, WORD_COUNT(szCmpWord), szCmpWord, bNoAl) );
            pos = subt.m_szCmpText.find(szCmpWord, pos + szCmpWord.length());
        }
    }
}

CGlossaryMaster::CGlossaryMaster()
{
    // first, find all csv files
    // next, parse them one by one
    // then, merge found glossary enteries into the master set
    g_logger << "Merging all glossary csv files. . .\n";
    for (auto& p : fs::directory_iterator(".")) {
        auto pExt = p.path().extension();
        if (pExt == ".csv" || pExt == ".CSV") {
            Merge(CGlossaryCsvParser(p.path()).GetGlossaryEntries());
        }
    }
    g_logger << "Merge complete!\n";
}

void CGlossaryMaster::LookupVariaties(std::vector<TSubtitle>&& vAllSubtitles)
{
    g_logger << "Lookup glossary words variaties. . .\n";
    std::vector<std::string> vFoundVariaties, vFoundVariatiesNoAl;
    for (auto const& glsRec : m_sMasterGlossary) {
        std::string szCmp = glsRec, szCmpNoAl;
        UNIFORMIZE(szCmp);
        szCmpNoAl = szCmp;
        REMOVE_LEADING_AL(szCmpNoAl);
        std::future<void> fNoAl = std::async(&CGlossaryMaster::FindVariaties, this, vFoundVariatiesNoAl, szCmpNoAl, vAllSubtitles, true);
        FindVariaties(vFoundVariaties, szCmp, vAllSubtitles);
        fNoAl.get();
    }
    Merge(vFoundVariaties);
    Merge(vFoundVariatiesNoAl);
    g_logger << "Lookup complete!\n";
}
