#include "stdafx.h"
#include "AccHistoryStore.h"

CAccHistoryStore& CAccHistoryStore::Instance()
{
	static CAccHistoryStore inst;
	return inst;
}

CString CAccHistoryStore::MakeCacheKey(const CString& strUserPath, const CString& strKey)
{
	CString key = strUserPath;
	key += _T("|");
	key += strKey;
	return key;
}

CString CAccHistoryStore::Read(const CString& strUserPath, const CString& strKey, const CString& strHome, const CString& regkey)
{
	CSingleLock lock(&m_cs, TRUE);

	CString slog;
	CString cacheKey = MakeCacheKey(strUserPath, strKey);

	CString cached;
	if (m_cache.Lookup(cacheKey, cached))
	{
		slog.Format(_T("[READ_CACHE] [ACCENC][%s]<%d>  strKey=[%s]"), __FUNCTION__, __LINE__, strKey);
		Output_DebugString(slog);
		return cached;
	}

	char readb[2048 * 64]{};
	GetPrivateProfileStringA("AccountHistory", strKey, "", readb, sizeof(readb), strUserPath);

	CString val(readb);
	if (val.IsEmpty())
	{
		m_cache.SetAt(cacheKey, val);
		return val;
	}

	// Check encryption enabled from axisai.ini
	CString axisaiPath;
	axisaiPath.Format(_T("%s\\tab\\axisai.ini"), strHome);
	int encEnabled = GetPrivateProfileIntA("ENC", "acchistory", 0, axisaiPath);

	// '|' present = plain text, return as-is
	// '|' absent + enc enabled = encrypted -> decrypt
	if (val.Find('|') < 0 && encEnabled == 1)
	{
		try
		{
			auto key = DeriveKeyFromRegkey(regkey);

			CString decrypted;
			CString remaining = val;
			int pos = 0;
			bool first = true;
			while ((pos = remaining.Find('\t')) != -1)
			{
				CString part = remaining.Left(pos);
				if (!part.IsEmpty())
				{
					CString dec = AesDecrypt(part, key);
					slog.Format(_T("[READ_ENC] [ACCENC][%s]<%d>  dec=[%s]"), __FUNCTION__, __LINE__, dec);
					Output_DebugString(slog);
					if (!first) decrypted += '\t';
					decrypted += dec;
					first = false;
				}
				remaining = remaining.Mid(pos + 1);
			}
			if (!remaining.IsEmpty())
			{
				CString lastDec = AesDecrypt(remaining, key);
				slog.Format(_T("[READ_ENC] [ACCENC][%s]<%d>  lastDec=[%s]"), __FUNCTION__, __LINE__, lastDec);
				Output_DebugString(slog);
				if (!first) decrypted += '\t';
				decrypted += lastDec;
			}
			val = decrypted;

			SecureZeroMemory(key.data(), key.size());
		}
		catch (const std::exception& e)
		{
			slog.Format(_T("[DEC_FAIL] [ACCENC][%s]<%d>  key=[%s] err=[%s]"), __FUNCTION__, __LINE__, strKey, e.what());
			Output_DebugString(slog);
		}
	}

	m_cache.SetAt(cacheKey, val);
	return val;
}

void CAccHistoryStore::Write(const CString& strUserPath, const CString& strKey, const CString& strData, const CString& strHome, const CString& regkey)
{
	CSingleLock lock(&m_cs, TRUE);

	CString cacheKey = MakeCacheKey(strUserPath, strKey);

	CString cached;
	if (m_cache.Lookup(cacheKey, cached) && cached == strData)
	{
		CString slog;
		slog.Format(_T("[WRITE_CACHE] [ACCENC][%s]<%d>  strKey=[%s] (skip-write)"), __FUNCTION__, __LINE__, strKey);
		Output_DebugString(slog);
		return;
	}

	// Check encryption enabled from axisai.ini
	CString axisaiPath;
	axisaiPath.Format(_T("%s\\tab\\axisai.ini"), strHome);
	int encEnabled = GetPrivateProfileIntA("ENC", "acchistory", 0, axisaiPath);

	CString valToWrite = strData;

	// '|' present + enc enabled = plain text -> encrypt before write
	if (!strData.IsEmpty() && strData.Find('|') >= 0 && encEnabled == 1)
	{
		auto key = DeriveKeyFromRegkey(regkey);
		valToWrite = AesEncrypt(strData, key);
		SecureZeroMemory(key.data(), key.size());

		CString slog;
		slog.Format(_T("[WRITE_ENC][ACCENC][%s]<%d>len=[%d] strData=[%s]  strKey=[%s]  [%d] [%s]"), __FUNCTION__, __LINE__, strData.GetLength(), strData.Left(10), strKey, valToWrite.GetLength(), valToWrite);
		Output_DebugString(slog);
	}

	WritePrivateProfileString(_T("AccountHistory"), (LPCTSTR)strKey, (LPCTSTR)valToWrite, (LPCTSTR)strUserPath);
	m_cache.SetAt(cacheKey, strData);
}
