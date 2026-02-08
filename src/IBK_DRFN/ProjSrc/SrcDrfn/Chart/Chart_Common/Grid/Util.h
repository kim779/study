#if !defined(AFX_DATAUTIL_H__62EF63C3_173F_11D2_AB82_00001C1CEC83__INCLUDED_)
#define AFX_DATAUTIL_H__62EF63C3_173F_11D2_AB82_00001C1CEC83__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// DicStringEx.h : main header file for DicStringEx.cpp

/////////////////////////////////////////////////////////////////////////////
// CDicStringEx : See DicStringEx.cpp for implementation.

class CUtil : public CObject
{
	DECLARE_DYNAMIC(CUtil)

public:
	void			AddDashToAccount(LPCSTR _lpIn, CString& szOut);
	CString			GetSignSimbol(int sign);
	BOOL			GetRegKey(LPCSTR szSection, LPCSTR szKey, CString& rData);
	CString			AdjustMask(CString mask, int nValue);
	CString			AdjustMask(CString mask,CString str);
	void			TrimRight(char* pIn);
	void			RemoveCharAll(CString& szData, TCHAR ch);

	CUtil()			{};

	// Modify E (Add Function) 98.10.24 By JunOk Lee
	BOOL			ValidNumber(LPCSTR strIn);
	// Modify E (Add Function) 98.10.24 By JunOk Lee
	BOOL			IsSign(LPCSTR pData);
	void			GetWord( LPSTR, LPSTR, TCHAR );
	int				GetString( CString&, CString&, TCHAR );

	BOOL			IsNumber(UINT nChar);
	BOOL			IsOnlyNumber( LPCTSTR szData);
	int				Remove(CString &strIn, TCHAR ch, int nIndex=1);	// nIndex -1:All , 0: noremove , 몇번째 단어가 제거 될것인지
					
	int				RemoveAll(CString &strIn, TCHAR ch=' ');
	char*			RemoveFront(LPCSTR, TCHAR);
	CString			GetComma(LPCSTR szData);
	CString			GetOnlyNumber( LPCSTR szData );
	CString			GetNumber( LPCSTR szData );
	CString			GetMakeText(TCHAR ch, int nCnt);
	CString			SetLengthString(LPCTSTR pData, int nCnt);
	CString			RemoveChar(LPCSTR pData, TCHAR ch);

	CString			GetPriceUnit(LPCSTR pData);			// JSJ_Add_040528
	CString			GetPriceUnitMan(LPCSTR pData);		// JSJ_Add_040609
	CString			GetVolUnit(LPCSTR pData);			// JSJ_Add_040603


	void			GetPre_N_NextHoga(int nJongMokGubun, long lPrice, long* lUpNextValue, long* lDownNextValue);	// 이전 및 다음 호가
	int				GetHogaTick(int nJongMokGubun, int lPrice);		// 호가단위

protected:

	
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATAUTIL_H__62EF63C3_173F_11D2_AB82_00001C1CEC83__INCLUDED_)

