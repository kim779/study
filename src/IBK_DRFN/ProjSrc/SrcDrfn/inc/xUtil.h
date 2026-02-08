#if !defined(AFX_XUTIL_H__8F6C92A1_58B1_11D4_B316_0048543A0D2E__INCLUDED_)
#define AFX_XUTIL_H__8F6C92A1_58B1_11D4_B316_0048543A0D2E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDesk.h : header file
//
#ifndef __AFXTEMPL_H__
#include <afxtempl.h>
#endif

//#include "../../include/xstring.h"
#include <d3dtypes.h>		//For RGB_MAKE
#include <ComDef.h>

class CConversion;
class CUtilConvert;

class CXFLog;
class CXTrace;


#define		XCONTROL_UNKNOWN		0
#define		XCONTROL_XINPUT			100
#define		XCONTROL_XGRID			200
#define		XCONTROL_XTABLE			300

#define		XVARTYPE_NONE			0
#define		XVARTYPE_BOOL			1
#define		XVARTYPE_STRING			2
#define		XVARTYPE_RECT			3
#define		XVARTYPE_COLOR			4
#define		XVARTYPE_FONT			5

/*
typedef struct 
{
	char*  aKey;
} STKEYWORD1, *LPSTKEYWORD1;

typedef struct 
{
	int	   nValue;
} STKEYWORD2, *LPSTKEYWORD2;

extern STKEYWORD1  g_resKey1[];
extern STKEYWORD1  g_resKey2[];
extern STKEYWORD2  g_varType[];
*/



class CXString
{
	DECLARE_DYNAMIC(CXString)

public:
	// 문자 트림
	static void TrimAll(CString&);

	// 문자 Parsing
	// 두번째 스트링을 " \t\n\r"을 기준으로 나눈다. 
	// 기준을 중심으로 앞부분은 앞스트링에 뒷부분은 두번째 스트링에 결과 처리된다.
	static void	GetWord( LPSTR, LPSTR);
	static void GetWord(CString& pWord, CString& pLine);
	static int	GetString( CString&, CString&, LPCSTR strStop=" \t\n\r" );
	static int	ReadString(CString& szRead, CString& szOrg, BOOL bSplit=TRUE);

	// 문자 처리
	// nIndex -1:All , 0: noremove , 몇번째 단어가 제거 될것인지
	static int		Remove(CString &strIn, TCHAR ch, int nIndex=1); 
	static int		RemoveAll(CString &strIn, TCHAR ch=' ');
	static char*	RemoveFront(LPCSTR, TCHAR);

	static int		GetCharCount(LPCSTR _szData, TCHAR ch);

	// 숫자 처리
	static BOOL	ValidNumber(LPCSTR strIn);
	static BOOL	IsNumber(UINT nChar);
	static BOOL	IsOnlyNumber( LPCTSTR szData);

	static CString	GetComma(LPCSTR szData);
	static CString  GetOnlyNumber( LPCSTR szData );
	static CString  GetNumber( LPCSTR szData );
};

/////////////////////////////////////////////////////////////////////////////
// 
class CConversion
{
public:

public:
	static int Str2Int(CString str)
	{
		str.TrimLeft();
		str.TrimRight();

		if(str.GetLength()>0)
			return atoi(str);
		return 0;
	}

	static long Str2Long(CString str)
	{
		str.TrimLeft();
		str.TrimRight();

		if(str.GetLength()>0)
			return atol(str);
		return 0L;
	}

	static CRect Str2Rect(CString str)
	{
		str.TrimLeft();
		str.TrimRight();

		int left,top,right,bottom;
		sscanf(str,"%d,%d,%d,%d", &left, &top, &right, &bottom);
		CRect tmpRect = CRect(left,top,right,bottom);
		return tmpRect;
	}

	static COLORREF Str2RGBColor(CString str)
	{
		str.TrimLeft();
		str.TrimRight();

		int red,green, blue;
		sscanf(str, "%d,%d,%d", &red, &green, &blue);
	
		COLORREF tmpColor = RGB(red,green,blue);
		return tmpColor;
	}

	static CString Color2String(COLORREF color)
	{
		CString szReturn;

		szReturn.Format("%d,%d,%d",GetRValue(color),GetGValue(color), GetBValue(color));

		return szReturn;
	}

	static BOOL Str2Boolean(CString str)
	{
		str.TrimLeft();
		str.TrimRight();

		if(str == _T("TRUE") || str == _T("1"))
			return TRUE;
		else
			return FALSE;
	}

