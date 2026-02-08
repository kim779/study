#if !defined(AFX_INTERFACE_OF_DATAUTIL_1464_45BB_BY_JUNOK_LEE__INCLUDED_)
#define AFX_INTERFACE_OF_DATAUTIL_1464_45BB_BY_JUNOK_LEE__INCLUDED_

// #include "../../_include/IDataUtilMng.h"
#include "IBaseDefine.h"
//  int nID = 24;
// 	IDataUtilManager* pManager = (IDataUtilManager*)AfxGetMainWnd()->SendMessage(RMSG_INTERFACE, nID, 0);
//	IDataUtilManager* pManager = (IDataUtilManager*)AfxGetPctrInterface(nID);

#define		UUID_IDataUtilManager			24
#define		UUID_IDataUtilManager2			24
#define		UUID_IDataUtilManagerLast		24
#define		IDataUtilManagerLast			IDataUtilManager2

DECLARE_INTERFACE(IDataUtilManager)
{
	// 소수점 형식으로
	STDMETHOD_(CString, GetFutureString)(CString strData, BOOL bJisu) PURE;
	STDMETHOD_(void, AddDashToAccount)(LPCSTR _lpIn, CString& szOut) PURE;
	STDMETHOD_(CString, GetSignSimbol)(int sign) PURE;
	STDMETHOD_(BOOL, GetRegKey)(LPCSTR szSection, LPCSTR szKey, CString& rData) PURE;
	STDMETHOD_(CString, AdjustMask)(CString mask, int nValue) PURE;
	STDMETHOD_(CString, AdjustMask)(CString mask,CString str) PURE;
	STDMETHOD_(void, TrimRight)(char* pIn) PURE;
	STDMETHOD_(void, RemoveCharAll)(CString& szData, TCHAR ch) PURE;
	STDMETHOD_(BOOL, ValidNumber)(LPCSTR strIn) PURE;
	STDMETHOD_(BOOL, IsSign)(LPCSTR pData) PURE;
	STDMETHOD_(void, GetWord)( LPSTR, LPSTR, TCHAR ) PURE;
	STDMETHOD_(int, GetString)( CString&, CString&, TCHAR ) PURE;
	STDMETHOD_(BOOL, IsNumber)(UINT nChar) PURE;
	STDMETHOD_(BOOL, IsOnlyNumber)( LPCTSTR szData) PURE;
	STDMETHOD_(int, Remove)(CString &strIn, TCHAR ch, int nIndex=1) PURE; 

	// nIndex -1:All , 0: noremove , 몇번째 단어가 제거 될것인지
	STDMETHOD_(int, RemoveAll)(CString &strIn, TCHAR ch=' ') PURE;
	STDMETHOD_(char*, RemoveFront)(LPCSTR, TCHAR) PURE;
	STDMETHOD_(CString, GetComma)(LPCSTR szData) PURE;
	STDMETHOD_(CString, GetOnlyNumber)( LPCSTR szData ) PURE;
	STDMETHOD_(CString, GetNumber)(LPCSTR szData ) PURE;
	STDMETHOD_(CString, GetMakeText)(TCHAR ch, int nCnt) PURE;
	STDMETHOD_(CString, SetLengthString)(LPCTSTR pData, int nCnt) PURE;
	STDMETHOD_(CString, RemoveChar)(LPCSTR pData, TCHAR ch) PURE;

	// {{ -- 추가 2005.12.03(Sat)
	STDMETHOD_(int, Str2Int)(CString& str) PURE;
	STDMETHOD_(long, Str2Long)(CString& str) PURE;
	STDMETHOD_(CRect, Str2Rect)(CString& str) PURE;
	STDMETHOD_(COLORREF, Str2RGBColor)(CString& str) PURE;
	STDMETHOD_(COLORREF, Str2RGBColor)(char* pchData) PURE;
	STDMETHOD_(CString, Color2String)(COLORREF color) PURE;
	STDMETHOD_(BOOL, Str2Boolean)(CString& str) PURE;
	STDMETHOD_(CSize, Str2Size)(CString& str) PURE;
	STDMETHOD_(void, String2Olechar)(LPCSTR _szFrom, OLECHAR& szTo, int nLen) PURE;
	STDMETHOD_(LOGFONT, Str2LogFont)(CString& str) PURE;
	// }} -- 추가 2005.12.03(Sat)
	
	STDMETHOD_(CString, ConvertIntToPoint)(CString strData, int nPointPosition, int nPointCurPosition=-1) PURE;
	STDMETHOD_(CString, ConvertPointToInt)(CString strData,int nPointPositoin) PURE;
	STDMETHOD_(void, InsertComma)(CString & str1) PURE;
	STDMETHOD_(int, chars_atoi)(LPSTR lpstrSrc, UINT uSize) PURE;
	STDMETHOD_(double, chars_atof)(LPSTR lpstrSrc, UINT uSize) PURE;
	STDMETHOD_(CString, DoubleToStrComma)(double dVal, int nSosu, BOOL bShowZero=TRUE) PURE;
	STDMETHOD_(CString, IntToStrComma)(int nVal, BOOL bShowZero=TRUE) PURE;
	STDMETHOD_(int, Str2CommaStr)(LPSTR dest, LPSTR src, UINT uSize) PURE;
	STDMETHOD_(void, StrToStrComma)(LPCTSTR lpstrSrc, CString & strReturn) PURE;
};

// typedef int (CALLBACK* SortCallbackProc)(LPVOID pKey1, LPVOID pKey2, UINT nOption);
DECLARE_INTERFACE_(IDataUtilManager2, IDataUtilManager)
{
	STDMETHOD_(CString, GetStringData)(CString& data, const CString& trimStr, const bool bIsInside = true) PURE;
	STDMETHOD_(int, GetFillString)(CString& szGetStr, int nStart, int nEnd, int nOneWordSize) PURE;

	STDMETHOD_(int, DoParsing)(char cParser, LPTSTR pData, int &nPos, int nEndPos, char* aString) PURE;	

//	STDMETHOD_(BOOL, DoSort)(SortCallbackProc pCallFunc) PURE;	
};



#endif //AFX_INTERFACE_OF_DATAUTIL_1464_45BB_BY_JUNOK_LEE__INCLUDED_
