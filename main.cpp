#include "utils/utils.h"
#include "glossary_master.h"
#include "srt-parser/srt_parser.h"
#include "utils/logs.h"

#include <future>
#include <iostream>

int main(int argc, char* argv[])
{
    // read args
    std::string spr = DFLT_SEPR;
    if ((argc == 3) && (std::string(argv[1]) == "-s")) {
        spr = argv[2];
    }
    // core logic
    std::cout << "mrgls is working hard.\nPlease wait. . .\n";
    LOG("mrgls started. . .\n");
    std::packaged_task<std::vector<TSubtitle>()> taskParseSrts([] {
        return CSrtParser(".").GetAllSubtitles();
        });
    std::future<std::vector<TSubtitle>> fParseSrts = taskParseSrts.get_future();
    taskParseSrts();

    std::ofstream ofs("unified-glossary.csv");
    ofs << (char)0xEF << (char)0xBB << (char)0xBF;
    ofs << "ar-SA" << spr << "Translation" << spr << "Comment" << spr << "Origin" << spr <<"\n"; // header
    CGlossaryMaster glsm(spr);
    glsm.LookupVariaties(fParseSrts.get());
    std::set<TGlossEntry, GlossRecCmp> s = glsm.GetGlossary();
    for (auto& item : s) {
        ofs << item.szTerm << spr << item.szTranslation << spr << spr << item.szFile << spr << "\n";
    }
    return 0;
}