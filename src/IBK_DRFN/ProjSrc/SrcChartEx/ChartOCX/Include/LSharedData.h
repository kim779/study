// (주)두리정보통신 기술연구소
// 작성일자 : 2000. 10. 26
// 작성자   : 이준옥
// 10. 26	: ILAdviseSink
//			  ILSharedData

#if !defined(AFX_LOCALSHAREDDATA_H__BDEE4DC3_EE3D_1DD3_A027_00C026CA36CC__INCLUDED_)
#define AFX_LOCALSHAREDDATA_H__BDEE4DC3_EE3D_1DD3_A027_00C026CA36CC__INCLUDED_

// #include "../include/LSharedData.h"

// {85BEBE21-AAC0-11D4-B319-0048543A0D2E}
static const GUID IID_ILAdviseSink = 
{ 0x85bebe21, 0xaac0, 0x11d4, { 0xb3, 0x19, 0x0, 0x48, 0x54, 0x3a, 0xd, 0x2e } };

interface ILAdviseSink : public IUnknown
{
   STDMETHOD(OnDataChange) (LPCSTR szSharedName, LPCSTR szData) PURE;
};

// {2F1F2502-AAC0-11D4-B319-0048543A0D2E}
static const GUID IID_ILSharedData = 
{ 0x2f1f2502, 0xaac0, 0x11d4, { 0xb3, 0x19, 0x0, 0x48, 0x54, 0x3a, 0xd, 0x2e } };

interface ILSharedData : public IUnknown
{
   STDMETHOD_(BOOL, GetData)(LPCSTR szSharedName, CString& szData) PURE;
   STDMETHOD_(BOOL, SetData) (HWND hWnd, LPCSTR szSharedName, LPCSTR szData) PURE;
   STDMETHOD_(BOOL, GetData)(LPCSTR szSharedName, LONG& dwData) PURE;
   STDMETHOD_(BOOL, SetData) (HWND hWnd, LPCSTR szSharedName, LONG dwData) PURE;
   STDMETHOD_(BOOL, DAdvise) (ILAdviseSink *pSink, LPCSTR szSharedName) PURE;
   STDMETHOD_(BOOL, UnDAdvise) (ILAdviseSink *pSink, LPCSTR szSharedName) PURE;
};

interface ILSharedData2 : public ILSharedData
{
   STDMETHOD_(LPCSTR, GetData2)(LPCSTR szSharedName) PURE;
};

// LocalSharedData Type
#define		LSDT_STRING			1
#define		LSDT_DWORD			2

typedef struct {
	int		m_nType;
	CString m_szShareName;
	CString m_szShareData;
	DWORD	m_dwShareData;
}ST_LSHAREDATA, *LPST_LSHAREDATA;

typedef struct {
	CString	m_szSharedName;
	CString m_szData;
	BOOL	m_bStart;
	BOOL	m_bSet;
	BOOL	m_bGet;
} ST_LSHAREDFORMAT, *LPST_LSHAREDFORMAT;

#endif // AFX_LOCALSHAREDDATA_H__BDEE4DC3_EE3D_1DD3_A027_00C026CA36CC__INCLUDED_
