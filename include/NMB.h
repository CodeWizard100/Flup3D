// ========================================================
// Copyright (c) 2020 Gabriel Schmitz. All rights reserved.
// ========================================================

#pragma once

#include <cstdio>
#include <cstdlib>
#include <string>
#include <sstream>
#include <locale>
#include <codecvt>

#include <FlupIncludes.h>  // Make sure this includes or precedes FlupStr typedef

#ifndef _FLUP_STR_
#define _FLUP_STR_

#if !defined(_UNICODE) && !defined(UNICODE)
typedef std::string FlupStr;
#define FLUP_LITERAL(str) str
#else
typedef std::wstring FlupStr;
#define FLUP_LITERAL(str) L##str
#endif

#endif // _FLUP_STR_

#if defined(_WIN32)
    #include <Windows.h>
#elif defined(__APPLE__)
    #include <CoreFoundation/CoreFoundation.h>
#elif defined(__linux__)
    // zenity usage does not need gtk headers anymore
#else
    #error "Platform not supported!"
#endif

namespace NMB
{

    enum Result
    {
        CANCEL,
        OK
    };

    enum Icon
    {
        ICON_INFO,
        ICON_WARNING,
        ICON_ERROR
    };

#if defined(_WIN32)
    inline const char* to_cstr(const FlupStr& str)
    {
    #if !defined(_UNICODE) && !defined(UNICODE)
        return str.c_str();
    #else
        static std::string utf8_str;
        int size_needed = WideCharToMultiByte(CP_UTF8, 0, str.c_str(), (int)str.size(), NULL, 0, NULL, NULL);
        utf8_str.resize(size_needed);
        WideCharToMultiByte(CP_UTF8, 0, str.c_str(), (int)str.size(), utf8_str.data(), size_needed, NULL, NULL);
        return utf8_str.c_str();
    #endif
    }
#elif defined(__APPLE__)
    inline CFStringRef to_CFStringRef(const FlupStr& str)
    {
    #if !defined(_UNICODE) && !defined(UNICODE)
        return CFStringCreateWithCString(kCFAllocatorDefault, str.c_str(), kCFStringEncodingUTF8);
    #else
        return CFStringCreateWithCharacters(kCFAllocatorDefault, (const UniChar*)str.c_str(), str.size());
    #endif
    }
#endif

    void show(const FlupStr& p_title, const FlupStr& p_message, Icon icon, bool enableCancel = false)
    {
#if defined(_WIN32)

        int icon_flag;
        switch (icon)
        {
        case NMB::Icon::ICON_INFO:    icon_flag = MB_ICONINFORMATION; break;
        case NMB::Icon::ICON_WARNING: icon_flag = MB_ICONWARNING; break;
        case NMB::Icon::ICON_ERROR:   icon_flag = MB_ICONERROR; break;
        default: icon_flag = MB_ICONINFORMATION; break;
        }

        UINT buttons = MB_OK | MB_SYSTEMMODAL | icon_flag;
        if (enableCancel)
            buttons = MB_OKCANCEL | MB_SYSTEMMODAL | icon_flag;

        int result = MessageBoxA(nullptr, to_cstr(p_message), to_cstr(p_title), buttons);

       

#elif defined(__APPLE__)

        CFOptionFlags cf_alert_icon;
        switch (icon)
        {
        case NMB::Icon::ICON_INFO:    cf_alert_icon = kCFUserNotificationNoteAlertLevel; break;
        case NMB::Icon::ICON_WARNING: cf_alert_icon = kCFUserNotificationCautionAlertLevel; break;
        case NMB::Icon::ICON_ERROR:   cf_alert_icon = kCFUserNotificationStopAlertLevel; break;
        default: cf_alert_icon = kCFUserNotificationNoteAlertLevel; break;
        }

        CFStringRef cf_title = to_CFStringRef(p_title);
        CFStringRef cf_message = to_CFStringRef(p_message);

        CFOptionFlags result;

        // Button titles depending on enableCancel
        CFStringRef okButton = FLUP_LITERAL("OK");
        CFStringRef cancelButton = enableCancel ? FLUP_LITERAL("Cancel") : nullptr;

        CFUserNotificationDisplayAlert(0, cf_alert_icon, nullptr, nullptr, nullptr,
                                       cf_title, cf_message,
                                       okButton,
                                       cancelButton,
                                       nullptr,
                                       &result);

        CFRelease(cf_title);
        CFRelease(cf_message);

    

#elif defined(__linux__)
       const char* icon_str = nullptr;
switch (icon)
{
    case NMB::Icon::ICON_INFO:    icon_str = "dialog-information"; break;
    case NMB::Icon::ICON_WARNING: icon_str = "dialog-warning"; break;
    case NMB::Icon::ICON_ERROR:   icon_str = "dialog-error"; break;
    default: icon_str = "dialog-information"; break;
}

#if !defined(_UNICODE) && !defined(UNICODE)
    std::string title_utf8 = p_title;
    std::string message_utf8 = p_message;
#else
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::string title_utf8 = converter.to_bytes(p_title);
    std::string message_utf8 = converter.to_bytes(p_message);
#endif

auto sanitize = [](const std::string& str) -> std::string {
    std::string result;
    for (char c : str)
        result += (c == '"') ? "\\\"" : std::string(1, c);
    return result;
};

std::string safe_title = sanitize(title_utf8);
std::string safe_message = sanitize(message_utf8);

// Compose zenity command using --icon instead of deprecated --window-icon
std::string command;
if (enableCancel)
{
    // question dialog with OK/Cancel
    command = "zenity --question --title=\"" + safe_title + "\" --text=\"" + safe_message + "\" --icon=" + icon_str;
}
else
{
    // info dialog with only OK button
    command = "zenity --info --title=\"" + safe_title + "\" --text=\"" + safe_message + "\" --icon=" + icon_str;
}

int ret = system(command.c_str());

       
#else

        #error "Platform not supported!"

#endif


} 

