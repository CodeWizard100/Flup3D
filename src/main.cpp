
#include <iostream>
#include <FlupError.h>
#include <FlupLogger.h>
#include <codecvt> // deprecated in C++17 but still usable

std::wstring utf8_to_wstring(const char* str) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
    return conv.from_bytes(str);
}
int main(int argc, char* argv[]) {
    const char* error = FLUP_GRGL0001;
    #ifdef UNICODE
    FlupLogger::LogError(utf8_to_wstring(FLUP_ERRORHEADER) + utf8_to_wstring(error), 1);
    #else
    FlupLogger::LogError(std::string(FLUP_ERRORHEADER) + std::string(error), 1);
    #endif // UNICODE
    return 0;
}
