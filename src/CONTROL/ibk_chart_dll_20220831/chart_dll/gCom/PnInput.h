#if !defined(AFX_PNINPUT_H__6860AB15_72AA_498F_AAC3_3405A7D60B4E__INCLUDED_)
#define AFX_PNINPUT_H__6860AB15_72AA_498F_AAC3_3405A7D60B4E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PnInput.h : header file
//
#include "PnBaseWnd.h"

/////////////////////////////////////////////////////////////////////////////
// CPnInput window

class CPnInput : public CPnBaseWnd
{
// Construction
public:
	CPnInput(CWnd *pwndView, CWnd* pwndGView, CFont* pFont, int ctrlID, char* info);

// Attributes
public:


private:
	bool	m_bInitResize;
	bool	m_bStar;
	BOOL	m_bMouseIN;
	int		m_BtnOrgStatus;

	int	m_iDtUnit;
	int	m_iDtIndex;
	int	m_iOrgUnit;
	int	m_iMGap;	// 碟除問
	int	m_iTGap;	// す除問
	int	m_iGKind;
	int	m_iTimerID;
	int	m_iTotalToday;
	int	m_aiGap[5];

	CRect		m_rcGigan;
	CRect		m_rcDisp;
	CRect		m_rcIndex;
	CRect		m_rcSelGap;
	CRect		m_rcDAY;
	CRect		m_rcMON;

	CPoint		m_ptCodeBtn;

	CSize		m_szHangul;
	CSize		m_szNumeric;
	CSize		m_szAlphabet;

	CString		m_strOutSymbol;
	CString		m_strTrigger;
	CString		m_strValidCode;

	CFont*		m_pfontButton;

	CImageList*	m_pCfgImage;
	CImageList*	m_pNTimeImage;
	CImageList*	m_pNBtnImage;
	CImageList*	m_pCodeImage;
	CImageList*	m_pIOptImage;
	CImageList*	m_p4BtnImage;
	CImageList* m_pIUnitImage;
	
	class CPnWndCombo*	m_pUnitCB;
	class CPnWndCombo*	m_pKKindCB;
	class CCodeCtrl*	m_pCodeCB;
	class CPnWndCombo*	m_pGapCB;
	class CPnWndEdit*	m_peditGigan;
	class CPnWndEdit*	m_peditDisp;
	class CPnWndCalendar*	m_pCalendar;
	class CPnToolTip*	m_pToolTip;
	class CCheckBox*	m_pchbToday;

// Operations
public:


private:
	void	Draw(CDC *pDC);
	void	CreateWndCtrl();
	void	ArrangeCtrl(CRect rectC, bool bChgIndex = true);
	void	DrawButton(CDC* pDC, struct _comp* comp);
	void	DrawButton2(CDC* pDC, struct _comp* comp);
	void	SetInfo(CRect wRC);

	void	MakeInputSymbol();
	void	ChangeUnit(int unit);
	void	ChangeIndex(int index, bool bChgGap = true, bool bRedraw = true, bool bMessage = true);
	void	ChangeGap(int gap = 0, bool bOnlyChange = false);
	void	ChangeKKind(int kkind);
	void	ChangeSKind(int skind);
	void	NotifyIndexChange();
	bool	CheckTR();
	void	ProcTab(int id);
	void	OpenCodeDialog();
	int		GetTRInfo(char* pcHeader);
	int		GetTRInfo2(char *pHeader);
	void	ReadEnv(struct _dataInfo *datainfo);
	void	ReadDisplay();
	void	GetDominoCode(char *dCode, CString &domino, bool bTrigger);
	CString	GetDefaultCode();

	void	SetTriggerCode();
	CString	GetHistory(int unit);
	
	//** macho
	void	ChangeMain(int kind);
	void	ReloadEnv(char *pData);
	void	SetGapValue(char *pGap);
	void	LButtonUp(CPoint point);
	void	mouseLeaveAction(UINT nFlags, CPoint point);

public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPnInput)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPnInput();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPnInput)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg LRESULT	OnInputEvent(WPARAM wParam, LPARAM lParam);
	afx_msg void OnClickedToday();
	DECLARE_MESSAGE_MAP()


//** macho add
private:
	BOOL	m_lock;
	CString m_userPath;
	CString GetCurrentUnit();
	void	DrawUnitButton(CDC* dc, CRect rc);
	void	TrackingUnitMenu(CPoint pt);
	int		FindUnit(int index);
	void	OnChangeUnit();
	void	ChangeUnitAndCode(LPCTSTR param);
	CString GetIndexCode(int indexCode);

	BOOL	SendRequest(BOOL bTrigger = TRUE);
	int	WhichFOCode(LPCTSTR code);

	BOOL	m_bSeriesChart;
	CString GetSiseName();

	CString m_savedDisp;

	void SetIndexCode(CString& code);
	void SetIndexTrigger(CString& domino);
	void SetIndexKind(LPCTSTR code);
public:
	int m_iDataCount;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PNINPUT_H__6860AB15_72AA_498F_AAC3_3405A7D60B4E__INCLUDED_)