	static CSize Str2Size(CString str)
	{
		str.TrimLeft();
		str.TrimRight();
		CSize size(0,0);
		

		int cx,cy;
		int nRet = sscanf(str, "%d,%d", &cx, &cy);

		if(nRet == 2)
			size = CSize(cx,cy);
		return size;


	}

	static void String2Olechar(LPCSTR _szFrom, OLECHAR& szTo, int nLen)
	{
		MultiByteToWideChar(CP_ACP, 0, (char*)_szFrom, -1, &szTo, nLen);
	}

	static LOGFONT Str2LogFont(CString str)
	{
		str.TrimLeft();
		str.TrimRight();

		char szFontName[LF_FACESIZE] = {0,};
		double size = 10;
		int italic,underline,strikeout,weight;
		italic = underline = strikeout = weight = 0;

		//FontName/Size/Italic/UnderLine/StrikeOut/Weight
//		int nFieldCount = sscanf(str,"%[^,],%d,%d,%d,%d,%d",
		int nFieldCount = sscanf(str,"%[^,],%lf,%d,%d,%d,%d",
									szFontName,&size,&italic,&underline,&strikeout,&weight);

//		TRACE("FontInfo[%s] --> FontName[%s],Height[%f],Italic[%d], UnderLine[%d], StrikeOut[%d], Weight[%d]\n",
//				str,szFontName, size, italic, underline, strikeout,weight);
		CClientDC dc(NULL);
		LOGFONT lf;
		lstrcpy(lf.lfFaceName, (LPCTSTR)szFontName);
		lf.lfHeight = MulDiv((int)size, dc.GetDeviceCaps(LOGPIXELSY), 72);
		lf.lfWidth = 0;
		lf.lfWeight = weight;
		
		lf.lfItalic = italic;
		lf.lfUnderline = underline;
		lf.lfStrikeOut = strikeout;

		lf.lfEscapement = 0;
		lf.lfOrientation = 0;
		//lf.lfPitchAndFamily = FIXED_PITCH; 
		lf.lfPitchAndFamily = DEFAULT_PITCH; 
		lf.lfQuality = DEFAULT_QUALITY;
		lf.lfCharSet = HANGEUL_CHARSET;                                                
		lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
		lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;

		return lf;

	}

	static void InitMapStringToString(CStringList* pList, CMapStringToString& map)
	{

		if(pList== NULL) return ;
		
		for(POSITION pos = pList->GetHeadPosition(); pos != NULL;)
		{
			CString szOneItem = _T("");
			CString szKey = _T("");
			CString szValue = _T("");

			szOneItem = pList->GetNext(pos);
			int index = index = szOneItem.Find('=');
			if(index < 0) continue;
			szKey = szOneItem.Left(index);
			szValue = szOneItem.Mid(index+1);

			map[szKey] = szValue;
		}
			
	}

	static void InitArrayStringToString(CStringList* pList, CArray <CString, CString>& array)
	{

		if(pList== NULL) return ;
		
		for(POSITION pos = pList->GetHeadPosition(); pos != NULL;)
		{
			CString szOneItem = _T("");
			CString szKey = _T("");
			CString szValue = _T("");
			int indexArray = 0;

			szOneItem = pList->GetNext(pos);
			int index = szOneItem.Find('=');
			if(index < 0) continue;
			szKey = szOneItem.Left(index);
			szValue = szOneItem.Mid(index+1);

			indexArray = atoi(szKey);
			array[indexArray] = szValue;

//			map[szKey] = szValue;
		}
			
	}





};


class CIniFileMgr
{
public:

public:
	static COLORREF GetColor(LPCTSTR lpAppName,       // section name
							 LPCTSTR lpKeyName,        // key name
							 LPCTSTR lpDefault,        // default string
							 LPCTSTR lpFileName)       // initialization file name
	{
		COLORREF color;
		char tmp[128];
		::GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,tmp,32,lpFileName);
		color=CConversion::Str2RGBColor(tmp);
		return color;
	}

	static CString GetString(LPCTSTR lpAppName,  // section name
							 LPCTSTR lpKeyName,  // key name
						     LPCTSTR lpDefault,  // default string
							 DWORD	 nSize,		 //size of destination buffer
						     LPCTSTR lpFileName) // initialization file name
	{
		CString szRet;
		char tmp[128];
		::GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,tmp,nSize,lpFileName);

		szRet.Format("%s",tmp);
		return szRet;

	}

	static int GetInt(  LPCTSTR lpAppName,  // section name
						LPCTSTR lpKeyName,  // key name
						INT nDefault,       // return value if key name not found
						LPCTSTR lpFileName)  // initialization file name
	{
		int value = ::GetPrivateProfileInt(lpAppName,lpKeyName,nDefault,lpFileName);
		return value;
	}
		

};


