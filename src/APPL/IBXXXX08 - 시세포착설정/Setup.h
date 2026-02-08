
#include "../../h/interst.h"
#include "exGrid.h"
#include "axStatic.h"

#if _MSC_VER > 1000
#pragma once
#endif

class CSetup : public CDialog
{
public:
	CSetup(CWnd* pParent, CWnd* pView);
	
	//{{AFX_DATA(CSetup)
	enum { IDD = IDD_SETUP };
	CAxStatic	m_info;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSetup)
	public:
	virtual int DoModal();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CSetup)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void	SetCodeInfo(_codeinfo *codeinfo);
	void	GetCodeInfo(_codeinfo *codeinfo);
	void	Alert(CMapStringToString &rtmStore);
	void	SetSise(CString dataS);
	bool	CheckPopup() { return m_bPopup; }
	void	SetTitle(CString titleS);
	void	SetFont(LOGFONT lf, CFont *pFont);

private:
	void	initConstruct();
	void	destroy();
	void	initControl();
	void	initGrid(CExGrid* pGrid, UINT idc);
	int	insertGrid(CExGrid* pGrid, UINT idc, int row);
	void	updateSise();
private:
	CWnd		*m_pParent;
	CWnd		*m_pView;
	CExGrid		*m_pSetupSise;
	bool		m_bPopup;
	_codeinfo	m_codeinfo;
	CString		m_titleS;
	CString		m_siseS;
	LOGFONT		m_lf;	CFont	*m_pFont;
};

