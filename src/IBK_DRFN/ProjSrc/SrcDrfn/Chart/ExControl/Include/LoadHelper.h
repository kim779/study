#if !defined(AFX_LOADSYSTEMTRADINGPARSERHELPER_H__DF553F68_69A3_11D6_994C_004854800CB4__INCLUDED_)
#define AFX_LOADSYSTEMTRADINGPARSERHELPER_H__DF553F68_69A3_11D6_994C_004854800CB4__INCLUDED_

#include "BaseLoadHelper.h"
#include "../../../ForSite/FS_Env.h"

/////////////////////////////////////////////////////////////////////////////
//	CLoaderDrawButton
/////////////////////////////////////////////////////////////////////////////
typedef long	(* FpActionControl)(LPVOID, LPVOID);
typedef LPVOID	(* FpInitDrawButton)(LPVOID);
typedef LPVOID	(* FpInitDrawButton2)(LPVOID, BOOL);
typedef void	(* FpExitButton)(LPVOID);
typedef void	(* FpDrawItemButton)(LPVOID, LPDRAWITEMSTRUCT);
typedef void	(* FpSetText)(LPVOID, LPCTSTR);
typedef BOOL	(* FpSetIconImage)(LPVOID, CSize, HICON, HICON, UINT);
typedef FpChangeSkinNotify (* FpGetSkinNotifyFunction)(LPVOID);
typedef BOOL	(* FpSetDrawButton)(LPVOID, UINT);
typedef void	(* FpPreSubclassWindow_DrawButton)(LPVOID);
typedef LPCTSTR (* FpGetText_Button)(LPVOID);
typedef void	(* FpUseToggleMode_DrawButton)(LPVOID, BOOL);
typedef BOOL	(* FpGetToggleState_DrawButton)(LPVOID);
typedef void	(* FpSetToggleState_DrawButton)(LPVOID, BOOL);
typedef void	(* FpReleasePushed_DrawButton)(LPVOID);
typedef void	(* FpOnClicked_DrawButton)(LPVOID);
typedef void	(* FpAddToggleGroup_DrawButton)(LPVOID, LPVOID);

class CLoaderDrawButton : public CExLoadLib
{
public:
	CLoaderDrawButton(LPVOID lpPointer, BOOL bUseDefSkin) 
	{
// 		char szPath[MAX_PATH] = {0, };
// 		GetCurrentWorkDirectory(szPath, sizeof(szPath));
// 
// 		if(szPath)
// 			::sprintf(m_szDllName, "%s\\%s", szPath, BUTTON_DLL);
// 		else
// 			::strcpy(m_szDllName, BUTTON_DLL);

		//-->Solomon-090829수정.
		CString szDir;
		GetDir_DrfnBin(szDir);
		::sprintf(m_szDllName, "%s\\%s", (LPCSTR)szDir, BUTTON_DLL);
		//<--Solomon-090829수정.

		pActionControl = NULL;
		pInitDrawButton = NULL;
		pInitDrawButton2 = NULL;
		pExitButton = NULL;
		pSetDrawButton = NULL;
		pDrawItemButton = NULL;
		pSetText = NULL;
		pSetIconImage = NULL;
		pGetSkinNotifyFunction = NULL;
		pPreSubclassWindow_DrawButton = NULL;
		pGetText = NULL;

		pUseToggleMode_DrawButton = NULL;
		pGetToggleState_DrawButton = NULL;
		pSetToggleState_DrawButton = NULL;
		pReleasePushed_DrawButton = NULL;
		pOnClicked_DrawButton = NULL;

		pAddToggleGroup_DrawButton = NULL;

		LoadLib();

		if(bUseDefSkin)
			m_lpPointer = pInitDrawButton2(lpPointer, bUseDefSkin);
		else
			m_lpPointer = pInitDrawButton(lpPointer);
	}

	~CLoaderDrawButton() 
	{
		if(m_hLib)
			pExitButton(m_lpPointer);
	}

	virtual BOOL LoadProcAddress() 
	{
		if(!m_hLib)
		{
			TRACE0("CLoaderDrawButton GetProcAddress에서 Load에러 발생");
			return FALSE;
		}

		pActionControl	= (FpActionControl)::GetProcAddress(m_hLib, "Ex_ActionControl");
		pInitDrawButton	= (FpInitDrawButton)::GetProcAddress(m_hLib, "Ex_InitDrawButton");
		pInitDrawButton2	= (FpInitDrawButton2)::GetProcAddress(m_hLib, "Ex_InitDrawButton2");
		pExitButton	= (FpExitButton)::GetProcAddress(m_hLib, "Ex_ExitButton");
		pSetDrawButton		= (FpSetDrawButton)::GetProcAddress(m_hLib, "Ex_SetDrawButton");
		pDrawItemButton		= (FpDrawItemButton)::GetProcAddress(m_hLib, "Ex_DrawItemButton");
		pSetText		= (FpSetText)::GetProcAddress(m_hLib, "Ex_SetText");
		pSetIconImage		= (FpSetIconImage)::GetProcAddress(m_hLib, "Ex_SetIcon");
		pGetSkinNotifyFunction		= (FpGetSkinNotifyFunction)::GetProcAddress(m_hLib, "Ex_GetSkinNotifyFunction");
		pPreSubclassWindow_DrawButton		= (FpPreSubclassWindow_DrawButton)::GetProcAddress(m_hLib, "Ex_PreSubclassWindow_DrawButton");
		pGetText		= (FpGetText_Button)::GetProcAddress(m_hLib, "Ex_GetText_DrawButton");
		return TRUE;
	}

public:
	// Implementation
	LPVOID			m_lpPointer;

	FpActionControl		pActionControl;
	FpInitDrawButton	pInitDrawButton;
	FpInitDrawButton2	pInitDrawButton2;
	FpExitButton	pExitButton;
	FpSetDrawButton		pSetDrawButton;
	FpDrawItemButton		pDrawItemButton;
	FpSetText		pSetText;
	FpSetIconImage		pSetIconImage;
	FpGetSkinNotifyFunction		pGetSkinNotifyFunction;
	FpPreSubclassWindow_DrawButton		pPreSubclassWindow_DrawButton;
	FpGetText_Button		pGetText;

	FpUseToggleMode_DrawButton		pUseToggleMode_DrawButton;
	FpGetToggleState_DrawButton		pGetToggleState_DrawButton;
	FpSetToggleState_DrawButton		pSetToggleState_DrawButton;
	FpReleasePushed_DrawButton		pReleasePushed_DrawButton;

	FpOnClicked_DrawButton		pOnClicked_DrawButton;

	FpAddToggleGroup_DrawButton	pAddToggleGroup_DrawButton;


public:
	// Function
	long ActionControl(LPVOID lpAction)
	{
		if(!m_hLib)
			return 0L;

		return pActionControl(m_lpPointer, lpAction);
	}

	BOOL SetDrawButton(UINT nButton)
	{
		if(!m_hLib)
			return FALSE;

		return pSetDrawButton(m_lpPointer, nButton);
	}

	void DrawItemButton(LPDRAWITEMSTRUCT lpDrawItemStruct)
	{
		if(!m_hLib)
			return;

		pDrawItemButton(m_lpPointer, lpDrawItemStruct);
	}

	void SetText(LPCTSTR lpszText)
	{
		if(!m_hLib)
			return;

		pSetText(m_lpPointer, lpszText);
	}

	BOOL SetIconImage(CSize size, HICON hIcon, HICON hIconPushed, UINT nIconAlign)
	{
		if(!m_hLib)
			return FALSE;

		return pSetIconImage(m_lpPointer, size, hIcon, hIconPushed, nIconAlign);
	}

	FpChangeSkinNotify GetSkinNotifyFunction()
	{
		if(!m_hLib)
			return NULL;

		return pGetSkinNotifyFunction(m_lpPointer);
	}

	void PreSubclassWindow() 
	{
		if(!m_hLib)
			return;
		
		pPreSubclassWindow_DrawButton(m_lpPointer);
	}

	LPCTSTR GetText()
	{
		if(!m_hLib)
			return _T("");

		return pGetText(m_lpPointer);
	}

	void UseToggleMode(BOOL bUse)
	{
		if(!m_hLib)
			return;

		if(!pUseToggleMode_DrawButton)
			pUseToggleMode_DrawButton = (FpUseToggleMode_DrawButton)::GetProcAddress(m_hLib, "Ex_UseToggleMode_DrawButton");

		if(!pUseToggleMode_DrawButton)
			return;

		pUseToggleMode_DrawButton(m_lpPointer, bUse);
	}

	BOOL GetToggleState()
	{
		if(!m_hLib)
			return FALSE;

		if(!pGetToggleState_DrawButton)
			pGetToggleState_DrawButton = (FpGetToggleState_DrawButton)::GetProcAddress(m_hLib, "Ex_GetToggleState_DrawButton");

		if(!pGetToggleState_DrawButton)
			return FALSE;

		return pGetToggleState_DrawButton(m_lpPointer);
	}

	void SetToggleState(BOOL bPush)
	{
		if(!m_hLib)
			return;

		if(!pSetToggleState_DrawButton)
			pSetToggleState_DrawButton = (FpSetToggleState_DrawButton)::GetProcAddress(m_hLib, "Ex_SetToggleState_DrawButton");

		if(!pSetToggleState_DrawButton)
			return;

		pSetToggleState_DrawButton(m_lpPointer, bPush);
	}

	void ReleasePushed()
	{
		if(!m_hLib)
			return;

		if(!pReleasePushed_DrawButton)
			pReleasePushed_DrawButton = (FpReleasePushed_DrawButton)::GetProcAddress(m_hLib, "Ex_ReleasePushed_DrawButton");

		if(!pReleasePushed_DrawButton)
			return;

		pReleasePushed_DrawButton(m_lpPointer);
	}

	void OnClicked()
	{
		if(!m_hLib)
			return;

		if(!pOnClicked_DrawButton)
			pOnClicked_DrawButton = (FpOnClicked_DrawButton)::GetProcAddress(m_hLib, "Ex_OnClicked_DrawButton");

		if(!pOnClicked_DrawButton)
			return;

		pOnClicked_DrawButton(m_lpPointer);
	}

	void AddToggleGroup(LPVOID lpAddPointer)
	{
		if(!m_hLib)
			return;

		if(!pAddToggleGroup_DrawButton)
			pAddToggleGroup_DrawButton = (FpAddToggleGroup_DrawButton)::GetProcAddress(m_hLib, "Ex_AddToggleGroup_DrawButton");

		if(!pAddToggleGroup_DrawButton)
			return;

		pAddToggleGroup_DrawButton(m_lpPointer, lpAddPointer);
	}
};

/////////////////////////////////////////////////////////////////////////////
//	CLoaderImgListButton
/////////////////////////////////////////////////////////////////////////////
typedef long	(* FpActionControl)(LPVOID, LPVOID);
typedef LPVOID	(* FpInitImgListButton)(LPVOID);
typedef void	(* FpExitButton)(LPVOID);
typedef BOOL	(* FpSetImageList)(LPVOID, CImageList*, int, int, int, int, LPCTSTR, COLORREF, COLORREF);
typedef void	(* FpDrawItemImageList)(LPVOID, LPDRAWITEMSTRUCT);
typedef void	(* FpSetText)(LPVOID, LPCTSTR);
typedef LPCTSTR (* FpGetText_Button)(LPVOID);
class CLoaderImgListButton : public CExLoadLib
{
public:
	CLoaderImgListButton(LPVOID lpPointer, LPCSTR lpszRootPath = NULL) 
	{
// 		if(lpszRootPath)
// 			::sprintf(m_szDllName, "%s\\Bin\\%s", lpszRootPath, BUTTON_DLL);
// 		else
// 		{
// 			char szPath[MAX_PATH] = {0, };
// 			GetCurrentWorkDirectory(szPath, sizeof(szPath));
// 
// 			if(szPath)
// 				::sprintf(m_szDllName, "%s\\%s", szPath, BUTTON_DLL);
// 			else
// 				::strcpy(m_szDllName, BUTTON_DLL);
// 		}

		//-->Solomon-090829수정. 
		CString szDir;
		GetDir_DrfnBin(szDir);
		::sprintf(m_szDllName, "%s\\%s", (LPCSTR)szDir, BUTTON_DLL);
		//<--Solomon-090829수정.

		pActionControl = NULL;
		pExitButton = NULL;
		pSetImageList = NULL;
		pDrawItemImageList = NULL;
		pSetText = NULL;
		pGetText = NULL;

		LoadLib();

		m_lpPointer = pInitImgListButton(lpPointer);
	}

	~CLoaderImgListButton() 
	{
		if(m_hLib)
			pExitButton(m_lpPointer);
	}

	virtual BOOL LoadProcAddress() 
	{
		if(!m_hLib)
		{
			TRACE0("CLoaderImgListButton GetProcAddress에서 Load에러 발생");
			return FALSE;
		}

		pActionControl	= (FpActionControl)::GetProcAddress(m_hLib, "Ex_ActionControl");
		pInitImgListButton	= (FpInitImgListButton)::GetProcAddress(m_hLib, "Ex_InitImgListButton");
		pExitButton	= (FpExitButton)::GetProcAddress(m_hLib, "Ex_ExitButton");
		pSetImageList		= (FpSetImageList)::GetProcAddress(m_hLib, "Ex_SetImageListButton");
		pDrawItemImageList		= (FpDrawItemImageList)::GetProcAddress(m_hLib, "Ex_DrawItemImageList");
		pSetText		= (FpSetText)::GetProcAddress(m_hLib, "Ex_SetText_ImgList");
		pGetText		= (FpGetText_Button)::GetProcAddress(m_hLib, "Ex_GetText_ImgList");
		return TRUE;
	}

public:
	// Implementation
	LPVOID			m_lpPointer;

	FpActionControl		pActionControl;
	FpInitImgListButton	pInitImgListButton;
	FpExitButton	pExitButton;
	FpSetImageList		pSetImageList;
	FpDrawItemImageList		pDrawItemImageList;
	FpSetText		pSetText;
	FpGetText_Button		pGetText;

public:
	// Function
	long ActionControl(LPVOID lpAction)
	{
		if(!m_hLib)
			return 0L;

		return pActionControl(m_lpPointer, lpAction);
	}

	BOOL SetImageList(CImageList* pImageList, int nNormal, int nDown, int nDisable, int nOver, LPCTSTR lpszText, COLORREF clrText, COLORREF clrTextDis)
	{
		if(!m_hLib)
			return FALSE;

		return pSetImageList(m_lpPointer, pImageList, nNormal, nDown, nDisable, nOver, lpszText, clrText, clrTextDis);
	}

	void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
	{
		if(!m_hLib)
			return;

		pDrawItemImageList(m_lpPointer, lpDrawItemStruct);
	}

	void SetText(LPCTSTR lpszText)
	{
		if(!m_hLib)
			return;

		pSetText(m_lpPointer, lpszText);
	}

	LPCTSTR GetText()
	{
		if(!m_hLib)
			return _T("");

		return pGetText(m_lpPointer);
	}
};

/////////////////////////////////////////////////////////////////////////////
//	CLoaderBitmapButton
/////////////////////////////////////////////////////////////////////////////
typedef long	(* FpActionControl)(LPVOID, LPVOID);
typedef LPVOID	(* FpInitBitmapButton)(LPVOID);
typedef void	(* FpExitButton)(LPVOID);
typedef BOOL	(* FpSetBitmapFile)(LPVOID, LPCTSTR, LPCTSTR, LPCTSTR);
typedef void	(* FpDrawItemBitmap)(LPVOID, LPDRAWITEMSTRUCT);
class CLoaderBitmapButton : public CExLoadLib
{
public:
	CLoaderBitmapButton(LPVOID lpPointer, LPCSTR lpszRootPath = NULL) 
	{
// 		if(lpszRootPath)
// 			::sprintf(m_szDllName, "%s\\Bin\\%s", lpszRootPath, BUTTON_DLL);
// 		else
// 		{
// 			char szPath[MAX_PATH] = {0, };
// 			GetCurrentWorkDirectory(szPath, sizeof(szPath));
// 
// 			if(szPath)
// 				::sprintf(m_szDllName, "%s\\%s", szPath, BUTTON_DLL);
// 			else
// 				::strcpy(m_szDllName, BUTTON_DLL);
// 		}

		//-->Solomon-090829수정. 
		CString szDir;
		GetDir_DrfnBin(szDir);
		::sprintf(m_szDllName, "%s\\%s", (LPCSTR)szDir, BUTTON_DLL);
		//<--Solomon-090829수정. 

		pActionControl = NULL;
		pExitButton = NULL;
		pSetBitmapFile = NULL;
		pDrawItemBitmap = NULL;

		LoadLib();

		m_lpPointer = pInitBitmapButton(lpPointer);
	}

	~CLoaderBitmapButton() 
	{
		if(m_hLib)
			pExitButton(m_lpPointer);
	}

	virtual BOOL LoadProcAddress() 
	{
		if(!m_hLib)
		{
			TRACE0("CLoaderBitmapButton GetProcAddress에서 Load에러 발생");
			return FALSE;
		}

		pActionControl	= (FpActionControl)::GetProcAddress(m_hLib, "Ex_ActionControl");
		pInitBitmapButton	= (FpInitBitmapButton)::GetProcAddress(m_hLib, "Ex_InitBitmapButton");
		pExitButton	= (FpExitButton)::GetProcAddress(m_hLib, "Ex_ExitButton");
		pSetBitmapFile	= (FpSetBitmapFile)::GetProcAddress(m_hLib, "Ex_SetBitmapFile");
		pDrawItemBitmap		= (FpDrawItemBitmap)::GetProcAddress(m_hLib, "Ex_DrawItemBitmap");
		return TRUE;
	}

public:
	// Implementation
	LPVOID			m_lpPointer;

	FpActionControl		pActionControl;
	FpInitBitmapButton	pInitBitmapButton;
	FpExitButton	pExitButton;
	FpSetBitmapFile	pSetBitmapFile;
	FpDrawItemBitmap		pDrawItemBitmap;

public:
	// Function
	long ActionControl(LPVOID lpAction)
	{
		if(!m_hLib)
			return 0L;

		return pActionControl(m_lpPointer, lpAction);
	}

	BOOL SetBitmapFile(LPCTSTR lpszUp, LPCTSTR lpszDown, LPCTSTR lpszDis)
	{
		if(!m_hLib)
			return FALSE;

		return pSetBitmapFile(m_lpPointer, lpszUp, lpszDown, lpszDis);
	}

	void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
	{
		if(!m_hLib)
			return;

		pDrawItemBitmap(m_lpPointer, lpDrawItemStruct);
	}
};

/////////////////////////////////////////////////////////////////////////////
//	CLoaderCheckGroupBoxButton
/////////////////////////////////////////////////////////////////////////////
typedef long	(* FpActionControl)(LPVOID, LPVOID);
typedef LPVOID	(* FpInitCheckGroupBox)(LPVOID);
typedef void	(* FpExitButton)(LPVOID);
typedef void	(* FpPreSubclassWindow)(LPVOID);
typedef void	(* FpSetCheck)(LPVOID, int);
typedef int		(* FpGetCheck)(LPVOID);
typedef void	(* FpOnSetFocus)(LPVOID, CWnd*);
typedef void	(* FpOnEnable)(LPVOID, BOOL);
typedef LRESULT	(* FpOnGetCheck)(LPVOID, WPARAM, LPARAM);
typedef LRESULT	(* FpOnSetCheck)(LPVOID, WPARAM, LPARAM);
typedef void	(* FpOnClicked)(LPVOID);
typedef void	(* FpSetStyle)(LPVOID, TouchStyle);
class CLoaderCheckGroupBoxButton : public CExLoadLib
{
public:
	CLoaderCheckGroupBoxButton(LPVOID lpPointer, LPCSTR lpszRootPath = NULL) 
	{
// 		if(lpszRootPath)
// 			::sprintf(m_szDllName, "%s\\Bin\\%s", lpszRootPath, BUTTON_DLL);
// 		else
// 		{
// 			char szPath[MAX_PATH] = {0, };
// 			GetCurrentWorkDirectory(szPath, sizeof(szPath));
// 
// 			if(szPath)
// 				::sprintf(m_szDllName, "%s\\%s", szPath, BUTTON_DLL);
// 			else
// 				::strcpy(m_szDllName, BUTTON_DLL);
// 		}

		//-->Solomon-090829수정. 
		CString szDir;
		GetDir_DrfnBin(szDir);
		::sprintf(m_szDllName, "%s\\%s", (LPCSTR)szDir, BUTTON_DLL);
		//<--Solomon-090829수정. 

		pActionControl = NULL;
		pExitButton = NULL;
		pInitCheckGroupBox = NULL;
		pPreSubclassWindow = NULL;
		pSetCheck = NULL;
		pGetCheck = NULL;
		pOnSetFocus = NULL;
		pOnEnable = NULL;
		pOnGetCheck = NULL;
		pOnSetCheck = NULL;
		pOnClicked = NULL;
		pSetStyle = NULL;

		LoadLib();

		m_lpPointer = pInitCheckGroupBox(lpPointer);
	}