///////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////
//Start EX

#define NEO_CONVERT_BEGIN_EX(STR_LIST)	\
	CArray <CString, CString>  strArray;\
	strArray.SetSize(100);\
	CConversion::InitArrayStringToString(STR_LIST, strArray);\
	int	nArraySize = strArray.GetSize();\
	CString szTempEx = _T("");
	
	
#define NEO_CONVERT_END_EX()\
	strArray.RemoveAll();

#define NEO_CONVERT_INT_EX(INDEX,VARIABLE,DEFVAL)\
	szTempEx = _T("");\
	if(INDEX > 0 || INDEX < nArraySize)\
	{\
		szTempEx = strArray[INDEX];\
		VARIABLE = CConversion::Str2Int(szTempEx);\
	}\
	else \
		VARIABLE = DEFVAL;

#define NEO_CONVERT_STRING_EX(INDEX,VARIABLE,DEFVAL)\
	szTempEx = _T("");\
	if(INDEX > 0 || INDEX < nArraySize)\
	{\
		szTempEx = strArray[INDEX];\
		VARIABLE = szTempEx;\
	}\
	else \
		VARIABLE = DEFVAL;

#define NEO_CONVERT_LONG_EX(INDEX,VARIABLE,DEFVAL)\
	szTempEx = _T("");\
	if(INDEX > 0 || INDEX < nArraySize)\
	{\
		szTempEx = strArray[INDEX];\
		VARIABLE = CConversion::Str2Long(szTempEx);\
	}\
	else \
		VARIABLE = DEFVAL;

		
#define NEO_CONVERT_COLOR_EX(INDEX, VARIABLE,DEFVAL)\
	szTempEx = _T("");\
	if(INDEX > 0 || INDEX < nArraySize)\
	{\
		szTempEx = strArray[INDEX];\
		VARIABLE = CConversion::Str2RGBColor(szTempEx);\
	}\
	else \
		VARIABLE = DEFVAL;

#define NEO_CONVERT_BOOL_EX(INDEX,VARIABLE,DEFVAL)\
	szTempEx = _T("");\
	if(INDEX > 0 || INDEX < nArraySize)\
	{\
		szTempEx = strArray[INDEX];\
		VARIABLE = CConversion::Str2Boolean(szTempEx);\
	}\
	else \
		VARIABLE = DEFVAL;

#define NEO_CONVERT_LOGFONT_EX(INDEX,VARIABLE,DEFVAL)\
	szTempEx = _T("");\
	if(INDEX > 0 || INDEX < nArraySize)\
	{\
		szTempEx = strArray[INDEX];\
		VARIABLE = CConversion::Str2LogFont(szTempEx);\
	}\
	else \
		VARIABLE = DEFVAL;


#define NEO_CONVERT_RECT_EX(INDEX,VARIABLE,DEFVAL)\
	szTempEx = _T("");\
	if(INDEX > 0 || INDEX < nArraySize)\
	{\
		szTempEx = strArray[INDEX];\
		VARIABLE = CConversion::Str2Rect(szTempEx);\
	}\
	else \
		VARIABLE = DEFVAL;


				//
///////////////////


//#define NEO_CONVERT_BEGIN()	
#define NEO_CONVERT_BEGIN(STR_LIST)	\
	CMapStringToString map;\
	CConversion::InitMapStringToString(STR_LIST, map);\
	BOOL bRet = FALSE;\
	CString szTemp = _T("");
	
#define NEO_CONVERT_END()\
	map.RemoveAll();

#define NEO_CONVERT_INT(STR_LIST,NAME,VARIABLE,DEFVAL)\
	szTemp = _T("");\
	bRet = map.Lookup(NAME,szTemp);\
	if(bRet == TRUE)\
		VARIABLE = CConversion::Str2Int(szTemp);\
	else \
		VARIABLE = DEFVAL;\