    Result show_withresult(const FlupStr& p_title, const FlupStr& p_message, Icon icon, bool enableCancel = false)
    {
#if defined(_WIN32)

        int icon_flag;
        switch (icon)
        {
        case NMB::Icon::ICON_INFO:    icon_flag = MB_ICONINFORMATION; break;
        case NMB::Icon::ICON_WARNING: icon_flag = MB_ICONWARNING; break;
        case NMB::Icon::ICON_ERROR:   icon_flag = MB_ICONERROR; break;
        default: icon_flag = MB_ICONINFORMATION; break;
        }

        UINT buttons = MB_OK | MB_SYSTEMMODAL | icon_flag;
        if (enableCancel)
            buttons = MB_OKCANCEL | MB_SYSTEMMODAL | icon_flag;

        int result = MessageBoxA(nullptr, to_cstr(p_message), to_cstr(p_title), buttons);

        if (result == IDOK)
            return NMB::Result::OK;
        else
            return NMB::Result::CANCEL;

#elif defined(__APPLE__)

        CFOptionFlags cf_alert_icon;
        switch (icon)
        {
        case NMB::Icon::ICON_INFO:    cf_alert_icon = kCFUserNotificationNoteAlertLevel; break;
        case NMB::Icon::ICON_WARNING: cf_alert_icon = kCFUserNotificationCautionAlertLevel; break;
        case NMB::Icon::ICON_ERROR:   cf_alert_icon = kCFUserNotificationStopAlertLevel; break;
        default: cf_alert_icon = kCFUserNotificationNoteAlertLevel; break;
        }

        CFStringRef cf_title = to_CFStringRef(p_title);
        CFStringRef cf_message = to_CFStringRef(p_message);

        CFOptionFlags result;

        // Button titles depending on enableCancel
        CFStringRef okButton = FLUP_LITERAL("OK");
        CFStringRef cancelButton = enableCancel ? FLUP_LITERAL("Cancel") : nullptr;

        CFUserNotificationDisplayAlert(0, cf_alert_icon, nullptr, nullptr, nullptr,
                                       cf_title, cf_message,
                                       okButton,
                                       cancelButton,
                                       nullptr,
                                       &result);

        CFRelease(cf_title);
        CFRelease(cf_message);

        // If no Cancel, any response other than default means OK
        if (result == kCFUserNotificationDefaultResponse)
            return NMB::Result::OK;
        else if (enableCancel && result == kCFUserNotificationAlternateResponse)
            return NMB::Result::CANCEL;
        else
            return NMB::Result::OK;

#elif defined(__linux__)

       const char* icon_str = nullptr;
switch (icon)
{
    case NMB::Icon::ICON_INFO:    icon_str = "dialog-information"; break;
    case NMB::Icon::ICON_WARNING: icon_str = "dialog-warning"; break;
    case NMB::Icon::ICON_ERROR:   icon_str = "dialog-error"; break;
    default: icon_str = "dialog-information"; break;
}

#if !defined(_UNICODE) && !defined(UNICODE)
    std::string title_utf8 = p_title;
    std::string message_utf8 = p_message;
#else
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::string title_utf8 = converter.to_bytes(p_title);
    std::string message_utf8 = converter.to_bytes(p_message);
#endif

auto sanitize = [](const std::string& str) -> std::string {
    std::string result;
    for (char c : str)
        result += (c == '"') ? "\\\"" : std::string(1, c);
    return result;
};

std::string safe_title = sanitize(title_utf8);
std::string safe_message = sanitize(message_utf8);

// Compose zenity command using --icon instead of deprecated --window-icon
std::string command;
if (enableCancel)
{
    // question dialog with OK/Cancel
    command = "zenity --question --title=\"" + safe_title + "\" --text=\"" + safe_message + "\" --icon=" + icon_str;
}
else
{
    // info dialog with only OK button
    command = "zenity --info --title=\"" + safe_title + "\" --text=\"" + safe_message + "\" --icon=" + icon_str;
}

int ret = system(command.c_str());

if (enableCancel)
{
    if (ret == 0)  // OK pressed
        return NMB::Result::OK;
    else           // Cancel or closed
        return NMB::Result::CANCEL;
}
else
{
    // info dialog always returns 0 on OK
    if (ret == 0)
        return NMB::Result::OK;
    else
        return NMB::Result::CANCEL;  // unlikely, but treat any other as cancel
}

#else

        #error "Platform not supported!"

#endif


} 
}