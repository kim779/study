#pragma once
// ContractWnd.h : header file
//
#include "ContractGrid.h"
#include "ContractScroll.h"
/////////////////////////////////////////////////////////////////////////////
// CContractWnd window

class CContractWnd : public CWnd
{
// Construction
public:
	CContractWnd(CWnd *pWizard, CWnd *pParent);
	void	Init(int DisplayCnt, int half);
	void	Clear();
	void	Dispatch(char *pData, int digit);
	void	Alert(CString sRts);
	void	Alert(struct _alertR* alertR);
	void	Font(CFont *pFont);
	void	Palette();
	void	ChangeDisplay(int DisplayCnt);

	CWnd	*m_pWizard;
	CWnd	*m_pParent;

	int	m_digit;
	int	m_half;
	int	m_max;
	int	m_display;

// Attributes
public:
	int	GetMax()	{ return m_max; }

private:
	CCX_DepthApp	*m_pApp;
	CSize	m_BasicSize;
	CFont	*m_pFont;
	std::unique_ptr<class CContractGrid> m_grid;
	bool	m_bAlert;

	void	WriteAlert(CString str, CString &sPrice, CString &sVolume);
	void	WriteAlert(DWORD* data);
	void	ResizeToFit();
	int	CommaModify(CString &str, int digit);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CContractWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CContractWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CContractWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};