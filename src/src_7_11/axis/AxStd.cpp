#include "stdafx.h"
#include "AxStd.h"
#include <string>

#ifdef _DEBUG
void AFXAPI AxStd::Msg(char* fmt, ...)
{
	CString	tmpstr, str;

	va_list	valist;
	va_start(valist, fmt);
	tmpstr.FormatV(fmt, valist);
	va_end(valist);

	if (tmpstr == "")
		str = "********************************************************************************************************";
	else
	{
		CTime time = CTime::GetCurrentTime();
		CString sTime;

		sTime.Format("[%02d%02d-%02d:%02d:%02d] ", time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
		str = "<Debug>" + sTime + tmpstr;
	}

	OutputDebugString(str + "\n");
	/*
	ofstream dFile(_T("c:\\Debug.log"), std::ios::app);
	dFile << str << endl;
	dFile.close();
	*/
}
#endif

CString AFXAPI AxStd::FORMAT(const char* formatString, ...)
{
	CString	s;

	va_list	valist;
	va_start(valist, formatString);
	s.FormatV(formatString, valist);
	va_end(valist);

	return s;
}

CString AxStd::Parser(CString &srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
		return temp;
	}
	else
	{
		CString  temp = srcstr.Left(srcstr.Find(substr));
		srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
		return temp;
	}
	return "";
}

int AxStd::SplitSub(vector<CString> &elems, CString& srcstr, CString substr)
{
	while (!srcstr.IsEmpty())
		elems.push_back(Parser(srcstr, substr));

	return elems.size();
}

int AxStd::Split(vector<CString> &elems, CString& srcstr, CString substr)
{
	char* ptxt = nullptr;
	char *next = nullptr;
	if (srcstr.IsEmpty())
		return 0;
	
	ptxt = strtok_s(srcstr.GetBuffer(0), substr, &next);
	while ( ptxt != NULL )
	{
		elems.push_back(ptxt);
		ptxt =strtok_s(nullptr, substr, &next);
	}
	return elems.size();
}

int AxStd::GetLineCount(CString str)
{
	vector<CString> vLine;
	return Split(vLine, string(str), '\n');
}

int AxStd::Split(vector<CString> &elems, const string &s, char delim)
{
	stringstream ss(s);
	string item;
	while (getline(ss, item, delim))
		elems.push_back(CSTRING(item));

	return elems.size();
}


/****************************************************************************************************
* Copyright (C) 2013 by Kyung-jin Kim
* e-mail		: devmachine@naver.com
*
*
* Description	: Easy Ini Helper Class
* Created		: Sep-5,2013
* Last Updated	: Jul-15,2014
* Version		: EZIni v1.2
*/

/**
* Sample code

************ Write Operation ************
try
{
EZIni ini(_T("D:\\ServerInfo.ini"));

ini["Primary"]["IP"] = _T("192.168.0.1");
ini["Primary"]["Port"] = 5678;
ini["Primary"]["Value"] = 89.97;
ini["Primary"]["HasSecondary"] = true;
ini["Primary"]["LastAccess"] = COleDateTime::GetCurrentTime();

EZIni::Section section = ini["Secondary"];
section["IP"] = _T("192.168.0.2");
section["Port"] = 6789;

EZIni::Key key = section["Value"];
key = 78.56;
key = 90.12;
}
catch (EZIni::Exception& e)
{
TRACE(_T("Ini Error: %s\n"), e.GetErrorMessage());
}

************ Read Operation ************
EZIni ini(_T("D:\\ServerInfo.ini"));

CString sIP = ini["Primary"]["IP"];
int nPort = ini["Primary"]["Port"];
double dValue = ini["Primary"]["Value"];
bool bHasSecondary = ini["Primary"]["HasSecondary"];
COleDateTime dtLastAccess = ini["Primary"]["LastAccess"];

TRACE(_T("Primary Server Info\n"));
TRACE(_T("IP: %s, Port: %d, Value: %.2f\n"), sIP, nPort, dValue);
TRACE(_T("HasSecondary: %s, LastAccess: %s\n\n"),
bHasSecondary ? _T("true") : _T("false"), dtLastAccess.Format(_T("%Y-%m-%d %H:%M:%S")));

// Use default value if it doesn't exist;
sIP = ini["Tertiary"]["IP"] << _T("192.168.0.3");
nPort = ini["Tertiary"]["Port"] << 7890;
dValue = ini["Tertiary"]["Value"] << 12.79;

TRACE(_T("Tertiary Server Info\n"));
TRACE(_T("IP: %s, Port: %d, Value: %.2f\n"), sIP, nPort, dValue);

************ Arithmetic/Logical Operation ************
try
{
EZIni ini(_T("D:\\ServerInfo.ini"));

ini["Primary"]["IP"] += _T(":5678");

++ini["Primary"]["Port"];
--ini["Primary"]["Port"];

ini["Primary"]["Value"] *= 10.0;
ini["Primary"]["LastAccess"] += COleDateTimeSpan(1, 0, 0, 0);
}
catch (EZIni::Exception& e)
{
TRACE(_T("Ini Error: %s\n"), e.GetErrorMessage());
}

************ Helper Functions ************
EZIni ini(_T("D:\\ServerInfo.ini"));

// Print all sections
std::vector<CString> listOfSectionNames;
ini.EnumSectionNames(listOfSectionNames);

TRACE(_T("List of Section names\n"));
for (auto it = listOfSectionNames.begin(); it != listOfSectionNames.end(); ++it)
TRACE(_T("%s\n"), *it);

// Print all keys in Primary Section
std::vector<CString> listOfKeyNames;
ini["Primary"].EnumKeyNames(listOfKeyNames);

TRACE(_T("\nList of Key names(Primary Section)\n"));
for (auto it = listOfKeyNames.begin(); it != listOfKeyNames.end(); ++it)
TRACE(_T("%s\n"), *it);

// Delete key if it exists
if (ini["Secondary"]["IP"].Exists())
ini["Secondary"]["IP"].Delete();

// Delete section if it exists
if (ini["Secondary"].Exists())
ini["Secondary"].Delete();

************ Unicode Support ************
EZIni ini(_T("D:\\ServerInfo.ini"), TRUE);
*/


// EZIni::Section class
AxStd::EZIni::Section::Section(LPCTSTR lpFileName, LPCTSTR lpSectionName)
	: m_sFileName(lpFileName),
	m_sSectionName(lpSectionName)
{
}
AxStd::EZIni::Key AxStd::EZIni::Section::operator[](LPCSTR lpKeyName)
{
	return Key(m_sFileName, m_sSectionName, CA2T(lpKeyName));
}

AxStd::EZIni::Key AxStd::EZIni::Section::operator[](LPCWSTR lpKeyName)
{
	return Key(m_sFileName, m_sSectionName, CW2T(lpKeyName));
}

BOOL AxStd::EZIni::Section::Exists() const
{
	std::vector<CString> listOfSections;

	EZIni dummy(m_sFileName);
	dummy.EnumSectionNames(listOfSections);

	return std::find(listOfSections.begin(), listOfSections.end(), m_sSectionName) != listOfSections.end();
}

BOOL AxStd::EZIni::Section::Delete()
{
	return WritePrivateProfileString(m_sSectionName, NULL, _T(""), m_sFileName);
}

BOOL AxStd::EZIni::Section::EnumKeyNames(std::vector<CString>& listOfKeyNames) const
{
	if (!PathFileExists(m_sFileName))
		return FALSE;

	listOfKeyNames.clear();

	DWORD dwLen = READ_BUFFER_SIZE;
	LPTSTR pBuffer = new TCHAR[dwLen + 1];
	DWORD dwCopied = GetPrivateProfileSection(m_sSectionName, pBuffer, dwLen, m_sFileName);
	while (dwCopied + 2 >= dwLen)
	{
		dwLen += READ_BUFFER_SIZE;
		delete[] pBuffer;
		pBuffer = new TCHAR[dwLen + 1];
		dwCopied = GetPrivateProfileSection(m_sSectionName, pBuffer, dwLen, m_sFileName);
	}

	CString sLine, sKeyName;
	LPCTSTR pLines = pBuffer;
	while (*pLines)
	{
		sLine = pLines;
		AfxExtractSubString(sKeyName, pLines, 0, _T('='));
		sKeyName.Trim();

		listOfKeyNames.push_back(sKeyName);
		pLines += sLine.GetLength() + 1;
	}

	delete[] pBuffer;
	return TRUE;
}