	~CLoaderCheckGroupBoxButton() 
	{
		if(m_hLib)
			pExitButton(m_lpPointer);
	}

	virtual BOOL LoadProcAddress() 
	{
		if(!m_hLib)
		{
			TRACE0("CLoaderCheckGroupBoxButton GetProcAddress에서 Load에러 발생");
			return FALSE;
		}

		pActionControl	= (FpActionControl)::GetProcAddress(m_hLib, "Ex_ActionControl");
		pExitButton	= (FpExitButton)::GetProcAddress(m_hLib, "Ex_ExitButton");
		pInitCheckGroupBox		= (FpInitCheckGroupBox)::GetProcAddress(m_hLib, "Ex_InitCheckGroupBox");
		pPreSubclassWindow		= (FpPreSubclassWindow)::GetProcAddress(m_hLib, "Ex_PreSubclassWindow_CheckGroupBox");
		pSetCheck		= (FpSetCheck)::GetProcAddress(m_hLib, "Ex_SetCheck_CheckGroupBox");
		pGetCheck		= (FpGetCheck)::GetProcAddress(m_hLib, "Ex_GetCheck_CheckGroupBox");
		pOnSetFocus		= (FpOnSetFocus)::GetProcAddress(m_hLib, "Ex_OnSetFocus_CheckGroupBox");
		pOnEnable		= (FpOnEnable)::GetProcAddress(m_hLib, "Ex_OnEnable_CheckGroupBox");
		pOnGetCheck		= (FpOnGetCheck)::GetProcAddress(m_hLib, "Ex_OnGetCheck_CheckGroupBox");
		pOnSetCheck		= (FpOnSetCheck)::GetProcAddress(m_hLib, "Ex_OnSetCheck_CheckGroupBox");
		pOnClicked		= (FpOnClicked)::GetProcAddress(m_hLib, "Ex_OnClicked_CheckGroupBox");
		pSetStyle		= (FpSetStyle)::GetProcAddress(m_hLib, "Ex_OnClicked_CheckGroupBox");
		return TRUE;
	}

public:
	// Implementation
	LPVOID			m_lpPointer;

	FpActionControl		pActionControl;
	FpExitButton	pExitButton;
	FpInitCheckGroupBox		pInitCheckGroupBox;
	FpPreSubclassWindow		pPreSubclassWindow;
	FpSetCheck		pSetCheck;
	FpGetCheck		pGetCheck;
	FpOnSetFocus		pOnSetFocus;
	FpOnEnable		pOnEnable;
	FpOnGetCheck		pOnGetCheck;
	FpOnSetCheck		pOnSetCheck;
	FpOnClicked		pOnClicked;
	FpSetStyle		pSetStyle;

public:
	// Function
	long ActionControl(LPVOID lpAction)
	{
		if(!m_hLib)
			return 0L;

		return pActionControl(m_lpPointer, lpAction);
	}

	void InitCheckGroupBox()
	{
		if(!m_hLib)
			return;

		pInitCheckGroupBox(m_lpPointer);
	}

	void SetCheck_CheckGroupBox(int nCheck)
	{
		if(!m_hLib)
			return;

		pSetCheck(m_lpPointer, nCheck);
	}

	void PreSubclassWindow_CheckGroupBox()
	{
		if(!m_hLib)
			return;

		pPreSubclassWindow(m_lpPointer);
	}

	int GetCheck_CheckGroupBox()
	{
		if(!m_hLib)
			return -1;

		return pGetCheck(m_lpPointer);
	}

	void OnSetFocus_CheckGroupBox(CWnd* pOldWnd)
	{
		if(!m_hLib)
			return;

		pOnSetFocus(m_lpPointer, pOldWnd);
	}

	void OnEnable_CheckGroupBox(BOOL bEnable)
	{
		if(!m_hLib)
			return;

		pOnEnable(m_lpPointer, bEnable);
	}

	LRESULT OnGetCheck_CheckGroupBox(WPARAM wParam, LPARAM lParam)
	{
		if(!m_hLib)
			return 0L;

		return pOnGetCheck(m_lpPointer, wParam, lParam);
	}

	LRESULT OnSetCheck_CheckGroupBox(WPARAM wParam, LPARAM lParam)
	{
		if(!m_hLib)
			return 0L;

		return pOnSetCheck(m_lpPointer, wParam, lParam);
	}

	void OnClicked_CheckGroupBox()
	{
		if(!m_hLib)
			return;

		pOnClicked(m_lpPointer);
	}

	void SetStyle_CheckGroupBox(TouchStyle nStyle)
	{
		if(!m_hLib)
			return;

		pSetStyle(m_lpPointer, nStyle);
	}
};

/////////////////////////////////////////////////////////////////////////////
//	CLoaderStatic
/////////////////////////////////////////////////////////////////////////////
typedef long	(* FpActionControl)(LPVOID, LPVOID);
typedef LPVOID	(* FpInitStatic)(LPVOID);
typedef LPVOID	(* FpInitStatic2)(LPVOID, BOOL);
typedef void	(* FpExitStatic)(LPVOID);
typedef BOOL	(* FpSetStatic)(LPVOID, LPCTSTR);
typedef BOOL	(* FpSetStaticColor)(LPVOID, COLORREF, COLORREF, COLORREF);
typedef BOOL	(* FpSetStaticString)(LPVOID, LPCTSTR, LPCTSTR, LPCTSTR);
typedef BOOL	(* FpSetStaticIndex)(LPVOID, UINT, UINT, UINT);
typedef	void	(* FpOnPaint)(LPVOID, CDC*);
typedef BOOL	(* FpSetStaticIcon)(LPVOID, CSize, HICON, UINT);
typedef void	(* FpSetText)(LPVOID, LPCTSTR);
typedef FpChangeSkinNotify (* FpGetSkinNotifyFunction)(LPVOID);
typedef void	(* FpPreSubclassWindow_Statc)(LPVOID);
typedef LRESULT	(* FpWindowProc_Statc)(LPVOID, UINT, WPARAM, LPARAM);
typedef BOOL	(* FpOnEraseBkgnd)(LPVOID, CDC*);
typedef void	(WINAPI* FpGetWindowText)(LPVOID, CString& rString);
class CLoaderStatic : public CExLoadLib
{
public:
	CLoaderStatic(LPVOID lpPointer, BOOL bUseDefSkin) 
	{
// 		char szPath[MAX_PATH] = {0, };
// 		GetCurrentWorkDirectory(szPath, sizeof(szPath));
// 
// 		if(szPath)
// 			::sprintf(m_szDllName, "%s\\%s", szPath, STATIC_DLL);
// 		else
// 			::strcpy(m_szDllName, STATIC_DLL);

		//-->Solomon-090829수정. 
		CString szDir;
		GetDir_DrfnBin(szDir);
		::sprintf(m_szDllName, "%s\\%s", (LPCSTR)szDir, STATIC_DLL);
		//<--Solomon-090829수정.. 

		pActionControl = NULL;
		pInitStatic = NULL;
		pInitStatic2 = NULL;
		pExitStatic = NULL;
		pSetStatic = NULL;
		pSetStaticColor = NULL;
		pSetStaticString = NULL;
		pSetStaticIndex = NULL;
		pOnPaint = NULL;
		pSetStaticIcon = NULL;
		pSetText = NULL;
		pGetSkinNotifyFunction = NULL;
		pPreSubclassWindow_Statc = NULL;
		pWindowProc = NULL;
		pOnEraseBkgnd = NULL;
		pGetWindowText = NULL;
		LoadLib();

		if(bUseDefSkin)
			m_lpPointer = pInitStatic2(lpPointer, bUseDefSkin);
		else
			m_lpPointer = pInitStatic(lpPointer);
	}

	~CLoaderStatic() 
	{
		if(m_hLib)
			pExitStatic(m_lpPointer);
	}

	virtual BOOL LoadProcAddress() 
	{
		if(!m_hLib)
		{
			TRACE0("CLoaderStatic GetProcAddress에서 Load에러 발생");
			return FALSE;
		}

		pActionControl	= (FpActionControl)::GetProcAddress(m_hLib, "Ex_ActionControl");
		pInitStatic	= (FpInitStatic)::GetProcAddress(m_hLib, "Ex_InitStatic");
		pInitStatic2	= (FpInitStatic2)::GetProcAddress(m_hLib, "Ex_InitStatic2");
		pExitStatic	= (FpExitStatic)::GetProcAddress(m_hLib, "Ex_ExitStatic");
		pSetStatic	= (FpSetStatic)::GetProcAddress(m_hLib, "Ex_SetStatic");
		pSetStaticColor	= (FpSetStaticColor)::GetProcAddress(m_hLib, "Ex_SetStaticColor");
		pSetStaticString	= (FpSetStaticString)::GetProcAddress(m_hLib, "Ex_SetStaticString");
		pSetStaticIndex	= (FpSetStaticIndex)::GetProcAddress(m_hLib, "Ex_SetStaticIndex");
		pOnPaint	= (FpOnPaint)::GetProcAddress(m_hLib, "Ex_OnPaint");
		pSetStaticIcon		= (FpSetStaticIcon)::GetProcAddress(m_hLib, "Ex_SetIcon");
		pSetText		= (FpSetText)::GetProcAddress(m_hLib, "Ex_SetText");
		pGetSkinNotifyFunction		= (FpGetSkinNotifyFunction)::GetProcAddress(m_hLib, "Ex_GetSkinNotifyFunction");
		pPreSubclassWindow_Statc	= (FpPreSubclassWindow_Statc)::GetProcAddress(m_hLib, "Ex_PreSubclassWindow_Statc");
		pWindowProc	= (FpWindowProc_Statc)::GetProcAddress(m_hLib, "Ex_WindowProc_Static");
		pOnEraseBkgnd	= (FpOnEraseBkgnd)::GetProcAddress(m_hLib, "Ex_OnEraseBkgnd_Static");
		pGetWindowText	= (FpGetWindowText)::GetProcAddress(m_hLib, "Ex_GetWindowText_Static");

		return TRUE;
	}

public:
	// Implementation
	LPVOID			m_lpPointer;

	FpActionControl		pActionControl;
	FpInitStatic	pInitStatic;
	FpInitStatic2	pInitStatic2;
	FpExitStatic	pExitStatic;
	FpSetStatic	pSetStatic;
	FpSetStaticColor	pSetStaticColor;
	FpSetStaticString	pSetStaticString;
	FpSetStaticIndex	pSetStaticIndex;
	FpOnPaint	pOnPaint;
	FpSetStaticIcon	pSetStaticIcon;
	FpSetText		pSetText;
	FpGetSkinNotifyFunction		pGetSkinNotifyFunction;
	FpPreSubclassWindow_Statc	pPreSubclassWindow_Statc;
	FpWindowProc_Statc	pWindowProc;
	FpOnEraseBkgnd			pOnEraseBkgnd;
	FpGetWindowText			pGetWindowText;

public:
	// Function
	long ActionControl(LPVOID lpAction)
	{
		if(!m_hLib)
			return 0L;

		return pActionControl(m_lpPointer, lpAction);
	}

	BOOL SetStatic(LPCTSTR lpszText)
	{
		if(!m_hLib)
			return FALSE;

		return pSetStatic(m_lpPointer, lpszText);
	}

	BOOL SetStaticColor(COLORREF clrOutLine, COLORREF clrBackground, COLORREF clrText)
	{
		if(!m_hLib)
			return FALSE;

		return pSetStaticColor(m_lpPointer, clrOutLine, clrBackground, clrText);
	}

	BOOL SetStaticString(LPCTSTR lpszOutLine, LPCTSTR lpszBackground, LPCTSTR lpszTextClr)
	{
		if(!m_hLib)
			return FALSE;

		return pSetStaticString(m_lpPointer, lpszOutLine, lpszBackground, lpszTextClr);
	}

	BOOL SetStaticIndex(UINT nOutLine, UINT nBackground, UINT nText)
	{
		if(!m_hLib)
			return FALSE;

		return pSetStaticIndex(m_lpPointer, nOutLine, nBackground, nText);
	}

	void OnPaint(CDC* pDC)
	{
		if(!m_hLib)
			return;

		pOnPaint(m_lpPointer, pDC);
	}

	BOOL SetIconImage(CSize size, HICON hIcon, UINT nIconAlign)
	{
		if(!m_hLib)
			return FALSE;

		return pSetStaticIcon(m_lpPointer, size, hIcon, nIconAlign);
	}

	void SetText(LPCTSTR lpszText)
	{
		if(!m_hLib)
			return;

		pSetText(m_lpPointer, lpszText);
	}

	FpChangeSkinNotify GetSkinNotifyFunction()
	{
		if(!m_hLib)
			return NULL;

		return pGetSkinNotifyFunction(m_lpPointer);
	}

	void PreSubclassWindow() 
	{
		if(!m_hLib)
			return;
		
		pPreSubclassWindow_Statc(m_lpPointer);
	}

	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		if(!m_hLib)
			return 0L;

		return pWindowProc(m_lpPointer, message, wParam, lParam);
	}

	BOOL OnEraseBkgnd(CDC* pDC)
	{
		if(!m_hLib)
			return FALSE;

		return pOnEraseBkgnd(m_lpPointer, pDC);
	}

	void GetWindowText(CString& rString)
	{
		if(!m_hLib)
			return;
		
		pGetWindowText(m_lpPointer, rString);
	}
};

/////////////////////////////////////////////////////////////////////////////
//	CLoaderEdit
/////////////////////////////////////////////////////////////////////////////
typedef long	(* FpActionControl)(LPVOID, LPVOID);
typedef LPVOID	(* FpInitEdit)(LPVOID);
typedef LPVOID	(* FpInitEdit2)(LPVOID, BOOL);
typedef void	(* FpExitEdit)(LPVOID);
typedef void	(* FpOnPaintEdit)(LPVOID);
typedef void	(* FpUseNumericType)(LPVOID, BOOL);
typedef void	(* FpOnChar)(LPVOID, UINT, UINT, UINT);
class CLoaderEdit : public CExLoadLib
{
public:
	CLoaderEdit(LPVOID lpPointer, BOOL bUseDefSkin) 
	{
// 		char szPath[MAX_PATH] = {0, };
// 		GetCurrentWorkDirectory(szPath, sizeof(szPath));
// 
// 		if(szPath)
// 			::sprintf(m_szDllName, "%s\\%s", szPath, EDIT_DLL);
// 		else
// 			::strcpy(m_szDllName, EDIT_DLL);

		//-->Solomon-090829수정. 
		CString szDir;
		GetDir_DrfnBin(szDir);
		::sprintf(m_szDllName, "%s\\%s", (LPCSTR)szDir, EDIT_DLL);
		//<--Solomon-090829수정.. 

		m_lpPointer = lpPointer;		// 초기화를 하기위해 임시로 저장

		pActionControl = NULL;
		pExitEdit = NULL;
		pInitEdit = NULL;
		pInitEdit2 = NULL;
		pOnPaintEdit = NULL;
		pUseNumericType = NULL;
		pOnChar = NULL;

		LoadLib();

		if(bUseDefSkin)
			m_lpPointer = pInitEdit2(m_lpPointer, bUseDefSkin);
		else
			m_lpPointer = pInitEdit(m_lpPointer);
	}

	virtual ~CLoaderEdit() 
	{
		if(m_hLib)
			pExitEdit(m_lpPointer);
	}

	virtual BOOL LoadProcAddress() 
	{
		if(!m_hLib)
		{
			TRACE0("CLoaderEdit GetProcAddress에서 Load에러 발생");
			return FALSE;
		}

		pActionControl	= (FpActionControl)::GetProcAddress(m_hLib, "Ex_ActionControl");
		pInitEdit	= (FpInitEdit)::GetProcAddress(m_hLib, "Ex_InitEdit");
		pInitEdit2	= (FpInitEdit2)::GetProcAddress(m_hLib, "Ex_InitEdit2");
		pExitEdit	= (FpExitEdit)::GetProcAddress(m_hLib, "Ex_ExitEdit");
		pOnPaintEdit	= (FpOnPaintEdit)::GetProcAddress(m_hLib, "Ex_OnPaintEdit");
		pUseNumericType	= (FpUseNumericType)::GetProcAddress(m_hLib, "Ex_UseNumericType");
		pOnChar	= (FpOnChar)::GetProcAddress(m_hLib, "Ex_OnChar_Edit");

		return TRUE;
	}

public:
	// Implementation
	LPVOID			m_lpPointer;

	FpActionControl		pActionControl;
	FpInitEdit		pInitEdit;
	FpInitEdit2		pInitEdit2;
	FpExitEdit		pExitEdit;
	FpOnPaintEdit		pOnPaintEdit;
	FpUseNumericType		pUseNumericType;
	FpOnChar		pOnChar;

public:
	// Function
	long ActionControl(LPVOID lpAction)
	{
		if(!m_hLib)
			return 0L;

		return pActionControl(m_lpPointer, lpAction);
	}

	void OnPaint()
	{
		if(!m_hLib)
			return;

		pOnPaintEdit(m_lpPointer);
	}
	
	void UseNumericType(BOOL bUse)
	{
		if(!m_hLib)
			return;

		pUseNumericType(m_lpPointer, bUse);
	}

	void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		if(!m_hLib)
			return;

		pOnChar(m_lpPointer, nChar, nRepCnt, nFlags);
	}
};

/////////////////////////////////////////////////////////////////////////////
//	CLoaderMaskEdit
/////////////////////////////////////////////////////////////////////////////
typedef LPVOID	(* FpInitEditMask)(LPVOID);
typedef LPVOID	(* FpInitEditMask2)(LPVOID, BOOL);
typedef LPVOID	(* FpExitEditMask)(LPVOID);
typedef BOOL	(* FpSetEditMask)(LPVOID, LPCTSTR, LPCTSTR, LPCTSTR);
typedef void	(* FpSetPromptChar)(LPVOID, TCHAR);
typedef BOOL	(* FpCanOverType)(LPVOID);
typedef BOOL	(* FpCanUseMask)(LPVOID);
typedef BOOL	(* FpIsModified)(LPVOID);
typedef BOOL	(* FpPosInRange)(LPVOID, int);
typedef TCHAR	(* FpGetPromptChar)(LPVOID);
typedef void	(* FpMaskClear)(LPVOID);
typedef void	(* FpMaskCopy)(LPVOID);
typedef void	(* FpMaskCut)(LPVOID);
typedef void	(* FpMaskPaste)(LPVOID);
typedef void	(* FpMaskSelectAll)(LPVOID);
typedef void	(* FpMaskUndo)(LPVOID);
typedef void	(* FpSetOverType)(LPVOID, BOOL);
typedef void	(* FpSetUseMask)(LPVOID, BOOL);
typedef BOOL	(* FpCheckChar)(LPVOID, UINT&, BOOL);
typedef BOOL	(* FpProcessMask)(LPVOID, UINT&, int);
typedef BOOL	(* FpPreTranslateMessage)(LPVOID, MSG*);
typedef BOOL	(* FpCorrectPosition)(LPVOID, int&, BOOL, BOOL);
typedef void	(* FpDeleteCharAt)(LPVOID, int);
typedef void	(* FpInsertCharAt)(LPVOID, int, TCHAR);
typedef BOOL	(* FpCopyToClipboard)(LPVOID, LPCTSTR);
typedef LPCTSTR	(* FpGetMaskedText)(LPVOID, int);
typedef BOOL	(* FpSelectionMade)(LPVOID);
typedef void	(* FpOnKeyDown)(LPVOID, UINT, UINT, UINT);
typedef void	(* FpOnLButtonDown)(LPVOID, UINT, CPoint);
typedef void	(* FpOnSetFocus)(LPVOID, CWnd*);
typedef void	(* FpOnLButtonDblClk)(LPVOID, UINT, CPoint);
typedef void	(* FpOnUpdateEditUndo)(LPVOID, CCmdUI*);
typedef BOOL	(* FpOnCommand)(LPVOID, WPARAM, LPARAM);
typedef void	(* FpOnPaintEdit)(LPVOID);
class CLoaderMaskEdit : public CExLoadLib
{
public:
	CLoaderMaskEdit(LPVOID lpPointer, BOOL bUseDefSkin)
	{
// 		char szPath[MAX_PATH] = {0, };
// 		GetCurrentWorkDirectory(szPath, sizeof(szPath));
// 
// 		if(szPath)
// 			::sprintf(m_szDllName, "%s\\%s", szPath, EDIT_DLL);
// 		else
// 			::strcpy(m_szDllName, EDIT_DLL);

		//-->Solomon-090829수정. 
		CString szDir;
		GetDir_DrfnBin(szDir);
		::sprintf(m_szDllName, "%s\\%s", (LPCSTR)szDir, EDIT_DLL);
		//<--Solomon-090829수정..

		m_lpPointer = lpPointer;		// 초기화를 하기위해 임시로 저장

		pActionControl = NULL;
		pExitEditMask = NULL;
		pInitEditMask = NULL;
		pInitEditMask2 = NULL;
		pSetEditMask = NULL;
		pSetPromptChar = NULL;
		pCanOverType = NULL;
		pCanUseMask = NULL;
		pIsModified = NULL;
		pPosInRange = NULL;
		pGetPromptChar = NULL;
		pMaskClear = NULL;
		pMaskCopy = NULL;
		pMaskCut = NULL;
		pMaskPaste = NULL;
		pMaskSelectAll = NULL;
		pMaskUndo = NULL;
		pSetOverType = NULL;
		pSetUseMask = NULL;
		pCheckChar = NULL;
		pProcessMask = NULL;
		pPreTranslateMessage = NULL;
		pCorrectPosition = NULL;
		pDeleteCharAt = NULL;
		pInsertCharAt = NULL;
		pCopyToClipboard = NULL;
		pGetMaskedText = NULL;
		pSelectionMade = NULL;
		pOnKeyDown = NULL;
		pOnChar = NULL;
		pOnLButtonDown = NULL;
		pOnSetFocus = NULL;
		pOnLButtonDblClk = NULL;
		pOnUpdateEditUndo = NULL;
		pOnCommand = NULL;
		pOnPaintEdit = NULL;

		LoadLib();

		if(bUseDefSkin)
			m_lpPointer = pInitEditMask2(m_lpPointer, bUseDefSkin);
		else
			m_lpPointer = pInitEditMask(m_lpPointer);
	}

