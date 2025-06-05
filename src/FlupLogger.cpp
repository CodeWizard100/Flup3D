#include <FlupLogger.h>
#include <NMB.h>

#define _CRT_SECURE_NO_WARNINGS
FlupStr _GetDateAndTime() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);

    std::tm tm = *std::localtime(&now_time_t);

    #ifdef UNICODE
    std::wostringstream oss;
    #else
    std::ostringstream oss;
    #endif // UNICODE
    oss << std::put_time(&tm, "%Y-%m-%d-%H-%M");
    return oss.str();
}

/*
Types:
1 - Text
2 - Warning
3 - Error
4 - Critical Error
*/
void _WriteToLog(FlupStr log, int type) {
    if (!std::filesystem::exists("logs")) {
        std::filesystem::create_directory("logs");
    }
    #ifdef UNICODE
    std::wofstream Log(FLUP_LITERAL("logs/") + _GetDateAndTime() + FLUP_LITERAL(".log"), std::ios::app);
    #else
    std::ofstream Log(FLUP_LITERAL("logs/") + _GetDateAndTime() + FLUP_LITERAL(".log"), std::ios::app);
    #endif

    Log << log + FLUP_LITERAL("\n");
    Log.close();
}

FlupStr _GetInitalals(int type) {
    if (type == 1) {
        return FLUP_LITERAL("[Info]");
    } else if (type == 2) {
        return FLUP_LITERAL("[Warning]");
    } else if (type == 3) {
        return FLUP_LITERAL("[Error]");
    } else if (type == 4) {
        return FLUP_LITERAL("[Critical]");
    }
    return FLUP_LITERAL("[Unkown]");
}


void FlupLogger::LogText(FlupStr text) {
    FlupStr finallog;
    finallog = finallog + _GetInitalals(1) + FLUP_LITERAL("\n");
    finallog = finallog + text;
    #ifdef UNICODE
    std::wcout << finallog;
    #else
    std::cout << finallog;
    #endif // UNICODE
    _WriteToLog(finallog, 1);
};

void FlupLogger::LogWarning(FlupStr text) {
    FlupStr finallog;
    finallog = finallog + _GetInitalals(2) + FLUP_LITERAL("\n");
    finallog = finallog + text;
    #ifdef UNICODE
    std::wcout << finallog;
    #else
    std::cout << finallog;
    #endif // UNICODE
    _WriteToLog(finallog, 2);
};

void FlupLogger::LogError(FlupStr text, int status) {
    FlupStr finallog;
    finallog = finallog + _GetInitalals(3 + status) + FLUP_LITERAL("\n");
    finallog = finallog + text;
    #ifdef UNICODE
    std::wcout << finallog;
    #else
    std::cout << finallog;
    #endif // UNICODE
    _WriteToLog(finallog, 1);
    if (status == 1) {
        NMB::show(FLUP_LITERAL("Flup Error!"), text, NMB::Icon::ICON_ERROR);
    }
};