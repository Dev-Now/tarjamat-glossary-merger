#include "glossary_master.h"
#include "utils/logs.h"

#include <filesystem>
#include <future>

namespace fs = std::filesystem;

void CGlossaryMaster::Merge(std::vector<TGlossEntry> const& vGlossEntries)
{
    for (auto const& szEntry : vGlossEntries) {
        if(szEntry.szTerm!="") m_sMasterGlossary.insert(szEntry);
    }
}

void CGlossaryMaster::FindVariaties(std::vector<TGlossEntry>& vVariaties, std::string const& szCmpWord, std::vector<TSubtitle> const& vAllSubtitles, 
    std::string const& szFile, std::string const& szTranslation)
{
    for (auto const& subt : vAllSubtitles) {
        auto pos = subt.m_szCmpText.find(szCmpWord);
        while (pos != std::string::npos) {
            TGlossEntry tGlsTerm; tGlsTerm.szFile = szFile; tGlsTerm.szTranslation = szTranslation;
            tGlsTerm.szTerm = subt.ExtractOriginal(pos, WORD_COUNT(szCmpWord), szCmpWord);
            vVariaties.push_back(tGlsTerm);
            pos = subt.m_szCmpText.find(szCmpWord, pos + szCmpWord.length());
        }
    }
}

CGlossaryMaster::CGlossaryMaster(std::string const& spr)
{
    // first, find all csv files
    // next, parse them one by one
    // then, merge found glossary enteries into the master set
    LOG("Merging all glossary csv files. . .\n");
    for (auto& p : fs::directory_iterator(".")) {
        auto pExt = p.path().extension();
        if (pExt == ".csv" || pExt == ".CSV") {
            Merge(CGlossaryCsvParser(p.path(), spr).GetGlossaryEntries());
        }
    }
    LOG("Merge complete!\n");
}

void CGlossaryMaster::LookupVariaties(std::vector<TSubtitle>&& vAllSubtitles)
{
    LOG("Lookup glossary words variaties. . .\n");
    std::vector<TGlossEntry> vFoundVariaties, vFoundVariatiesNoAl;
    for (auto const& glsRec : m_sMasterGlossary) {
        std::string szCmp = glsRec.szTerm, szCmpNoAl;
        UNIFORMIZE(szCmp);
        szCmpNoAl = szCmp;
        REMOVE_LEADING_AL(szCmpNoAl);
        std::future<void> fNoAl = std::async(&CGlossaryMaster::FindVariaties, this, std::ref(vFoundVariatiesNoAl), std::ref(szCmpNoAl), std::ref(vAllSubtitles),
            std::ref(glsRec.szFile), std::ref(glsRec.szTranslation));
        FindVariaties(vFoundVariaties, szCmp, vAllSubtitles, glsRec.szFile, glsRec.szTranslation);
        fNoAl.get();
    }
    Merge(vFoundVariaties);
    Merge(vFoundVariatiesNoAl);
    LOG("Lookup complete!\n");
}