	virtual ~CLoaderMaskEdit() 
	{
		if(m_hLib)
			pExitEditMask(m_lpPointer);
	}

	virtual BOOL LoadProcAddress() 
	{
		if(!m_hLib)
		{
			TRACE0("CLoaderMaskEdit GetProcAddress에서 Load에러 발생");
			return FALSE;
		}

		pActionControl	= (FpActionControl)::GetProcAddress(m_hLib, "Ex_ActionControl");
		pInitEditMask	= (FpInitEditMask)::GetProcAddress(m_hLib, "Ex_InitEditMask");
		pInitEditMask2	= (FpInitEditMask2)::GetProcAddress(m_hLib, "Ex_InitEditMask2");
		pExitEditMask	= (FpExitEditMask)::GetProcAddress(m_hLib, "Ex_ExitEditMask");
		pSetEditMask	= (FpSetEditMask)::GetProcAddress(m_hLib, "Ex_SetEditMask");
		pSetPromptChar	= (FpSetPromptChar)::GetProcAddress(m_hLib, "Ex_SetPromptChar");
		pCanOverType	= (FpCanOverType)::GetProcAddress(m_hLib, "Ex_CanOverType");
		pCanUseMask	= (FpCanUseMask)::GetProcAddress(m_hLib, "Ex_CanUseMask");
		pIsModified	= (FpIsModified)::GetProcAddress(m_hLib, "Ex_IsModified");
		pPosInRange	= (FpPosInRange)::GetProcAddress(m_hLib, "Ex_PosInRange");
		pGetPromptChar	= (FpGetPromptChar)::GetProcAddress(m_hLib, "Ex_GetPromptChar");
		pMaskClear	= (FpMaskClear)::GetProcAddress(m_hLib, "Ex_MaskClear");
		pMaskCopy	= (FpMaskCopy)::GetProcAddress(m_hLib, "Ex_MaskCopy");
		pMaskCut	= (FpMaskCut)::GetProcAddress(m_hLib, "Ex_MaskCut");
		pMaskPaste	= (FpMaskPaste)::GetProcAddress(m_hLib, "Ex_MaskPaste");
		pMaskSelectAll	= (FpMaskSelectAll)::GetProcAddress(m_hLib, "Ex_MaskSelectAll");
		pMaskUndo	= (FpMaskUndo)::GetProcAddress(m_hLib, "Ex_MaskUndo");
		pSetOverType	= (FpSetOverType)::GetProcAddress(m_hLib, "Ex_SetOverType");
		pSetUseMask	= (FpSetUseMask)::GetProcAddress(m_hLib, "Ex_SetUseMask");
		pCheckChar	= (FpCheckChar)::GetProcAddress(m_hLib, "Ex_CheckChar");
		pProcessMask	= (FpProcessMask)::GetProcAddress(m_hLib, "Ex_ProcessMask");
		pPreTranslateMessage	= (FpPreTranslateMessage)::GetProcAddress(m_hLib, "Ex_PreTranslateMessage");
		pCorrectPosition	= (FpCorrectPosition)::GetProcAddress(m_hLib, "Ex_CorrectPosition");
		pDeleteCharAt	= (FpDeleteCharAt)::GetProcAddress(m_hLib, "Ex_DeleteCharAt");
		pInsertCharAt	= (FpInsertCharAt)::GetProcAddress(m_hLib, "Ex_InsertCharAt");
		pCopyToClipboard	= (FpCopyToClipboard)::GetProcAddress(m_hLib, "Ex_CopyToClipboard");
		pGetMaskedText	= (FpGetMaskedText)::GetProcAddress(m_hLib, "Ex_GetMaskedText");
		pSelectionMade	= (FpSelectionMade)::GetProcAddress(m_hLib, "Ex_SelectionMade");
		pOnKeyDown	= (FpOnKeyDown)::GetProcAddress(m_hLib, "Ex_OnKeyDown_MaskEdit");
		pOnChar	= (FpOnChar)::GetProcAddress(m_hLib, "Ex_OnChar_MaskEdit");
		pOnLButtonDown	= (FpOnLButtonDown)::GetProcAddress(m_hLib, "Ex_OnLButtonDown_MaskEdit");
		pOnSetFocus	= (FpOnSetFocus)::GetProcAddress(m_hLib, "Ex_OnSetFocus_MaskEdit");
		pOnLButtonDblClk	= (FpOnLButtonDblClk)::GetProcAddress(m_hLib, "Ex_OnLButtonDblClk_MaskEdit");
		pOnUpdateEditUndo	= (FpOnUpdateEditUndo)::GetProcAddress(m_hLib, "Ex_OnUpdateEditUndo_MaskEdit");
		pOnCommand	= (FpOnCommand)::GetProcAddress(m_hLib, "Ex_OnCommand_MaskEdit");
		pOnPaintEdit	= (FpOnPaintEdit)::GetProcAddress(m_hLib, "Ex_OnPaintMaskEdit");

		return TRUE;
	}

public:
	// Implementation
	LPVOID			m_lpPointer;

	FpActionControl		pActionControl;
	FpInitEditMask	pInitEditMask;
	FpInitEditMask2	pInitEditMask2;
	FpExitEditMask	pExitEditMask;
	FpSetEditMask	pSetEditMask;
	FpSetPromptChar	pSetPromptChar;
	FpCanOverType	pCanOverType;
	FpCanUseMask	pCanUseMask;
	FpIsModified	pIsModified;
	FpPosInRange	pPosInRange;
	FpGetPromptChar	pGetPromptChar;
	FpMaskClear	pMaskClear;
	FpMaskCopy	pMaskCopy;
	FpMaskCut	pMaskCut;
	FpMaskPaste	pMaskPaste;
	FpMaskSelectAll	pMaskSelectAll;
	FpMaskUndo	pMaskUndo;
	FpSetOverType	pSetOverType;
	FpSetUseMask	pSetUseMask;
	FpCheckChar	pCheckChar;
	FpProcessMask	pProcessMask;
	FpPreTranslateMessage	pPreTranslateMessage;
	FpCorrectPosition	pCorrectPosition;
	FpDeleteCharAt	pDeleteCharAt;
	FpInsertCharAt	pInsertCharAt;
	FpCopyToClipboard	pCopyToClipboard;
	FpGetMaskedText	pGetMaskedText;
	FpSelectionMade	pSelectionMade;
	FpOnKeyDown	pOnKeyDown;
	FpOnChar	pOnChar;
	FpOnLButtonDown	pOnLButtonDown;
	FpOnSetFocus	pOnSetFocus;
	FpOnLButtonDblClk	pOnLButtonDblClk;
	FpOnUpdateEditUndo	pOnUpdateEditUndo;
	FpOnCommand	pOnCommand;
	FpOnPaintEdit	pOnPaintEdit;

public:
	// Function
	long ActionControl(LPVOID lpAction)
	{
		if(!m_hLib)
			return 0L;

		return pActionControl(m_lpPointer, lpAction);
	}

	BOOL SetEditMask(LPCTSTR lpszMask, LPCTSTR lpszLiteral, LPCTSTR lpszDefault)
	{
		if(!m_hLib)
			return FALSE;

		return pSetEditMask(m_lpPointer, lpszMask, lpszLiteral, lpszDefault);
	}

	void SetPromptChar(TCHAR ch)
	{
		if(!m_hLib)
			return;

		pSetPromptChar(m_lpPointer, ch);
	}

	BOOL CanOverType()
	{
		if(!m_hLib)
			return FALSE;

		return pCanOverType(m_lpPointer);
	}

	BOOL CanUseMask()
	{
		if(!m_hLib)
			return FALSE;

		return pCanUseMask(m_lpPointer);
	}

	BOOL IsModified()
	{
		if(!m_hLib)
			return FALSE;

		return pIsModified(m_lpPointer);
	}

	BOOL PosInRange(int nPos)
	{
		if(!m_hLib)
			return FALSE;

		return pPosInRange(m_lpPointer, nPos);
	}
	
	TCHAR GetPromptChar()
	{
		if(!m_hLib)
			return NULL;
		
		return pGetPromptChar(m_lpPointer);
	}

	void MaskClear()
	{
		if(!m_hLib)
			return;

		pMaskClear(m_lpPointer);
	}

	void MaskCopy()
	{
		if(!m_hLib)
			return;

		pMaskCopy(m_lpPointer);
	}
	
	void MaskCut()
	{
		if(!m_hLib)
			return;

		pMaskCut(m_lpPointer);
	}
	
	void MaskPaste()
	{
		if(!m_hLib)
			return;

		pMaskPaste(m_lpPointer);
	}
	
	void MaskSelectAll()
	{
		if(!m_hLib)
			return;

		pMaskSelectAll(m_lpPointer);
	}
	
	void MaskUndo()
	{
		if(!m_hLib)
			return;

		pMaskUndo(m_lpPointer);
	}
	
	void SetOverType(BOOL bOverType)
	{
		if(!m_hLib)
			return;

		pSetOverType(m_lpPointer, bOverType);
	}
	
	void SetUseMask(BOOL bUseMask)
	{
		if(!m_hLib)
			return;

		pSetUseMask(m_lpPointer, bUseMask);
	}

	BOOL CheckChar(UINT& nChar, BOOL bBeep)
	{
		if(!m_hLib)
			return FALSE;

		return pCheckChar(m_lpPointer, nChar, bBeep);
	}

	BOOL ProcessMask(UINT& nChar, int nEndPos)
	{
		if(!m_hLib)
			return FALSE;

		return pProcessMask(m_lpPointer, nChar, nEndPos);
	}

	BOOL PreTranslateMessage(MSG* pMsg)
	{
		if(!m_hLib)
			return FALSE;

		return pPreTranslateMessage(m_lpPointer, pMsg);
	}
	
	BOOL CorrectPosition(int& nPos, BOOL bForward, BOOL bBeep)
	{
		if(!m_hLib)
			return FALSE;
		
		return pCorrectPosition(m_lpPointer, nPos, bForward, bBeep);
	}
	
	void DeleteCharAt(int nPos)
	{
		if(!m_hLib)
			return;

		pDeleteCharAt(m_lpPointer, nPos);
	}
	
	void InsertCharAt(int nPos, TCHAR nChar)
	{
		if(!m_hLib)
			return;

		pInsertCharAt(m_lpPointer, nPos, nChar);
	}
	
	BOOL CopyToClipboard(LPCTSTR lpszText)
	{
		if(!m_hLib)
			return FALSE;
		
		return pCopyToClipboard(m_lpPointer, lpszText);
	}
	
	CString GetMaskedText(int nPos)
	{
		if(!m_hLib)
			return _T("");
		
		return pGetMaskedText(m_lpPointer, nPos);
	}
	
	BOOL SelectionMade()
	{
		if(!m_hLib)
			return FALSE;
		
		return pSelectionMade(m_lpPointer);
	}
	
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		if(!m_hLib)
			return;

		pOnKeyDown(m_lpPointer, nChar, nRepCnt, nFlags);
	}
	
	void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		if(!m_hLib)
			return;

		pOnChar(m_lpPointer, nChar, nRepCnt, nFlags);
	}
	
	void OnLButtonDown(UINT nFlags, CPoint point)
	{
		if(!m_hLib)
			return;

		pOnLButtonDown(m_lpPointer, nFlags, point);
	}
	
	void OnSetFocus(CWnd* pOldWnd)
	{
		if(!m_hLib)
			return;

		pOnSetFocus(m_lpPointer, pOldWnd);
	}
	
	void OnLButtonDblClk(UINT nFlags, CPoint point)
	{
		if(!m_hLib)
			return;

		pOnLButtonDblClk(m_lpPointer, nFlags, point);
	}
	
	void OnUpdateEditUndo(CCmdUI* pCmdUI)
	{
		if(!m_hLib)
			return;

		pOnUpdateEditUndo(m_lpPointer, pCmdUI);
	}
	
	BOOL OnCommand(WPARAM wParam, LPARAM lParam)
	{
		if(!m_hLib)
			return FALSE;
		
		return pOnCommand(m_lpPointer, wParam, lParam);
	}

	void OnPaint()
	{
		if(!m_hLib)
			return;

		pOnPaintEdit(m_lpPointer);
	}
};

/////////////////////////////////////////////////////////////////////////////
//	CLoaderAcctInput
/////////////////////////////////////////////////////////////////////////////
typedef void	(* FpExitAcctInput)(LPVOID);
typedef LPVOID	(* FpCreateAcctInputList)(LPVOID, CWnd*, CRect*, UINT, long, CStringList*);
typedef LPVOID	(* FpCreateAcctInputArray)(LPVOID, CWnd*, CRect*, UINT, long, LPCTSTR*, int);
typedef LPVOID	(* FpCreateAcctInputString)(LPVOID, CWnd*, CRect*, UINT, long, LPCTSTR);
typedef LPVOID	(* FpCreateAcctInputInt)(LPVOID, CWnd*, CRect*, UINT, long, UINT);
typedef long	(* FpGetBase)(LPVOID, LPCTSTR);
typedef void	(* FpSetFocus)(LPVOID);
typedef OLE_HANDLE	(* FpGetHWnd)(LPVOID);
typedef HWND	(* FpGetSafeHwnd)(LPVOID);
typedef void	(* FpSetText)(LPVOID, LPCTSTR);
typedef CString	(WINAPI* FpGetText_OCX)(LPVOID);
typedef CString	(WINAPI* FpGetDataText)(LPVOID);
typedef void	(* FpMoveWindow)(LPVOID, int, int, int, int, BOOL);
typedef void	(* FpMoveWindow2)(LPVOID, LPCRECT, BOOL);
typedef BOOL	(* FpSetWindowPos)(LPVOID, const CWnd*, int, int, int, int, UINT);
typedef BOOL	(* FpShowWindow)(LPVOID, int);
typedef void	(* FpGetWindowRect)(LPVOID, LPRECT);
typedef void	(* FpGetClientRect)(LPVOID, LPRECT);
typedef void	(* FpSetEnabled)(LPVOID, BOOL);
typedef BOOL	(* FpGetEnabled)(LPVOID);
class CLoaderAcctInput : public CExLoadLib
{
public:
	CLoaderAcctInput(LPVOID lpPointer, LPCSTR lpszRootPath = NULL) 
	{
// 		if(lpszRootPath)
// 			::sprintf(m_szDllName, "%s\\Bin\\%s", lpszRootPath, ACCTINPUT_DLL);
// 		else
// 		{
// 			char szPath[MAX_PATH] = {0, };
// 			GetCurrentWorkDirectory(szPath, sizeof(szPath));
// 
// 			if(szPath)
// 				::sprintf(m_szDllName, "%s\\%s", szPath, ACCTINPUT_DLL);
// 			else
// 				::strcpy(m_szDllName, ACCTINPUT_DLL);
// 		}

		//-->Solomon-090829수정. 
		CString szDir;
		GetDir_DrfnBin(szDir);
		::sprintf(m_szDllName, "%s\\%s", (LPCSTR)szDir, ACCTINPUT_DLL);
		//<--Solomon-090829수정..

		m_lpPointer = lpPointer;		// 초기화를 하기위해 임시로 저장

		pActionControl = NULL;
		pExitAcctInput = NULL;

		pCreateAcctInputList = NULL;
		pCreateAcctInputArray = NULL;
		pCreateAcctInputString = NULL;
		pCreateAcctInputList = NULL;
		pGetBase = NULL;
		pSetFocus = NULL;
		pGetHWnd = NULL;
		pGetSafeHwnd = NULL;
		pSetText = NULL;
		pGetText = NULL;
		pGetDataText = NULL;

		pMoveWindow = NULL;
		pMoveWindow2 = NULL;
		pSetWindowPos = NULL;
		pShowWindow = NULL;
		pGetWindowRect = NULL;
		pGetClientRect = NULL;

		pSetEnabled = NULL;
		pGetEnabled = NULL;

		LoadLib();
	}

	virtual ~CLoaderAcctInput() 
	{
		if(m_hLib)
			pExitAcctInput(m_lpPointer);
	}

	virtual BOOL LoadProcAddress() 
	{
		if(!m_hLib)
		{
			TRACE0("CLoaderAcctInput GetProcAddress에서 Load에러 발생");
			return FALSE;
		}

		pActionControl	= (FpActionControl)::GetProcAddress(m_hLib, "Ex_ActionControl");
		pExitAcctInput	= (FpExitAcctInput)::GetProcAddress(m_hLib, "Ex_ExitAcctInput");

		pCreateAcctInputList	= (FpCreateAcctInputList)::GetProcAddress(m_hLib, "Ex_CreateAcctInputList");
		pCreateAcctInputArray	= (FpCreateAcctInputArray)::GetProcAddress(m_hLib, "Ex_CreateAcctInputArray");
		pCreateAcctInputString	= (FpCreateAcctInputString)::GetProcAddress(m_hLib, "Ex_CreateAcctInputString");
		pCreateAcctInputInt	= (FpCreateAcctInputInt)::GetProcAddress(m_hLib, "Ex_CreateAcctInputInt");
		pGetBase	= (FpGetBase)::GetProcAddress(m_hLib, "Ex_GetBase");
		pSetFocus	= (FpSetFocus)::GetProcAddress(m_hLib, "Ex_SetFocus");
		pGetHWnd	= (FpGetHWnd)::GetProcAddress(m_hLib, "Ex_GetHWnd");
		pGetSafeHwnd	= (FpGetSafeHwnd)::GetProcAddress(m_hLib, "Ex_GetSafeHwnd");

		pSetText	= (FpSetText)::GetProcAddress(m_hLib, "Ex_SetText");
		pGetText	= (FpGetText_OCX)::GetProcAddress(m_hLib, "Ex_GetText");
		pGetDataText	= (FpGetDataText)::GetProcAddress(m_hLib, "Ex_GetDataText");

		pMoveWindow	= (FpMoveWindow)::GetProcAddress(m_hLib, "Ex_MoveWindow");
		pMoveWindow2	= (FpMoveWindow2)::GetProcAddress(m_hLib, "Ex_MoveWindow2");
		pSetWindowPos	= (FpSetWindowPos)::GetProcAddress(m_hLib, "Ex_SetWindowPos");
		pShowWindow	= (FpShowWindow)::GetProcAddress(m_hLib, "Ex_ShowWindow");
		pGetWindowRect	= (FpGetWindowRect)::GetProcAddress(m_hLib, "Ex_GetWindowRect");
		pGetClientRect	= (FpGetClientRect)::GetProcAddress(m_hLib, "Ex_GetClientRect");

		pSetEnabled	= (FpSetEnabled)::GetProcAddress(m_hLib, "Ex_SetEnabled");
		pGetEnabled	= (FpGetEnabled)::GetProcAddress(m_hLib, "Ex_GetEnabled");
		return TRUE;
	}

public:
	// Implementation
	LPVOID				m_lpPointer;

	FpActionControl		pActionControl;
	FpExitAcctInput	pExitAcctInput;

	FpCreateAcctInputList		pCreateAcctInputList;
	FpCreateAcctInputArray		pCreateAcctInputArray;
	FpCreateAcctInputString		pCreateAcctInputString;
	FpCreateAcctInputInt		pCreateAcctInputInt;
	FpGetBase		pGetBase;
	FpSetFocus		pSetFocus;
	FpGetHWnd		pGetHWnd;
	FpGetSafeHwnd		pGetSafeHwnd;

	FpSetText		pSetText;
	FpGetText_OCX		pGetText;
	FpGetDataText		pGetDataText;

	FpMoveWindow		pMoveWindow;
	FpMoveWindow2		pMoveWindow2;
	FpSetWindowPos		pSetWindowPos;
	FpShowWindow		pShowWindow;
	FpGetWindowRect		pGetWindowRect;
	FpGetClientRect		pGetClientRect;

	FpSetEnabled		pSetEnabled;
	FpGetEnabled		pGetEnabled;

public:
	// Function
	long ActionControl(LPVOID lpAction)
	{
		if(!m_hLib)
			return 0L;

		return pActionControl(m_lpPointer, lpAction);
	}

