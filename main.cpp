#include "utils/utils.h"
#include "glossary_master.h"
#include "srt-parser/srt_parser.h"
#include "utils/logs.h"

#include <future>
#include <iostream>

int main()
{
    std::cout << "mrgls is working hard.\nPlease wait. . .\n";
    g_logger << "mrgls started. . .\n";
    std::packaged_task<std::vector<TSubtitle>()> taskParseSrts([] {
        return CSrtParser(".").GetAllSubtitles();
        });
    std::future<std::vector<TSubtitle>> fParseSrts = taskParseSrts.get_future();
    taskParseSrts();

    std::ofstream ofs("unified-glossary.csv");
    ofs << (char)0xEF << (char)0xBB << (char)0xBF;
    ofs << "ar-SA,,,\n"; // header
    CGlossaryMaster glsm;
    glsm.LookupVariaties(fParseSrts.get());
    std::set<std::string, GlossRecCmp> s = glsm.GetGlossary();
    for (auto& item : s) {
        ofs << item << ",,,\n";
    }
    return 0;
}