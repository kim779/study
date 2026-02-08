#pragma once
// DialogConfirm.h : header file
//

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CDialogConfirm dialog

class CDialogConfirm : public CDialog
{
// Construction
public:
	CDialogConfirm(CRect rc, class CTool* pTool, CWnd* pParent = NULL);
	virtual ~CDialogConfirm();

public:
	void	SetData(char cOrderKind, CString strAccount, CString strName, CString strJM, int iOrderCount,
			CString strOrderPrice, char cTradeKind, CString strOne);

protected:
	bool			m_bInit;
	CRect 			m_rc;
	class CTextWnd*		m_patw[16];
	class CTextWnd*		m_ptwTitle;
	class CTextWnd*		m_ptwBar;
	class CTextWnd*		m_ptwStatic1;
	class CTextWnd*		m_ptwStatic2;
	class CTextWnd*		m_ptwStatic3;
	class CfxImgButton*	m_pBtnOK;
	class CfxImgButton*	m_pBtnCancel;
	class CTool*		m_pTool;

	CString		m_strWindowText;


	CString		m_strOrderKind;
	CString		m_strOne;	// ¿øÁÖ¹®
	

	CString		m_strAccount;
	CString		m_strName;
	CString		m_strJM;
	CString		m_strOrderCount;
	CString		m_strOrderPrice;
	CString		m_strTradeKind;

	COLORREF	m_rgbBG;
	COLORREF	m_rgbBorder;

protected:
	BOOL	Initialize();
	void	CreateTextWnd();

	CString	AddComma(CString strData);

public:
// Dialog Data
	//{{AFX_DATA(CDialogConfirm)
	enum { IDD = IDD_DIALOG_CONFIRM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogConfirm)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogConfirm)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

