#include <string>
#include <vector>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

#ifndef _SRT_PARSER_INCLUDED
#define _SRT_PARSER_INCLUDED

struct TSubtitle
{
    fs::path    m_pSrtFile;
    unsigned    m_nSubNdx = 0u;
    std::string m_szText;
    std::string m_szCmpText;

    std::string ExtractOriginal(size_t nPosInCmp, size_t nWordCount, std::string const& szCmpWord, bool bNoAl) const;
};

class CSrtParser
{
    std::vector<TSubtitle> m_vAllSubtitles;

    TSubtitle NextSubtitle(fs::path const& pFile, std::ifstream& ifs);
public:
    explicit CSrtParser(fs::path pSrtLocation);
    std::vector<TSubtitle> GetAllSubtitles() const { return m_vAllSubtitles; }
};

#endif // !_SRT_PARSER_INCLUDED