// EzIni::Key class
AxStd::EZIni::Key::Key(LPCTSTR lpFileName, LPCTSTR lpSectionName, LPCTSTR lpKeyName)
	: m_sFileName(lpFileName),
	m_sSectionName(lpSectionName),
	m_sKeyName(lpKeyName),
	m_sDefaultValue(_T(""))
{
	m_nDefaultValue = 0;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator=(int nValue)
{
	_SetKeyValue(nValue, _T("%d"));
	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator=(UINT uValue)
{
	_SetKeyValue(uValue, _T("%u"));
	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator=(INT64 n64Value)
{
	_SetKeyValue(n64Value, _T("%I64d"));
	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator=(UINT64 u64Value)
{
	_SetKeyValue(u64Value, _T("%I64u"));
	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator=(bool bValue)
{
	_SetKeyValue(bValue, _T("%d"));
	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator=(double dValue)
{
	_SetKeyValue(dValue, _T("%f"));
	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator=(LPCTSTR lpValue)
{
	_SetKeyValue(lpValue, _T("%s"));
	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator=(POINT ptValue)
{
	_SetKeyValue(_Point2String(ptValue), _T("%s"));
	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator=(LPCRECT lpRectValue)
{
	_SetKeyValue(_Rect2String(lpRectValue), _T("%s"));
	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator=(const COleDateTime& dtValue)
{
	_SetKeyValue(_DateTime2String(dtValue), _T("%s"));
	return *this;
}

AxStd::EZIni::Key::operator int()
{
	CString sValue = _GetKeyValue(m_nDefaultValue, _T("%d"));
	return _tcstol(sValue, NULL, 10);
}

AxStd::EZIni::Key::operator UINT()
{
	CString sValue = _GetKeyValue(m_uDefaultValue, _T("%u"));
	return _tcstoul(sValue, NULL, 10);
}

AxStd::EZIni::Key::operator INT64()
{
	CString sValue = _GetKeyValue(m_n64DefaultValue, _T("%I64d"));
	return _tcstoi64(sValue, NULL, 10);
}

AxStd::EZIni::Key::operator UINT64()
{
	CString sValue = _GetKeyValue(m_u64DefaultValue, _T("%I64u"));
	return _tcstoui64(sValue, NULL, 10);
}

AxStd::EZIni::Key::operator bool()
{
	CString sValue = _GetKeyValue(m_bDefaultValue, _T("%d"));
	return (_tcstol(sValue, NULL, 10)) == 0 ? false : true;
}

AxStd::EZIni::Key::operator double()
{
	CString sValue = _GetKeyValue(m_dDefaultValue, _T("%f"));
	return _tstof(sValue);
}

AxStd::EZIni::Key::operator CString()
{
	CString sValue = _GetKeyValue(m_sDefaultValue, _T("%s"));
	return sValue;
}

AxStd::EZIni::Key::operator CPoint()
{
	CString sValue = _GetKeyValue(_Point2String(m_ptDefaultValue), _T("%s"));
	return _String2Point(sValue);
}

AxStd::EZIni::Key::operator CRect()
{
	CString sValue = _GetKeyValue(_Rect2String(&m_rcDefaultValue), _T("%s"));
	return _String2Rect(sValue);
}

AxStd::EZIni::Key::operator COleDateTime()
{
	CString sValue = _GetKeyValue(_DateTime2String(m_dtDefaultValue), _T("%s"));
	return _String2DateTime(sValue);
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator++()
{
	int nValue = *this;
	*this = ++nValue;

	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator--()
{
	int nValue = *this;
	*this = --nValue;

	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator+=(int nValue)
{
	int nRead = *this;
	*this = nRead + nValue;

	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator-=(int nValue)
{
	int nRead = *this;
	*this = nRead - nValue;
	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator*=(int nValue)
{
	int nRead = *this;
	*this = nRead * nValue;

	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator/=(int nValue)
{
	ASSERT(0 != nValue);

	int nRead = *this;
	*this = nRead / nValue;

	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator%=(int nValue)
{
	ASSERT(0 != nValue);

	int nRead = *this;
	*this = nRead % nValue;

	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator+=(UINT uValue)
{
	UINT uRead = *this;
	*this = uRead + uValue;

	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator-=(UINT uValue)
{
	UINT uRead = *this;
	*this = uRead - uValue;

	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator*=(UINT uValue)
{
	UINT uRead = *this;
	*this = uRead * uValue;

	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator/=(UINT uValue)
{
	ASSERT(0 != uValue);

	UINT uRead = *this;
	*this = uRead / uValue;

	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator%=(UINT uValue)
{
	ASSERT(0 != uValue);

	UINT uRead = *this;
	*this = uRead + uValue;

	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator<<=(UINT uValue)
{
	UINT uRead = *this;
	*this = uRead << uValue;

	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator>>=(UINT uValue)
{
	UINT uRead = *this;
	*this = uRead >> uValue;

	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator&=(UINT uValue)
{
	UINT uRead = *this;
	*this = uRead & uValue;

	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator|=(UINT uValue)
{
	UINT uRead = *this;
	*this = uRead | uValue;

	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator^=(UINT uValue)
{
	UINT uRead = *this;
	*this = uRead ^ uValue;

	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator+=(INT64 n64Value)
{
	INT64 n64Read = *this;
	*this = n64Read + n64Value;

	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator-=(INT64 n64Value)
{
	INT64 n64Read = *this;
	*this = n64Read - n64Value;

	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator*=(INT64 n64Value)
{
	INT64 n64Read = *this;
	*this = n64Read * n64Value;

	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator/=(INT64 n64Value)
{
	ASSERT(0 != n64Value);

	INT64 n64Read = *this;
	*this = n64Read / n64Value;

	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator%=(INT64 n64Value)
{
	ASSERT(0 != n64Value);

	INT64 n64Read = *this;
	*this = n64Read % n64Value;

	return *this;
}


AxStd::EZIni::Key& AxStd::EZIni::Key::operator+=(UINT64 u64Value)
{
	UINT64 u64Read = *this;
	*this = u64Read + u64Value;

	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator-=(UINT64 u64Value)
{
	UINT64 u64Read = *this;
	*this = u64Read - u64Value;

	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator*=(UINT64 u64Value)
{
	UINT64 u64Read = *this;
	*this = u64Read * u64Value;

	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator/=(UINT64 u64Value)
{
	ASSERT(0 != u64Value);

	UINT64 u64Read = *this;
	*this = u64Read / u64Value;

	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator%=(UINT64 u64Value)
{
	ASSERT(0 != u64Value);

	UINT64 u64Read = *this;
	*this = u64Read % u64Value;

	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator<<=(UINT64 u64Value)
{
	UINT64 u64Read = *this;
	*this = u64Read << u64Value;

	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator>>=(UINT64 u64Value)
{
	UINT64 u64Read = *this;
	*this = u64Read >> u64Value;

	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator&=(UINT64 u64Value)
{
	UINT64 u64Read = *this;
	*this = u64Read & u64Value;

	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator|=(UINT64 u64Value)
{
	UINT64 u64Read = *this;
	*this = u64Read | u64Value;

	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator^=(UINT64 u64Value)
{
	UINT64 u64Read = *this;
	*this = u64Read ^ u64Value;

	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator+=(double dValue)
{
	double dRead = *this;
	*this = dRead + dValue;

	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator-=(double dValue)
{
	double dRead = *this;
	*this = dRead - dValue;
	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator*=(double dValue)
{
	double dRead = *this;
	*this = dRead * dValue;

	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator/=(double dValue)
{
	ASSERT(0.0 != dValue);

	double dRead = *this;
	*this = dRead / dValue;

	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator+=(LPCTSTR lpText)
{
	CString sRead = *this;
	*this = sRead + lpText;

	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator+=(POINT ptValue)
{
	CString sRead, sTemp;
	int nDelimiter;

	sTemp = sRead = *this;
	nDelimiter = sTemp.Remove(_T(','));

	if (1 == nDelimiter)
		*this = _String2Point(sRead) + ptValue;

	else if (3 == nDelimiter)
		*this = _String2Rect(sRead) + ptValue;

	else
		ASSERT(FALSE);

	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator-=(POINT ptValue)
{
	*this += CPoint(-ptValue.x, -ptValue.y);
	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator+=(SIZE sizeValue)
{
	CString sRead, sTemp;
	int nDelimiter;

	sTemp = sRead = *this;
	nDelimiter = sTemp.Remove(_T(','));

	if (1 == nDelimiter)
		*this = _String2Point(sRead) + sizeValue;

	else if (3 == nDelimiter)
		*this = _String2Rect(sRead) + sizeValue;

	else
		ASSERT(FALSE);

	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator-=(SIZE sizeValue)
{
	*this += CSize(-sizeValue.cx, -sizeValue.cy);
	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator+=(LPCRECT lpRectValue)
{
	CRect rcRead = *this;
	*this = rcRead + lpRectValue;

	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator-=(LPCRECT lpRectValue)
{
	CRect rcRead = *this;
	*this = rcRead - lpRectValue;

	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator+=(COleDateTimeSpan dateSpan)
{
	COleDateTime dtRead = *this;
	*this = dtRead + dateSpan;

	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator-=(COleDateTimeSpan dateSpan)
{
	COleDateTime dtRead = *this;
	*this = dtRead - dateSpan;

	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator<<(int nDefaultValue)
{
	m_nDefaultValue = nDefaultValue;
	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator<<(UINT uDefaultValue)
{
	m_uDefaultValue = uDefaultValue;
	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator<<(INT64 n64DefaultValue)
{
	m_n64DefaultValue = n64DefaultValue;
	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator<<(UINT64 u64DefaultValue)
{
	m_u64DefaultValue = u64DefaultValue;
	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator<<(bool bDefaultValue)
{
	m_bDefaultValue = bDefaultValue;
	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator<<(double dDefaultValue)
{
	m_dDefaultValue = dDefaultValue;
	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator<<(LPCTSTR lpDefaultValue)
{
	m_sDefaultValue = lpDefaultValue;
	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator<<(POINT ptDefaultValue)
{
	m_ptDefaultValue = ptDefaultValue;
	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator<<(LPCRECT lpDefaultValue)
{
	m_rcDefaultValue = *lpDefaultValue;
	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator<<(const COleDateTime& dtDefaultValue)
{
	m_dtDefaultValue = dtDefaultValue;
	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator>>(int nDefaultValue)
{
	m_nDefaultValue = nDefaultValue;
	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator>>(UINT uDefaultValue)
{
	m_uDefaultValue = uDefaultValue;
	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator>>(INT64 n64DefaultValue)
{
	m_n64DefaultValue = n64DefaultValue;
	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator>>(UINT64 u64DefaultValue)
{
	m_u64DefaultValue = u64DefaultValue;
	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator>>(bool bDefaultValue)
{
	m_bDefaultValue = bDefaultValue;
	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator>>(double dDefaultValue)
{
	m_dDefaultValue = dDefaultValue;
	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator>>(LPCTSTR lpDefaultValue)
{
	m_sDefaultValue = lpDefaultValue;
	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator>>(POINT ptDefaultValue)
{
	m_ptDefaultValue = ptDefaultValue;
	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator>>(LPCRECT lpDefaultValue)
{
	m_rcDefaultValue = *lpDefaultValue;
	return *this;
}

AxStd::EZIni::Key& AxStd::EZIni::Key::operator>>(const COleDateTime& dtDefaultValue)
{
	m_dtDefaultValue = dtDefaultValue;
	return *this;
}

BOOL AxStd::EZIni::Key::Exists() const
{
	std::vector<CString> listOfKeyNames;

	EZIni dummy(m_sFileName);
	dummy[m_sSectionName].EnumKeyNames(listOfKeyNames);

	return std::find(listOfKeyNames.begin(), listOfKeyNames.end(), m_sKeyName) != listOfKeyNames.end();
}

BOOL AxStd::EZIni::Key::Delete()
{
	return WritePrivateProfileString(m_sSectionName, m_sKeyName, NULL, m_sFileName);
}

CString AxStd::EZIni::Key::_Point2String(const POINT& ptValue)
{
	CString sResult;
	sResult.Format(_T("%d,%d"), ptValue.x, ptValue.y);
	return sResult;
}

CString AxStd::EZIni::Key::_Rect2String(LPCRECT lpRectValue)
{
	CString sResult;
	sResult.Format(_T("%d,%d,%d,%d"), lpRectValue->left, lpRectValue->top, lpRectValue->right, lpRectValue->bottom);
	return sResult;
}

CString AxStd::EZIni::Key::_DateTime2String(const COleDateTime& dtValue)
{
	return dtValue.Format(_T("%Y-%m-%d %H:%M:%S"));
}

CPoint AxStd::EZIni::Key::_String2Point(LPCTSTR lpValue)
{
	CString sPoint[2];
	for (int i = 0; i < 2; ++i)
		AfxExtractSubString(sPoint[i], lpValue, i, _T(','));

	return CPoint(_tstoi(sPoint[0]), _tstoi(sPoint[1]));
}

CRect AxStd::EZIni::Key::_String2Rect(LPCTSTR lpValue)
{
	CString sRect[4];
	for (int i = 0; i < 4; ++i)
		AfxExtractSubString(sRect[i], lpValue, i, _T(','));

	return CRect(_tstoi(sRect[0]), _tstoi(sRect[1]), _tstoi(sRect[2]), _tstoi(sRect[3]));
}

COleDateTime AxStd::EZIni::Key::_String2DateTime(LPCTSTR lpValue)
{
	COleDateTime dtResult;
	bool bParsed = dtResult.ParseDateTime(lpValue);
	ASSERT(bParsed);
	return dtResult;
}

// EZIni class
AxStd::EZIni::EZIni(void)
{
}

AxStd::EZIni::EZIni(LPCTSTR lpFileName, BOOL bCreateAsUnicode /*= FALSE*/)
{
	SetFileName(lpFileName, bCreateAsUnicode);
}

AxStd::EZIni::~EZIni(void)
{
}

AxStd::EZIni::Section AxStd::EZIni::operator[](LPCSTR lpSectionName)
{
	return Section(m_sFileName, CA2T(lpSectionName));
}

AxStd::EZIni::Section AxStd::EZIni::operator[](LPCWSTR lpSectionName)
{
	return Section(m_sFileName, CW2T(lpSectionName));
}

void AxStd::EZIni::SetFileName(LPCTSTR lpFileName, BOOL bCreateAsUnicode /*= FALSE*/)
{
	m_sFileName = lpFileName;
	if (bCreateAsUnicode && !PathFileExists(lpFileName))
	{
		WORD BOM = 0xFEFF;
		HANDLE hFile = CreateFile(lpFileName, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
		if (INVALID_HANDLE_VALUE != hFile)
		{
			WriteFile(hFile, &BOM, sizeof(WORD), NULL, NULL);
			CloseHandle(hFile);
		}
	}
}

BOOL AxStd::EZIni::EnumSectionNames(std::vector<CString>& listOfSectionNames) const
{
	if (!PathFileExists(m_sFileName))
		return FALSE;

	listOfSectionNames.clear();

	DWORD dwLen = READ_BUFFER_SIZE;
	LPTSTR pBuffer = new TCHAR[dwLen + 1];
	DWORD dwCopied = GetPrivateProfileSectionNames(pBuffer, dwLen, m_sFileName);
	while (dwCopied + 2 >= dwLen)
	{
		dwLen += READ_BUFFER_SIZE;
		delete[] pBuffer;
		pBuffer = new TCHAR[dwLen + 1];
		dwCopied = GetPrivateProfileSectionNames(pBuffer, dwLen, m_sFileName);
	}

	LPCTSTR pSections = pBuffer;
	while (*pSections)
	{
		listOfSectionNames.push_back(pSections);
		pSections += listOfSectionNames.back().GetLength() + 1;
	}

	delete[] pBuffer;
	return TRUE;
}

CString AxStd::EZIni::_GetErrorMessage()
{
	LPVOID lpMessage;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMessage, 0, NULL);

	CString sResult((LPTSTR)lpMessage);
	LocalFree(lpMessage);
	sResult.TrimRight(_T(" \t\r\n"));

	return sResult;
}


/**
* Copyright(C) 2014 by Kyung - jin Kim
* e - mail		: devmachine@naver.com
*
*
* Description	: Easy Registry Helper Class
* Created : Jul - 16, 2014
* Last Updated : Jul - 16, 2014
* Version : EZRegistry v1.0
* /

/**
* Sample code

// ************ Write Operation ************
try
{
EZRegistry reg(EZRegistry::currentUser,
_T("SOFTWARE\\EZRegistryDemo\\ServerInfo"),
EZRegistry::modeWrite);

reg["IP"] = _T("192.168.0.1");
reg["Port"] = 5678;
reg["Value"] = 89.97;
reg["HasSecondary"] = true;
reg["LastAccess"] = COleDateTime::GetCurrentTime();

EZRegistry::Value port = reg["Port"];
port = 6789;
port = 4567;
}
catch (EZRegistry::Exception& e)
{
TRACE(_T("Registry Error: %s\n"), e.GetErrorMessage());
}

// ************ Read Operation ************
try
{
EZRegistry reg(EZRegistry::currentUser,
_T("SOFTWARE\\EZRegistryDemo\\ServerInfo"),
EZRegistry::modeRead);

CString sIP = reg["IP"];
int nPort = reg["Port"];
double dValue = reg["Value"];
bool bHasSecondary = reg["HasSecondary"];
COleDateTime dtLastAccess = reg["LastAccess"];

TRACE(_T("Server Info\n"));
TRACE(_T("IP: %s, Port: %d, Value: %.2f\n"), sIP, nPort, dValue);
TRACE(_T("HasSecondary: %s, LastAccess: %s\n"),
bHasSecondary ? _T("true") : _T("false"), dtLastAccess.Format(_T("%Y-%m-%d %H:%M:%S")));
}
catch (EZRegistry::Exception& e)
{
TRACE(_T("Registry Error: %s\n"), e.GetErrorMessage());
}

// ************ Read Operation with Default Value ************

EZRegistry reg(EZRegistry::currentUser,
_T("SOFTWARE\\EZRegistryDemo\\SecondaryServerInfo"),
EZRegistry::modeRead | EZRegistry::optUseDefaultValue);

// Use default value if it doesn't exist
CString sIP = reg["Secondary IP"] << _T("192.168.0.2");
int nPort = reg["Secondary Port"] << 7890;
double dValue = reg["Secondary Value"] << 12.79;

TRACE(_T("Secondary Server Info\n"));
TRACE(_T("IP: %s, Port: %d, Value: %.2f\n"), sIP, nPort, dValue);

// ************ Access with Subkey ************
try
{
EZRegistry reg(EZRegistry::currentUser,
_T("SOFTWARE\\EZRegistryDemo"),
EZRegistry::modeReadWrite);

reg("ServerInfo")["IP"] = _T("192.168.0.10");
(reg + "ServerInfo")["Port"] = 1234;

reg("Layout")("Dialog")["Rect"] = CRect(100, 100, 500, 600);
}
catch (EZRegistry::Exception& e)
{
TRACE(_T("Registry Error: %s\n"), e.GetErrorMessage());
}

// ************ Arithmetic/Logical Operation ************
try
{
EZRegistry reg(EZRegistry::currentUser,
_T("SOFTWARE\\EZRegistryDemo\\ServerInfo"),
EZRegistry::modeReadWrite);

reg["IP"] += _T(":5678");

++reg["Port"];
--reg["Port"];

reg["Value"] *= 10.0;
reg["LastAccess"] += COleDateTimeSpan(1, 0, 0, 0);
}
catch (EZRegistry::Exception& e)
{
TRACE(_T("Registry Error: %s\n"), e.GetErrorMessage());
}

// ************ Binary Data Read/Write ************
try
{
EZRegistry reg(EZRegistry::currentUser,
_T("SOFTWARE\\EZRegistryDemo"),
EZRegistry::modeReadWrite);

BYTE bin[10] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99 };
EZRegistry::Binary binaryForWrite(bin, 10);

reg["Binary"] = binaryForWrite;

EZRegistry::Binary binaryForRead = reg["Binary"];

int nCount = binaryForRead.GetLength();
BYTE* pData = binaryForRead.GetData();

for (int i = 0; i < nCount; ++i)
{
TRACE(_T("0x%02X "), pData[i]);
}
TRACE(_T("\n"));
}
catch (EZRegistry::Exception& e)
{
TRACE(_T("Registry Error: %s\n"), e.GetErrorMessage());
}

// ************ Helper Functions ************
try
{
EZRegistry reg(EZRegistry::currentUser,
_T("SOFTWARE\\EZRegistryDemo"),
EZRegistry::modeReadWrite);

// Print all subkeys if it has subkeys.
if (reg.HasSubKeys())
{
std::vector<CString> listOfSubKeyNames;
reg.EnumSubKeys(listOfSubKeyNames);

TRACE(_T("List of Subkey names\n"));
for (auto it = listOfSubKeyNames.begin(); it != listOfSubKeyNames.end(); ++it)
TRACE(_T("%s\n"), *it);
}

// Print all values of ServerInfo
std::vector<EZRegistry::EnumeratedValue> listOfValues;
reg("ServerInfo").EnumValues(listOfValues);

TRACE(_T("\nList of Values\n"));
for (auto it = listOfValues.begin(); it != listOfValues.end(); ++it)
{
TRACE(_T("Name: %s, Type: "), it->GetName());
DWORD dwType = it->GetType();
switch (dwType)
{
case REG_SZ:
TRACE(_T("REG_SZ, Value: %s\n"), (CString)*it);
break;

case REG_DWORD:
TRACE(_T("REG_DWORD, Value: %d\n"), (int)*it);
break;

case REG_BINARY:
TRACE(_T("REG_BINARY\n"));
break;
}
}

// If ServerInfo key exists.
if (reg.SubKeyExists(_T("ServerInfo")))
{
EZRegistry& regServerInfo = reg("ServerInfo");

// Delete IP Value if it exists
if (regServerInfo["IP"].Exists())
regServerInfo["IP"].Delete();

// Delete ServerInfo key.
// Do not use regServerInfo variable after deletion.
regServerInfo.Delete();
}
}
catch (EZRegistry::Exception& e)
{
TRACE(_T("Registry Error: %s\n"), e.GetErrorMessage());
}

// ************ WOW64_32/WOW64_64 ************
try
{
// Access 32bit registry in 64 bit application
EZRegistry reg32(EZRegistry::localMachine,
_T("SOFTWARE\\EZRegistryDemo\\ServerInfo"),
EZRegistry::modeRead | EZRegistry::wow64Key32);

// Access 64bit registry in 32 bit application
EZRegistry reg64(EZRegistry::localMachine,
_T("SOFTWARE\\EZRegistryDemo\\ServerInfo"),
EZRegistry::modeRead | EZRegistry::wow64Key64);

CString sIP32 = reg32["IP"];
CString sIP64 = reg64["IP"];
}
catch (EZRegistry::Exception& e)
{
TRACE(_T("Registry Error: %s\n"), e.GetErrorMessage());
}

// ************ Volatile ************
try
{
EZRegistry reg(EZRegistry::currentUser,
_T("SOFTWARE\\EZRegistryDemo\\Volatile"),
EZRegistry::modeWrite | EZRegistry::optVolatile);

reg["Temp"] = 100;
}
catch (EZRegistry::Exception& e)
{
TRACE(_T("Registry Error: %s\n"), e.GetErrorMessage());
}
*/


/*******************************************************************************************************************/
// AxStd::EZRegistry::Binary class
/*******************************************************************************************************************/

AxStd::EZRegistry::Binary::Binary()
	: m_pData(NULL),
	m_nLength(0)
{
}

AxStd::EZRegistry::Binary::Binary(BYTE* pData, int nLength)
	: m_pData(NULL),
	m_nLength(0)
{
	SetData(pData, nLength);
}

AxStd::EZRegistry::Binary::Binary(const Binary& other)
{
	m_nLength = other.m_nLength;
	if (m_nLength > 0)
	{
		m_pData = new BYTE[m_nLength];
		CopyMemory(m_pData, other.m_pData, m_nLength);
	}
	else
		m_pData = NULL;
}

AxStd::EZRegistry::Binary& AxStd::EZRegistry::Binary::operator=(const Binary& other)
{
	if (this != &other)
	{
		delete[] m_pData;

		m_nLength = other.m_nLength;
		if (m_nLength > 0)
		{
			m_pData = new BYTE[m_nLength];
			CopyMemory(m_pData, other.m_pData, m_nLength);
		}
		else
			m_pData = NULL;
	}

	return *this;
}

#if _MSC_VER >= 1600
AxStd::EZRegistry::Binary::Binary(Binary&& other)
{
	m_pData = other.m_pData;
	m_nLength = other.m_nLength;

	other.m_pData = nullptr;
	other.m_nLength = 0;
}

AxStd::EZRegistry::Binary& AxStd::EZRegistry::Binary::operator=(Binary&& other)
{
	if (this != &other)
	{
		delete[] m_pData;

		m_pData = other.m_pData;
		m_nLength = other.m_nLength;

		other.m_pData = nullptr;
		other.m_nLength = 0;
	}
	return *this;
}
#endif

AxStd::EZRegistry::Binary::~Binary()
{
	Clear();
}

void AxStd::EZRegistry::Binary::SetData(BYTE* pData, int nLength)
{
	ASSERT(pData);
	ASSERT(nLength > 0);

	Clear();

	m_nLength = nLength;
	m_pData = new BYTE[nLength];
	CopyMemory(m_pData, pData, nLength);
}

void AxStd::EZRegistry::Binary::FillData(int nLength, BYTE byFill/* = 0*/)
{
	ASSERT(nLength > 0);

	Clear();

	m_nLength = nLength;
	m_pData = new BYTE[nLength];
	FillMemory(m_pData, nLength, byFill);
}

BYTE* AxStd::EZRegistry::Binary::GetData() const
{
	return m_pData;
}

int AxStd::EZRegistry::Binary::GetLength() const
{
	return m_nLength;
}

BOOL AxStd::EZRegistry::Binary::IsEmpty() const
{
	return (NULL == m_pData);
}

void AxStd::EZRegistry::Binary::Clear()
{
	delete[] m_pData;
	m_pData = NULL;
	m_nLength = 0;
}

// AxStd::EZRegistry::Value class
AxStd::EZRegistry::Value::Value(const EZRegistry& rRegistry, LPCTSTR lpValueName)
	: m_rRegistry(rRegistry),
	m_sValueName(lpValueName)
{
	ZeroMemory(&m_nDefaultValue, sizeof(RECT));
}

AxStd::EZRegistry::Value::Value(const Value& other)
	: m_rRegistry(other.m_rRegistry),
	m_sValueName(other.m_sValueName),
	m_sDefaultValue(other.m_sDefaultValue),
	m_rcDefaultValue(other.m_rcDefaultValue)
{
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator=(const Value& other)
{
	if (this != &other)
	{
		ASSERT(&m_rRegistry == &other.m_rRegistry);

		m_sValueName = other.m_sValueName;
		m_sDefaultValue = other.m_sDefaultValue;
		m_rcDefaultValue = other.m_rcDefaultValue;
	}
	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator=(int nValue)
{
	_SetRegValue(REG_DWORD, &nValue, sizeof(int), _T("%d"), nValue);
	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator=(UINT uValue)
{
	_SetRegValue(REG_DWORD, &uValue, sizeof(UINT), _T("%u"), uValue);
	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator=(INT64 n64Value)
{
	_SetRegValue(REG_QWORD, &n64Value, sizeof(INT64), _T("%I64d"), n64Value);
	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator=(UINT64 u64Value)
{
	_SetRegValue(REG_QWORD, &u64Value, sizeof(UINT64), _T("%I64u"), u64Value);
	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator=(bool bValue)
{
	_SetRegValue(REG_BINARY, &bValue, sizeof(bool), _T("%s"), bValue ? _T("true") : _T("false"));
	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator=(double dValue)
{
	_SetRegValue(REG_BINARY, &dValue, sizeof(double), _T("%f"), dValue);
	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator=(LPCTSTR lpValue)
{
	_SetRegValue(REG_SZ, (LPVOID)lpValue, static_cast<int>(_tcslen(lpValue) + 1) * sizeof(TCHAR), _T("%s"), lpValue);
	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator=(POINT ptValue)
{
	_SetRegValue(REG_BINARY, &ptValue, sizeof(POINT), _T("%d,%d"), ptValue.x, ptValue.y);
	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator=(LPCRECT lpRectValue)
{
	_SetRegValue(REG_BINARY, (LPVOID)lpRectValue, sizeof(RECT), _T("%d,%d,%d,%d"),
		lpRectValue->left, lpRectValue->top, lpRectValue->right, lpRectValue->bottom);
	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator=(const COleDateTime& dtValue)
{
	ASSERT(dtValue.GetStatus() == COleDateTime::valid);

	_SetRegValue(REG_BINARY, (LPVOID)&dtValue.m_dt, sizeof(DATE), _T("%s"), dtValue.Format(_T("%Y-%m-%d %H:%M:%S")));
	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator=(const AxStd::EZRegistry::Binary& binaryValue)
{
	ASSERT(!binaryValue.IsEmpty());

	_SetRegValue(REG_BINARY, (LPVOID)binaryValue.GetData(), binaryValue.GetLength(), _T("Binary Value"));
	return *this;
}

AxStd::EZRegistry::Value::operator int()
{
	return _GetRegValue(m_nDefaultValue, REG_DWORD, sizeof(int));
}

AxStd::EZRegistry::Value::operator UINT()
{
	return _GetRegValue(m_uDefaultValue, REG_DWORD, sizeof(UINT));
}

AxStd::EZRegistry::Value::operator INT64()
{
	return _GetRegValue(m_n64DefaultValue, REG_QWORD, sizeof(INT64));
}

AxStd::EZRegistry::Value::operator UINT64()
{
	return _GetRegValue(m_u64DefaultValue, REG_QWORD, sizeof(UINT64));
}

AxStd::EZRegistry::Value::operator bool()
{
	return _GetRegValue(m_bDefaultValue, REG_BINARY, sizeof(bool));
}

AxStd::EZRegistry::Value::operator double()
{
	return _GetRegValue(m_dDefaultValue, REG_BINARY, sizeof(double));
}

AxStd::EZRegistry::Value::operator CString()
{
	return _GetRegValue(m_sDefaultValue, REG_SZ, 0);
}

AxStd::EZRegistry::Value::operator CPoint()
{
	return _GetRegValue(m_ptDefaultValue, REG_BINARY, sizeof(POINT));
}

AxStd::EZRegistry::Value::operator CRect()
{
	return _GetRegValue(m_rcDefaultValue, REG_BINARY, sizeof(RECT));
}

AxStd::EZRegistry::Value::operator COleDateTime()
{
	return _GetRegValue(m_dtDefaultValue, REG_BINARY, sizeof(DATE));
}

AxStd::EZRegistry::Value::operator AxStd::EZRegistry::Binary()
{
	return _GetRegValue(AxStd::EZRegistry::Binary(), REG_BINARY, 0);
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator++()
{
	DWORD dwValueType = _GetRegValueType();
	if (REG_QWORD == dwValueType)
	{
		INT64 n64Value = *this;
		*this = ++n64Value;
	}
	else if (REG_DWORD == dwValueType)
	{
		int nValue = *this;
		*this = ++nValue;
	}
	else
		ASSERT(FALSE);

	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator--()
{
	DWORD dwValueType = _GetRegValueType();
	if (REG_QWORD == dwValueType)
	{
		INT64 n64Value = *this;
		*this = --n64Value;
	}
	else if (REG_DWORD == dwValueType)
	{
		int nValue = *this;
		*this = --nValue;
	}
	else
		ASSERT(FALSE);

	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator+=(int nValue)
{
	ASSERT(REG_DWORD == _GetRegValueType());

	int nRead = *this;
	*this = nRead + nValue;

	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator-=(int nValue)
{
	ASSERT(REG_DWORD == _GetRegValueType());

	int nRead = *this;
	*this = nRead - nValue;
	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator*=(int nValue)
{
	ASSERT(REG_DWORD == _GetRegValueType());

	int nRead = *this;
	*this = nRead * nValue;

	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator/=(int nValue)
{
	ASSERT(REG_DWORD == _GetRegValueType());
	ASSERT(0 != nValue);

	int nRead = *this;
	*this = nRead / nValue;

	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator%=(int nValue)
{
	ASSERT(REG_DWORD == _GetRegValueType());
	ASSERT(0 != nValue);

	int nRead = *this;
	*this = nRead % nValue;

	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator+=(UINT uValue)
{
	ASSERT(REG_DWORD == _GetRegValueType());

	UINT uRead = *this;
	*this = uRead + uValue;

	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator-=(UINT uValue)
{
	ASSERT(REG_DWORD == _GetRegValueType());

	UINT uRead = *this;
	*this = uRead - uValue;

	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator*=(UINT uValue)
{
	ASSERT(REG_DWORD == _GetRegValueType());

	UINT uRead = *this;
	*this = uRead * uValue;

	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator/=(UINT uValue)
{
	ASSERT(REG_DWORD == _GetRegValueType());
	ASSERT(0 != uValue);

	UINT uRead = *this;
	*this = uRead / uValue;

	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator%=(UINT uValue)
{
	ASSERT(REG_DWORD == _GetRegValueType());
	ASSERT(0 != uValue);

	UINT uRead = *this;
	*this = uRead + uValue;

	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator<<=(UINT uValue)
{
	ASSERT(REG_DWORD == _GetRegValueType());

	UINT uRead = *this;
	*this = uRead << uValue;

	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator>>=(UINT uValue)
{
	ASSERT(REG_DWORD == _GetRegValueType());

	UINT uRead = *this;
	*this = uRead >> uValue;

	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator&=(UINT uValue)
{
	ASSERT(REG_DWORD == _GetRegValueType());

	UINT uRead = *this;
	*this = uRead & uValue;

	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator|=(UINT uValue)
{
	ASSERT(REG_DWORD == _GetRegValueType());

	UINT uRead = *this;
	*this = uRead | uValue;

	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator^=(UINT uValue)
{
	ASSERT(REG_DWORD == _GetRegValueType());

	UINT uRead = *this;
	*this = uRead ^ uValue;

	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator+=(INT64 n64Value)
{
	ASSERT(REG_QWORD == _GetRegValueType());

	INT64 n64Read = *this;
	*this = n64Read + n64Value;

	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator-=(INT64 n64Value)
{
	ASSERT(REG_QWORD == _GetRegValueType());

	INT64 n64Read = *this;
	*this = n64Read - n64Value;

	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator*=(INT64 n64Value)
{
	ASSERT(REG_QWORD == _GetRegValueType());

	INT64 n64Read = *this;
	*this = n64Read * n64Value;

	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator/=(INT64 n64Value)
{
	ASSERT(REG_QWORD == _GetRegValueType());
	ASSERT(0 != n64Value);

	INT64 n64Read = *this;
	*this = n64Read / n64Value;

	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator%=(INT64 n64Value)
{
	ASSERT(REG_QWORD == _GetRegValueType());
	ASSERT(0 != n64Value);

	INT64 n64Read = *this;
	*this = n64Read % n64Value;

	return *this;
}


AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator+=(UINT64 u64Value)
{
	ASSERT(REG_QWORD == _GetRegValueType());

	UINT64 u64Read = *this;
	*this = u64Read + u64Value;

	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator-=(UINT64 u64Value)
{
	ASSERT(REG_QWORD == _GetRegValueType());

	UINT64 u64Read = *this;
	*this = u64Read - u64Value;

	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator*=(UINT64 u64Value)
{
	ASSERT(REG_QWORD == _GetRegValueType());

	UINT64 u64Read = *this;
	*this = u64Read * u64Value;

	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator/=(UINT64 u64Value)
{
	ASSERT(REG_QWORD == _GetRegValueType());
	ASSERT(0 != u64Value);

	UINT64 u64Read = *this;
	*this = u64Read / u64Value;

	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator%=(UINT64 u64Value)
{
	ASSERT(REG_QWORD == _GetRegValueType());
	ASSERT(0 != u64Value);

	UINT64 u64Read = *this;
	*this = u64Read % u64Value;

	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator<<=(UINT64 u64Value)
{
	ASSERT(REG_QWORD == _GetRegValueType());

	UINT64 u64Read = *this;
	*this = u64Read << u64Value;

	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator>>=(UINT64 u64Value)
{
	ASSERT(REG_QWORD == _GetRegValueType());

	UINT64 u64Read = *this;
	*this = u64Read >> u64Value;

	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator&=(UINT64 u64Value)
{
	ASSERT(REG_QWORD == _GetRegValueType());

	UINT64 u64Read = *this;
	*this = u64Read & u64Value;

	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator|=(UINT64 u64Value)
{
	ASSERT(REG_QWORD == _GetRegValueType());

	UINT64 u64Read = *this;
	*this = u64Read | u64Value;

	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator^=(UINT64 u64Value)
{
	ASSERT(REG_QWORD == _GetRegValueType());

	UINT64 u64Read = *this;
	*this = u64Read ^ u64Value;

	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator+=(double dValue)
{
	double dRead = *this;
	*this = dRead + dValue;

	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator-=(double dValue)
{
	double dRead = *this;
	*this = dRead - dValue;
	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator*=(double dValue)
{
	double dRead = *this;
	*this = dRead * dValue;

	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator/=(double dValue)
{
	ASSERT(0.0 != dValue);

	double dRead = *this;
	*this = dRead / dValue;

	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator+=(LPCTSTR lpText)
{
	CString sRead = *this;
	*this = sRead + lpText;

	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator+=(POINT ptValue)
{
	int nValueSize = _GetRegValueSize();
	if (sizeof(CPoint) == nValueSize)
	{
		CPoint ptRead = *this;
		*this = ptRead + ptValue;
	}
	else if (sizeof(CRect) == nValueSize)
	{
		CRect rcRead = *this;
		*this = rcRead + ptValue;
	}
	else
		ASSERT(FALSE);

	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator-=(POINT ptValue)
{
	*this += CPoint(-ptValue.x, -ptValue.y);
	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator+=(SIZE sizeValue)
{
	int nValueSize = _GetRegValueSize();
	if (sizeof(CPoint) == nValueSize)
	{
		CPoint ptRead = *this;
		*this = ptRead + sizeValue;
	}
	else if (sizeof(CRect) == nValueSize)
	{
		CRect rcRead = *this;
		*this = rcRead + sizeValue;
	}
	else
		ASSERT(FALSE);

	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator-=(SIZE sizeValue)
{
	*this += CSize(-sizeValue.cx, -sizeValue.cy);
	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator+=(LPCRECT lpRectValue)
{
	CRect rcRead = *this;
	*this = rcRead + lpRectValue;

	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator-=(LPCRECT lpRectValue)
{
	CRect rcRead = *this;
	*this = rcRead - lpRectValue;

	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator+=(COleDateTimeSpan dateSpan)
{
	COleDateTime dtRead = *this;
	*this = dtRead + dateSpan;

	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator-=(COleDateTimeSpan dateSpan)
{
	COleDateTime dtRead = *this;
	*this = dtRead - dateSpan;

	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator<<(int nDefaultValue)
{
	m_nDefaultValue = nDefaultValue;
	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator<<(UINT uDefaultValue)
{
	m_uDefaultValue = uDefaultValue;
	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator<<(INT64 n64DefaultValue)
{
	m_n64DefaultValue = n64DefaultValue;
	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator<<(UINT64 u64DefaultValue)
{
	m_u64DefaultValue = u64DefaultValue;
	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator<<(bool bDefaultValue)
{
	m_bDefaultValue = bDefaultValue;
	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator<<(double dDefaultValue)
{
	m_dDefaultValue = dDefaultValue;
	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator<<(LPCTSTR lpDefaultValue)
{
	m_sDefaultValue = lpDefaultValue;
	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator<<(POINT ptDefaultValue)
{
	m_ptDefaultValue = ptDefaultValue;
	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator<<(LPCRECT lpDefaultValue)
{
	m_rcDefaultValue = *lpDefaultValue;
	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator<<(const COleDateTime& dtDefaultValue)
{
	m_dtDefaultValue = dtDefaultValue;
	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator>>(int nDefaultValue)
{
	m_nDefaultValue = nDefaultValue;
	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator>>(UINT uDefaultValue)
{
	m_uDefaultValue = uDefaultValue;
	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator>>(INT64 n64DefaultValue)
{
	m_n64DefaultValue = n64DefaultValue;
	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator>>(UINT64 u64DefaultValue)
{
	m_u64DefaultValue = u64DefaultValue;
	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator>>(bool bDefaultValue)
{
	m_bDefaultValue = bDefaultValue;
	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator>>(double dDefaultValue)
{
	m_dDefaultValue = dDefaultValue;
	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator>>(LPCTSTR lpDefaultValue)
{
	m_sDefaultValue = lpDefaultValue;
	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator>>(POINT ptDefaultValue)
{
	m_ptDefaultValue = ptDefaultValue;
	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator>>(LPCRECT lpDefaultValue)
{
	m_rcDefaultValue = *lpDefaultValue;
	return *this;
}

AxStd::EZRegistry::Value& AxStd::EZRegistry::Value::operator>>(const COleDateTime& dtDefaultValue)
{
	m_dtDefaultValue = dtDefaultValue;
	return *this;
}

BOOL AxStd::EZRegistry::Value::Exists() const
{
	return (REG_NONE != _GetRegValueType());
}

BOOL AxStd::EZRegistry::Value::Delete()
{
	return (ERROR_SUCCESS == ::RegDeleteValue(m_rRegistry.m_hKey, m_sValueName));
}

void AxStd::EZRegistry::Value::_SetRegValue(DWORD dwType, LPVOID lpValue, int nSize, LPCTSTR lpFormatSpec, ...)
{
	ASSERT(m_rRegistry.m_hKey);
	ASSERT(m_rRegistry.m_nOpenFlags & modeWrite);

	LONG nResult = ::RegSetValueEx(m_rRegistry.m_hKey, m_sValueName, 0, dwType, (const BYTE*)lpValue, nSize);
	if (nResult != ERROR_SUCCESS)
	{
		CString sErrMessage, sFormat;
		sFormat.Format(_T("Failed to write value(%s)."), lpFormatSpec);

		va_list args = NULL;
		va_start(args, lpFormatSpec);
		sErrMessage.FormatV(sFormat, args);
		va_end(args);

		sErrMessage.AppendFormat(_T(" %s"), AxStd::EZRegistry::_GetErrorMessage(nResult));
		throw AxStd::EZRegistry::Exception(nResult, sErrMessage);
	}
}

DWORD AxStd::EZRegistry::Value::_GetRegValueType() const
{
	DWORD dwType = REG_NONE;
	::RegQueryValueEx(m_rRegistry.m_hKey, m_sValueName, NULL, &dwType, NULL, NULL);
	return dwType;
}

int AxStd::EZRegistry::Value::_GetRegValueSize() const
{
	DWORD dwSize = 0;
	::RegQueryValueEx(m_rRegistry.m_hKey, m_sValueName, NULL, NULL, NULL, &dwSize);
	return static_cast<int>(dwSize);
}

// AxStd::EZRegistry::EnumeratedValue class
AxStd::EZRegistry::EnumeratedValue::EnumeratedValue(const EZRegistry& rRegistry, LPCTSTR lpValueName, DWORD dwType, int nSize)
	: AxStd::EZRegistry::Value(rRegistry, lpValueName),
	m_dwType(dwType),
	m_nSize(nSize)
{
}

AxStd::EZRegistry::EnumeratedValue::EnumeratedValue(const EnumeratedValue& other)
	: AxStd::EZRegistry::Value(other),
	m_dwType(other.m_dwType),
	m_nSize(other.m_nSize)
{
}

AxStd::EZRegistry::EnumeratedValue& AxStd::EZRegistry::EnumeratedValue::operator=(const EnumeratedValue& other)
{
	if (this != &other)
	{
		AxStd::EZRegistry::Value::operator=(other);
		m_dwType = other.m_dwType;
		m_nSize = other.m_nSize;
	}
	return *this;
}

AxStd::EZRegistry::EnumeratedValue& AxStd::EZRegistry::EnumeratedValue::operator=(int nValue)
{
	Value::operator=(nValue);
	return *this;
}

AxStd::EZRegistry::EnumeratedValue& AxStd::EZRegistry::EnumeratedValue::operator=(UINT uValue)
{
	Value::operator=(uValue);
	return *this;
}

AxStd::EZRegistry::EnumeratedValue& AxStd::EZRegistry::EnumeratedValue::operator=(INT64 n64Value)
{
	Value::operator=(n64Value);
	return *this;
}

AxStd::EZRegistry::EnumeratedValue& AxStd::EZRegistry::EnumeratedValue::operator=(UINT64 u64Value)
{
	Value::operator=(u64Value);
	return *this;
}

AxStd::EZRegistry::EnumeratedValue& AxStd::EZRegistry::EnumeratedValue::operator=(bool bValue)
{
	Value::operator=(bValue);
	return *this;
}

AxStd::EZRegistry::EnumeratedValue& AxStd::EZRegistry::EnumeratedValue::operator=(double dValue)
{
	Value::operator=(dValue);
	return *this;
}

AxStd::EZRegistry::EnumeratedValue& AxStd::EZRegistry::EnumeratedValue::operator=(LPCTSTR lpValue)
{
	Value::operator=(lpValue);
	return *this;
}

AxStd::EZRegistry::EnumeratedValue& AxStd::EZRegistry::EnumeratedValue::operator=(POINT ptValue)
{
	Value::operator=(ptValue);
	return *this;
}

AxStd::EZRegistry::EnumeratedValue& AxStd::EZRegistry::EnumeratedValue::operator=(LPCRECT lpRectValue)
{
	Value::operator=(lpRectValue);
	return *this;
}

AxStd::EZRegistry::EnumeratedValue& AxStd::EZRegistry::EnumeratedValue::operator=(const COleDateTime& dtValue)
{
	Value::operator=(dtValue);
	return *this;
}

// EZRegistry class
const AxStd::EZRegistry AxStd::EZRegistry::classesRoot(HKEY_CLASSES_ROOT);
const AxStd::EZRegistry AxStd::EZRegistry::currentUser(HKEY_CURRENT_USER);
const AxStd::EZRegistry AxStd::EZRegistry::localMachine(HKEY_LOCAL_MACHINE);
const AxStd::EZRegistry AxStd::EZRegistry::users(HKEY_USERS);
const AxStd::EZRegistry AxStd::EZRegistry::currentConfig(HKEY_CURRENT_CONFIG);

AxStd::EZRegistry::EZRegistry(void)
	: m_hKey(NULL),
	m_nOpenFlags(static_cast<OpenFlags>(0))
{
}

AxStd::EZRegistry::EZRegistry(const EZRegistry& baseKey, LPCTSTR lpSubKey, UINT nOpenFlags /*= modeReadWrite*/)
	: m_hKey(NULL),
	m_nOpenFlags(static_cast<OpenFlags>(0))
{
	LONG nResult = _Open(baseKey, lpSubKey, nOpenFlags);
	if (ERROR_SUCCESS != nResult && !(m_nOpenFlags & AxStd::EZRegistry::optUseDefaultValue))
	{
		CString sErrMessage;
		sErrMessage.Format(_T("Failed to open registry. %s"), AxStd::EZRegistry::_GetErrorMessage(nResult));
		throw AxStd::EZRegistry::Exception(nResult, sErrMessage);
	}
}

AxStd::EZRegistry::~EZRegistry(void)
{
	Close();
}

BOOL AxStd::EZRegistry::Open(const EZRegistry& baseKey, LPCTSTR lpSubKey, UINT nOpenFlags /*= modeReadWrite*/)
{
	return (ERROR_SUCCESS == _Open(baseKey, lpSubKey, nOpenFlags));
}

BOOL AxStd::EZRegistry::IsOpen() const
{
	return (m_hKey != NULL);
}

void AxStd::EZRegistry::Close()
{
	if (IsOpen() && !_IsPredefinedKey(m_hKey))
		::RegCloseKey(m_hKey);

	ClearSubKeyCache();

	m_hKey = NULL;
	m_nOpenFlags = 0;
}

void AxStd::EZRegistry::ClearSubKeyCache()
{
	for (map<CString, EZRegistry*>::iterator it = m_subKeyMap.begin(); it != m_subKeyMap.end(); ++it)
		delete it->second;

	m_subKeyMap.clear();
}

AxStd::EZRegistry::Value AxStd::EZRegistry::operator[](LPCSTR lpValueName)
{
	return Value(*this, CA2T(lpValueName));
}

AxStd::EZRegistry::Value AxStd::EZRegistry::operator[](LPCWSTR lpValueName)
{
	return Value(*this, CW2T(lpValueName));
}

AxStd::EZRegistry& AxStd::EZRegistry::operator+(LPCSTR lpSubKey)
{
	return _OpenSubKey(CA2T(lpSubKey));
}

AxStd::EZRegistry& AxStd::EZRegistry::operator+(LPCWSTR lpSubKey)
{
	return _OpenSubKey(CW2T(lpSubKey));
}

AxStd::EZRegistry& AxStd::EZRegistry::operator()(LPCSTR lpSubKey)
{
	return _OpenSubKey(CA2T(lpSubKey));
}

AxStd::EZRegistry& AxStd::EZRegistry::operator()(LPCWSTR lpSubKey)
{
	return _OpenSubKey(CW2T(lpSubKey));
}

AxStd::EZRegistry& AxStd::EZRegistry::operator+=(LPCSTR lpSubKey)
{
	ASSERT(m_nOpenFlags & modeWrite);

	_OpenSubKey(CA2T(lpSubKey));
	return *this;
}

AxStd::EZRegistry& AxStd::EZRegistry::operator+=(LPCWSTR lpSubKey)
{
	ASSERT(m_nOpenFlags & modeWrite);

	_OpenSubKey(CW2T(lpSubKey));
	return *this;
}

BOOL AxStd::EZRegistry::KeyExists(const EZRegistry& baseKey, LPCTSTR lpSubKey)
{
	HKEY hKey = NULL;
	LONG nResult = ::RegOpenKeyEx(baseKey.m_hKey, lpSubKey, 0, KEY_QUERY_VALUE, &hKey);
	if (hKey)
		::RegCloseKey(hKey);

	return (ERROR_SUCCESS == nResult);
}

BOOL AxStd::EZRegistry::SubKeyExists(LPCTSTR lpSubKey) const
{
	return KeyExists(*this, lpSubKey);
}

BOOL AxStd::EZRegistry::Delete(BOOL bRecursive/* = TRUE*/)
{
	BOOL bResult = DeleteSubKey(_T(""), bRecursive);
	if (bResult)
		Close();

	return bResult;
}

BOOL AxStd::EZRegistry::DeleteSubKey(LPCTSTR lpSubKey, BOOL bRecursive /*= TRUE*/)
{
	LONG nResult;
	if (bRecursive)
		nResult = ::SHDeleteKey(m_hKey, lpSubKey);
	else
		nResult = ::RegDeleteKey(m_hKey, lpSubKey);

	if (ERROR_SUCCESS == nResult)
	{
		map<CString, EZRegistry*>::iterator it = m_subKeyMap.find(lpSubKey);
		if (it != m_subKeyMap.end())
		{
			delete it->second;
			m_subKeyMap.erase(it);
		}
		return TRUE;
	}
	return FALSE;
}

BOOL AxStd::EZRegistry::HasSubKeys() const
{
	TCHAR szKeyName[MAX_PATH];
	DWORD dwKeyNameLen = MAX_PATH;
	return (ERROR_SUCCESS == ::RegEnumKeyEx(m_hKey, 0, szKeyName, &dwKeyNameLen, NULL, NULL, NULL, NULL));
}

BOOL AxStd::EZRegistry::EnumSubKeys(std::vector<CString>& listOfSubKeys) const
{
	if (!IsOpen())
		return FALSE;

	listOfSubKeys.clear();

	TCHAR szKeyName[MAX_PATH];
	DWORD dwKeyNameLen;
	LONG nResult;

	for (DWORD dwIndex = 0;; ++dwIndex)
	{
		dwKeyNameLen = MAX_PATH;
		nResult = ::RegEnumKeyEx(m_hKey, dwIndex, szKeyName, &dwKeyNameLen, NULL, NULL, NULL, NULL);

		if (ERROR_SUCCESS != nResult)
			break;

		listOfSubKeys.push_back(szKeyName);
	}
	return TRUE;
}

BOOL AxStd::EZRegistry::EnumValues(std::vector<EnumeratedValue>& listOfValues) const
{
	if (!IsOpen())
		return FALSE;

	listOfValues.clear();

	TCHAR szValueName[MAX_PATH];
	DWORD dwValueNameLen, dwType, dwSize;
	LONG nResult;
	for (DWORD dwIndex = 0;; ++dwIndex)
	{
		dwValueNameLen = MAX_PATH;
		nResult = ::RegEnumValue(m_hKey, dwIndex, szValueName, &dwValueNameLen, NULL, &dwType, NULL, &dwSize);

		if (ERROR_SUCCESS != nResult)
			break;

		listOfValues.push_back(EnumeratedValue(*this, szValueName, dwType, static_cast<int>(dwSize)));
	}
	return TRUE;
}

LONG AxStd::EZRegistry::_Open(const EZRegistry& baseKey, LPCTSTR lpSubKey, UINT nOpenFlags)
{
	ASSERT((nOpenFlags & modeRead) | (nOpenFlags & modeWrite));
	ASSERT(!((nOpenFlags & wow64Key32) && (nOpenFlags & wow64Key64)));
	ASSERT((nOpenFlags & optVolatile) ? (nOpenFlags & modeWrite) : 1);

	Close();

	m_nOpenFlags = nOpenFlags;

	LONG nResult;
	REGSAM samDesired;
	if (nOpenFlags & modeWrite)
	{
		DWORD dwOptions = (nOpenFlags & optVolatile) ? REG_OPTION_VOLATILE : REG_OPTION_NON_VOLATILE;
		samDesired = (nOpenFlags & modeRead) ? KEY_ALL_ACCESS : KEY_WRITE;
		samDesired |= (nOpenFlags & wow64Key32) ? KEY_WOW64_32KEY : (nOpenFlags & wow64Key64) ? KEY_WOW64_64KEY : 0;
		nResult = ::RegCreateKeyEx(baseKey.m_hKey, lpSubKey, 0, NULL, dwOptions, samDesired, NULL, &m_hKey, NULL);
	}
	else
	{
		samDesired = KEY_READ;
		samDesired |= (nOpenFlags & wow64Key32) ? KEY_WOW64_32KEY : (nOpenFlags & wow64Key64) ? KEY_WOW64_64KEY : 0;
		nResult = ::RegOpenKeyEx(baseKey.m_hKey, lpSubKey, 0, samDesired, &m_hKey);
	}

	return nResult;
}

AxStd::EZRegistry& AxStd::EZRegistry::_OpenSubKey(LPCTSTR lpSubKey)
{
	map<CString, EZRegistry*>::iterator it = m_subKeyMap.find(lpSubKey);
	if (it != m_subKeyMap.end())
	{
		if (it->second->m_hKey)
			return *it->second;
		else
		{
			delete it->second;
			m_subKeyMap.erase(it);
		}
	}

	EZRegistry* pSubRegistry = new EZRegistry(*this, lpSubKey, m_nOpenFlags);
	m_subKeyMap.insert(make_pair(lpSubKey, pSubRegistry));

	return *pSubRegistry;
}

BOOL AxStd::EZRegistry::_IsPredefinedKey(HKEY hKey)
{
	if (HKEY_CLASSES_ROOT == hKey ||
		HKEY_CURRENT_CONFIG == hKey ||
		HKEY_CURRENT_USER == hKey ||
		HKEY_LOCAL_MACHINE == hKey ||
		HKEY_USERS == hKey)
		return TRUE;

	return FALSE;
}

CString AxStd::EZRegistry::_GetErrorMessage(DWORD dwError)
{
	LPVOID lpMessage;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, dwError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMessage, 0, NULL);

	CString sResult((LPTSTR)lpMessage);
	LocalFree(lpMessage);
	sResult.TrimRight(_T(" \t\r\n"));

	return sResult;
}