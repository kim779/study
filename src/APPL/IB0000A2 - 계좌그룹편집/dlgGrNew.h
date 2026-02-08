#pragma once

class CDlgGrNew : public CDialog
{
public:
	CDlgGrNew(CWnd* pParent = NULL); 

	//{{AFX_DATA(CDlgGrNew)
	enum { IDD = IDD_DLGGRNEW };
	CString	m_EditGrName;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgGrNew)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgGrNew)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CString	GetNewGrName() { return m_grName; }
	void	SetTitle(CString titleS) { m_titleS = titleS; }
private:
	CString	m_grName;
	CString m_titleS;
};

