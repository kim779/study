// axlog.h - unified debug log helper, category on/off via DF_LOG_* defines.
// Shared across Wizard.dll, axisvbs.dll and axSock.ocx (inline, header-only - no link dependency).
#pragma once
#include <stdarg.h>

// Module tag shown as "[TAG][category]...". Define this before #include-ing
// axlog.h in a given .cpp to override; existing Wizard/axisvbs files that
// don't define it keep the original "WIZARD" tag unchanged.
#ifndef AXLOG_MODULE_TAG
#define AXLOG_MODULE_TAG "WIZARD"
#endif

#define DF_LOG_INIT
#define DF_LOG_EVENT
#define DF_LOG_DATA
#define DF_LOG_RTM
#define DF_LOG_SCRIPT
#define DF_LOG_FILEPATCH
#define DF_LOG_LOGIN
#define DF_LOG_SOCK_SEND
#define DF_LOG_SOCK_RECEIVE
#define DF_LOG_CERTIFY
// OFF by default: axisform.dll's CfmEdit::Draw/UpdateData fire on every repaint/keystroke, very high volume.
// #define DF_LOG_AXISFORM

enum axLogCat { LOG_INIT, LOG_EVENT, LOG_DATA, LOG_RTM, LOG_SCRIPT, LOG_AXISFORM, LOG_FILEPATCH, LOG_LOGIN, LOG_SOCK_SEND, LOG_SOCK_RECEIVE, LOG_CERTIFY };

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
#ifdef DF_LOG_AXISFORM
	case LOG_AXISFORM: return true;
#endif
#ifdef DF_LOG_FILEPATCH
	case LOG_FILEPATCH: return true;
#endif
#ifdef DF_LOG_LOGIN
	case LOG_LOGIN:  return true;
#endif
#ifdef DF_LOG_SOCK_SEND
	case LOG_SOCK_SEND:     return true;
#endif
#ifdef DF_LOG_SOCK_RECEIVE
	case LOG_SOCK_RECEIVE: return true;
#endif
#ifdef DF_LOG_CERTIFY
	case LOG_CERTIFY: return true;
#endif
	default: return false;
	}
}

inline void axlogImpl(axLogCat cat, const char* func, int line, LPCTSTR fmt, ...)
{
	if (!axLogOn(cat)) return;

	// Category names padded to the width of the longest one (SOCK_RECEIVE=12) so
	// "[TAG][xxx]" is always the same length regardless of category.
	static const char* catNames[] = {
		"INIT", "EVENT", "DATA", "RTM", "SCRIPT", "AXISFORM", "FILEPATCH", "LOGIN", "SOCK_SEND", "SOCK_RECEIVE", "CERTIFY"
	};

	va_list args;
	va_start(args, fmt);
	CString msg;
	msg.FormatV(fmt, args);
	va_end(args);

	CString funcLine;
	funcLine.Format("[%s:%d]", func, line);

	// funcLine is left-justified into a fixed field so the actual message
	// lines up in the same column regardless of function/class name length.
	// Longer entries just overflow the field without truncation or misalignment
	// of their own message - only later, shorter lines stay aligned.
	CString line_out;
	line_out.Format("[%s][%-12s] %-42s %s\n", AXLOG_MODULE_TAG, catNames[cat], (LPCTSTR)funcLine, (LPCTSTR)msg);

	OutputDebugString(line_out);
}

// Macro so __FUNCTION__/__LINE__ expand at the call site, not inside axlogImpl.
#define axlog(cat, ...) axlogImpl(cat, __FUNCTION__, __LINE__, __VA_ARGS__)
