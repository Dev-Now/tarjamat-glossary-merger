#include "srt_parser.h"
#include "../utils/utils.h"

#include <sstream>

extern std::ofstream g_logger;

std::string TSubtitle::ExtractOriginal(size_t nPosInCmp, size_t nWordCount) const
{
    // deduce from nPosInCmp the word position
    // extact [nWordCount] words from m_szText starting with the word at deduced position
    size_t nWordPos = WORD_POS(m_szCmpText, nPosInCmp);
    std::string szExtracted(m_szText);
    return EXTRACT_WORDS(szExtracted, nWordPos, nWordCount);
}

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
        g_logger << "srt file " << pFile << " parsing failed around :\n";
        g_logger << szLine << "\n";
        g_logger << e.what() << "\n";
        throw e;
    }
}

CSrtParser::CSrtParser(fs::path pSrtLocation)
{
    if (!fs::exists(pSrtLocation)) return;
    if (fs::is_directory(pSrtLocation)) { // when the location is a directory, parse all .srt files inside...
        g_logger << "Parsing srt files. . .\n";
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
        g_logger << "Parsing srt file. . .\n";
        std::ifstream ifs(pSrtLocation.string().c_str());
        while (!ifs.eof()) {
            m_vAllSubtitles.push_back(NextSubtitle(pSrtLocation, ifs));
        }
    }
    g_logger << "srt parsed!\n";
}