	void ExitAcctInput()
	{
		if(!m_hLib)
			return;

		pExitAcctInput(m_lpPointer);
	}

	LPVOID CreateAcctInput(CWnd* pWnd, CRect* pRect, UINT nID, long lBaseDesk, CStringList* pListProperty)
	{
		if(!m_hLib)
			return NULL;

		m_lpPointer = pCreateAcctInputList(m_lpPointer, pWnd, pRect, nID, lBaseDesk, pListProperty);
		return m_lpPointer;
	}

	LPVOID CreateAcctInput(CWnd* pWnd, CRect* pRect, UINT nID, long lBaseDesk, LPCTSTR* lpszProperties, int nSize)
	{
		if(!m_hLib)
			return NULL;

		m_lpPointer = pCreateAcctInputArray(m_lpPointer, pWnd, pRect, nID, lBaseDesk, lpszProperties, nSize);
		return m_lpPointer;
	}

	LPVOID CreateAcctInput(CWnd* pWnd, CRect* pRect, UINT nID, long lBaseDesk, LPCTSTR lpszProperties)
	{
		if(!m_hLib)
			return NULL;

		m_lpPointer = pCreateAcctInputString(m_lpPointer, pWnd, pRect, nID, lBaseDesk, lpszProperties);
		return m_lpPointer;
	}
	
	LPVOID CreateAcctInput(CWnd* pWnd, CRect* pRect, UINT nID, long lBaseDesk, UINT nProperties)
	{
		if(!m_hLib)
			return NULL;

		m_lpPointer = pCreateAcctInputInt(m_lpPointer, pWnd, pRect, nID, lBaseDesk, nProperties);
		return m_lpPointer;
	}

	long GetBase(LPCTSTR lpszKey)
	{
		if(!m_hLib)
			return 0L;

		return pGetBase(m_lpPointer, lpszKey);
	}

	void SetFocus()
	{
		if(!m_hLib)
			return;
		
		pSetFocus(m_lpPointer);
	}

	OLE_HANDLE GetHWnd()
	{
		if(!m_hLib)
			return 0;
		
		return pGetHWnd(m_lpPointer);
	}

	HWND GetSafeHwnd()
	{
		if(!m_hLib)
			return 0;
		
		return pGetSafeHwnd(m_lpPointer);
	}

	CString GetText()
	{
		if(!m_hLib)
			return _T("");

		return pGetText(m_lpPointer);
	}

	CString GetDataText()
	{
		if(!m_hLib)
			return _T("");

		return pGetDataText(m_lpPointer);
	}

	void SetText(LPCTSTR lpszText)
	{
		if(!m_hLib)
			return;

		pSetText(m_lpPointer, lpszText);
	}

	void MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint)
	{
		if(!m_hLib)
			return;

		pMoveWindow(m_lpPointer, x, y, nWidth, nHeight, bRepaint);
	}
	
	void MoveWindow(LPCRECT lpRect, BOOL bRepaint)
	{
		if(!m_hLib)
			return;

		pMoveWindow2(m_lpPointer, lpRect, bRepaint);
	}

	BOOL SetWindowPos(const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, UINT nFlags)
	{
		if(!m_hLib)
			return FALSE;

		return pSetWindowPos(m_lpPointer, pWndInsertAfter, x, y, cx, cy, nFlags);
	}

	BOOL ShowWindow(int nCmdShow)
	{
		if(!m_hLib)
			return FALSE;

		return pShowWindow(m_lpPointer, nCmdShow);
	}

	void GetWindowRect(LPRECT lpRect)
	{
		if(!m_hLib)
			return;

		pGetWindowRect(m_lpPointer, lpRect);
	}

	void GetClientRect(LPRECT lpRect)
	{
		if(!m_hLib)
			return;

		pGetClientRect(m_lpPointer, lpRect);
	}

	void SetEnabled(BOOL bEnable)
	{
		if(!m_hLib)
			return;

		pSetEnabled(m_lpPointer, bEnable);
	}

	BOOL GetEnabled()
	{
		if(!m_hLib)
			return FALSE;

		return pGetEnabled(m_lpPointer);
	}
};

/////////////////////////////////////////////////////////////////////////////
//	CLoaderCodeInput
/////////////////////////////////////////////////////////////////////////////
typedef LPVOID	(* FpCreateCodeInputList)(LPVOID, CWnd*, CRect*, UINT, long, CStringList*);
typedef LPVOID	(* FpCreateCodeInputArray)(LPVOID, CWnd*, CRect*, UINT, long, LPCTSTR*, int);
typedef LPVOID	(* FpCreateCodeInputString)(LPVOID, CWnd*, CRect*, UINT, long, LPCTSTR);
typedef LPVOID	(* FpCreateCodeInputInt)(LPVOID, CWnd*, CRect*, UINT, long, UINT);
typedef void	(* FpExitCodeInput)(LPVOID);
typedef short	(* FpGetGubunFromCode)(LPVOID, LPCTSTR);
typedef void	(* FpSetDataText)(LPVOID, LPCTSTR);
typedef void	(* FpSetDataText2)(LPVOID, LPCTSTR);
typedef CString	(WINAPI* FpGetDataText)(LPVOID);
typedef long	(* FpGetBase)(LPVOID, LPCTSTR);
typedef void	(* FpSetEnabled)(LPVOID, BOOL);
typedef BOOL	(* FpGetEnabled)(LPVOID);
typedef void	(* FpSetFocus)(LPVOID);
typedef void	(* FpSetCurSel)(LPVOID, short);
class CLoaderCodeInput : public CExLoadLib
{
public:
	CLoaderCodeInput(LPVOID lpPointer, LPCSTR lpszRootPath = NULL) 
	{
// 		if(lpszRootPath)
// 			::sprintf(m_szDllName, "%s\\Bin\\%s", lpszRootPath, CODEINPUT_DLL);
// 		else
// 		{
// 			char szPath[MAX_PATH] = {0, };
// 			GetCurrentWorkDirectory(szPath, sizeof(szPath));
// 
// 			if(szPath)
// 				::sprintf(m_szDllName, "%s\\%s", szPath, CODEINPUT_DLL);
// 			else
// 				::strcpy(m_szDllName, CODEINPUT_DLL);
// 		}

		//-->Solomon-090829수정. 
		CString szDir;
		GetDir_DrfnBin(szDir);
		::sprintf(m_szDllName, "%s\\%s", (LPCSTR)szDir, CODEINPUT_DLL);
		//<--Solomon-090829수정..

		m_lpPointer = lpPointer;		// 초기화를 하기위해 임시로 저장

		pActionControl = NULL;
		pCreateCodeInputList = NULL;
		pCreateCodeInputArray = NULL;
		pCreateCodeInputString = NULL;
		pCreateCodeInputList = NULL;
		pExitCodeInput = NULL;

		pGetGubunFromCode = NULL;

		pSetDataText = NULL;
		pSetDataText2 = NULL;
		pGetDataText = NULL;

		pSetEnabled = NULL;
		pGetEnabled = NULL;
		
		pGetBase = NULL;

		pSetFocus = NULL;

		pSetCurSel = NULL;

		pGetHWnd = NULL;
		pGetSafeHwnd = NULL;

		pMoveWindow = NULL;
		pMoveWindow2 = NULL;
		pSetWindowPos = NULL;
		pShowWindow = NULL;
		pGetWindowRect = NULL;
		pGetClientRect = NULL;

		LoadLib();
	}

	virtual ~CLoaderCodeInput() 
	{
		if(m_hLib)
			pExitCodeInput(m_lpPointer);
	}

	virtual BOOL LoadProcAddress() 
	{
		if(!m_hLib)
		{
			TRACE0("CLoaderCodeInput GetProcAddress에서 Load에러 발생");
			return FALSE;
		}

		pActionControl	= (FpActionControl)::GetProcAddress(m_hLib, "Ex_ActionControl");
		pCreateCodeInputList	= (FpCreateCodeInputList)::GetProcAddress(m_hLib, "Ex_CreateCodeInputList");
		pCreateCodeInputArray	= (FpCreateCodeInputArray)::GetProcAddress(m_hLib, "Ex_CreateCodeInputArray");
		pCreateCodeInputString	= (FpCreateCodeInputString)::GetProcAddress(m_hLib, "Ex_CreateCodeInputString");
		pCreateCodeInputInt	= (FpCreateCodeInputInt)::GetProcAddress(m_hLib, "Ex_CreateCodeInputInt");
		pExitCodeInput	= (FpExitCodeInput)::GetProcAddress(m_hLib, "Ex_ExitCodeInput");

		pGetGubunFromCode	= (FpGetGubunFromCode)::GetProcAddress(m_hLib, "Ex_GetGubunFromCode");

		pSetDataText	= (FpSetDataText)::GetProcAddress(m_hLib, "Ex_SetDataText");
		pSetDataText2	= (FpSetDataText2)::GetProcAddress(m_hLib, "Ex_SetDataText2");
		pGetDataText	= (FpGetDataText)::GetProcAddress(m_hLib, "Ex_GetDataText");

		pSetEnabled	= (FpSetEnabled)::GetProcAddress(m_hLib, "Ex_SetEnabled");
		pGetEnabled	= (FpGetEnabled)::GetProcAddress(m_hLib, "Ex_GetEnabled");

		pGetBase	= (FpGetBase)::GetProcAddress(m_hLib, "Ex_GetBase");

		pSetFocus	= (FpSetFocus)::GetProcAddress(m_hLib, "Ex_SetFocus");

		pSetCurSel	= (FpSetCurSel)::GetProcAddress(m_hLib, "Ex_SetCurSel");

		pGetHWnd	= (FpGetHWnd)::GetProcAddress(m_hLib, "Ex_GetHWnd");
		pGetSafeHwnd	= (FpGetSafeHwnd)::GetProcAddress(m_hLib, "Ex_GetSafeHwnd");

		pMoveWindow	= (FpMoveWindow)::GetProcAddress(m_hLib, "Ex_MoveWindow");
		pMoveWindow2	= (FpMoveWindow2)::GetProcAddress(m_hLib, "Ex_MoveWindow2");
		pSetWindowPos	= (FpSetWindowPos)::GetProcAddress(m_hLib, "Ex_SetWindowPos");
		pShowWindow	= (FpShowWindow)::GetProcAddress(m_hLib, "Ex_ShowWindow");
		pGetWindowRect	= (FpGetWindowRect)::GetProcAddress(m_hLib, "Ex_GetWindowRect");
		pGetClientRect	= (FpGetClientRect)::GetProcAddress(m_hLib, "Ex_GetClientRect");
		return TRUE;
	}

public:
	// Implementation
	LPVOID				m_lpPointer;

	FpActionControl		pActionControl;

	FpCreateCodeInputList		pCreateCodeInputList;
	FpCreateCodeInputArray		pCreateCodeInputArray;
	FpCreateCodeInputString		pCreateCodeInputString;
	FpCreateCodeInputInt		pCreateCodeInputInt;
	FpExitCodeInput		pExitCodeInput;

	FpGetGubunFromCode		pGetGubunFromCode;

	FpSetDataText		pSetDataText;
	FpSetDataText2		pSetDataText2;
	FpGetDataText		pGetDataText;

	FpSetEnabled		pSetEnabled;
	FpGetEnabled		pGetEnabled;
	
	FpGetBase		pGetBase;

	FpSetFocus		pSetFocus;
	FpSetCurSel		pSetCurSel;

	FpGetHWnd		pGetHWnd;
	FpGetSafeHwnd		pGetSafeHwnd;

	FpMoveWindow		pMoveWindow;
	FpMoveWindow2		pMoveWindow2;
	FpSetWindowPos		pSetWindowPos;
	FpShowWindow		pShowWindow;
	FpGetWindowRect		pGetWindowRect;
	FpGetClientRect		pGetClientRect;

public:
	// Function
	long ActionControl(LPVOID lpAction)
	{
		if(!m_hLib)
			return 0L;

		return pActionControl(m_lpPointer, lpAction);
	}
	
	void ExitCodeInput()
	{
		if(!m_hLib)
			return;

		pExitCodeInput(m_lpPointer);
	}

	LPVOID CreateCodeInput(CWnd* pWnd, CRect* pRect, UINT nID, long lBaseDesk, CStringList* pListProperty)
	{
		if(!m_hLib)
			return FALSE;

		m_lpPointer = pCreateCodeInputList(m_lpPointer, pWnd, pRect, nID, lBaseDesk, pListProperty);
		return m_lpPointer;
	}

	LPVOID CreateCodeInput(CWnd* pWnd, CRect* pRect, UINT nID, long lBaseDesk, LPCTSTR* lpszProperties, int nSize)
	{
		if(!m_hLib)
			return FALSE;

		m_lpPointer = pCreateCodeInputArray(m_lpPointer, pWnd, pRect, nID, lBaseDesk, lpszProperties, nSize);
		return m_lpPointer;
	}

	LPVOID CreateCodeInput(CWnd* pWnd, CRect* pRect, UINT nID, long lBaseDesk, LPCTSTR lpszProperties)
	{
		if(!m_hLib)
			return FALSE;

		m_lpPointer = pCreateCodeInputString(m_lpPointer, pWnd, pRect, nID, lBaseDesk, lpszProperties);
		return m_lpPointer;
	}
	
	LPVOID CreateCodeInput(CWnd* pWnd, CRect* pRect, UINT nID, long lBaseDesk, UINT nProperties)
	{
		if(!m_hLib)
			return FALSE;

		m_lpPointer = pCreateCodeInputInt(m_lpPointer, pWnd, pRect, nID, lBaseDesk, nProperties);
		return m_lpPointer;
	}

	short GetGubunFromCode(LPCTSTR szCode)
	{
		if(!m_hLib)
			return -1;

		return pGetGubunFromCode(m_lpPointer, szCode);
	}

	void SetDataText(LPCTSTR szText)
	{
		if(!m_hLib)
			return;

		pSetDataText(m_lpPointer, szText);
	}

	void SetDataText2(LPCTSTR szCode)
	{
		if(!m_hLib)
			return;

		pSetDataText2(m_lpPointer, szCode);
	}

	CString GetDataText()
	{
		if(!m_hLib)
			return _T("");

		return pGetDataText(m_lpPointer);
	}

	void SetEnabled(BOOL bEnable)
	{
		if(!m_hLib)
			return;

		pSetEnabled(m_lpPointer, bEnable);
	}

	BOOL GetEnabled()
	{
		if(!m_hLib)
			return FALSE;

		return pGetEnabled(m_lpPointer);
	}

	long GetBase(LPCTSTR lpszKey)
	{
		if(!m_hLib)
			return 0L;

		return pGetBase(m_lpPointer, lpszKey);
	}

	void SetFocus()
	{
		if(!m_hLib)
			return;

		pSetFocus(m_lpPointer);
	}

	void SetCurSel(short nIndex)
	{
		if(!m_hLib)
			return;
		
		pSetCurSel(m_lpPointer, nIndex);
	}

	OLE_HANDLE GetHWnd()
	{
		if(!m_hLib)
			return 0;
		
		return pGetHWnd(m_lpPointer);
	}

	HWND GetSafeHwnd()
	{
		if(!m_hLib)
			return 0;
		
		return pGetSafeHwnd(m_lpPointer);
	}

	void MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint)
	{
		if(!m_hLib)
			return;

		pMoveWindow(m_lpPointer, x, y, nWidth, nHeight, bRepaint);
	}
	
	void MoveWindow(LPCRECT lpRect, BOOL bRepaint)
	{
		if(!m_hLib)
			return;

		pMoveWindow2(m_lpPointer, lpRect, bRepaint);
	}

	BOOL SetWindowPos(const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, UINT nFlags)
	{
		if(!m_hLib)
			return FALSE;

		return pSetWindowPos(m_lpPointer, pWndInsertAfter, x, y, cx, cy, nFlags);
	}

	BOOL ShowWindow(int nCmdShow)
	{
		if(!m_hLib)
			return FALSE;

		return pShowWindow(m_lpPointer, nCmdShow);
	}

	void GetWindowRect(LPRECT lpRect)
	{
		if(!m_hLib)
			return;

		pGetWindowRect(m_lpPointer, lpRect);
	}

	void GetClientRect(LPRECT lpRect)
	{
		if(!m_hLib)
			return;

		pGetClientRect(m_lpPointer, lpRect);
	}
};

/////////////////////////////////////////////////////////////////////////////
//	CLoaderSpinInput
/////////////////////////////////////////////////////////////////////////////
typedef LPVOID	(* FpCreateSpinInputList)(LPVOID, CWnd*, CRect*, UINT, long, CStringList*);
typedef LPVOID	(* FpCreateSpinInputArray)(LPVOID, CWnd*, CRect*, UINT, long, LPCTSTR*, int);
typedef LPVOID	(* FpCreateSpinInputString)(LPVOID, CWnd*, CRect*, UINT, long, LPCTSTR);
typedef LPVOID	(* FpCreateSpinInputInt)(LPVOID, CWnd*, CRect*, UINT, long, UINT);
typedef void	(* FpExitSpinInput)(LPVOID);
typedef void	(* FpSetInitValue)(LPVOID, long);
typedef long	(* FpGetInitValue)(LPVOID);
typedef void	(* FpSetDataText)(LPVOID, LPCTSTR);
typedef CString	(WINAPI* FpGetData)(LPVOID);
typedef void	(* FpSetEnabled)(LPVOID, BOOL);
typedef BOOL	(* FpGetEnabled)(LPVOID);
typedef long	(* FpGetBase)(LPVOID, LPCTSTR);
typedef void	(* FpSetVariation)(LPVOID, long);
typedef long	(* FpGetVariation)(LPVOID);
typedef void	(* FpSetFocus)(LPVOID);
typedef void	(* FpSetRange)(LPVOID, int, int);
typedef void	(* FpClearEditText)(LPVOID);
class CLoaderSpinInput : public CExLoadLib
{
public:
	CLoaderSpinInput(LPVOID lpPointer, LPCSTR lpszRootPath = NULL) 
	{
// 		if(lpszRootPath)
// 			::sprintf(m_szDllName, "%s\\Bin\\%s", lpszRootPath, SPININPUT_DLL);
// 		else
// 		{
// 			char szPath[MAX_PATH] = {0, };
// 			GetCurrentWorkDirectory(szPath, sizeof(szPath));
// 
// 			if(szPath)
// 				::sprintf(m_szDllName, "%s\\%s", szPath, SPININPUT_DLL);
// 			else
// 				::strcpy(m_szDllName, SPININPUT_DLL);
// 		}

		//-->Solomon-090829수정. 
		CString szDir;
		GetDir_DrfnBin(szDir);
		::sprintf(m_szDllName, "%s\\%s", (LPCSTR)szDir, SPININPUT_DLL);
		//<--Solomon-090829수정..

//		m_lpPointer = lpPointer;		// 초기화를 하기위해 임시로 저장

		pActionControl = NULL;
		pExitSpinInput = NULL;

		pCreateSpinInputList = NULL;
		pCreateSpinInputArray = NULL;
		pCreateSpinInputString = NULL;
		pCreateSpinInputList = NULL;

		pGetInitValue = NULL;
		pSetInitValue = NULL;

		pSetDataText = NULL;
		pGetData = NULL;

		pSetEnabled = NULL;
		pGetEnabled = NULL;

		pSetVariation = NULL;
		pGetVariation = NULL;

		pGetBase = NULL;

		pSetFocus = NULL;
		pGetHWnd = NULL;
		pGetSafeHwnd = NULL;

		pSetRange = NULL;
		pClearEditText = NULL;

		pMoveWindow = NULL;
		pMoveWindow2 = NULL;
		pSetWindowPos = NULL;
		pShowWindow = NULL;
		pGetWindowRect = NULL;
		pGetClientRect = NULL;

		LoadLib();
	}

	virtual ~CLoaderSpinInput() 
	{
		if(m_hLib)
			pExitSpinInput(m_lpPointer);
	}

