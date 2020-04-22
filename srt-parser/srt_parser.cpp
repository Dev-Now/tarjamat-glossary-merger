#include "srt_parser.h"
#include "../utils/utils.h"

#include <sstream>

extern std::ofstream g_logger;

std::string TSubtitle::ExtractOriginal(size_t nPosInCmp, size_t nWordCount, std::string const& szCmpWord, bool bNoAl) const
{
    // deduce from nPosInCmp the word position
    size_t nWordPos = WORD_POS(m_szCmpText, nPosInCmp);
    // extact [nWordCount] words from m_szText starting with the word at deduced position
    std::string szExtracted(m_szText);
    EXTRACT_WORDS(szExtracted, nWordPos, nWordCount);
    // cleanup the extracted word from extra letters
    //g_logger << "Cmp: " << szCmpWord << "\n";
    //g_logger << "Raw: " << szExtracted << "\n";
    std::vector<char> vExtractedSignature = GEN_SIGNATURE(szExtracted);
    //g_logger << "Signature: "; for (auto c : vExtractedSignature) { g_logger << (int)c; } g_logger << "\n";
    std::string szUnifExtracted(szExtracted);
    UNIFORMIZE(szUnifExtracted);
    if (bNoAl) REMOVE_LEADING_AL(szUnifExtracted);
    //g_logger << "Unif: " << szUnifExtracted << "\n";
    auto preChars = szUnifExtracted.find(szCmpWord);
    auto postChars = szUnifExtracted.length() - szCmpWord.length() - preChars;
    //g_logger << "Pre: " << preChars << ", Post: " << postChars << "\n";
    size_t nStartPos = 0u, nEndPos = szExtracted.length()-1u;
    while (preChars > 0u) {
        if (vExtractedSignature[nStartPos] == 1) { --preChars; }
        ++nStartPos;
    }
    while (vExtractedSignature[nStartPos] != 1) { ++nStartPos; }
    while (postChars > 0u) {
        if (vExtractedSignature[nEndPos] == 1) { --postChars; }
        --nEndPos;
    }
    while (vExtractedSignature[nEndPos] != 1) { --nEndPos; }
    //g_logger << "start: " << nStartPos << ", end: " << nEndPos << "\n";
    //g_logger << "Clean: " << szExtracted.substr(nStartPos, nEndPos - nStartPos + 1u) << "\n";
    return szExtracted.substr(nStartPos, nEndPos - nStartPos + 1u);
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

