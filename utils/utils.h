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
    return szStr;
}

template< class InputIt, class UnaryPredicate >
constexpr bool any_of(InputIt first, InputIt last, UnaryPredicate p)
{
    return std::find_if(first, last, p) != last;
}

inline static std::string& TRIM(std::string& szStr, std::vector<std::string> const& vExtraChars)
{
    size_t nxL;
    while (any_of(vExtraChars.begin(), vExtraChars.end(), [&szStr, &nxL](std::string const& extra) {
        auto pos = szStr.find(extra);
        nxL = (pos == 0u) ? extra.length() : 0u;
        return (pos == 0u);
        }) ) {
        szStr.erase(0u, nxL);
    }
    while (any_of(vExtraChars.begin(), vExtraChars.end(), [&szStr, &nxL](std::string const& extra) {
        auto pos = szStr.rfind(extra);
        nxL = ( pos == (szStr.length()-extra.length()) ) ? extra.length() : 0u;
        return ( pos == (szStr.length() - extra.length()) );
        })) {
        szStr.erase(szStr.length() - nxL, nxL);
    }
    return szStr;
}

#endif // !_UTILS_INCLUDED
