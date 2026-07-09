#pragma once


// CScreenSetDlg dialog

class CScreenSetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CScreenSetDlg)

public:
	CScreenSetDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CScreenSetDlg();

// Dialog Data
	enum { IDD = IDD_DLG_SCREENSET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CTabCtrl m_tab;
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};
