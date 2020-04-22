#include "utils/utils.h"
#include "glossary_master.h"
#include "srt-parser/srt_parser.h"

#include <fstream>

int main()
{
    std::ofstream ofs("test1.txt");

    std::vector<TSubtitle> v = CSrtParser(".").GetAllSubtitles();
    for (auto& item : v) {
        ofs << item.m_pSrtFile << ": " << item.m_nSubNdx << "=\n";
        ofs << item.m_szText << "\n";
        ofs << item.m_szCmpText << "\n";
    }
    /*CGlossaryMaster glsm;
    std::vector<std::string> v = glsm.GetGlossary();
    for (auto& item : v) {
        ofs << item << "\n";
    }*/
    return 0;
}