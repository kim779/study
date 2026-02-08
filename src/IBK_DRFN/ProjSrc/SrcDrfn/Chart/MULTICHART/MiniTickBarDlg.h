// MiniTickBarDlg.h: interface for the CMiniTickBarDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MINITICKBARDLG_H__5EB05AE9_BC3F_44A2_A5EF_5289C866BD50__INCLUDED_)
#define AFX_MINITICKBARDLG_H__5EB05AE9_BC3F_44A2_A5EF_5289C866BD50__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "InputBarDlg.h"

typedef struct _ST_SETTICK
{
	CString strTitle;
	int nMarket;
	int nDate;
	int nTick;
	BOOL bAllDay;
}ST_SETTICK;

class CSetTickButton
{
public:
	CSetTickButton();
	~CSetTickButton();

	CSetTickButton& operator = (CSetTickButton& right);
	
	void Init();
	void SetButton(CILButton* m_pBtn, CImageList* pIL_W20H20);

	CString GetTitle(int nIndex);
	int GetMarket(int nIndex);
	char GetDate(int nIndex);
	int GetDateIndex(int nIndex);
	int GetTick(int nIndex);
	BOOL GetAllDay(int nIndex);

	void SetTitle(int nIndex, CString strTitle);
	void SetMarket(int nIndex, int nMarket);
	void SetDateIndex(int nIndex, int nDate);
	void SetTick(int nIndex, int nTick);
	void SetAllDay(int nIndex, BOOL bAllDay);

	CString MakeTickButtonTitle(int nIndex);

protected:
	ST_SETTICK* m_pSetTick;
};

class CMiniTickBarDlg : public CInputBarDlg  
{
public:
	CMiniTickBarDlg(CWnd* pParent = NULL);

	virtual void Init();
	virtual void LoadIniValue();
	virtual void SaveIniValue();
	virtual void SetCodeName(int nMarketType, LPCSTR szCode, LPCSTR szName);

	virtual void SetType(BOOL bSendChartMng = TRUE); // UpdateBy m_chType(Æ½(0)/ºÐ(1)/ÀÏ(2)/ÁÖ(3)/¿ù(4)) and m_nUnitPos
	virtual void MoveCodeInputCtrl();

	void SetAllPosition(int nType);
	void MoveToLeftRight(UINT nMoveDirection);

	UINT	m_nMoveDirection;	// 0: None, 1:Left, 2:Right 
	UINT	GetMoveDirection() { return m_nMoveDirection; }
	void	SetMoveDirection(UINT nMoveDirection) { m_nMoveDirection = nMoveDirection; }


// Dialog Data
	//{{AFX_DATA(CMiniTickBarDlg)
	enum { IDD = IDD_MINITICKBAR };
	CILButton	m_btnInitail;
	CILButton	m_btnMoveToLeftRight;
	CILButton	m_btnConfig;
	//}}AFX_DATA
	CILButton	m_btn[9];

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMiniTickBarDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CMiniTickBarDlg)
	afx_msg void OnBtnConfig();
	afx_msg void OnBtnMovetoleftright();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBtinitial();
	//}}AFX_MSG
	afx_msg void OnBtn(UINT nID);
	DECLARE_MESSAGE_MAP()

	CSetTickButton	m_SetTickBtn;
};

#endif // !defined(AFX_MINITICKBARDLG_H__5EB05AE9_BC3F_44A2_A5EF_5289C866BD50__INCLUDED_)