	virtual BOOL LoadProcAddress() 
	{
		if(!m_hLib)
		{
			TRACE0("CLoaderSpinInput GetProcAddress에서 Load에러 발생");
			return FALSE;
		}

		pActionControl	= (FpActionControl)::GetProcAddress(m_hLib, "Ex_ActionControl");
		pExitSpinInput	= (FpExitSpinInput)::GetProcAddress(m_hLib, "Ex_ExitSpinInput");

		pCreateSpinInputList	= (FpCreateSpinInputList)::GetProcAddress(m_hLib, "Ex_CreateSpinInputList");
		pCreateSpinInputArray	= (FpCreateSpinInputArray)::GetProcAddress(m_hLib, "Ex_CreateSpinInputArray");
		pCreateSpinInputString	= (FpCreateSpinInputString)::GetProcAddress(m_hLib, "Ex_CreateSpinInputString");
		pCreateSpinInputInt	= (FpCreateSpinInputInt)::GetProcAddress(m_hLib, "Ex_CreateSpinInputInt");

		pSetInitValue	= (FpSetInitValue)::GetProcAddress(m_hLib, "Ex_SetInitValue");
		pGetInitValue	= (FpGetInitValue)::GetProcAddress(m_hLib, "Ex_GetInitValue");

		pSetDataText	= (FpSetDataText)::GetProcAddress(m_hLib, "Ex_SetDataText");
		pGetData	= (FpGetData)::GetProcAddress(m_hLib, "Ex_GetData");

		pSetEnabled	= (FpSetEnabled)::GetProcAddress(m_hLib, "Ex_SetEnabled");
		pGetEnabled	= (FpGetEnabled)::GetProcAddress(m_hLib, "Ex_GetEnabled");
		
		pSetVariation	= (FpSetVariation)::GetProcAddress(m_hLib, "Ex_SetVariation");
		pGetVariation	= (FpGetVariation)::GetProcAddress(m_hLib, "Ex_GetVariation");

		pGetBase	= (FpGetBase)::GetProcAddress(m_hLib, "Ex_GetBase");

		pSetFocus	= (FpSetFocus)::GetProcAddress(m_hLib, "Ex_SetFocus");
		pGetHWnd	= (FpGetHWnd)::GetProcAddress(m_hLib, "Ex_GetHWnd");
		pGetSafeHwnd	= (FpGetSafeHwnd)::GetProcAddress(m_hLib, "Ex_GetSafeHwnd");

		pSetRange	= (FpSetRange)::GetProcAddress(m_hLib, "Ex_SetRange");
		pClearEditText	= (FpClearEditText)::GetProcAddress(m_hLib, "Ex_ClearEditText");

		pMoveWindow	= (FpMoveWindow)::GetProcAddress(m_hLib, "Ex_MoveWindow");
		pMoveWindow2	= (FpMoveWindow2)::GetProcAddress(m_hLib, "Ex_MoveWindow2");
		pSetWindowPos	= (FpSetWindowPos)::GetProcAddress(m_hLib, "Ex_SetWindowPos");
		pShowWindow	= (FpShowWindow)::GetProcAddress(m_hLib, "Ex_ShowWindow");
		pGetWindowRect	= (FpGetWindowRect)::GetProcAddress(m_hLib, "Ex_GetWindowRect");
		pGetClientRect	= (FpGetClientRect)::GetProcAddress(m_hLib, "Ex_GetClientRect");
		return TRUE;
	}

public:
	// Implementation
	LPVOID				m_lpPointer;

	FpActionControl		pActionControl;
	FpExitSpinInput			pExitSpinInput;

	FpCreateSpinInputList		pCreateSpinInputList;
	FpCreateSpinInputArray		pCreateSpinInputArray;
	FpCreateSpinInputString		pCreateSpinInputString;
	FpCreateSpinInputInt		pCreateSpinInputInt;

	FpSetInitValue		pSetInitValue;
	FpGetInitValue		pGetInitValue;

	FpSetDataText		pSetDataText;
	FpGetData		pGetData;

	FpSetEnabled		pSetEnabled;
	FpGetEnabled		pGetEnabled;

	FpSetVariation		pSetVariation;
	FpGetVariation		pGetVariation;

	FpGetBase		pGetBase;

	FpSetFocus		pSetFocus;
	FpGetHWnd		pGetHWnd;
	FpGetSafeHwnd		pGetSafeHwnd;

	FpSetRange		pSetRange;
	FpClearEditText		pClearEditText;

	FpMoveWindow		pMoveWindow;
	FpMoveWindow2		pMoveWindow2;
	FpSetWindowPos		pSetWindowPos;
	FpShowWindow		pShowWindow;
	FpGetWindowRect		pGetWindowRect;
	FpGetClientRect		pGetClientRect;

public:
	// Function
	long ActionControl(LPVOID lpAction)
	{
		if(!m_hLib)
			return 0L;

		return pActionControl(m_lpPointer, lpAction);
	}

	void ExitSpinInput()
	{
		if(!m_hLib)
			return;
		
		pExitSpinInput(m_lpPointer);
	}

	LPVOID CreateSpinInput(CWnd* pWnd, CRect* pRect, UINT nID, long lBaseDesk, CStringList* pListProperty)
	{
		if(!m_hLib)
			return FALSE;

		m_lpPointer = pCreateSpinInputList(m_lpPointer, pWnd, pRect, nID, lBaseDesk, pListProperty);
		return m_lpPointer;
	}

	LPVOID CreateSpinInput(CWnd* pWnd, CRect* pRect, UINT nID, long lBaseDesk, LPCTSTR* lpszProperties, int nSize)
	{
		if(!m_hLib)
			return FALSE;

		m_lpPointer = pCreateSpinInputArray(m_lpPointer, pWnd, pRect, nID, lBaseDesk, lpszProperties, nSize);
		return m_lpPointer;
	}

	LPVOID CreateSpinInput(CWnd* pWnd, CRect* pRect, UINT nID, long lBaseDesk, LPCTSTR lpszProperties)
	{
		if(!m_hLib)
			return FALSE;

		m_lpPointer = pCreateSpinInputString(m_lpPointer, pWnd, pRect, nID, lBaseDesk, lpszProperties);
		return m_lpPointer;
	}
	
	LPVOID CreateSpinInput(CWnd* pWnd, CRect* pRect, UINT nID, long lBaseDesk, UINT nProperties)
	{
		if(!m_hLib)
			return FALSE;

		m_lpPointer = pCreateSpinInputInt(m_lpPointer, pWnd, pRect, nID, lBaseDesk, nProperties);
		return m_lpPointer;
	}

	void SetInitValue(long lValue)
	{
		if(!m_hLib)
			return;

		pSetInitValue(m_lpPointer, lValue);
	}

	long GetInitValue()
	{
		if(!m_hLib)
			return 0L;

		return pGetInitValue(m_lpPointer);
	}

	CString GetData()
	{
		if(!m_hLib)
			return _T("");

		return pGetData(m_lpPointer);
	}

	void SetDataText(LPCTSTR lpszText)
	{
		if(!m_hLib)
			return;

		pSetDataText(m_lpPointer, lpszText);
	}

	void SetEnabled(BOOL bEnable)
	{
		if(!m_hLib)
			return;

		pSetEnabled(m_lpPointer, bEnable);
	}

	BOOL GetEnabled()
	{
		if(!m_hLib)
			return FALSE;

		return pGetEnabled(m_lpPointer);
	}
	
	void SetVariation(long lValue)
	{
		if(!m_hLib)
			return;

		pSetVariation(m_lpPointer, lValue);
	}

	long GetVariation()
	{
		if(!m_hLib)
			return 0L;

		return pGetVariation(m_lpPointer);
	}

	long GetBase(LPCTSTR lpszKey)
	{
		if(!m_hLib)
			return 0L;

		return pGetBase(m_lpPointer, lpszKey);
	}

	void SetFocus()
	{
		if(!m_hLib)
			return;

		pSetFocus(m_lpPointer);
	}

	OLE_HANDLE GetHWnd()
	{
		if(!m_hLib)
			return 0;
		
		return pGetHWnd(m_lpPointer);
	}

	HWND GetSafeHwnd()
	{
		if(!m_hLib)
			return 0;
		
		return pGetSafeHwnd(m_lpPointer);
	}

	void SetRange(int nMin, int nMax)
	{
		if(!m_hLib)
			return;
		
		pSetRange(m_lpPointer, nMin, nMax);
	}

	void ClearEditText()
	{
		if(!m_hLib)
			return;
		
		pClearEditText(m_lpPointer);
	}

	void MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint)
	{
		if(!m_hLib)
			return;

		pMoveWindow(m_lpPointer, x, y, nWidth, nHeight, bRepaint);
	}
	
	void MoveWindow(LPCRECT lpRect, BOOL bRepaint)
	{
		if(!m_hLib)
			return;

		pMoveWindow2(m_lpPointer, lpRect, bRepaint);
	}

	BOOL SetWindowPos(const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, UINT nFlags)
	{
		if(!m_hLib)
			return FALSE;

		return pSetWindowPos(m_lpPointer, pWndInsertAfter, x, y, cx, cy, nFlags);
	}

	BOOL ShowWindow(int nCmdShow)
	{
		if(!m_hLib)
			return FALSE;

		return pShowWindow(m_lpPointer, nCmdShow);
	}

	void GetWindowRect(LPRECT lpRect)
	{
		if(!m_hLib)
			return;

		pGetWindowRect(m_lpPointer, lpRect);
	}

	void GetClientRect(LPRECT lpRect)
	{
		if(!m_hLib)
			return;

		pGetClientRect(m_lpPointer, lpRect);
	}
};

/////////////////////////////////////////////////////////////////////////////
//	CLoaderTabCtrl
/////////////////////////////////////////////////////////////////////////////
typedef void	(* FpExitTabCtrl)(LPVOID);
typedef LPVOID	(* FpInitTabCtrl)(LPVOID);
typedef LPVOID	(* FpInitTabCtrl2)(LPVOID, BOOL);
typedef void	(* FpPreSubclassWindow)(LPVOID);
typedef BOOL	(* FpOnEraseBkgnd)(LPVOID, CDC*);
typedef void	(* FpDrawItem)(LPVOID, LPDRAWITEMSTRUCT);
typedef void	(* FpInsertItem1)(LPVOID, int, LPSTR, UINT);
typedef void	(* FpInsertItem2)(LPVOID, int, LPSTR, int, UINT);
typedef void	(* FpInsertItem3)(LPVOID, int, TCITEM*, UINT);
typedef void	(* FpOnMouseMove)(LPVOID, UINT, CPoint);
typedef	void	(* FpOnPaint)(LPVOID, CDC*);
typedef	void	(* FpOnHScroll)(LPVOID, UINT, UINT, CScrollBar*);
typedef BOOL	(* FpOnSelchange)(LPVOID, NMHDR*, LRESULT*);
typedef BOOL	(* FpOnSelchanging)(LPVOID, NMHDR*, LRESULT*);
typedef LRESULT	(* FpOnMouseLeave)(LPVOID, WPARAM, LPARAM);
typedef	void	(* FpOnKillFocus)(LPVOID, CWnd*);
typedef	void	(* FpOnSetFocus)(LPVOID, CWnd*);
typedef LRESULT	(* FpWindowProc)(LPVOID, UINT, WPARAM, LPARAM);
typedef void	(* FpAddChildControl)(LPVOID, CWnd*);
typedef void	(* FpRemoveChildControl)(LPVOID, CWnd*);
typedef void	(* FpSetTabColor)(LPVOID, COLORREF, COLORREF, COLORREF);
typedef void	(* FpSetSTTabColor)(LPVOID, long);
typedef void	(* FpOnSize)(LPVOID, UINT, int, int);
class CLoaderTabCtrl : public CExLoadLib
{
public:
	CLoaderTabCtrl(LPVOID lpPointer, BOOL bUseDefSkin) 
	{
// 		char szPath[MAX_PATH] = {0, };
// 		GetCurrentWorkDirectory(szPath, sizeof(szPath));
// 
// 		if(szPath)
// 			::sprintf(m_szDllName, "%s\\%s", szPath, TABCTRL_DLL);
// 		else
// 			::strcpy(m_szDllName, TABCTRL_DLL);

		//-->Solomon-090829수정. 
		CString szDir;
		GetDir_DrfnBin(szDir);
		::sprintf(m_szDllName, "%s\\%s", (LPCSTR)szDir, TABCTRL_DLL);
		//<--Solomon-090829수정..

		pInitTabCtrl = NULL;
		pInitTabCtrl2 = NULL;
		pActionControl = NULL;
		pExitTabCtrl = NULL;
		pPreSubclassWindow = NULL;
		pOnEraseBkgnd = NULL;
		pDrawItem = NULL;
		pInsertItem1 = NULL;
		pInsertItem2 = NULL;
		pInsertItem3 = NULL;
		pOnMouseMove = NULL;
		pOnPaint = NULL;
		pOnHScroll = NULL;
		pOnSelchange = NULL;
		pOnSelchanging = NULL;
		pOnMouseLeave = NULL;
		pOnKillFocus = NULL;
		pOnSetFocus = NULL;
		pWindowProc = NULL;
		pAddChildControl = NULL;
		pRemoveChildControl = NULL;
		pSetTabColor = NULL;
		pSetSTTabColor = NULL;
		pOnSize = NULL;

		LoadLib();

		if(bUseDefSkin)
			m_lpPointer = pInitTabCtrl2(lpPointer, bUseDefSkin);
		else
			m_lpPointer = pInitTabCtrl(lpPointer);
	}

	virtual ~CLoaderTabCtrl() 
	{
		if(m_hLib)
			pExitTabCtrl(m_lpPointer);
	}

	virtual BOOL LoadProcAddress() 
	{
		if(!m_hLib)
		{
			TRACE0("CLoaderTabCtrl GetProcAddress에서 Load에러 발생");
			return FALSE;
		}

		pActionControl	= (FpActionControl)::GetProcAddress(m_hLib, "Ex_ActionControl");
		pInitTabCtrl	= (FpInitTabCtrl)::GetProcAddress(m_hLib, "Ex_InitTabCtrlDraw");
		pInitTabCtrl2	= (FpInitTabCtrl2)::GetProcAddress(m_hLib, "Ex_InitTabCtrlDraw2");
		pExitTabCtrl	= (FpExitTabCtrl)::GetProcAddress(m_hLib, "Ex_ExitTabCtrl");
		pPreSubclassWindow	= (FpPreSubclassWindow)::GetProcAddress(m_hLib, "Ex_PreSubclassWindow");
		pOnEraseBkgnd	= (FpOnEraseBkgnd)::GetProcAddress(m_hLib, "Ex_OnEraseBkgnd");
		pDrawItem	= (FpDrawItem)::GetProcAddress(m_hLib, "Ex_DrawItem");
		pInsertItem1	= (FpInsertItem1)::GetProcAddress(m_hLib, "Ex_InsertItem1");
		pInsertItem2	= (FpInsertItem2)::GetProcAddress(m_hLib, "Ex_InsertItem2");
		pInsertItem3	= (FpInsertItem3)::GetProcAddress(m_hLib, "Ex_InsertItem3");
		pOnMouseMove	= (FpOnMouseMove)::GetProcAddress(m_hLib, "Ex_OnMouseMove");
		pOnPaint	= (FpOnPaint)::GetProcAddress(m_hLib, "Ex_OnPaint");
		pOnHScroll	= (FpOnHScroll)::GetProcAddress(m_hLib, "Ex_OnHScroll");
		pOnSelchange	= (FpOnSelchange)::GetProcAddress(m_hLib, "Ex_OnSelchange");
		pOnSelchanging	= (FpOnSelchanging)::GetProcAddress(m_hLib, "Ex_OnSelchanging");
		pOnMouseLeave	= (FpOnMouseLeave)::GetProcAddress(m_hLib, "Ex_OnMouseLeave");
		pOnKillFocus	= (FpOnKillFocus)::GetProcAddress(m_hLib, "Ex_OnKillFocus");
		pOnSetFocus	= (FpOnSetFocus)::GetProcAddress(m_hLib, "Ex_OnSetFocus");
		pWindowProc	= (FpWindowProc)::GetProcAddress(m_hLib, "Ex_WindowProc");
		pAddChildControl	= (FpAddChildControl)::GetProcAddress(m_hLib, "Ex_AddChildControl");
		pRemoveChildControl	= (FpRemoveChildControl)::GetProcAddress(m_hLib, "Ex_RemoveChildControl");
		pSetTabColor	= (FpSetTabColor)::GetProcAddress(m_hLib, "Ex_SetTabColor");
		pSetSTTabColor	= (FpSetSTTabColor)::GetProcAddress(m_hLib, "Ex_SetSTTabColor");
		pOnSize	= (FpOnSize)::GetProcAddress(m_hLib, "Ex_OnSize");

		return TRUE;
	}

public:
	// Implementation
	LPVOID				m_lpPointer;

	FpActionControl		pActionControl;
	FpInitTabCtrl			pInitTabCtrl;
	FpInitTabCtrl2			pInitTabCtrl2;
	FpExitTabCtrl			pExitTabCtrl;
	FpPreSubclassWindow		pPreSubclassWindow;
	FpOnEraseBkgnd			pOnEraseBkgnd;
	FpDrawItem			pDrawItem;
	FpInsertItem1			pInsertItem1;
	FpInsertItem2			pInsertItem2;
	FpInsertItem3			pInsertItem3;
	FpOnMouseMove			pOnMouseMove;
	FpOnPaint			pOnPaint;
	FpOnHScroll			pOnHScroll;
	FpOnSelchange			pOnSelchange;
	FpOnSelchanging			pOnSelchanging;
	FpOnMouseLeave			pOnMouseLeave;
	FpOnKillFocus			pOnKillFocus;
	FpOnSetFocus			pOnSetFocus;
	FpWindowProc			pWindowProc;
	FpAddChildControl			pAddChildControl;
	FpRemoveChildControl			pRemoveChildControl;
	FpSetTabColor			pSetTabColor;
	FpSetSTTabColor			pSetSTTabColor;
	FpOnSize			pOnSize;

public:
	// Function
	long ActionControl(LPVOID lpAction)
	{
		if(!m_hLib)
			return 0L;

		return pActionControl(m_lpPointer, lpAction);
	}

	void PreSubclassWindow()
	{
		if(!m_hLib)
			return;

		pPreSubclassWindow(m_lpPointer);
	}

	BOOL OnEraseBkgnd(CDC* pDC)
	{
		if(!m_hLib)
			return FALSE;

		return pOnEraseBkgnd(m_lpPointer, pDC);
	}

	void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
	{
		if(!m_hLib)
			return;

		pDrawItem(m_lpPointer, lpDrawItemStruct);
	}

	void InsertItem(int nItem, LPSTR lpszItem, UINT nTab)
	{
		if(!m_hLib)
			return;

		pInsertItem1(m_lpPointer, nItem, lpszItem, nTab);
	}

	void InsertItem(int nItem, LPSTR lpszItem, int nImage, UINT nTab)
	{
		if(!m_hLib)
			return;

		pInsertItem2(m_lpPointer, nItem, lpszItem, nImage, nTab);
	}

	void InsertItem(int nItem, TCITEM* pTabCtrlItem, UINT nTab)
	{
		if(!m_hLib)
			return;

		pInsertItem3(m_lpPointer, nItem, pTabCtrlItem, nTab);
	}

	void OnMouseMove(UINT nFlags, CPoint point) 
	{
		if(!m_hLib)
			return;

		pOnMouseMove(m_lpPointer, nFlags, point);
	}

	void OnPaint(CDC* pDC) 
	{
		if(!m_hLib)
			return;

		pOnPaint(m_lpPointer, pDC);
	}

	void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
	{
		if(!m_hLib)
			return;

		pOnHScroll(m_lpPointer, nSBCode, nPos, pScrollBar);
	}

	BOOL OnSelchange(NMHDR* pNMHDR, LRESULT* pResult)
	{
		if(!m_hLib)
			return TRUE;

		return pOnSelchange(m_lpPointer, pNMHDR, pResult);
	}

	BOOL OnSelchanging(NMHDR* pNMHDR, LRESULT* pResult)
	{
		if(!m_hLib)
			return TRUE;

		return pOnSelchanging(m_lpPointer, pNMHDR, pResult);
	}

	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam)
	{
		if(!m_hLib)
			return 0L;

		return pOnMouseLeave(m_lpPointer, wParam, lParam);
	}

	void OnKillFocus(CWnd* pNewWnd) 
	{
		if(!m_hLib)
			return;

		pOnKillFocus(m_lpPointer, pNewWnd);
	}

	void OnSetFocus(CWnd* pOldWnd) 
	{
		if(!m_hLib)
			return;

		pOnSetFocus(m_lpPointer, pOldWnd);
	}

	LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
	{
		if(!m_hLib)
			return 0L;

		return pWindowProc(m_lpPointer, message, wParam, lParam);
	}

	void AddChildControl(CWnd* pControl)
	{
		if(!m_hLib)
			return;

		pAddChildControl(m_lpPointer, pControl);
	}

	void RemoveChildControl(CWnd* pControl)
	{
		if(!m_hLib)
			return;

		pRemoveChildControl(m_lpPointer, pControl);
	}

	void SetTabColor(COLORREF clrBkgrnd, COLORREF clrActive, COLORREF clrInactive)
	{
		if(!m_hLib)
			return;

		pSetTabColor(m_lpPointer, clrBkgrnd, clrActive, clrInactive);
	}

	void SetSTTabColor(long lSTTabColor)
	{
		if(!m_hLib)
			return;

		pSetSTTabColor(m_lpPointer, lSTTabColor);
	}

	void OnSize(UINT nType, int cx, int cy)
	{
		if(!m_hLib)
			return;

		pOnSize(m_lpPointer, nType, cx, cy);
	}
};

