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

// 임시 최진원
#define dfMaxStringLength 4096
#include "../include/drstring.h"

class CConversion
{
public:
	static int Str2Int(CString& str)
	{
		CDrString::TrimLeft(str);
		CDrString::TrimRight(str);

		if(str.GetLength()>0)
			return atoi(str);
		return 0;
	}

	static long Str2Long(CString& str)
	{
		CDrString::TrimLeft(str);
		CDrString::TrimRight(str);

		if(str.GetLength()>0)
			return atol(str);
		return 0L;//
	}

	static CRect Str2Rect(CString& str)
	{
		CDrString::TrimLeft(str);
		CDrString::TrimRight(str);

		int left,top,right,bottom;
		sscanf(str,"%d,%d,%d,%d", &left, &top, &right, &bottom);
		CRect tmpRect = CRect(left,top,right,bottom);
		return tmpRect;
	}

	static COLORREF Str2RGBColor(CString& str)
	{
		int red,green, blue;
		sscanf(str, "%d,%d,%d", &red, &green, &blue);
	
		COLORREF tmpColor = RGB(red,green,blue);
		return tmpColor;
	}

	// Hi~, char* version.. added by Choi Jin Won(2003.02.20)
	static COLORREF Str2RGBColor(char* pchData)
	{
		int red,green, blue;
		sscanf(pchData, "%d,%d,%d", &red, &green, &blue);
	
		COLORREF tmpColor = RGB(red,green,blue);
		return tmpColor;
	}

	static CString Color2String(COLORREF color)
	{
		CString szReturn;

		szReturn.Format("%d,%d,%d",GetRValue(color),GetGValue(color), GetBValue(color));

		return szReturn;
	}

	static BOOL Str2Boolean(CString& str)
	{
		CDrString::TrimLeft(str);
		CDrString::TrimRight(str);

		if(str == _T("TRUE") || str == _T("1"))
			return TRUE;
		else
			return FALSE;
	}

