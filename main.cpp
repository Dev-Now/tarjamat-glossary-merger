#include "utils/utils.h"
#include "glossary_master.h"
#include "srt-parser/srt_parser.h"

#include <fstream>
#include <future>

int main()
{
    /*std::ofstream ofst("test-wc.txt");  
    std::string sz(u8"للمهندسِ عبدِ اللهِ العُجَيْرِيِّ حفظَهُ اللهُ");
    ofst << "Extracted: " << EXTRACT_WORDS(sz, 1u, 3u);
    return 0;*/

    std::packaged_task<std::vector<TSubtitle>()> taskParseSrts([] {
        return CSrtParser(".").GetAllSubtitles();
        });
    std::future<std::vector<TSubtitle>> fParseSrts = taskParseSrts.get_future();
    taskParseSrts();

    std::ofstream ofs("unified-glossary.csv");
    ofs << "ar-SA,,,\n"; // header
    CGlossaryMaster glsm;
    glsm.LookupVariaties(fParseSrts.get());
    std::set<std::string, GlossRecCmp> s = glsm.GetGlossary();
    for (auto& item : s) {
        ofs << item << ",,,\n";
    }
    return 0;
}