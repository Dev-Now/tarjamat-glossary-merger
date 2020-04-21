#include "utils/utils.h"
#include <fstream>

int main()
{
    std::ofstream ofs("test.txt");
    std::string sz = u8"؟!...! الفَرَنسيَّةُ:؟  		";
    ofs << sz << "\n";
    TRIM(sz, { u8":", u8"؟", u8" ", u8"!", u8"\t", u8"." });
    ofs << sz << "\n";
    REMOVE_VOCALS(sz);
    ofs << sz << "\n";
    UNIFORMIZE_TAAS(sz);
    ofs << sz << "\n";
    UNIFORMIZE_HMZAS(sz);
    ofs << sz << "\n";
    REMOVE_LEADING_AL(sz);
    ofs << sz << "\n";
    return 0;
}