/////////////////////////////////////////////////////////////////////////////
//	CLoaderListCtrl
/////////////////////////////////////////////////////////////////////////////
typedef void	(* FpExitListCtrl)(LPVOID);
typedef LPVOID	(* FpInitListCtrl)(LPVOID);
typedef LPVOID	(* FpInitListCtrl2)(LPVOID, BOOL);
typedef void	(* FpOnDestroyPre)(LPVOID);
typedef void	(* FpDrawItemListCtrl)(LPVOID, LPDRAWITEMSTRUCT);
typedef void	(* FpPreSubclassWindowListCtrl)(LPVOID);
typedef void	(* FpDisableColumnSizing)(LPVOID, BOOL);
typedef BOOL	(* FpOnNotifyListCtrl)(LPVOID, WPARAM, LPARAM, LRESULT*);
typedef void	(* FpSetHeaderHeight)(LPVOID, int);
typedef void	(* FpSetRowHeight)(LPVOID, int);
typedef void	(* FpSetAutoColSize)(LPVOID, BOOL);
typedef void	(* FpSetTwotoneRowColor)(LPVOID, int);
typedef FpChangeSkinNotify (* FpGetSkinNotifyFunction)(LPVOID);
class CLoaderListCtrl : public CExLoadLib
{
public:
	CLoaderListCtrl(LPVOID lpPointer, BOOL bUseDefSkin) 
	{
// 		char szPath[MAX_PATH] = {0, };
// 		GetCurrentWorkDirectory(szPath, sizeof(szPath));
// 
// 		if(szPath)
// 			::sprintf(m_szDllName, "%s\\%s", szPath, LISTCTRL_DLL);
// 		else
// 			::strcpy(m_szDllName, LISTCTRL_DLL);

		//-->Solomon-090829수정. 
		CString szDir;
		GetDir_DrfnBin(szDir);
		::sprintf(m_szDllName, "%s\\%s", (LPCSTR)szDir, LISTCTRL_DLL);
		//<--Solomon-090829수정..

		pActionControl = NULL;
		pExitListCtrl = NULL;
		pOnEraseBkgnd = NULL;
		pInitListCtrl = NULL;
		pInitListCtrl2 = NULL;
		pOnDestroyPre = NULL;
		pDisableColumnSizing = NULL;
		pGetSkinNotifyFunction = NULL;
		pDrawItemListCtrl = NULL;
		pPreSubclassWindowListCtrl = NULL;
		pOnNotifyListCtrl = NULL;
		pSetHeaderHeight = NULL;
		pSetRowHeight = NULL;
		pSetAutoColSize = NULL;
		pSetTwotoneRowColor = NULL;

		LoadLib();

		if(bUseDefSkin)
			m_lpPointer = pInitListCtrl2(lpPointer, bUseDefSkin);
		else
			m_lpPointer = pInitListCtrl(lpPointer);
	}

	virtual ~CLoaderListCtrl() 
	{
		if(m_hLib)
			pExitListCtrl(m_lpPointer);
	}

	virtual BOOL LoadProcAddress() 
	{
		if(!m_hLib)
		{
			TRACE0("CLoaderListCtrl GetProcAddress에서 Load에러 발생");
			return FALSE;
		}

		pActionControl	= (FpActionControl)::GetProcAddress(m_hLib, "Ex_ActionControl");
		pExitListCtrl	= (FpExitTabCtrl)::GetProcAddress(m_hLib, "Ex_ExitListCtrl");
		pOnEraseBkgnd	= (FpOnEraseBkgnd)::GetProcAddress(m_hLib, "Ex_OnEraseBkgnd");
		pInitListCtrl	= (FpInitListCtrl)::GetProcAddress(m_hLib, "Ex_InitListCtrl");
		pInitListCtrl2	= (FpInitListCtrl2)::GetProcAddress(m_hLib, "Ex_InitListCtrl2");
		pOnDestroyPre	= (FpOnDestroyPre)::GetProcAddress(m_hLib, "Ex_OnDestroyPre");
		pDisableColumnSizing	= (FpDisableColumnSizing)::GetProcAddress(m_hLib, "Ex_DisableColumnSizing");
		pGetSkinNotifyFunction		= (FpGetSkinNotifyFunction)::GetProcAddress(m_hLib, "Ex_GetSkinNotifyFunction");
		pDrawItemListCtrl		= (FpDrawItemListCtrl)::GetProcAddress(m_hLib, "Ex_DrawItemListCtrl");
		pPreSubclassWindowListCtrl		= (FpPreSubclassWindowListCtrl)::GetProcAddress(m_hLib, "Ex_PreSubclassWindowListCtrl");
		pOnNotifyListCtrl		= (FpOnNotifyListCtrl)::GetProcAddress(m_hLib, "Ex_OnNotifyListCtrl");
		pSetHeaderHeight		= (FpSetHeaderHeight)::GetProcAddress(m_hLib, "Ex_SetHeaderHeight");
		pSetRowHeight		= (FpSetRowHeight)::GetProcAddress(m_hLib, "Ex_SetRowHeight");
		pSetAutoColSize		= (FpSetAutoColSize)::GetProcAddress(m_hLib, "Ex_SetAutoColSize");
		pSetTwotoneRowColor		= (FpSetTwotoneRowColor)::GetProcAddress(m_hLib, "Ex_SetTwotoneRowColor");

		return TRUE;
	}

public:
	// Implementation
	LPVOID				m_lpPointer;

	FpActionControl		pActionControl;
	FpExitListCtrl			pExitListCtrl;
	FpOnEraseBkgnd			pOnEraseBkgnd;
	FpInitListCtrl			pInitListCtrl;
	FpInitListCtrl2			pInitListCtrl2;
	FpOnDestroyPre			pOnDestroyPre;
	FpDisableColumnSizing			pDisableColumnSizing;
	FpGetSkinNotifyFunction		pGetSkinNotifyFunction;
	FpDrawItemListCtrl		pDrawItemListCtrl;
	FpPreSubclassWindowListCtrl		pPreSubclassWindowListCtrl;
	FpOnNotifyListCtrl		pOnNotifyListCtrl;
	FpSetHeaderHeight		pSetHeaderHeight;
	FpSetRowHeight		pSetRowHeight;
	FpSetAutoColSize		pSetAutoColSize;
	FpSetTwotoneRowColor		pSetTwotoneRowColor;

public:
	// Function
	long ActionControl(LPVOID lpAction)
	{
		if(!m_hLib)
			return 0L;

		return pActionControl(m_lpPointer, lpAction);
	}

	BOOL OnEraseBkgnd(CDC* pDC)
	{
		if(!m_hLib)
			return FALSE;

		return pOnEraseBkgnd(m_lpPointer, pDC);
	}

	void OnDestroyPre()
	{
		if(!m_hLib)
			return;

		pOnDestroyPre(m_lpPointer);
	}

	void DisableColumnSizing(BOOL bDisable)
	{
		if(!m_hLib)
			return;

		pDisableColumnSizing(m_lpPointer, bDisable);
	}

	void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
	{
		if(!m_hLib)
			return;

		pDrawItemListCtrl(m_lpPointer, lpDrawItemStruct);
	}

	void PreSubclassWindow()
	{
		if(!m_hLib)
			return;

		pPreSubclassWindowListCtrl(m_lpPointer);
	}

	FpChangeSkinNotify GetSkinNotifyFunction()
	{
		if(!m_hLib)
			return NULL;

		return pGetSkinNotifyFunction(m_lpPointer);
	}

	BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
	{
		if(!m_hLib)
		{
			pResult = 0L;
			return FALSE;
		}

		return pOnNotifyListCtrl(m_lpPointer, wParam, lParam, pResult);
	}

	void SetHeaderHeight(int nHeight)
	{
		if(!m_hLib)
			return;

		pSetHeaderHeight(m_lpPointer, nHeight);
	}

	void SetRowHeight(int nHeight)
	{
		if(!m_hLib)
			return;

		pSetRowHeight(m_lpPointer, nHeight);
	}

	void SetAutoColSize(BOOL bAutoSize)
	{
		if(!m_hLib)
			return;

		pSetAutoColSize(m_lpPointer, bAutoSize);
	}

	void SetTwotoneRowColor(int nRow)
	{
		if(!m_hLib)
			return;

		pSetTwotoneRowColor(m_lpPointer, nRow);
	}
};

/////////////////////////////////////////////////////////////////////////////
//	CLoaderToolTipCtrl
/////////////////////////////////////////////////////////////////////////////
typedef long	(* FpActionControl)(LPVOID, LPVOID);
typedef LPVOID	(* FpInitToolTipCtrl)(LPVOID);
typedef void	(* FpExitToolTipCtrl)(LPVOID);
typedef BOOL	(* FpAddToolText)(LPVOID, CWnd*, LPCTSTR, LPCRECT, UINT);
typedef BOOL	(* FpAddToolID)(LPVOID, CWnd*, UINT, LPCRECT, UINT);
typedef void	(* FpActivate)(LPVOID, BOOL);
typedef void	(* FpUpdateTipText)(LPVOID, LPCTSTR, CWnd*, UINT);
typedef void	(* FpUpdateTipID)(LPVOID, UINT, CWnd*, UINT);
typedef void	(* FpRelayEvent)(LPVOID, LPMSG);
typedef HWND	(* FpGetSafeHwnd)(LPVOID);
typedef void	(* FpUpdate)(LPVOID);
typedef void	(* FpDelTool)(LPVOID, CWnd*, UINT);
typedef void	(* FpFillInToolInfo)(LPVOID, TOOLINFO&, CWnd*, UINT);
typedef int		(* FpGetDelayTime)(LPVOID, DWORD);
typedef void	(* FpGetMargin)(LPVOID, LPRECT);
typedef int		(* FpGetMaxTipWidth)(LPVOID);
typedef void	(* FpGetText)(LPVOID, CString&, CWnd*, UINT);
typedef COLORREF	(* FpGetTipBkColor)(LPVOID);
typedef COLORREF	(* FpGetTipTextColor)(LPVOID);
typedef int		(* FpGetToolCount)(LPVOID);
typedef BOOL	(* FpGetToolInfo)(LPVOID, CToolInfo&, CWnd*, UINT);
typedef BOOL	(* FpHitTest)(LPVOID, CWnd*, CPoint, LPTOOLINFO);
typedef void	(* FpPop)(LPVOID);
typedef void	(* FpSetDelayTime)(LPVOID, UINT);
typedef void	(* FpSetMargin)(LPVOID, LPRECT);
typedef int		(* FpSetMaxTipWidth)(LPVOID, int);
typedef void	(* FpSetTipBkColor)(LPVOID, COLORREF);
typedef void	(* FpSetTipTextColor)(LPVOID, COLORREF);
typedef void	(* FpSetToolInfo)(LPVOID, LPTOOLINFO);
typedef void	(* FpSetToolRect)(LPVOID, CWnd*, UINT, LPCRECT);
class CLoaderToolTipCtrl : public CExLoadLib
{
public:
	CLoaderToolTipCtrl(LPVOID lpPointer, LPCSTR lpszRootPath = NULL) 
	{
// 		if(lpszRootPath)
// 			::sprintf(m_szDllName, "%s\\Bin\\%s", lpszRootPath, TOOLTIPCTRL_DLL);
// 		else
// 		{
// 			char szPath[MAX_PATH] = {0, };
// 			GetCurrentWorkDirectory(szPath, sizeof(szPath));
// 
// 			if(szPath)
// 				::sprintf(m_szDllName, "%s\\%s", szPath, TOOLTIPCTRL_DLL);
// 			else
// 				::strcpy(m_szDllName, TOOLTIPCTRL_DLL);
// 		}

		//-->Solomon-090829수정. 
		CString szDir;
		GetDir_DrfnBin(szDir);
		::sprintf(m_szDllName, "%s\\%s", (LPCSTR)szDir, TOOLTIPCTRL_DLL);
		//<--Solomon-090829수정..

		pSetToolRect = NULL;
		pSetToolInfo = NULL;
		pSetTipTextColor = NULL;
		pSetTipBkColor = NULL;
		pSetMaxTipWidth = NULL;
		pSetMargin = NULL;
		pSetDelayTime = NULL;
		pPop = NULL;
		pHitTest = NULL;
		pGetToolInfo = NULL;
		pGetToolCount = NULL;
		pGetTipTextColor = NULL;
		pGetTipBkColor = NULL;
		pGetText = NULL;
		pGetMaxTipWidth = NULL;
		pGetMargin = NULL;
		pGetDelayTime = NULL;
		pFillInToolInfo = NULL;
		pDelTool = NULL;
		pUpdate = NULL;
		pGetSafeHwnd = NULL;
		pRelayEvent = NULL;
		pUpdateTipID = NULL;
		pUpdateTipText = NULL;
		pActivate = NULL;
		pAddToolID = NULL;
		pAddToolText = NULL;
		pActionControl = NULL;
		pExitToolTipCtrl = NULL;
		pInitToolTipCtrl = NULL;

		LoadLib();

		m_lpPointer = pInitToolTipCtrl(lpPointer);
	}

	~CLoaderToolTipCtrl() 
	{
		if(m_hLib)
			pExitToolTipCtrl(m_lpPointer);
	}

	virtual BOOL LoadProcAddress() 
	{
		if(!m_hLib)
		{
			TRACE0("CLoaderToolTipCtrl GetProcAddress에서 Load에러 발생");
			return FALSE;
		}

		pActionControl	= (FpActionControl)::GetProcAddress(m_hLib, "Ex_ActionControl");
		pInitToolTipCtrl	= (FpInitToolTipCtrl)::GetProcAddress(m_hLib, "Ex_InitToolTipCtrl");
		pExitToolTipCtrl	= (FpExitToolTipCtrl)::GetProcAddress(m_hLib, "Ex_ExitToolTipCtrl");
		pSetToolRect		= (FpSetToolRect)::GetProcAddress(m_hLib, "Ex_SetToolRect");
		pSetToolInfo		= (FpSetToolInfo)::GetProcAddress(m_hLib, "Ex_SetToolInfo");
		pSetTipTextColor		= (FpSetTipTextColor)::GetProcAddress(m_hLib, "Ex_SetTipTextColor");
		pSetTipBkColor		= (FpSetTipBkColor)::GetProcAddress(m_hLib, "Ex_SetTipBkColor");
		pSetMaxTipWidth		= (FpSetMaxTipWidth)::GetProcAddress(m_hLib, "Ex_SetMaxTipWidth");
		pSetMargin		= (FpSetMargin)::GetProcAddress(m_hLib, "Ex_SetMargin");
		pSetDelayTime		= (FpSetDelayTime)::GetProcAddress(m_hLib, "Ex_SetDelayTime");
		pPop		= (FpPop)::GetProcAddress(m_hLib, "Ex_Pop");
		pHitTest		= (FpHitTest)::GetProcAddress(m_hLib, "Ex_HitTest");
		pGetToolInfo		= (FpGetToolInfo)::GetProcAddress(m_hLib, "Ex_GetToolInfo");
		pGetToolCount		= (FpGetToolCount)::GetProcAddress(m_hLib, "Ex_GetToolCount");
		pGetTipTextColor		= (FpGetTipTextColor)::GetProcAddress(m_hLib, "Ex_GetTipTextColor");
		pGetTipBkColor		= (FpGetTipBkColor)::GetProcAddress(m_hLib, "Ex_GetTipBkColor");
		pGetText		= (FpGetText)::GetProcAddress(m_hLib, "Ex_GetText");
		pGetMaxTipWidth		= (FpGetMaxTipWidth)::GetProcAddress(m_hLib, "Ex_GetMaxTipWidth");
		pGetMargin		= (FpGetMargin)::GetProcAddress(m_hLib, "Ex_GetMargin");
		pGetDelayTime		= (FpGetDelayTime)::GetProcAddress(m_hLib, "Ex_GetDelayTime");
		pFillInToolInfo		= (FpFillInToolInfo)::GetProcAddress(m_hLib, "Ex_FillInToolInfo");
		pDelTool		= (FpDelTool)::GetProcAddress(m_hLib, "Ex_DelTool");
		pUpdate		= (FpUpdate)::GetProcAddress(m_hLib, "Ex_Update");
		pGetSafeHwnd		= (FpGetSafeHwnd)::GetProcAddress(m_hLib, "Ex_GetSafeHwnd");
		pRelayEvent		= (FpRelayEvent)::GetProcAddress(m_hLib, "Ex_RelayEvent");
		pUpdateTipID		= (FpUpdateTipID)::GetProcAddress(m_hLib, "Ex_UpdateTipID");
		pUpdateTipText		= (FpUpdateTipText)::GetProcAddress(m_hLib, "Ex_UpdateTipText");
		pActivate		= (FpActivate)::GetProcAddress(m_hLib, "Ex_Activate");
		pAddToolID		= (FpAddToolID)::GetProcAddress(m_hLib, "Ex_AddToolID");
		pAddToolText		= (FpAddToolText)::GetProcAddress(m_hLib, "Ex_AddToolText");
		return TRUE;
	}

public:
	// Implementation
	LPVOID			m_lpPointer;

	FpActionControl		pActionControl;
	FpInitToolTipCtrl	pInitToolTipCtrl;
	FpExitToolTipCtrl	pExitToolTipCtrl;
	FpSetToolRect		pSetToolRect;
	FpSetToolInfo		pSetToolInfo;
	FpSetTipTextColor		pSetTipTextColor;
	FpSetTipBkColor		pSetTipBkColor;
	FpSetMaxTipWidth		pSetMaxTipWidth;
	FpSetMargin		pSetMargin;
	FpSetDelayTime		pSetDelayTime;
	FpPop		pPop;
	FpHitTest		pHitTest;
	FpGetToolInfo		pGetToolInfo;
	FpGetToolCount		pGetToolCount;
	FpGetTipTextColor		pGetTipTextColor;
	FpGetTipBkColor		pGetTipBkColor;
	FpGetText		pGetText;
	FpGetMaxTipWidth		pGetMaxTipWidth;
	FpGetMargin		pGetMargin;
	FpGetDelayTime		pGetDelayTime;
	FpFillInToolInfo		pFillInToolInfo;
	FpDelTool		pDelTool;
	FpUpdate		pUpdate;
	FpGetSafeHwnd		pGetSafeHwnd;
	FpRelayEvent		pRelayEvent;
	FpUpdateTipID		pUpdateTipID;
	FpUpdateTipText		pUpdateTipText;
	FpActivate		pActivate;
	FpAddToolID		pAddToolID;
	FpAddToolText		pAddToolText;

public:
	// Function
	long ActionControl(LPVOID lpAction)
	{
		if(!m_hLib)
			return 0L;

		return pActionControl(m_lpPointer, lpAction);
	}

	BOOL AddTool(CWnd* pWnd, LPCTSTR lpszText = LPSTR_TEXTCALLBACK, LPCRECT lpRectTool = NULL, UINT nIDTool = 0)
	{
		if(!m_hLib)
			return FALSE;

		return pAddToolText(m_lpPointer, pWnd, lpszText, lpRectTool, nIDTool);
	}

	BOOL AddTool(CWnd* pWnd, UINT nIDText, LPCRECT lpRectTool = NULL, UINT nIDTool = 0)
	{
		if(!m_hLib)
			return FALSE;

		return pAddToolID(m_lpPointer, pWnd, nIDText, lpRectTool, nIDTool);
	}

	void Activate(BOOL bActivate)
	{
		if(!m_hLib)
			return;

		pActivate(m_lpPointer, bActivate);
	}

	void UpdateTipText(LPCTSTR lpszText, CWnd* pWnd, UINT nIDTool = 0)
	{
		if(!m_hLib)
			return;

		pUpdateTipText(m_lpPointer, lpszText, pWnd, nIDTool);
	}

	void UpdateTipText(UINT nIDText, CWnd* pWnd, UINT nIDTool = 0)
	{
		if(!m_hLib)
			return;

		pUpdateTipID(m_lpPointer, nIDText, pWnd, nIDTool);
	}

	void RelayEvent(LPMSG lpMsg)
	{
		if(!m_hLib)
			return;

		pRelayEvent(m_lpPointer, lpMsg);
	}

	HWND GetSafeHwnd()
	{
		if(!m_hLib)
			return NULL;

		return pGetSafeHwnd(m_lpPointer);
	}

	void Update()
	{
		if(!m_hLib)
			return;

		pUpdate(m_lpPointer);
	}

	void DelTool(CWnd* pWnd, UINT nIDTool = 0)
	{
		if(!m_hLib)
			return;

		pDelTool(m_lpPointer, pWnd, nIDTool);
	}

	void FillInToolInfo(TOOLINFO& ti, CWnd* pWnd, UINT nIDTool)
	{
		if(!m_hLib)
			return;

		pFillInToolInfo(m_lpPointer, ti, pWnd, nIDTool);
	}

	int	GetDelayTime(DWORD dwDuration)
	{
		if(!m_hLib)
			return 0;

		return pGetDelayTime(m_lpPointer, dwDuration);
	}

	void GetMargin(LPRECT lprc)
	{
		if(!m_hLib)
			return;

		pGetMargin(m_lpPointer, lprc);
	}

	int	GetMaxTipWidth()
	{
		if(!m_hLib)
			return 0;

		return pGetMaxTipWidth(m_lpPointer);
	}

	void GetText(CString& str, CWnd* pWnd, UINT nIDTool = 0)
	{
		if(!m_hLib)
			return;

		pGetText(m_lpPointer, str, pWnd, nIDTool);
	}

