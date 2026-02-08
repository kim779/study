#pragma once
// IMAXApi.h : header file
//

#ifndef IMXEVENTPROC
typedef void (* IMXEVENTPROC)(DWORD, LPVOID, LPARAM);
#endif

typedef void	(*pfnimax_SetRootPath)		(char*);
typedef int	(*pfnimax_Init)			(HWND, char*, INT);
typedef HANDLE	(*pfnimax_Open)			(HWND, char*, char*, int, int);
typedef void	(*pfnimax_SetGroup)		(HANDLE, int);
typedef void	(*pfnimax_ChangeSkin)		(int);
typedef void	(*pfnimax_Close)		(HANDLE);
typedef void	(*pfnimax_Free)			();
typedef void	(*pfnimax_Resize)		(HANDLE,int, int);
typedef int	(*pfnimax_PreTranslateMessage)	(HANDLE, MSG*);
typedef int	(*pfnimax_GetStockType)		(char*);
typedef HWND	(*pfnimax_CreateCodeWindow)	(HWND, char*, int, int, int, int, DWORD);
typedef void	(*pfnimax_MoveCodeWindow)	(HWND, int, int, int, int);
typedef void	(*pfnimax_DeleteCodeWindow)	(HWND);
typedef void	(*pfnimax_ChangeCodeWindow)	(HWND, char*);
typedef HWND	(*pfnimax_GetHwnd)		(HANDLE);
typedef BOOL	(*pfnimax_GetScreen)		(LPTSTR , LPTSTR );
typedef	BOOL	(*pfnimax_GetScreenInfo)	(char*, char*, int*, int*);
typedef void	(*pfnimax_SetEventWindow)	(HANDLE, HWND);
typedef void	(*pfnimax_SetEventProc)		(HANDLE, IMXEVENTPROC, LPARAM);
typedef INT	(*pfnimax_GetViewCount)		();
typedef INT	(*pfnimax_GetISIN)		(char*, char*);
typedef void(*pfnimax_SetAuth)		(char* szAuth);

/////////////////////////////////////////////////////////////////////////////
// CIMAXApi

class CIMAXApi
{
public:
	CIMAXApi();           // protected constructor used by dynamic creation

// Attributes
public:
	BOOL	g_bLoadImaxGate;

// for OutputDebugString
	CString	m_tmpX;
	void	dprint();

// Operations
public:
	void	LoadImaxGate();
	void	FreeImaxGate();

	// IMaxGate Api
	void	SetRootPath(char *szRootPath);
	int	IMAXInit(HWND hFrameWnd, char *szLogin, INT nType);
	HANDLE	Open(HWND hwnd, char *szScreen, char *szCodeInfo, int nWidth, int nHeight);
	void	SetGroup(HANDLE hView, int nGroupNo);
	void	ChangeSkin(int nSkin);
	void	Close(HANDLE hView);
	void	Free();
	void	Resize(HANDLE hView ,int cx, int cy);
	int	PreTranslateMessage(HANDLE hView, MSG* pMsg);
	int	GetStockType(char *szCodeInfo);
	HWND	CreateCodeWindow(HWND hParentWnd, char *szCodeInfo, int nLeft, int nTop, int nWidth, int nHeight, DWORD dwOption);
	void	MoveCodeWindow(HWND hWnd, int nLeft, int nTop, int nWidth, int nHeight);
	void	DeleteCodeWindow(HWND hWnd);
	void	ChangeCodeWindow(HWND hWnd, char *szCodeInfo);
	HWND	GetHwnd(HANDLE hView);
	void	SetEventWindow(HANDLE hView, HWND hRecvWnd);
	void	SetEventProc(HANDLE hView, IMXEVENTPROC lpProc, LPARAM lParam);
	INT	GetViewCount();
	BOOL	GetISIN(char *szCodeInfo, CString& strISIN);
	void		SetAuth(char* sAuth);
	// Screen Info
	BOOL	GetScreenInfo(CString strScr, CString& strImaxNo, int& nScrWidth, int& nScrHeight);
	BOOL	GetScreen(CString strImaxNo, CString& strScr);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIMAXApi)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CIMAXApi();

	// Generated message map functions
	//{{AFX_MSG(CIMAXApi)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
};