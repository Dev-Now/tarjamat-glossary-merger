#include "glossary_csv_parser.h"

#include <fstream>
#include <string>

CGlossaryCsvParser::CGlossaryCsvParser(fs::path pCsv)
{
    if (!fs::exists(pCsv)) return;
    std::ifstream ifs(pCsv.string().c_str());
    std::string szLine;
    if (!std::getline(ifs, szLine)) return; // first line is just the header, not needed
    while (std::getline(ifs, szLine)) {
        m_vGlossaryEntries.push_back( szLine.substr(0u, szLine.find(",")) );
    }
}
