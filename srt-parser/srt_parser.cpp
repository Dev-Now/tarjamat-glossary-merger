#include "srt_parser.h"
#include "../utils/utils.h"

#include <sstream>

TSubtitle CSrtParser::NextSubtitle(fs::path const& pFile, std::ifstream& ifs)
{
    std::string szLine;
    try {
        // extract the subtitle index
        std::getline(ifs, szLine);
        std::istringstream iss(szLine);
        unsigned nNdx; iss >> nNdx;
        // ignore timing line
        std::getline(ifs, szLine);
        // read all subtitle text
        std::string szRawSub = "";
        do {
            std::getline(ifs, szLine);
            szRawSub += szLine + "\n";
        } while (szLine != "");
        szRawSub.pop_back();
        szRawSub.pop_back();
        // make comparison subtitle text
        std::string szCmpText(szRawSub);
        REMOVE_VOCALS(szCmpText);
        UNIFORMIZE_TAAS(szCmpText);
        UNIFORMIZE_HMZAS(szCmpText);
        return { pFile, nNdx, szRawSub, szCmpText };
    } 
    catch (std::exception const& e) {
        std::ofstream ofs("log.txt");
        ofs << "srt file " << pFile << " parsing failed around :\n";
        ofs << szLine << "\n";
        ofs << e.what() << "\n";
        throw e;
    }
}

CSrtParser::CSrtParser(fs::path pSrtLocation)
{
    if (!fs::exists(pSrtLocation)) return;
    if (fs::is_directory(pSrtLocation)) { // when the location is a directory, parse all .srt files inside...
        for (auto& p : fs::directory_iterator(pSrtLocation)) {
            auto pExt = p.path().extension();
            if (pExt == ".srt" || pExt == ".SRT") {
                std::ifstream ifs(p.path().string().c_str());
                while (!ifs.eof()) {
                    m_vAllSubtitles.push_back(NextSubtitle(p.path(), ifs));
                }
            }
        }
    }
    else if ((pSrtLocation.extension() == ".srt") || (pSrtLocation.extension() == ".SRT")) {
        std::ifstream ifs(pSrtLocation.string().c_str());
        while (!ifs.eof()) {
            m_vAllSubtitles.push_back(NextSubtitle(pSrtLocation, ifs));
        }
    }
}

