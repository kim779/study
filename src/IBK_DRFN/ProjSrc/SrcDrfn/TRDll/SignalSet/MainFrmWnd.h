#if !defined(AFX_MAINFRMWND_H__B7336197_30FA_48AB_8523_E521785836C7__INCLUDED_)
#define AFX_MAINFRMWND_H__B7336197_30FA_48AB_8523_E521785836C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainFrmWnd.h : header file
//

#include "../../../SrcSite/control/fx_misc/fxImgButton.h"
#include "../../../SrcSite/control/fx_misc/misctype.h"
#include "../../DrCommon/Dr_Control/DrTabCtrl.h"
#include "../../../SrcSite/include/CodeCtrl.h"

/*extern HINSTANCE g_libSignalMng;*/

/////////////////////////////////////////////////////////////////////////////
// CMainFrmWnd frame

class CNewsSetWnd;
class CConditionSetWnd;
class CFilterSetWnd;
class CSoundSetWnd;
class CMainFrmWnd : public CFrameWnd
{
public:
	CMainFrmWnd();
	virtual ~CMainFrmWnd();
 
	COLORREF m_clrMainSkin;
	BOOL m_bIsFnUser;

	CFont m_fontDefault, m_fontBold;
	CBrush  m_brushStatic, m_brushTitlePoint, m_brushWhite;
    
	BOOL m_bLoaded_Lib;
  	
public:
	CRect m_reSetBar;
	CfxImgButton	m_btnSet_News;
	CfxImgButton	m_btnSet_Sys;
	CfxImgButton	m_btnSet_Filter;

	CStatic			m_ctrlTitlePoint;
	CStatic			m_ctrlTitle;

	CfxImgButton	m_btnInit;

	CfxImgButton	m_btnOk;
	CfxImgButton	m_btnCancel;
	CfxImgButton	m_btnApply;

	CNewsSetWnd				*m_pwndNewsSet;
	CConditionSetWnd		*m_pwndSysSet;

	CFilterSetWnd			*m_pwndFilterSet;
	CSoundSetWnd			*m_pwndSoundSet;

	CCodeCtrl*				m_pCodeInput;
	HWND					m_hfxCodeCtrl;
	BOOL m_bInit;

	CDrTabCtrl*		m_ptabCondition;
	CRect			m_rtTab;

public:
	void SetInit(CMapStringToOb	*pMapControlName);
	void SetInit_LibPortPolio(BOOL bInit);

	BOOL SetLoad_SignalMngLib();

	int SetMessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption = PROGRAM_TITLE, UINT nType = MB_OK);
	void Debug_SignalSetInfo(CMapStringToPtr *pmapSignalSet);

	void SetChangeTab(int nCurSel);
	void ChangeSkinColor(COLORREF clrBkgnd, ST_TABCOLOR stTabColor);
	void SetCodeComboInfo(CCodeCtrl* pCodeCombo, HWND hCodeCombo)
	{
	  m_pCodeInput  = pCodeCombo;
	  m_hfxCodeCtrl = hCodeCombo;
	}

	void OnPF_Contorl_Move();
	void PlatFormBtnSystem();
	void PlatFormBtnTarget();
	void PlatFormBtnInit();
	void PlatFormBtnOK();
	void PlatFormBtnCancel();
	void PlatFormBtnApply();
	void PlatFormBtnNewsSignal();

	void SetBtnImg(CfxImgButton *pBtn, CString strFileName);
	void SetBtnImg(CfxImgButton *pBtn, int nTextLen, BOOL bBold = FALSE);

	void GetData_PortGroup(CStringArray& saKey, CStringArray& saGroupName);

	CWnd*	GetModuleMain();
	CFont*	GetFont(BOOL bBold = FALSE);
	CString	Variant(int nComm, CString strData = "");
	HBRUSH	GetAxBrush(COLORREF clr);
	COLORREF GetColor(int nColor);
	COLORREF GetAxColor(int nColor);
	CPen*	 GetAxPen(COLORREF clr, int nWidth, int nStyle);
	void	GetSignalDef(); // 0016487: [7134] 신호검색 설정화면에서 신호 항목 변경후 [초기화] 클릭시 정렬만 되는 현상 수정
	HRESULT ReceiveData(LPCSTR szTR, LPVOID	aTRData, long dwTRDateLen, int nKey = 0);
	void SetWaitCursor(BOOL bState);
	int ParseGroup(char* pReceiveBuff, CStringArray& arrKey, CStringArray& arrName);
private:
	void OnSize();
	void OnSize_Bottom(CRect reClient);
	void OnSize_Left(CRect reClient);
	void OnSize_Right(CRect reClient);

	void SetDrewBox(CPaintDC *pDC, CRect reData, COLORREF clLine);


	int GetBtnWidth(int nTextLen) { return (int)BTN_WIDTH_BASE + (int)BTN_WIDTH_STEP * nTextLen; }

	HINSTANCE	m_hSignalMng;
	typedef int	 (*_DLL_SetSiganl)(BOOL bAdvise, CWnd* pwndMain, CString strType, CString sPBID, CString strKey);
	typedef void (*_DLL_GetSignalData)(CString strType, CMapStringToPtr &mapSignalSet);
	typedef void (*_DLL_SetSignalData)(CString strType, CMapStringToPtr *pmapSignalSet);
	typedef void (*_DLL_GetFilterData)(long &stFilterInfo);
	typedef void (*_DLL_SetFilterData)(long *pstFilterInfo);
	typedef CWnd*(*_DLL_SetLibOpen)(CWnd* pwndMain, HINSTANCE hInstance, CWnd* pWndModuleWnd);
	typedef void (*_DLL_SetLibClose)(CWnd* pWndTarget);
	typedef void (*_DLL_SetDefSysSignalData)(CMapStringToPtr *pmapSignalSet); // 0016487: [7134] 신호검색 설정화면에서 신호 항목 변경후 [초기화] 클릭시 정렬만 되는 현상 수정	

	_DLL_SetLibOpen		Lib_Open;
	_DLL_SetLibClose	Lib_Close; 
	_DLL_SetSiganl		Lib_SetSignal;
	_DLL_GetSignalData	Lib_GetSignalData;
	_DLL_SetSignalData	Lib_SetSignalData;
	_DLL_GetFilterData	Lib_GetFilterData;
	_DLL_SetFilterData	Lib_SetFilterData;
	_DLL_SetDefSysSignalData	Lib_SetDefSysSignalData; // 0016487: [7134] 신호검색 설정화면에서 신호 항목 변경후 [초기화] 클릭시 정렬만 되는 현상 수정

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrmWnd)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(CMainFrmWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
  //afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	afx_msg void OnBtnSet_News();
	afx_msg void OnBtnSet_Sys();
	afx_msg void OnBtnSet_Filter();

	afx_msg void OnBtnInit();
	afx_msg void OnBtnOk();
	afx_msg void OnBtnCancel();
	afx_msg void OnBtnApply();

	afx_msg void OnNotify_PortPolio(WPARAM wParam, LPARAM lParam);

	afx_msg void OnChangeTab(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
		
protected:
	LONG m_dwAdviseKey;	// INewsSignalManager

	HWND		m_hChildFrame;
	void FnRButtonUp(CPoint point);
	void SetCommMenu(POINT *point);
	HWND GetFrameWnd(CWnd* pWnd);
	afx_msg long OnChangeChildFrameWindowHandle( WPARAM wParam, LPARAM lParam);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRMWND_H__B7336197_30FA_48AB_8523_E521785836C7__INCLUDED_)
