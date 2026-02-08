#if !defined(AFX_MAINWND_H__E1BA04B5_3780_4DFD_9459_1EC731DD4530__INCLUDED_)
#define AFX_MAINWND_H__E1BA04B5_3780_4DFD_9459_1EC731DD4530__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMainWnd window
#include "../../../H/axisfire.h"



class CMainWnd : public CWnd
{
// Construction
public:
	CMainWnd(CWnd* pParent, void* pParam);

#ifdef DF_USE_GDIPLUS
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::Bitmap* m_pTBitmap{};
	Gdiplus::Bitmap* m_pKBitmap{};
	Gdiplus::Bitmap* m_pNBitmap{};
#endif

// Attributes
public:
	CString m_slog{};
	CString m_sRoot{}, m_sImagePath{};;
	CMapStringToString m_mapStrToStr;
	CMapStringToString m_mapCodeToNXT{};   //종목코드와 NXT 여부 저장
	//DllInvoker codedll("AxisCode.dll");
	DllInvoker codedll{};

	CWnd* m_pWizard{};
	CWnd* m_pContainerWnd{};   //DLL 에서 cx_marketpicker를 사용할때 
	CWnd* m_pMainWnd{};
	_param m_Param{};
	COLORREF m_clrBack{};

	BOOL m_bInDLL{};  //DLL 내부에서 사용여부
	//Permission
	BOOL	m_bBroadMsg{};   
	enum {
		dispidIsMajor = 18L,
		dispidsNick = 17L,
		dispidPermission = 16L,
		dispidMinor = 15L,
		dispidGetMapPermission = 14L,
		dispidMNum = 13L,
		dispidMajor = 12L,
		dispidsCode = 11L,
		dispidvisible = 10L,
		dispidTraceLog = 9L,
		dispidReadFile = 8L,
		dispidWriteFile = 7L,
		dispiddisplayGuide = 6L,
		dispidGetBroadData = 5L,
		dispidIsNXTAble = 4L,
		dispidSetKey = 3L,
		dispidsMarket = 2,
		TYPE_NORMAL = 0, TYPE_SMALL };
	int m_sizetype{};

	enum { TYPE_KRX = 1, TYPE_NXT , TYPE_TOTAL	};
	int m_markettype{};
	int m_orimarkettype{};
	int m_mapKey{};

	CString Parser(CString& srcstr, CString substr);
	CString Variant(int comm, CString data);
	CBitmap* getBitmap(CString path);
	COLORREF GetIndexColor(int index);
	CSize BitmapSize(HBITMAP hBitmap);

	int    m_major{};
	int    m_minor{};
	int	m_iPermission = -1;
	CString m_sCode{};
	CString m_sCtrlName{};
	CString m_strMarket{};   //통합, KRX, NXT
	CString m_sID{};			   //대표화면 번호 (빌더에서 지정)
	HBITMAP		m_hIBitmap, m_hKBitmap, m_hNBitmap;
	CMapStringToString m_mapPermissions{};
	BOOL	m_bTriggerMK = TRUE;
	BOOL   m_bAutoMarketChange = TRUE;
	void Init();
	void SetImages();
	void parseOptions();
	void ReadMarketFile();
	BOOL IsNXTCode(CString sCode);
	BOOL IsInMajor();
	
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainWnd)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMainWnd)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CMainWnd)
	afx_msg BSTR GetMarketType();
	//}}AFX_DISPATCH
	afx_msg long OnMessage(WPARAM wParam, LPARAM lParam);
	afx_msg long OnMsgFromDll(WPARAM wParam, LPARAM lParam);
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
protected:
	BSTR GetsNick();
	void SetsNick(BSTR newVal);
	SHORT GetPermission();
	void SetPermission(BSTR newVal);
	BSTR GetMinor();
	void SetMinor(BSTR newVal);
	BSTR GetMajor();
	void SetMajor(BSTR newVal);
	BSTR GetMNum();
	void SetMNum(BSTR newVal);
	BSTR GetsCode();
	void SetsCode(BSTR newVal);
	BSTR GetsMarket();
	void SetsMarket(BSTR newVal);

	void SetKey(SHORT key);
	SHORT IsNXTAble(BSTR sCode);
	BSTR GetBroadData(BSTR sVal);
	SHORT GetMapPermission(BSTR sVal);
public:
	afx_msg void OnDestroy();
protected:
	void displayGuide(BSTR smsg);
	void WriteFile(BSTR sFile, BSTR sKey, BSTR sSection, BSTR sData);
	BSTR ReadFile(BSTR sFile, BSTR sKey, BSTR sSection);
	void TraceLog(BSTR slog, BSTR sKey);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
protected:
	void visible(SHORT bShow);
public:
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
protected:
	SHORT IsMajor();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINWND_H__E1BA04B5_3780_4DFD_9459_1EC731DD4530__INCLUDED_)