#define NEO_CONVERT_STRING(STR_LIST,NAME,VARIABLE,DEFVAL)\
	szTemp = _T("");\
	bRet = map.Lookup(NAME,szTemp);\
	if(bRet == TRUE)\
		VARIABLE =  szTemp;\
	else \
		VARIABLE = DEFVAL;

#define NEO_CONVERT_LONG(STR_LIST, NAME,VARIABLE,DEFVAL)\
	szTemp = _T("");\
	bRet = map.Lookup(NAME,szTemp);\
	if(bRet == TRUE) \
		VARIABLE = CConversion::Str2Long(szTemp);\
	else \
		VARIABLE = DEFVAL;
	
		
#define NEO_CONVERT_COLOR(STR_LIST,NAME, VARIABLE,DEFVAL)\
	szTemp = _T("");\
	bRet = map.Lookup(NAME,szTemp);\
	if(bRet == TRUE) \
		VARIABLE = CConversion::Str2RGBColor(szTemp);\
	else \
		VARIABLE = DEFVAL;

#define NEO_CONVERT_BOOL(STR_LIST,NAME,VARIABLE,DEFVAL)\
	szTemp = _T("");\
	bRet = map.Lookup(NAME,szTemp);\
	if(bRet == TRUE)\
		VARIABLE = CConversion::Str2Boolean(szTemp);\
	else \
		VARIABLE = DEFVAL;

#define NEO_CONVERT_LOGFONT(STR_LIST,NAME,VARIABLE,DEFVAL)\
	szTemp = _T("");\
	bRet = map.Lookup(NAME,szTemp);\
	if(bRet == TRUE)\
		VARIABLE = CConversion::Str2LogFont(szTemp);\
	else\
		VARIABLE  = DEFVAL;

#define NEO_CONVERT_RECT(STR_LIST,NAME,VARIABLE,DEFVAL)\
	szTemp = _T("");\
	bRet = map.Lookup(NAME,szTemp);\
	if(bRet == TRUE)\
		VARIABLE = CConversion::Str2Rect(szTemp);\
	else\
		VARIABLE= DEFVAL;


#define NEO_CONVERT_LOGFONT2(STR_LIST,NAME,VARIABLE,DEFVAL)\
	szTemp = _T("");\
	bRet = map.Lookup(NAME,szTemp);\
	if(bRet == TRUE)\
		VARIABLE = szTemp;\
	else\
		VARIABLE  = DEFVAL;
/*
#define NEO_CONVERT_END()\
	;

#define NEO_CONVERT_INT(STR_LIST,NAME,VARIABLE,DEFVAL)\
	szTemp = _T("");\
	bRet = 
	bRet = CConversion::FindValue(STR_LIST,NAME,szTemp);\
	if(bRet == TRUE)\
		VARIABLE = CConversion::Str2Int(szTemp);\
	else \
		VARIABLE = DEFVAL;\

#define NEO_CONVERT_STRING(STR_LIST,NAME,VARIABLE,DEFVAL)\
	szTemp = _T("");\
	bRet = CConversion::FindValue(STR_LIST,NAME,szTemp);\
	if(bRet == TRUE)\
		VARIABLE =  szTemp;\
	else \
		VARIABLE = DEFVAL;

#define NEO_CONVERT_LONG(STR_LIST, NAME,VARIABLE,DEFVAL)\
	szTemp = _T("");\
	bRet = CConversion::FindValue(STR_LIST,NAME,szTemp);\
	if(bRet == TRUE) \
		VARIABLE = CConversion::Str2Long(szTemp);\
	else \
		VARIABLE = DEFVAL;
	
		
#define NEO_CONVERT_COLOR(STR_LIST,NAME, VARIABLE,DEFVAL)\
	szTemp = _T("");\
	bRet = CConversion::FindValue(STR_LIST,NAME,szTemp);\
	if(bRet == TRUE) \
		VARIABLE = CConversion::Str2RGBColor(szTemp);\
	else \
		VARIABLE = DEFVAL;

#define NEO_CONVERT_BOOL(STR_LIST,NAME,VARIABLE,DEFVAL)\
	szTemp = _T("");\
	bRet = CConversion::FindValue(STR_LIST,NAME,szTemp);\
	if(bRet == TRUE)\
		VARIABLE = CConversion::Str2Boolean(szTemp);\
	else \
		VARIABLE = DEFVAL;

#define NEO_CONVERT_LOGFONT(STR_LIST,NAME,VARIABLE,DEFVAL)\
	szTemp = _T("");\
	bRet = CConversion::FindValue(STR_LIST,NAME,szTemp);\
	if(bRet == TRUE)\
		VARIABLE = CConversion::Str2LogFont(szTemp);\
	else\
		VARIABLE  = DEFVAL;


  */



	

	

	




