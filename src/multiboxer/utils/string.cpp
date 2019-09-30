#include "string.h"

using namespace std;

namespace utils {

    string ToLower(string str) {
        transform(str.begin(), str.end(), str.begin(), [](unsigned char c) -> unsigned char { return tolower(c); });
        return str;
    }

    wstring ToLower(wstring wstr) {
        transform(wstr.begin(), wstr.end(), wstr.begin(), [](wchar_t c) -> wchar_t { return tolower(c); });
        return wstr;
    }

}
