// axlog.h - unified debug log helper, category on/off via DF_LOG_* defines.
// Shared across Wizard.dll and axisvbs.dll (inline, header-only - no link dependency).
#pragma once
#include <stdarg.h>

#define DF_LOG_INIT
#define DF_LOG_EVENT
#define DF_LOG_DATA
#define DF_LOG_RTM
//#define DF_LOG_SCRIPT

enum axLogCat { LOG_INIT, LOG_EVENT, LOG_DATA, LOG_RTM, LOG_SCRIPT };

inline bool axLogOn(axLogCat cat)
{
	switch (cat)
	{
#ifdef DF_LOG_INIT
	case LOG_INIT:   return true;
#endif
#ifdef DF_LOG_EVENT
	case LOG_EVENT:  return true;
#endif
#ifdef DF_LOG_DATA
	case LOG_DATA:   return true;
#endif
#ifdef DF_LOG_RTM
	case LOG_RTM:    return true;
#endif
#ifdef DF_LOG_SCRIPT
	case LOG_SCRIPT: return true;
#endif
	default: return false;
	}
}

inline void axlogImpl(axLogCat cat, const char* func, int line, LPCTSTR fmt, ...)
{
	if (!axLogOn(cat)) return;

	static const char* tags[] = {
		"[WIZARD][INIT][DEBUG] ", "[WIZARD][EVENT][DEBUG] ",
		"[WIZARD][DATA][DEBUG] ", "[WIZARD][RTM][DEBUG] ", "[WIZARD][SCRIPT][DEBUG] "
	};

	va_list args;
	va_start(args, fmt);
	CString msg;
	msg.FormatV(fmt, args);
	va_end(args);

	CString head;
	head.Format("[%s:%d] ", func, line);

	OutputDebugString(tags[cat] + head + msg + "\n");
}

// Macro so __FUNCTION__/__LINE__ expand at the call site, not inside axlogImpl.
#define axlog(cat, ...) axlogImpl(cat, __FUNCTION__, __LINE__, __VA_ARGS__)
