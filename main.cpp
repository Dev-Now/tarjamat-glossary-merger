#include "utils/utils.h"
#include "glossary_master.h"

#include <fstream>

int main()
{
    std::ofstream ofs("test.txt");

    CGlossaryMaster glsm;
    std::vector<std::string> v = glsm.GetGlossary();
    for (auto& item : v) {
        ofs << item << "\n";
    }
    return 0;
}