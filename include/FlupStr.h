#pragma once
#include <FlupIncludes.h>
#ifndef _FLUP_STR_
#define _FLUP_STR_
#endif // _FLUP_STR_

#if !defined(_UNICODE) && !defined(UNICODE)
typedef std::string FlupStr;
#define FLUP_LITERAL(str) str
#else
typedef std::wstring FlupStr;
#define FLUP_LITERAL(str) L##str
#endif // !defined(_UNICODE) && !defined(UNICODE)