	COLORREF GetTipBkColor()
	{
		if(!m_hLib)
			return 0xffffff;

		return pGetTipBkColor(m_lpPointer);
	}

	COLORREF GetTipTextColor()
	{
		if(!m_hLib)
			return 0x000000;

		return pGetTipTextColor(m_lpPointer);
	}

	int	GetToolCount()
	{
		if(!m_hLib)
			return 0;

		return pGetToolCount(m_lpPointer);
	}

	BOOL GetToolInfo(CToolInfo& ToolInfo, CWnd* pWnd, UINT nIDTool = 0)
	{
		if(!m_hLib)
			return FALSE;

		return pGetToolInfo(m_lpPointer, ToolInfo, pWnd, nIDTool);
	}

	BOOL HitTest(CWnd* pWnd, CPoint pt, LPTOOLINFO lpToolInfo)
	{
		if(!m_hLib)
			return FALSE;

		return pHitTest(m_lpPointer, pWnd, pt, lpToolInfo);
	}

	void Pop()
	{
		if(!m_hLib)
			return;

		pPop(m_lpPointer);
	}

	void SetDelayTime(UINT nDelay)
	{
		if(!m_hLib)
			return;

		pSetDelayTime(m_lpPointer, nDelay);
	}

	void SetMargin(LPRECT lprc)
	{
		if(!m_hLib)
			return;

		pSetMargin(m_lpPointer, lprc);
	}

	int	SetMaxTipWidth(int iWidth)
	{
		if(!m_hLib)
			return 0;

		return pSetMaxTipWidth(m_lpPointer, iWidth);
	}

	void SetTipBkColor(COLORREF clr)
	{
		if(!m_hLib)
			return;

		pSetTipBkColor(m_lpPointer, clr);
	}

	void SetTipTextColor(COLORREF clr)
	{
		if(!m_hLib)
			return;

		pSetTipTextColor(m_lpPointer, clr);
	}

	void SetToolInfo(LPTOOLINFO lpToolInfo)
	{
		if(!m_hLib)
			return;

		pSetToolInfo(m_lpPointer, lpToolInfo);
	}

	void SetToolRect(CWnd* pWnd, UINT nIDTool, LPCRECT lpRect)
	{
		if(!m_hLib)
			return;

		pSetToolRect(m_lpPointer, pWnd, nIDTool, lpRect);
	}
};

typedef LPVOID	(* FpInitCheckBox)(LPVOID);
typedef LPVOID	(* FpInitCheckBox2)(LPVOID, BOOL);
typedef void	(* FpExitCheckBox)(LPVOID);
typedef void 	(* FpSetColor_CheckBox)(LPVOID, COLORREF, COLORREF);	
typedef void 	(* FpSetSkinString_CheckBox)(LPVOID, LPCTSTR, LPCTSTR);	
typedef void 	(* FpSetSkinIndex_CheckBox)(LPVOID, UINT, UINT);	
typedef HBRUSH 	(* FpCtlColor_CheckBox)(LPVOID, CDC*, UINT);	
class CLoaderCheckBoxButton : public CExLoadLib
{
public:
	CLoaderCheckBoxButton(LPVOID lpPointer, BOOL bUseDefSkin) 
	{
// 		char szPath[MAX_PATH] = {0, };
// 		GetCurrentWorkDirectory(szPath, sizeof(szPath));
// 
// 		if(szPath)
// 			::sprintf(m_szDllName, "%s\\%s", szPath, BUTTON_DLL);
// 		else
// 			::strcpy(m_szDllName, BUTTON_DLL);

		//-->Solomon-090829수정. 
		CString szDir;
		GetDir_DrfnBin(szDir);
		::sprintf(m_szDllName, "%s\\%s", (LPCSTR)szDir, BUTTON_DLL);
		//<--Solomon-090829수정..

		m_lpPointer = lpPointer;		// 초기화를 하기위해 임시로 저장

		pActionControl = NULL;
		pInitCheckBox = NULL;
		pExitCheckBox = NULL;
		pSetColor_CheckBox = NULL;
		pSetSkinString_CheckBox = NULL;
		pSetSkinIndex_CheckBox = NULL;
		pCtlColor_CheckBox = NULL;

		LoadLib();

		if(bUseDefSkin)
			m_lpPointer = pInitCheckBox2(lpPointer, bUseDefSkin);
		else
			m_lpPointer = pInitCheckBox(lpPointer);
	}

	virtual ~CLoaderCheckBoxButton() 
	{
		if(m_hLib)
			pExitCheckBox(m_lpPointer);
	}

	virtual BOOL LoadProcAddress() 
	{
		if(!m_hLib)
		{
			TRACE0("CLoaderCheckBoxButton GetProcAddress에서 Load에러 발생");
			return FALSE;
		}

		pActionControl	= (FpActionControl)::GetProcAddress(m_hLib, "Ex_ActionControl");
		pExitCheckBox	= (FpExitAcctInput)::GetProcAddress(m_hLib, "Ex_ExitButton");
		pInitCheckBox	= (FpInitCheckBox)::GetProcAddress(m_hLib, "Ex_InitCheckBox");
		pInitCheckBox2	= (FpInitCheckBox2)::GetProcAddress(m_hLib, "Ex_InitCheckBox2");
		pSetColor_CheckBox		= (FpSetColor_CheckBox)::GetProcAddress(m_hLib, "Ex_SetColor_CheckBox");
		pSetSkinString_CheckBox		= (FpSetSkinString_CheckBox)::GetProcAddress(m_hLib, "Ex_SetSkinString_CheckBox");
		pSetSkinIndex_CheckBox		= (FpSetSkinIndex_CheckBox)::GetProcAddress(m_hLib, "Ex_SetSkinIndex_CheckBox");
		pCtlColor_CheckBox		= (FpCtlColor_CheckBox)::GetProcAddress(m_hLib, "Ex_CtlColor_CheckBox");

		return TRUE;
	}

public:
	// Implementation
	LPVOID				m_lpPointer;

	FpActionControl		pActionControl;
	FpExitCheckBox	pExitCheckBox;
	FpInitCheckBox	pInitCheckBox;
	FpInitCheckBox2	pInitCheckBox2;
	FpSetColor_CheckBox		pSetColor_CheckBox;
	FpSetSkinString_CheckBox		pSetSkinString_CheckBox;
	FpSetSkinIndex_CheckBox		pSetSkinIndex_CheckBox;
	FpCtlColor_CheckBox		pCtlColor_CheckBox;

public:
	// Function
	long ActionControl(LPVOID lpAction)
	{
		if(!m_hLib)
			return 0L;

		return pActionControl(m_lpPointer, lpAction);
	}

	void SetColor(COLORREF clrText, COLORREF clrBackground)
	{
		if(!m_hLib)
			return;

		pSetColor_CheckBox(m_lpPointer, clrText, clrBackground);
	}

	void SetSkinString(LPCTSTR lpszText, LPCTSTR lpszBackground)
	{
		if(!m_hLib)
			return;
		
		pSetSkinString_CheckBox(m_lpPointer, lpszText, lpszBackground);
	}

	void SetSkinIndex(UINT nText, UINT nBackground)
	{
		if(!m_hLib)
			return;
		
		pSetSkinIndex_CheckBox(m_lpPointer, nText, nBackground);
	}

	HBRUSH CtlColor(CDC* pDC, UINT nCtlColor) 
	{
		if(!m_hLib)
			return NULL;
		
		return pCtlColor_CheckBox(m_lpPointer, pDC, nCtlColor);
	}
};

/////////////////////////////////////////////////////////////////////////////
//	CLoaderComboBox
/////////////////////////////////////////////////////////////////////////////
typedef long	(* FpActionControl)(LPVOID, LPVOID);
typedef LPVOID	(* FpInitComboBox)(LPVOID);
typedef LPVOID	(* FpInitComboBox2)(LPVOID, BOOL);
typedef void	(* FpExitComboBox)(LPVOID);
typedef void	(* FpPreSubclassWindow_ComboBox)(LPVOID);
typedef void	(* FpDrawItem_ComboBox)(LPVOID, LPDRAWITEMSTRUCT);
typedef void	(* FpOnPaint_ComboBox)(LPVOID);
typedef	void	(* FpOnMouseMove_ComboBox)(LPVOID, UINT, CPoint);
typedef	void	(* FpOnLButtonDown_ComboBox)(LPVOID, UINT, CPoint);
typedef	void	(* FpOnLButtonUp_ComboBox)(LPVOID, UINT, CPoint);
typedef void	(* FpOnTimer_ComboBox)(LPVOID, UINT);
class CLoaderComboBox : public CExLoadLib
{
public:
	CLoaderComboBox(LPVOID lpPointer, BOOL bUseDefSkin) 
	{
// 		char szPath[MAX_PATH] = {0, };
// 		GetCurrentWorkDirectory(szPath, sizeof(szPath));
// 		if(szPath)
// 			::sprintf(m_szDllName, "%s\\%s", szPath, COMBOBOX_DLL);
// 		else
// 			::strcpy(m_szDllName, COMBOBOX_DLL);

		//-->Solomon-090829수정. 
		CString szDir;
		GetDir_DrfnBin(szDir);
		::sprintf(m_szDllName, "%s\\%s", (LPCSTR)szDir, COMBOBOX_DLL);
		//<--Solomon-090829수정..

		m_lpPointer = lpPointer;		// 초기화를 하기위해 임시로 저장

		pActionControl = NULL;
		pInitComboBox = NULL;
		pInitComboBox2 = NULL;
		pExitComboBox = NULL;
		pPreSubclassWindow_ComboBox = NULL;
		pDrawItem_ComboBox = NULL;
		pOnPaint_ComboBox = NULL;
		pOnMouseMove_ComboBox = NULL;
		pOnLButtonDown_ComboBox = NULL;
		pOnLButtonUp_ComboBox = NULL;
		pOnTimer_ComboBox = NULL;

		LoadLib();

		if(bUseDefSkin)
		{
			if(pInitComboBox2)	m_lpPointer = pInitComboBox2(m_lpPointer, bUseDefSkin);
		}
		else
		{
			if(pInitComboBox) m_lpPointer = pInitComboBox(m_lpPointer);
		}
	}

	virtual ~CLoaderComboBox() 
	{
		if(m_hLib)
			pExitComboBox(m_lpPointer);
	}

	virtual BOOL LoadProcAddress() 
	{
		if(!m_hLib)
		{
			TRACE0("CLoaderEdit GetProcAddress에서 Load에러 발생");
			return FALSE;
		}

		pActionControl	= (FpActionControl)::GetProcAddress(m_hLib, "Ex_ActionControl");
		pInitComboBox	= (FpInitEdit)::GetProcAddress(m_hLib, "Ex_InitComboBox");
		pInitComboBox2	= (FpInitEdit2)::GetProcAddress(m_hLib, "Ex_InitComboBox2");
		pExitComboBox	= (FpExitEdit)::GetProcAddress(m_hLib, "Ex_ExitComboBox");
		pPreSubclassWindow_ComboBox	= (FpPreSubclassWindow_ComboBox)::GetProcAddress(m_hLib, "Ex_PreSubclassWindow_ComboBox");
		pDrawItem_ComboBox	= (FpDrawItem_ComboBox)::GetProcAddress(m_hLib, "Ex_DrawItem_ComboBox");
		pOnPaint_ComboBox	= (FpOnPaint_ComboBox)::GetProcAddress(m_hLib, "Ex_OnPaint_ComboBox");
		pOnMouseMove_ComboBox	= (FpOnMouseMove_ComboBox)::GetProcAddress(m_hLib, "Ex_OnMouseMove_ComboBox");
		pOnLButtonDown_ComboBox	= (FpOnLButtonDown_ComboBox)::GetProcAddress(m_hLib, "Ex_OnLButtonDown_ComboBox");
		pOnLButtonUp_ComboBox	= (FpOnLButtonUp_ComboBox)::GetProcAddress(m_hLib, "Ex_OnLButtonUp_ComboBox");
		pOnTimer_ComboBox	= (FpOnTimer_ComboBox)::GetProcAddress(m_hLib, "Ex_OnTimer_ComboBox");

		return TRUE;
	}

public:
	// Implementation
	LPVOID			m_lpPointer;

	FpActionControl		pActionControl;
	FpInitComboBox		pInitComboBox;
	FpInitComboBox2		pInitComboBox2;
	FpExitComboBox		pExitComboBox;
	FpPreSubclassWindow_ComboBox		pPreSubclassWindow_ComboBox;
	FpDrawItem_ComboBox		pDrawItem_ComboBox;
	FpOnPaint_ComboBox		pOnPaint_ComboBox;
	FpOnMouseMove_ComboBox		pOnMouseMove_ComboBox;
	FpOnLButtonDown_ComboBox		pOnLButtonDown_ComboBox;
	FpOnLButtonUp_ComboBox		pOnLButtonUp_ComboBox;
	FpOnTimer_ComboBox		pOnTimer_ComboBox;

public:
	// Function
	long ActionControl(LPVOID lpAction)
	{
		if(!m_hLib)
			return 0L;

		return pActionControl(m_lpPointer, lpAction);
	}

	void PreSubclassWindow()
	{
		if(!m_hLib)
			return;
		
		pPreSubclassWindow_ComboBox(m_lpPointer);
	}

	void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
	{
		if(!m_hLib)
			return;

		pDrawItem_ComboBox(m_lpPointer, lpDrawItemStruct);
	}

	void OnPaint()
	{
		if(!m_hLib)
			return;
		
		pOnPaint_ComboBox(m_lpPointer);
	}

	void OnMouseMove(UINT nFlags, CPoint point) 
	{
		if(!m_hLib)
			return;
		
		pOnMouseMove_ComboBox(m_lpPointer, nFlags, point);
	}

	void OnLButtonDown(UINT nFlags, CPoint point) 
	{
		if(!m_hLib)
			return;
		
		pOnLButtonDown_ComboBox(m_lpPointer, nFlags, point);
	}

	void OnLButtonUp(UINT nFlags, CPoint point) 
	{
		if(!m_hLib)
			return;
		
		pOnLButtonUp_ComboBox(m_lpPointer, nFlags, point);
	}

	void OnTimer(UINT nIDEvent) 
	{
		if(!m_hLib)
			return;
		
		pOnTimer_ComboBox(m_lpPointer, nIDEvent);
	}
};

/////////////////////////////////////////////////////////////////////////////
//	CLoaderSignal
/////////////////////////////////////////////////////////////////////////////
typedef void	(* FpExitSignal)(LPVOID);
typedef LPVOID	(* FpCreateSignalList)(LPVOID, CWnd*, CRect*, UINT, long, CStringList*);
typedef LPVOID	(* FpCreateSignalArray)(LPVOID, CWnd*, CRect*, UINT, long, LPCTSTR*, int);
typedef LPVOID	(* FpCreateSignalString)(LPVOID, CWnd*, CRect*, UINT, long, LPCTSTR);
typedef LPVOID	(* FpCreateSignalInt)(LPVOID, CWnd*, CRect*, UINT, long, UINT);
typedef OLE_HANDLE	(* FpGetHWnd)(LPVOID);
typedef HWND	(* FpGetSafeHwnd)(LPVOID);
typedef void	(* FpMoveWindow)(LPVOID, int, int, int, int, BOOL);
typedef void	(* FpMoveWindow2)(LPVOID, LPCRECT, BOOL);
typedef BOOL	(* FpSetWindowPos)(LPVOID, const CWnd*, int, int, int, int, UINT);
typedef BOOL	(* FpShowWindow)(LPVOID, int);
typedef void	(* FpGetWindowRect)(LPVOID, LPRECT);
typedef void	(* FpGetClientRect)(LPVOID, LPRECT);
typedef void	(* FpSetFocus)(LPVOID);
typedef BOOL	(* FpOnDrdsAdvise)(LPVOID, LPCTSTR);
typedef BOOL	(* FpOffDrdsAdvise)(LPVOID);
class CLoaderSignal : public CExLoadLib
{
public:
	CLoaderSignal(LPVOID lpPointer, LPCSTR lpszRootPath = NULL) 
	{
// 		if(lpszRootPath)
// 			::sprintf(m_szDllName, "%s\\Bin\\%s", lpszRootPath, SIGNAL_DLL);
// 		else
// 		{
// 			char szPath[MAX_PATH] = {0, };
// 			GetCurrentWorkDirectory(szPath, sizeof(szPath));
// 
// 			if(szPath)
// 				::sprintf(m_szDllName, "%s\\%s", szPath, SIGNAL_DLL);
// 			else
// 				::strcpy(m_szDllName, SIGNAL_DLL);
// 		}

		//-->Solomon-090829수정. 
		CString szDir;
		GetDir_DrfnBin(szDir);
		::sprintf(m_szDllName, "%s\\%s", (LPCSTR)szDir, SIGNAL_DLL);
		//<--Solomon-090829수정..

		m_lpPointer = lpPointer;		// 초기화를 하기위해 임시로 저장

		pActionControl = NULL;
		pExitSignal = NULL;

		pCreateSignalList = NULL;
		pCreateSignalArray = NULL;
		pCreateSignalString = NULL;
		pCreateSignalList = NULL;

		pGetHWnd = NULL;
		pGetSafeHwnd = NULL;

		pSetFocus = NULL;
		pMoveWindow = NULL;
		pMoveWindow2 = NULL;
		pSetWindowPos = NULL;
		pShowWindow = NULL;
		pGetWindowRect = NULL;
		pGetClientRect = NULL;
		pOnDrdsAdvise = NULL;
		pOffDrdsAdvise = NULL;

		LoadLib();
	}

	virtual ~CLoaderSignal() 
	{
		if(m_hLib)
			pExitSignal(m_lpPointer);
	}

	virtual BOOL LoadProcAddress() 
	{
		if(!m_hLib)
		{
			TRACE0("CLoaderSignal GetProcAddress에서 Load에러 발생");
			return FALSE;
		}

		pActionControl	= (FpActionControl)::GetProcAddress(m_hLib, "Ex_ActionControl");
		pExitSignal	= (FpExitSignal)::GetProcAddress(m_hLib, "Ex_ExitSignal");

		pCreateSignalList	= (FpCreateSignalList)::GetProcAddress(m_hLib, "Ex_CreateSignalList");
		pCreateSignalArray	= (FpCreateSignalArray)::GetProcAddress(m_hLib, "Ex_CreateSignalArray");
		pCreateSignalString	= (FpCreateSignalString)::GetProcAddress(m_hLib, "Ex_CreateSignalString");
		pCreateSignalInt	= (FpCreateSignalInt)::GetProcAddress(m_hLib, "Ex_CreateSignalInt");

		pGetHWnd	= (FpGetHWnd)::GetProcAddress(m_hLib, "Ex_GetHWnd");
		pGetSafeHwnd	= (FpGetSafeHwnd)::GetProcAddress(m_hLib, "Ex_GetSafeHwnd");

		pSetFocus	= (FpSetFocus)::GetProcAddress(m_hLib, "Ex_SetFocus");
		pMoveWindow	= (FpMoveWindow)::GetProcAddress(m_hLib, "Ex_MoveWindow");
		pMoveWindow2	= (FpMoveWindow2)::GetProcAddress(m_hLib, "Ex_MoveWindow2");
		pSetWindowPos	= (FpSetWindowPos)::GetProcAddress(m_hLib, "Ex_SetWindowPos");
		pShowWindow	= (FpShowWindow)::GetProcAddress(m_hLib, "Ex_ShowWindow");
		pGetWindowRect	= (FpGetWindowRect)::GetProcAddress(m_hLib, "Ex_GetWindowRect");
		pGetClientRect	= (FpGetClientRect)::GetProcAddress(m_hLib, "Ex_GetClientRect");
		pOnDrdsAdvise	= (FpOnDrdsAdvise)::GetProcAddress(m_hLib, "Ex_OnDrdsAdvise");
		pOffDrdsAdvise	= (FpOffDrdsAdvise)::GetProcAddress(m_hLib, "Ex_OffDrdsAdvise");
		return TRUE;
	}

public:
	// Implementation
	LPVOID				m_lpPointer;

	FpActionControl		pActionControl;
	FpExitSignal	pExitSignal;

	FpCreateSignalList		pCreateSignalList;
	FpCreateSignalArray		pCreateSignalArray;
	FpCreateSignalString		pCreateSignalString;
	FpCreateSignalInt		pCreateSignalInt;

	FpGetHWnd		pGetHWnd;
	FpGetSafeHwnd		pGetSafeHwnd;

	FpSetFocus			pSetFocus;
	FpMoveWindow		pMoveWindow;
	FpMoveWindow2		pMoveWindow2;
	FpSetWindowPos		pSetWindowPos;
	FpShowWindow		pShowWindow;
	FpGetWindowRect		pGetWindowRect;
	FpGetClientRect		pGetClientRect;
	FpOnDrdsAdvise		pOnDrdsAdvise;
	FpOffDrdsAdvise		pOffDrdsAdvise;

public:
	// Function
	long ActionControl(LPVOID lpAction)
	{
		if(!m_hLib)
			return 0L;

		return pActionControl(m_lpPointer, lpAction);
	}

