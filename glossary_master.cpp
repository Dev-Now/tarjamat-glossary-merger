#include "glossary_master.h"
#include "glossary-csv-parser/glossary_csv_parser.h"

#include <filesystem>

namespace fs = std::filesystem;

void CGlossaryMaster::Merge(std::vector<std::string> const& vGlossEntries)
{
    for (auto const& szEntry : vGlossEntries) {
        m_sMasterGlossary.insert(szEntry);
    }
}

CGlossaryMaster::CGlossaryMaster()
{
    // first, find all csv files
    // next, parse them one by one
    // then, merge found glossary enteries into the master set
    for (auto& p : fs::directory_iterator(".")) {
        auto pExt = p.path().extension();
        if (pExt == ".csv" || pExt == ".CSV") {
            Merge(CGlossaryCsvParser(p.path()).GetGlossaryEntries());
        }
    }
}
