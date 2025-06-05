#pragma once
#ifndef _FLUP_LOGGER_
#define _FLUP_LOGGER_
#endif // _FLUP_LOGGER_
#include <FlupIncludes.h>
#include <FlupStr.h>
#ifndef _FLUP_INCLUDES_
#error "FlupIncludes.h Not Included in FlupLogger.h!"
#endif // _FLUP_INCLUDES_

class FlupLogger {
public:
    /*
    Statuses:
    0 - Default
    1 - Critical
    */
    static void LogError(FlupStr text, int status);

    static void LogWarning(FlupStr text);

    static void LogText(FlupStr text);
};