	static CSize Str2Size(CString& str)
	{
		CDrString::TrimLeft(str);
		CDrString::TrimRight(str);
		
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

	static LOGFONT Str2LogFont(CString& str)
	{
		CDrString::TrimLeft(str);
		CDrString::TrimRight(str);

		char szFontName[LF_FACESIZE] = {0,};
		double size = 10;
		int italic,underline,strikeout,weight;
		italic = underline = strikeout = weight = 0;

		//FontName/Size/Italic/UnderLine/StrikeOut/Weight
		int nFieldCount = sscanf(str,"%[^,],%lf,%d,%d,%d,%d", szFontName,&size,&italic,&underline,&strikeout,&weight);

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
		lf.lfPitchAndFamily = DEFAULT_PITCH; 
		lf.lfQuality = DEFAULT_QUALITY;
		lf.lfCharSet = HANGEUL_CHARSET;                                                
		lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
		lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;

		return lf;

	}

	// "A=B" StringList를 CMapStringToString에 저장하는 함수
	// CDrString 클래스 사용하여 속도 개선...
	// 수정자 : 최진원(2003.02.20)
	static void InitMapStringToString(CStringList* pList, CMapStringToString& map)
	{
		if(pList== NULL) return ;

		CString szOneItem = _T("");
		CString szKey = _T("");
		CString szValue = _T("");		
		for(POSITION pos = pList->GetHeadPosition(); pos != NULL;)
		{
			szOneItem = pList->GetNext(pos);
			int index = index = szOneItem.Find('=');
			if(index < 0) continue;

			CDrString::Left(szOneItem, szKey, index);
			CDrString::Mid(szOneItem, szValue, index+1);

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
		}
	}
	
	// 맵 미사용을 위해서 만든 함수....
	// 현재 사용하고 있지 않습니다.
	// 2003.02.21 최진원
	static BOOL findStringinStringList(CStringList* pList, CString strQuery, int nCurCnt, CString& strResult)
	{
		CString strTemp;
		CString strTemp2;		
		
		POSITION pos = pList->FindIndex(nCurCnt);
		strTemp = pList->GetAt(pos);

		int index = strTemp.Find('=');
		CDrString::Left(strTemp, strTemp2, index);

		if( strQuery != strTemp2 )	return FALSE;
		
		CDrString::Mid(strTemp, strResult, index+1);

		return TRUE;
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

#define CHART_CONVERT_BEGIN_EX(PMAP)	\
	CArray <CString, CString>  strArray;\
	strArray.SetSize(100);\
	CConversion::InitArrayStringToString(PMAP, strArray);\
	int	nArraySize = strArray.GetSize();\
	char szTempEx = NULL;
	
	
#define CHART_CONVERT_END_EX()\
	strArray.RemoveAll();

#define CHART_CONVERT_INT_EX(INDEX,VARIABLE,DEFVAL)\
	szTempEx = NULL;\
	if(INDEX > 0 || INDEX < nArraySize)\
	{\
		szTempEx = strArray[INDEX];\
		VARIABLE = CConversion::Str2Int(szTempEx);\
	}\
	else \
		VARIABLE = DEFVAL;

#define CHART_CONVERT_STRING_EX(INDEX,VARIABLE,DEFVAL)\
	szTempEx = NULL;\
	if(INDEX > 0 || INDEX < nArraySize)\
	{\
		szTempEx = strArray[INDEX];\
		VARIABLE = szTempEx;\
	}\
	else \
		VARIABLE = DEFVAL;

#define CHART_CONVERT_LONG_EX(INDEX,VARIABLE,DEFVAL)\
	szTempEx = NULL;\
	if(INDEX > 0 || INDEX < nArraySize)\
	{\
		szTempEx = strArray[INDEX];\
		VARIABLE = CConversion::Str2Long(szTempEx);\
	}\
	else \
		VARIABLE = DEFVAL;

		
#define CHART_CONVERT_COLOR_EX(INDEX, VARIABLE,DEFVAL)\
	szTempEx = NULL;\
	if(INDEX > 0 || INDEX < nArraySize)\
	{\
		szTempEx = strArray[INDEX];\
		VARIABLE = CConversion::Str2RGBColor(szTempEx);\
	}\
	else \
		VARIABLE = DEFVAL;

#define CHART_CONVERT_BOOL_EX(INDEX,VARIABLE,DEFVAL)\
	szTempEx = NULL;\
	if(INDEX > 0 || INDEX < nArraySize)\
	{\
		szTempEx = strArray[INDEX];\
		VARIABLE = CConversion::Str2Boolean(szTempEx);\
	}\
	else \
		VARIABLE = DEFVAL;

#define CHART_CONVERT_LOGFONT_EX(INDEX,VARIABLE,DEFVAL)\
	szTempEx = NULL;\
	if(INDEX > 0 || INDEX < nArraySize)\
	{\
		szTempEx = strArray[INDEX];\
		VARIABLE = CConversion::Str2LogFont(szTempEx);\
	}\
	else \
		VARIABLE = DEFVAL;


#define CHART_CONVERT_RECT_EX(INDEX,VARIABLE,DEFVAL)\
	szTempEx = NULL;\
	if(INDEX > 0 || INDEX < nArraySize)\
	{\
		szTempEx = strArray[INDEX];\
		VARIABLE = CConversion::Str2Rect(szTempEx);\
	}\
	else \
		VARIABLE = DEFVAL;



//////////////////////////////////////////////////////////////////////
#define CHART_CONVERT_BEGIN()	\
	BOOL bRet = FALSE;\
	char *szTemp = NULL;
	
#define CHART_CONVERT_END()	//\
							//PMAP->RemoveAll();

#define CHART_CONVERT_INT(PMAP,NAME,VARIABLE,DEFVAL)\
	bRet = PMAP->Lookup(NAME,szTemp);\
	if(bRet == TRUE)\
		VARIABLE = atoi(szTemp);\
	else \
		VARIABLE = DEFVAL;

#define CHART_CONVERT_STRING(PMAP,NAME,VARIABLE,DEFVAL)\
	bRet = PMAP->Lookup(NAME,szTemp);\
	if(bRet == TRUE)\
		VARIABLE =  szTemp;\
	else \
		VARIABLE = DEFVAL;

#define CHART_CONVERT_LONG(PMAP, NAME,VARIABLE,DEFVAL)\
	bRet = PMAP->Lookup(NAME,szTemp);\
	if(bRet == TRUE) \
		VARIABLE = atol(szTemp);\
	else \
		VARIABLE = DEFVAL;
		
#define CHART_CONVERT_COLOR(PMAP,NAME, VARIABLE,DEFVAL)\
	bRet = PMAP->Lookup(NAME,szTemp);\
	if(bRet == TRUE) \
		VARIABLE = CConversion::Str2RGBColor( szTemp);\
	else \
		VARIABLE = DEFVAL;

#define CHART_CONVERT_BOOL(PMAP,NAME,VARIABLE,DEFVAL)\
	bRet = PMAP->Lookup(NAME,szTemp);\
	if(bRet == TRUE)\
		VARIABLE = atoi(szTemp);\
	else \
		VARIABLE = DEFVAL;

#define CHART_CONVERT_LOGFONT(PMAP,NAME,VARIABLE,DEFVAL)\
	bRet = PMAP->Lookup(NAME,szTemp);\
	if(bRet == TRUE)\
		VARIABLE = CConversion::Str2LogFont( CString( szTemp));\
	else\
		VARIABLE  = DEFVAL;

#define CHART_CONVERT_RECT(PMAP,NAME,VARIABLE,DEFVAL)\
	bRet = PMAP->Lookup(NAME,szTemp);\
	if(bRet == TRUE)\
		VARIABLE = CConversion::Str2Rect( CString( szTemp));\
	else\
		VARIABLE= DEFVAL;

#define CHART_CONVERT_LOGFONT2(PMAP,NAME,VARIABLE,DEFVAL)\
	bRet = PMAP->Lookup(NAME,szTemp);\
	if(bRet == TRUE)\
		VARIABLE = szTemp;\
	else\
		VARIABLE  = DEFVAL;
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// CMap을 사용하지 않는 경우.. 최진원 2003. 02. 19
// 현재는 사용되고 있지 않은 매크로입니다...
#define CHART_CONVERT2_BEGIN(PMAP, PMAP_CNT)	\
	CString szTemp = NULL;			\
	int x_nListCtn = PMAP_CNT;		\
	BOOL bRet = FALSE;					\
	int x_nCurCnt = 0;
	
#define CHART_CONVERT2_END() ;

#define CHART_CONVERT2_INT(PMAP,NAME,VARIABLE,DEFVAL)	\
	szTemp = NULL;	\
	bRet = CConversion::findStringinStringList(PMAP,NAME,x_nCurCnt,szTemp);	\
	if(bRet == TRUE)\
		VARIABLE = CConversion::Str2Int(szTemp);\
	else \
		VARIABLE = DEFVAL;\
	x_nCurCnt++;

#define CHART_CONVERT2_STRING(PMAP,NAME,VARIABLE,DEFVAL)\
	szTemp = NULL;	\
	bRet = CConversion::findStringinStringList(PMAP,NAME,x_nCurCnt,szTemp);	\
	if(bRet == TRUE)\
		VARIABLE =  szTemp;\
	else \
		VARIABLE = DEFVAL;	\
	x_nCurCnt++;

#define CHART_CONVERT2_LONG(PMAP, NAME,VARIABLE,DEFVAL)\
	szTemp = NULL;	\
	bRet = CConversion::findStringinStringList(PMAP,NAME,x_nCurCnt,szTemp);	\
	if(bRet == TRUE) \
		VARIABLE = CConversion::Str2Long(szTemp);\
	else \
		VARIABLE = DEFVAL;	\
	x_nCurCnt++;
	
		
#define CHART_CONVERT2_COLOR(PMAP,NAME, VARIABLE,DEFVAL)\
	szTemp = NULL;	\
	bRet = CConversion::findStringinStringList(PMAP,NAME,x_nCurCnt,szTemp);	\
	if(bRet == TRUE) \
		VARIABLE = CConversion::Str2RGBColor(szTemp);\
	else \
		VARIABLE = DEFVAL;	\
	x_nCurCnt++;

#define CHART_CONVERT2_BOOL(PMAP,NAME,VARIABLE,DEFVAL)\
	szTemp = NULL;	\
	bRet = CConversion::findStringinStringList(PMAP,NAME,x_nCurCnt,szTemp);	\
	if(bRet == TRUE)\
		VARIABLE = CConversion::Str2Boolean(szTemp);\
	else \
		VARIABLE = DEFVAL;

#define CHART_CONVERT2_LOGFONT(PMAP,NAME,VARIABLE,DEFVAL)\
	szTemp = NULL;	\
	bRet = CConversion::findStringinStringList(PMAP,NAME,x_nCurCnt,szTemp);	\
	if(bRet == TRUE)\
		VARIABLE = CConversion::Str2LogFont(szTemp);\
	else\
		VARIABLE  = DEFVAL;	\
	x_nCurCnt++;

#define CHART_CONVERT2_RECT(PMAP,NAME,VARIABLE,DEFVAL)\
	szTemp = NULL;	\
	bRet = CConversion::findStringinStringList(PMAP,NAME,x_nCurCnt,szTemp);	\
	if(bRet == TRUE)\
		VARIABLE = CConversion::Str2Rect(szTemp);\
	else\
		VARIABLE= DEFVAL;	\
	x_nCurCnt++;


#define CHART_CONVERT2_LOGFONT2(PMAP,NAME,VARIABLE,DEFVAL)\
	szTemp = NULL;	\
	bRet = CConversion::findStringinStringList(PMAP,NAME,x_nCurCnt,szTemp);	\
	if(bRet == TRUE)\
		VARIABLE = szTemp;\
	else\
		VARIABLE  = DEFVAL;	\
	x_nCurCnt++;
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
// 2003. 02 .14 (최진원)
// CAdditionToStringList 
// "A=B" 형태의 스트링으로 포맷팅한 데이터를 입력받은 StringList에 추가한다.
//////////////////////////////////////////////////////////////////////////////
#define cnstMaxCharLength 1024*10 // 100K
class CAdditionToStringList	
{
// constructor & destructor	
public:
	CAdditionToStringList() {  }
	virtual ~CAdditionToStringList() {}

// operation for temporary data setting
public:
	void createTempdata()
	{
		;
	}
	void setTempdataToNULL()
	{
		memset(m_lpstrTemp, NULL , sizeof(m_lpstrTemp));
	}
	void destroyTempdata()
	{
		;
	}
	
// main operation 
public:
	// for "A = integer type"
	void addInteger(CStringList* strlist, CString strLhs, int nRhs, int nDef)
	{
		if( m_lpstrTemp == NULL ) return;
		if( nRhs == nDef ) return;
		
		sprintf(m_lpstrTemp, "%s=%d", (LPCSTR)strLhs, nRhs);
		strlist->AddTail(m_lpstrTemp);

		setTempdataToNULL();
	}
	// for "A = string type"
	void addString(CStringList* strlist, CString strLhs, CString strRhs, CString strDef)
	{
		if( m_lpstrTemp == NULL ) return;
		if( strRhs == strDef ) return;

		int nRhsLength = strRhs.GetLength();

		if( nRhsLength < cnstMaxCharLength )
		{
			sprintf(m_lpstrTemp, "%s=%s", (LPCSTR)strLhs, (LPCSTR)strRhs);
			strlist->AddTail(m_lpstrTemp);
		}
		else
		{
			CString strTemp;
			strTemp.Format("%s=%s", (LPCSTR)strLhs, (LPCSTR)strRhs);
			strlist->AddTail(strTemp);
		}

		setTempdataToNULL();
	}
	// for "A = rect type"
	void addRect(CStringList* strlist, CString strLhs, CRect rectRhs, CRect rectDef)
	{
		if( m_lpstrTemp == NULL ) return;
		if( rectRhs == rectDef ) return;
		
		sprintf(m_lpstrTemp, "%s=%d,%d,%d,%d", strLhs, rectRhs.left, rectRhs.top, rectRhs.right, rectRhs.bottom);
		strlist->AddTail(m_lpstrTemp);

		setTempdataToNULL();
	}
	// for "A = color type"
	void addColor(CStringList* strlist, CString strLhs, COLORREF colorRhs, COLORREF colorDef)
	{
		if( m_lpstrTemp == NULL ) return;
		if( colorRhs == colorDef ) return;
		
		sprintf(m_lpstrTemp, "%s=%d,%d,%d", (LPCSTR)strLhs, GetRValue(colorRhs), GetGValue(colorRhs), GetBValue(colorRhs));
		strlist->AddTail(m_lpstrTemp);

		setTempdataToNULL();		
	}
	// font 부분이 좀 이상하다... 확실한 이해가 필요하다.
	void addFont(CStringList* strlist, CString strLhs, LOGFONT fontRhs, LOGFONT fontDef)
	{
		if( m_lpstrTemp == NULL ) return;
		if( memcmp(&fontRhs, &fontDef, sizeof(LOGFONT)) != 0 ) return;

		CClientDC dc(NULL);
		LONG lSize;
		lSize = MulDiv(72, fontRhs.lfHeight, dc.GetDeviceCaps(LOGPIXELSY));

		sprintf(m_lpstrTemp, "%s,%lf,%d,%d,%d,%d", fontRhs.lfFaceName, lSize, fontRhs.lfItalic,
											fontRhs.lfUnderline, fontRhs.lfStrikeOut, fontRhs.lfWeight);
		strlist->AddTail(m_lpstrTemp);

		setTempdataToNULL();
	}
	
// attribute
private:
	char m_lpstrTemp[cnstMaxCharLength];
};
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// CHART_ADDTOSTRLIST series											//
// (2003.2.14) 최진원												//
// 한 함수에서 START와 END는 한번만 사용하자...						//
// 컴포넌트의 프로퍼티를 StringList에 넣는다.						//
//////////////////////////////////////////////////////////////////////
#define CHART_ADDTOSTRLIST_START( bWithFull)	\
	CString strData;						\
	BOOL	bFull = bWithFull;

#define CHART_ADDTOSTRLIST_END	

#define CHART_ADDTOSTRLIST_BOOL(PMAP,NAME,VARIABLE, DEFVAL)	\
	if( bFull || VARIABLE != DEFVAL) PMAP->SetAt( NAME, VARIABLE ? "1" : "0");

#define CHART_ADDTOSTRLIST_INT(PMAP,NAME,VARIABLE, DEFVAL)	\
	if( bFull || VARIABLE != DEFVAL)								\
	{													\
		strData.Format( "%d", VARIABLE);				\
		PMAP->SetAt( NAME, strData);					\
	}

#define CHART_ADDTOSTRLIST_STRING(PMAP,NAME,VARIABLE, DEFVAL)	\
	if( bFull || strcmp( VARIABLE, DEFVAL)) PMAP->SetAt( NAME, VARIABLE);

#define CHART_ADDTOSTRLIST_LONG(PMAP,NAME,VARIABLE, DEFVAL)	\
	if( bFull || VARIABLE != DEFVAL)								\
	{													\
		strData.Format( "%ld", VARIABLE);				\
		PMAP->SetAt( NAME, strData);					\
	}

#define CHART_ADDTOSTRLIST_COLOR(PMAP,NAME,VARIABLE, DEFVAL)												\
	if( bFull || VARIABLE != DEFVAL)																				\
	{																									\
		strData.Format( "%d,%d,%d", GetRValue( VARIABLE), GetGValue( VARIABLE), GetBValue( VARIABLE));	\
		PMAP->SetAt( NAME, strData);																	\
	}

#define CHART_ADDTOSTRLIST_LOGFONT(PMAP,NAME,VARIABLE, DEFVAL)										\
	if( bFull || memcmp( &VARIABLE, &DEFVAL, sizeof(LOGFONT)) != 0 )										\
	{																							\
		CClientDC dc(NULL);																		\
		LONG lSize;																				\
		lSize = MulDiv(72, VARIABLE.lfHeight, dc.GetDeviceCaps(LOGPIXELSY));					\
		strData.Format( "%s,%lf,%d,%d,%d,%d", VARIABLE.lfFaceName, lSize,						\
			VARIABLE.lfItalic, VARIABLE.lfUnderline, VARIABLE.lfStrikeOut, VARIABLE.lfWeight);	\
		PMAP->SetAt( NAME, strData);															\
	}

#define CHART_ADDTOSTRLIST_RECT(PMAP,NAME,VARIABLE, DEFVAL)												\
	if( bFull || VARIABLE != DEFVAL)																				\
	{																									\
		strData.Format( "%d,%d,%d,%d", VARIABLE.left, VARIABLE.top, VARIABLE.right, VARIABLE.bottom);	\
		PMAP->SetAt( NAME, strData);																	\
	}

#define CHART_ADDTOSTRLIST_LOGFONT2(PMAP,NAME,VARIABLE, DEFVAL)	\
	if( bFull || strcmp( VARIABLE, DEFVAL)) PMAP->SetAt( NAME, VARIABLE);

//////////////////////////////////////////////////////////////////////



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
