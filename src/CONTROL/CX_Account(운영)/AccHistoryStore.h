#pragma once

// Process-wide singleton that serializes read/write access to the
// [AccountHistory] section of user\<name>\<name>.ini.
//
// Background: CX_Account is embedded in many different map screens, and a
// user can have several screens (several CAccountCtrl instances) open at
// once, all pointing at the SAME ini file. Each instance used to keep its
// own private cache, so one instance could overwrite another instance's
// more recent write with a stale value (lost update), and the read-check-
// write sequence around encryption was not atomic across instances.
// Routing every access through this singleton fixes both problems: one
// shared cache, one critical section around each full read/write sequence.

#include <afxmt.h>
#include "AccCrypto.h"

class CAccHistoryStore
{
public:
	static CAccHistoryStore& Instance();

	// strUserPath : full path to user\<name>\<name>.ini
	// strKey      : control instance name used as the ini key (e.g. "AN1A")
	// strHome     : AXIS home root (used to locate tab\axisai.ini for the ENC flag)
	// regkey      : per-user key material used to derive the AES key
	CString Read(const CString& strUserPath, const CString& strKey, const CString& strHome, const CString& regkey);
	void    Write(const CString& strUserPath, const CString& strKey, const CString& strData, const CString& strHome, const CString& regkey);

private:
	CAccHistoryStore() = default;
	CAccHistoryStore(const CAccHistoryStore&) = delete;
	CAccHistoryStore& operator=(const CAccHistoryStore&) = delete;

	static CString MakeCacheKey(const CString& strUserPath, const CString& strKey);

	CCriticalSection    m_cs;
	CMapStringToString  m_cache;
};
