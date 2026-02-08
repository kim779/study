#ifndef _UTILS_H
#define _UTILS_H

class CUtils
{
public:
	static CString		Parser(CString &sSrc, CString sDelimiter);
	static int			GetBtnWidth(int nTextLen);
	static int			MakeRecentRtMap(const char *pszData, CMapStringToString &mapRt, BOOL bTrace = FALSE);
	static CString		PeekCode(CString sData);
	static CString		AddComma(CString &sSrc);
//	static int			H2NL(int n);
	static CString		GetString(const char *p, int nSize, BOOL bTrim = FALSE);
	static long			GetLong(const char *p, int nSize);

	static BOOL			CheckResource();//20120112 이문수 리소스체크기능추가 >> 
};

#endif