	void ExitSignal()
	{
		if(!m_hLib)
			return;

		pExitSignal(m_lpPointer);
	}

	LPVOID CreateSignal(CWnd* pWnd, CRect* pRect, UINT nID, long lBaseDesk, CStringList* pListProperty)
	{
		if(!m_hLib)
			return NULL;

		m_lpPointer = pCreateSignalList(m_lpPointer, pWnd, pRect, nID, lBaseDesk, pListProperty);
		return m_lpPointer;
	}

	LPVOID CreateSignal(CWnd* pWnd, CRect* pRect, UINT nID, long lBaseDesk, LPCTSTR* lpszProperties, int nSize)
	{
		if(!m_hLib)
			return NULL;

		m_lpPointer = pCreateSignalArray(m_lpPointer, pWnd, pRect, nID, lBaseDesk, lpszProperties, nSize);
		return m_lpPointer;
	}

	LPVOID CreateSignal(CWnd* pWnd, CRect* pRect, UINT nID, long lBaseDesk, LPCTSTR lpszProperties)
	{
		if(!m_hLib)
			return NULL;

		m_lpPointer = pCreateSignalString(m_lpPointer, pWnd, pRect, nID, lBaseDesk, lpszProperties);
		return m_lpPointer;
	}
	
	LPVOID CreateSignal(CWnd* pWnd, CRect* pRect, UINT nID, long lBaseDesk, UINT nProperties)
	{
		if(!m_hLib)
			return NULL;

		m_lpPointer = pCreateSignalInt(m_lpPointer, pWnd, pRect, nID, lBaseDesk, nProperties);
		return m_lpPointer;
	}

	OLE_HANDLE GetHWnd()
	{
		if(!m_hLib)
			return 0;
		
		return pGetHWnd(m_lpPointer);
	}

	HWND GetSafeHwnd()
	{
		if(!m_hLib)
			return 0;
		
		return pGetSafeHwnd(m_lpPointer);
	}

	void MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint)
	{
		if(!m_hLib)
			return;

		pMoveWindow(m_lpPointer, x, y, nWidth, nHeight, bRepaint);
	}
	
	void MoveWindow(LPCRECT lpRect, BOOL bRepaint)
	{
		if(!m_hLib)
			return;

		pMoveWindow2(m_lpPointer, lpRect, bRepaint);
	}

	BOOL SetWindowPos(const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, UINT nFlags)
	{
		if(!m_hLib)
			return FALSE;

		return pSetWindowPos(m_lpPointer, pWndInsertAfter, x, y, cx, cy, nFlags);
	}

	BOOL ShowWindow(int nCmdShow)
	{
		if(!m_hLib)
			return FALSE;

		return pShowWindow(m_lpPointer, nCmdShow);
	}

	void GetWindowRect(LPRECT lpRect)
	{
		if(!m_hLib)
			return;

		pGetWindowRect(m_lpPointer, lpRect);
	}

	void GetClientRect(LPRECT lpRect)
	{
		if(!m_hLib)
			return;

		pGetClientRect(m_lpPointer, lpRect);
	}
	
	void SetFocus()
	{
		if(!m_hLib)
			return;
		
		pSetFocus(m_lpPointer);
	}

	BOOL OnDrdsAdvise(LPCTSTR lpszCode)
	{
		if(!m_hLib)
			return FALSE;

		return pOnDrdsAdvise(m_lpPointer, lpszCode);
	}

	BOOL OffDrdsAdvise()
	{
		if(!m_hLib)
			return FALSE;

		return pOffDrdsAdvise(m_lpPointer);
	}
};

/////////////////////////////////////////////////////////////////////////////
//	CLoaderDepositImg
/////////////////////////////////////////////////////////////////////////////
typedef long	(* FpActionControl)(LPVOID, LPVOID);
typedef LPVOID	(* FpInitDepositImg)(LPVOID);
typedef void	(* FpExitDepositImg)(LPVOID);
typedef void	(* FpOnPaintDepositImg)(LPVOID, CDC*);
typedef BOOL	(* FpOnEraseBkgndDepositImg)(LPVOID, CDC*);
typedef void	(* FpPreSubclassWindowDepositImg)(LPVOID);
typedef void	(* FpSetDepositIndex)(LPVOID, UINT);
typedef void	(* FpSizeToImage)(LPVOID);
class CLoaderDepositImg : public CExLoadLib
{
public:
	CLoaderDepositImg(LPVOID lpPointer) 
	{
// 		char szPath[MAX_PATH] = {0, };
// 		GetCurrentWorkDirectory(szPath, sizeof(szPath));
// 
// 		if(szPath)
// 			::sprintf(m_szDllName, "%s\\%s", szPath, DEPOSITIMG_DLL);
// 		else
// 			::strcpy(m_szDllName, DEPOSITIMG_DLL);

		//-->Solomon-090829수정. 
		CString szDir;
		GetDir_DrfnBin(szDir);
		::sprintf(m_szDllName, "%s\\%s", (LPCSTR)szDir, DEPOSITIMG_DLL);
		//<--Solomon-090829수정..

		m_lpPointer = lpPointer;		// 초기화를 하기위해 임시로 저장

		pActionControl = NULL;
		pExitDepositImg = NULL;
		pInitDepositImg = NULL;
		pOnPaintDepositImg = NULL;
		pOnEraseBkgndDepositImg = NULL;
		pPreSubclassWindowDepositImg = NULL;
		pSetDepositIndex = NULL;
		pSizeToImage = NULL;

		LoadLib();

		m_lpPointer = pInitDepositImg(m_lpPointer);
	}

	virtual ~CLoaderDepositImg() 
	{
		if(m_hLib)
			pExitDepositImg(m_lpPointer);
	}

	virtual BOOL LoadProcAddress() 
	{
		if(!m_hLib)
		{
			TRACE0("CLoaderDepositImg GetProcAddress에서 Load에러 발생");
			return FALSE;
		}

		pActionControl	= (FpActionControl)::GetProcAddress(m_hLib, "Ex_ActionControl");
		pInitDepositImg	= (FpInitDepositImg)::GetProcAddress(m_hLib, "Ex_InitDepositImg");
		pExitDepositImg	= (FpExitDepositImg)::GetProcAddress(m_hLib, "Ex_ExitDepositImg");
		pOnPaintDepositImg	= (FpOnPaintDepositImg)::GetProcAddress(m_hLib, "Ex_OnPaintDepositImg");
		pOnEraseBkgndDepositImg	= (FpOnEraseBkgndDepositImg)::GetProcAddress(m_hLib, "Ex_OnEraseBkgndDepositImg");
		pPreSubclassWindowDepositImg	= (FpPreSubclassWindowDepositImg)::GetProcAddress(m_hLib, "Ex_PreSubclassWindowDepositImg");
		pSetDepositIndex	= (FpSetDepositIndex)::GetProcAddress(m_hLib, "Ex_SetDepositIndex");
		pSizeToImage	= (FpSizeToImage)::GetProcAddress(m_hLib, "Ex_SizeToImage");

		return TRUE;
	}

public:
	// Implementation
	LPVOID			m_lpPointer;

	FpActionControl		pActionControl;
	FpInitDepositImg		pInitDepositImg;
	FpExitDepositImg		pExitDepositImg;
	FpOnPaintDepositImg		pOnPaintDepositImg;
	FpOnEraseBkgndDepositImg		pOnEraseBkgndDepositImg;
	FpPreSubclassWindowDepositImg		pPreSubclassWindowDepositImg;
	FpSetDepositIndex		pSetDepositIndex;
	FpSizeToImage		pSizeToImage;

public:
	// Function
	long ActionControl(LPVOID lpAction)
	{
		if(!m_hLib)
			return 0L;

		return pActionControl(m_lpPointer, lpAction);
	}

	void OnPaint(CDC* pDC)
	{
		if(!m_hLib)
			return;

		pOnPaintDepositImg(m_lpPointer, pDC);
	}
	
	BOOL OnEraseBkgnd(CDC* pDC) 
	{	
		if(!m_hLib)
			return TRUE;

		return pOnEraseBkgndDepositImg(m_lpPointer, pDC);
	}

	void PreSubclassWindow() 
	{
		if(!m_hLib)
			return;

		pPreSubclassWindowDepositImg(m_lpPointer);
	}

	void SetDepositIndex(UINT nIndex)
	{
		if(!m_hLib)
			return;

		pSetDepositIndex(m_lpPointer, nIndex);
	}

	void SizeToImage()
	{
		if(!m_hLib)
			return;

		pSizeToImage(m_lpPointer);
	}
};

/////////////////////////////////////////////////////////////////////////////
//	CLoaderComboList
/////////////////////////////////////////////////////////////////////////////
//typedef long	(* FpActionControl)(LPVOID, LPVOID);
//typedef LPVOID	(* FpInitComboList)(LPVOID);
//typedef LPVOID	(* FpInitComboList2)(LPVOID, BOOL);
//typedef void	(* FpExitComboList)(LPVOID);
//typedef void	(* FpPreSubclassWindowComboList)(LPVOID);
//typedef void	(* FpOnSizeComboList)(LPVOID, UINT, int, int);
//typedef void	(* FpInsertColumn)(LPVOID, int, LPCTSTR, int, int, int);
//typedef void	(WINAPI* FpInsertItem)(LPVOID, int, CString);
//typedef void	(WINAPI* FpAddItem)(LPVOID, CString);
//typedef void	(* FpDeleteItem)(LPVOID, int);
//typedef void	(* FpDeleteAllItems)(LPVOID);
//typedef CString	(WINAPI* FpGetAllItemText)(LPVOID, int);
//typedef CString	(WINAPI* FpGetItemText)(LPVOID, int, int);
//typedef void	(WINAPI* FpSetWindowText)(LPVOID, CString);
//typedef void	(WINAPI* FpGetWindowText)(LPVOID, CString&);
//typedef void	(* FpOnPaintComboList)(LPVOID);
//typedef void	(* FpSetDefaultSelectionItem)(LPVOID, int);
//typedef void	(* FpSetListWidth)(LPVOID, int);
//typedef void	(* FpSetListHeight)(LPVOID, int);
//typedef void	(WINAPI* FpOnSetFocusComboList)(LPVOID, CWnd*);
//typedef BOOL	(* FpSetItemData)(LPVOID, int, DWORD);
//typedef DWORD	(* FpGetItemData)(LPVOID, int);
//class CLoaderComboList : public CExLoadLib
//{
//public:
//	CLoaderComboList(LPVOID lpPointer, BOOL bUseDefSkin) 
//	{
//		char szPath[MAX_PATH] = {0, };
//		GetCurrentWorkDirectory(szPath, sizeof(szPath));
//
//		if(szPath)
//			::sprintf(m_szDllName, "%s\\%s", szPath, COMBOLIST_DLL);
//		else
//			::strcpy(m_szDllName, COMBOLIST_DLL);
//
//		m_lpPointer = lpPointer;		// 초기화를 하기위해 임시로 저장
//
//		pActionControl = NULL;
//		pExitComboList = NULL;
//		pInitComboList = NULL;
//		pInitComboList2 = NULL;
//		pPreSubclassWindowComboList = NULL;
//		pOnSizeComboList = NULL;
//		pOnPaintComboList = NULL;
//		pOnSetFocusComboList = NULL;
//
//		pInsertColumn = NULL;
//		pInsertItem = NULL;
//		pAddItem = NULL;
//		pDeleteItem = NULL;
//		pDeleteAllItems = NULL;
//		pGetAllItemText = NULL;
//		pGetItemText = NULL;
//		
//		pSetWindowText = NULL;
//		pGetWindowText = NULL;
//
//		pSetDefaultSelectionItem = NULL;
//		pSetListWidth = NULL;
//		pSetListHeight = NULL;
//
//		pSetItemData = NULL;
//		pGetItemData = NULL;
//
//		LoadLib();
//
//		if(bUseDefSkin)
//			m_lpPointer = pInitComboList2(m_lpPointer, bUseDefSkin);
//		else
//			m_lpPointer = pInitComboList(m_lpPointer);
//	}
//
//	virtual ~CLoaderComboList() 
//	{
//		if(m_hLib)
//			pExitComboList(m_lpPointer);
//	}
//
//	virtual BOOL LoadProcAddress() 
//	{
//		if(!m_hLib)
//		{
//			TRACE0("CLoaderComboList GetProcAddress에서 Load에러 발생");
//			return FALSE;
//		}
//
//		pActionControl	= (FpActionControl)::GetProcAddress(m_hLib, "Ex_ActionControl");
//		pInitComboList	= (FpInitComboList)::GetProcAddress(m_hLib, "Ex_InitComboList");
//		pInitComboList2	= (FpInitComboList2)::GetProcAddress(m_hLib, "Ex_InitComboList2");
//		pExitComboList	= (FpExitComboList)::GetProcAddress(m_hLib, "Ex_ExitComboList");
//		pPreSubclassWindowComboList	= (FpPreSubclassWindowComboList)::GetProcAddress(m_hLib, "Ex_PreSubclassWindowComboList");
//		pOnSizeComboList	= (FpOnSizeComboList)::GetProcAddress(m_hLib, "Ex_OnSizeComboList");
//		pOnPaintComboList	= (FpOnPaintComboList)::GetProcAddress(m_hLib, "Ex_OnPaintComboList");
//		pOnSetFocusComboList	= (FpOnSetFocusComboList)::GetProcAddress(m_hLib, "Ex_OnSetFocusComboList");
//
//		return TRUE;
//	}
//
//public:
//	// Implementation
//	LPVOID			m_lpPointer;
//
//	FpActionControl		pActionControl;
//	FpInitComboList		pInitComboList;
//	FpInitComboList2		pInitComboList2;
//	FpExitComboList		pExitComboList;
//	FpPreSubclassWindowComboList		pPreSubclassWindowComboList;
//	FpOnSizeComboList		pOnSizeComboList;
//	FpOnPaintComboList		pOnPaintComboList;
//	FpOnSetFocusComboList		pOnSetFocusComboList;
//
//	FpInsertColumn		pInsertColumn;
//	FpInsertItem		pInsertItem;
//	FpAddItem		pAddItem;
//	FpDeleteItem		pDeleteItem;
//	FpDeleteAllItems		pDeleteAllItems;
//	FpGetAllItemText		pGetAllItemText;
//	FpGetItemText		pGetItemText;
//
//	FpSetWindowText		pSetWindowText;
//	FpGetWindowText		pGetWindowText;
//
//	FpSetDefaultSelectionItem		pSetDefaultSelectionItem;
//	FpSetListWidth		pSetListWidth;
//	FpSetListHeight		pSetListHeight;
//
//	FpSetItemData		pSetItemData;
//	FpGetItemData		pGetItemData;
//
//public:
//	// Function
//	long ActionControl(LPVOID lpAction)
//	{
//		if(!m_hLib)
//			return 0L;
//
//		return pActionControl(m_lpPointer, lpAction);
//	}
//
//	void PreSubclassWindow() 
//	{
//		if(!m_hLib)
//			return;
//
//		pPreSubclassWindowComboList(m_lpPointer);
//	}
//
//	void OnSize(UINT nType, int cx, int cy) 
//	{
//		if(!m_hLib)
//			return;
//
//		pOnSizeComboList(m_lpPointer, nType, cx, cy);
//	}
//
//	void OnPaint() 
//	{
//		if(!m_hLib)
//			return;
//
//		pOnPaintComboList(m_lpPointer);
//	}
//
//	void OnSetFocus(CWnd* pOldWnd)
//	{
//		if(!m_hLib)
//			return;
//
//		pOnSetFocusComboList(m_lpPointer, pOldWnd);
//	}
//
//	void InsertColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat, int nWidth, int nSubItem)
//	{
//		if(!m_hLib)
//			return;
//
//		if(!pInsertColumn)
//			pInsertColumn	= (FpInsertColumn)::GetProcAddress(m_hLib, "Ex_InsertColumn");
//
//		if(pInsertColumn)
//			pInsertColumn(m_lpPointer, nCol, lpszColumnHeading, nFormat, nWidth, nSubItem);
//	}
//
//	void InsertItem(int nItem, CString strItem)
//	{
//		if(!m_hLib)
//			return;
//
//		if(!pInsertItem)
//			pInsertItem	= (FpInsertItem)::GetProcAddress(m_hLib, "Ex_InsertItem");
//
//		if(pInsertItem)
//			pInsertItem(m_lpPointer, nItem, strItem);
//	}
//
//	void AddItem(CString strItem)
//	{
//		if(!m_hLib)
//			return;
//
//		if(!pAddItem)
//			pAddItem	= (FpAddItem)::GetProcAddress(m_hLib, "Ex_AddItem");
//
//		if(pAddItem)
//			pAddItem(m_lpPointer, strItem);
//	}
//
//	void DeleteItem(int nItem)
//	{
//		if(!m_hLib)
//			return;
//
//		if(!pDeleteItem)
//			pDeleteItem	= (FpDeleteItem)::GetProcAddress(m_hLib, "Ex_DeleteItem");
//
//		if(pDeleteItem)
//			pDeleteItem(m_lpPointer, nItem);
//	}
//
//	void DeleteAllItems()
//	{
//		if(!m_hLib)
//			return;
//
//		if(!pDeleteAllItems)
//			pDeleteAllItems	= (FpDeleteAllItems)::GetProcAddress(m_hLib, "Ex_DeleteAllItems");
//
//		if(pDeleteAllItems)
//			pDeleteAllItems(m_lpPointer);
//	}
//
//	CString GetAllItemText(int nItem)
//	{
//		if(!m_hLib)
//			return _T("");
//
//		if(!pGetAllItemText)
//			pGetAllItemText	= (FpGetAllItemText)::GetProcAddress(m_hLib, "Ex_GetAllItemText");
//
//		if(pGetAllItemText)
//			return pGetAllItemText(m_lpPointer, nItem);
//		else
//			return _T("");
//	}
//
//	CString GetItemText(int nItem, int nSubItem)
//	{
//		if(!m_hLib)
//			return _T("");
//
//		if(!pGetItemText)
//			pGetItemText	= (FpGetItemText)::GetProcAddress(m_hLib, "Ex_GetItemText");
//
//		if(pGetItemText)
//			return pGetItemText(m_lpPointer, nItem, nSubItem);
//		else
//			return _T("");
//	}
//
//	void SetWindowText(CString strText)
//	{
//		if(!m_hLib)
//			return;
//
//		if(!pSetWindowText)
//			pSetWindowText	= (FpSetWindowText)::GetProcAddress(m_hLib, "Ex_SetWindowText");
//
//		if(pSetWindowText)
//			pSetWindowText(m_lpPointer, strText);
//	}
//
//	void GetWindowText(CString& strText)
//	{
//		if(!m_hLib)
//			return;
//
//		if(!pGetWindowText)
//			pGetWindowText	= (FpGetWindowText)::GetProcAddress(m_hLib, "Ex_GetWindowText");
//
//		if(pGetWindowText)
//			pGetWindowText(m_lpPointer, strText);
//	}
//
//	void SetDefaultSelectionItem(int nSubItem)
//	{
//		if(!m_hLib)
//			return;
//
//		if(!pSetDefaultSelectionItem)
//			pSetDefaultSelectionItem	= (FpSetDefaultSelectionItem)::GetProcAddress(m_hLib, "Ex_SetDefaultSelectionItem");
//
//		if(pSetDefaultSelectionItem)
//			pSetDefaultSelectionItem(m_lpPointer, nSubItem);
//	}
//
//	void SetListWidth(int nWidth)
//	{
//		if(!m_hLib)
//			return;
//
//		if(!pSetListWidth)
//			pSetListWidth	= (FpSetDefaultSelectionItem)::GetProcAddress(m_hLib, "Ex_SetListWidth");
//
//		if(pSetListWidth)
//			pSetListWidth(m_lpPointer, nWidth);
//	}
//
//	void SetListHeight(int nHeight)
//	{
//		if(!m_hLib)
//			return;
//
//		if(!pSetListHeight)
//			pSetListHeight	= (FpSetDefaultSelectionItem)::GetProcAddress(m_hLib, "Ex_SetListHeight");
//
//		if(pSetListHeight)
//			pSetListHeight(m_lpPointer, nHeight);
//	}
//
//	BOOL SetItemData(int nItem, DWORD dwData)
//	{
//		if(!m_hLib)
//			return FALSE;
//
//		if(!pSetItemData)
//			pSetItemData	= (FpSetItemData)::GetProcAddress(m_hLib, "Ex_SetItemData");
//
//		if(pSetItemData)
//			return pSetItemData(m_lpPointer, nItem, dwData);
//		else
//			return FALSE;
//	}
//
//	DWORD GetItemData(int nItem)
//	{
//		if(!m_hLib)
//			return 0;
//
//		if(!pGetItemData)
//			pGetItemData	= (FpGetItemData)::GetProcAddress(m_hLib, "Ex_GetItemData");
//
//		if(pGetItemData)
//			return pGetItemData(m_lpPointer, nItem);
//		else
//			return 0;
//	}
//};

#endif // !defined(AFX_LOADSYSTEMTRADINGPARSERHELPER_H__DF553F68_69A3_11D6_994C_004854800CB4__INCLUDED_)