///////////////////////
//



#define XCONVERT_BEGIN()	\
		/* TRACE("XCONVERT_BEGIN\n");*/	\
		CString tmpString;

#define XCONVERT_END()	\
		/* TRACE("XCONVERT_END\n");*/;

#define XCONVERT_INT(theMap, XXXX, YYYY, DEFXXX) \
		tmpString.Empty();	\
		if(theMap.Lookup(XXXX,tmpString))	\
			YYYY = CConversion::Str2Int(tmpString);	\
		else YYYY = DEFXXX;


#define XCONVERT_STRING(theMap, XXXX,YYYY, DEFXXX) \
		tmpString.Empty();	\
		if(theMap.Lookup(XXXX,tmpString)) YYYY = tmpString;	\
		else YYYY = DEFXXX;

#define XCONVERT_LONG(theMap, XXXX,YYYY, DEFXXX)	\
		tmpString.Empty();	\
		if(theMap.Lookup(XXXX,tmpString)) YYYY = CConversion::Str2Long(tmpString);	\
		else YYYY = DEFXXX;

#define XCONVERT_RECT(theMap, XXXX,YYYY, DEFXXX)	\
		tmpString.Empty();	\
		if(theMap.Lookup(XXXX,tmpString))	YYYY = CConversion::Str2Rect(tmpString);	\
		else YYYY = DEFXXX;

#define XCONVERT_COLOR(theMap, XXXX,YYYY, DEFXXX)	\
		tmpString.Empty();	\
		if(theMap.Lookup(XXXX,tmpString))	YYYY = CConversion::Str2RGBColor(tmpString);	\
		else YYYY = DEFXXX;

#define XCONVERT_BOOL(theMap, XXXX,YYYY, DEFXXX)	\
		tmpString.Empty();	\
		if(theMap.Lookup(XXXX,tmpString))	YYYY = CConversion::Str2Boolean(tmpString);	\
		else YYYY = DEFXXX;

class CUtilConvert
{
public:

public:
	static BOOL ParseComponent(CString& szValue);
	static BOOL ParseWsStyle(LPCSTR _szValue, DWORD& _dwStyle);
	static BOOL ParseWsExStyle(LPCSTR _szValue, DWORD& _dwExStyle);

	static int	 GetCharCount(LPCSTR _szData, char ch);
};

class CWLCInfo
{
public:
	CString m_szWindowName;
	CRect	m_rect;
	UINT	m_nID;
	DWORD	m_dwStyle;
	DWORD	m_dwExStyle;
};

class CXTrace
{
public:
	CXTrace()
	{
		m_nType = xt_DEBUG;
	}

	CXTrace(int nType)
	{
		m_nType = nType;
	}


	void Trace(LPCSTR szMsg)
	{
		if(m_nType & xt_DEBUG)
			TRACE("%s\n", szMsg);
		if(m_nType & xt_FILE) {
			int nSwitch = AfxGetApp()->GetProfileInt("DEBUG", "Switch", 0);
			if(nSwitch=222) {
				CString szFile = AfxGetApp()->GetProfileString("DEBUG", "XDebug", "");
				if(szFile.GetLength()>0) {
					FILE *fp = fopen(szFile, "at");
					if(fp) {
						fprintf(fp, "%s\n", szMsg);
						fclose(fp);
					}
				}
			}
		}
	}

	int		m_nType;
	enum {
		xt_DEBUG=1,
		xt_FILE=2,
	} xtrace_Type;
};

class CXFLog
{
public:
	static void Trace(LPCTSTR lpszFormat, ...)
	{
		if(!lpszFormat)
			return;

		va_list args;
		va_start(args, lpszFormat);

		int nBuf;
		TCHAR szBuffer[512];

		nBuf = _vsntprintf(szBuffer, sizeof(szBuffer), lpszFormat, args);

		// was there an error? was the expanded string too long?
		ASSERT(nBuf >= 0);

		va_end(args);

		CString szMsg = szBuffer;

		CXTrace xTrace(CXTrace::xt_FILE);
		xTrace.Trace(szMsg);
	}
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XUTIL_H__8F6C92A1_58B1_11D4_B316_0048543A0D2E__INCLUDED_)
