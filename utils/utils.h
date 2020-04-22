#include <string>
#include <vector>

#ifndef _UTILS_INCLUDED
#define _UTILS_INCLUDED

static std::vector<std::string> const VOCALS{ u8"\u064b", u8"\u064c", u8"\u064d", u8"\u064e", u8"\u064f", u8"\u0650", u8"\u0651",
                                              u8"\u0652", u8"\u0653", u8"\u0654", u8"\u0655", u8"\u0656", u8"\u0657", u8"\u0658",
                                              u8"\u0659", u8"\u065a", u8"\u065b", u8"\u065c", u8"\u065d", u8"\u065e", u8"\u065f" };

static std::string const CLSD_TAA = u8"\u0629";
static std::string const OPEN_TAA = u8"\u062a";

static std::vector<std::string> const HAMZAS{ u8"\u0621", u8"\u0622", u8"\u0623", u8"\u0624", u8"\u0625", u8"\u0626" };

static std::string const AL = u8"\u0627\u0644";

static std::vector<std::string> const SPACES{ " ", "\f", "\n", "\r", "\t", "\v" };

inline static std::string& REMOVE_VOCALS(std::string& szStr)
{
    for (auto const& voc : VOCALS) {
        auto pos = szStr.find(voc);
        while (pos != std::string::npos) {
            szStr.erase(pos, 2u);
            pos = szStr.find(voc);
        }
    }
    return szStr;
}

inline static std::string& UNIFORMIZE_TAAS(std::string& szStr)
{
    auto pos = szStr.find(CLSD_TAA);
    while (pos != std::string::npos) {
        szStr.replace(pos, 2u, OPEN_TAA);
        pos = szStr.find(CLSD_TAA);
    }
    return szStr;
}

inline static std::string& UNIFORMIZE_HMZAS(std::string& szStr)
{
    for (auto const& hmz : HAMZAS) {
        if (hmz == HAMZAS[0]) continue;
        auto pos = szStr.find(hmz);
        while (pos != std::string::npos) {
            szStr.replace(pos, 2u, HAMZAS[0]);
            pos = szStr.find(hmz);
        }
    }
    return szStr;
}

inline static std::string& REMOVE_LEADING_AL(std::string& szStr)
{
    if (szStr.substr(0u, 4u) == AL) { szStr.erase(0u, 4u); }
    auto pos = szStr.find(u8" ");
    while ( (pos!= std::string::npos) && ((pos + 4u) < szStr.length()) ) {
        if (szStr.substr(pos+1u, 4u) == AL) { szStr.erase(pos+1u, 4u); }
        pos = szStr.find(u8" ", pos + 1u);
    }
    return szStr;
}

template< class InputIt, class UnaryPredicate >
constexpr bool ANY_OF(InputIt first, InputIt last, UnaryPredicate p)
{
    return std::find_if(first, last, p) != last;
}

inline static std::string& TRIM(std::string& szStr, std::vector<std::string> const& vExtraChars)
{
    if (szStr.length() == 0u) return szStr;
    size_t nxL;
    while (ANY_OF(vExtraChars.begin(), vExtraChars.end(), [&szStr, &nxL](std::string const& extra) {
        auto pos = szStr.find(extra);
        nxL = (pos == 0u) ? extra.length() : 0u;
        return (pos == 0u);
        }) ) {
        szStr.erase(0u, nxL);
    }
    if (szStr.length() == 0u) return szStr;
    while (ANY_OF(vExtraChars.begin(), vExtraChars.end(), [&szStr, &nxL](std::string const& extra) {
        auto pos = szStr.rfind(extra);
        nxL = ( pos == (szStr.length()-extra.length()) ) ? extra.length() : 0u;
        return ( pos == (szStr.length() - extra.length()) );
        })) {
        szStr.erase(szStr.length() - nxL, nxL);
    }
    return szStr;
}

inline static std::string& UNIFORMIZE(std::string& szStr)
{
    TRIM(szStr, SPACES);
    REMOVE_VOCALS(szStr);
    UNIFORMIZE_TAAS(szStr);
    UNIFORMIZE_HMZAS(szStr);
    return szStr;
}

inline static size_t WORD_COUNT(std::string const& szStr)
{
    std::string sz(szStr); // make a copy of the string to use it
    TRIM(sz, SPACES);
    size_t nCount = 0u;
    size_t nspos;
    while (ANY_OF(SPACES.begin(), SPACES.end(), [&sz, &nspos](std::string const& space) {
        nspos = sz.find(space);
        return (nspos != std::string::npos);
        })) {
        ++nCount;
        sz.erase(0u, nspos);
        TRIM(sz, SPACES);
    }
    TRIM(sz, SPACES);
    if (sz.length() > 0u) ++nCount;
    return nCount;
}

inline static size_t WORD_POS(std::string const& szStr, size_t nCharPos)
{
    std::string sz(szStr); // make a copy of the string to use it
    for (;;) {
        if (nCharPos == 0u) break;
        if (std::find(SPACES.begin(), SPACES.end(), szStr.substr(nCharPos, 1u)) != SPACES.end()) break;
        --nCharPos;
    }
    sz.erase(nCharPos, std::string::npos);
    return WORD_COUNT(sz);
}

inline static std::string& EXTRACT_WORDS(std::string& szStr, size_t nWordPos, size_t nWordCount = 1u)
{
    TRIM(szStr, SPACES);
    while (!szStr.empty() && (nWordPos > 0u)) {
        size_t nspos = 0u;
        if (ANY_OF(SPACES.begin(), SPACES.end(), [&szStr, &nspos](std::string const& space) {
            nspos = szStr.find(space);
            return nspos != std::string::npos;
            })) {
            --nWordPos;
            szStr.erase(0u, nspos);
            TRIM(szStr, SPACES);
        }
        else {
            szStr.clear();
        }
    }
    TRIM(szStr, SPACES);
    if (szStr.empty()) return szStr;
    while (WORD_COUNT(szStr) > nWordCount) { szStr.pop_back(); TRIM(szStr, SPACES); }
    return szStr;
}

#endif // !_UTILS_INCLUDED
