#pragma once


// CMainWnd

#include "include/EAPIInterface.h"
#include "include/EAPIInterfaceDefine.h"

#define WM_APP_LOG      (WM_APP + 1)

class CParam
{
public:
	int		key = 0;			// screen key
	CString		name;			// control symbol name
	CRect		rect;			// control rect
	CString		fonts;			// font name
	int		point = 0;		// font point
	int		style = 0;		// font style
	DWORD		tRGB = 0;			// text color index
	DWORD		pRGB = 0;			// paint color index
	CString		options;		// properties
};

class CMainWnd : public CWnd
{
	DECLARE_DYNAMIC(CMainWnd)

public:
	CMainWnd(CWnd* pParent, _param* pParam);
	virtual ~CMainWnd();

	CParam m_Param;



	CWnd* m_pWizard{};
	void	SetParam(_param* pParam);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnAppLog(WPARAM wParam, LPARAM lParam);
public:
	void SetStatus(const char* pszStatus);
	void PostLogFromCallback(const char* pszMsg);

	static DWORD CB_Resp(int nMessageID, char* pszUniqueNO, char* pszDeviceType,
		char* pszResult, char* pszCause, char* pszCauseStr, HWND hWnd, HANDLE hHnd);
	static DWORD CB_Event(int nMessageID, char* pszCallID1, char* pszCallID2,
		char* pszANI, char* pszDNIS, char* pszCallType, char* pszCallDirection,
		char* pszPrevStatus, char* pszCallStatus, char* pszDeviceID,
		HWND hWnd, HANDLE hHnd);
	static DWORD CB_UUI(int nMessageID, char* pszCallID, char* pszUUIData,
		char* pszResult, char* pszCause, char* pszCauseStr, HWND hWnd, HANDLE hHnd);

	static CMainWnd* s_pDlg;
};


