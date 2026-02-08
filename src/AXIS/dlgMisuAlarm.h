#if !defined(AFX_DLGMISUALARM_H__96B96EE3_1E99_4D4D_AD95_32E3AC51E190__INCLUDED_)
#define AFX_DLGMISUALARM_H__96B96EE3_1E99_4D4D_AD95_32E3AC51E190__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

class CDlgMisuAlarm : public CDialog
{
public:
	CDlgMisuAlarm(CWnd* pParent);
	~CDlgMisuAlarm();

	//{{AFX_DATA(CDlgMisuAlarm)
	enum { IDD = IDD_DLGMISU };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDlgMisuAlarm)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDlgMisuAlarm)
	virtual BOOL OnInitDialog();
	afx_msg void OnMap();
	virtual void OnOK();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void	Init();
private:
	CWnd	*m_mainframe;
	CBrush	m_brush;
};

//{{AFX_INSERT_LOCATION}}
#endif // !defined(AFX_DLGMISUALARM_H__96B96EE3_1E99_4D4D_AD95_32E3AC51E190__INCLUDED_)
