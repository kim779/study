#if !defined(AFX_MAINPREVIEW_H__80301E3A_BE10_4AB8_B8DA_4856AABC79BE__INCLUDED_)
#define AFX_MAINPREVIEW_H__80301E3A_BE10_4AB8_B8DA_4856AABC79BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPreviewWnd;
class CMainPreview : public CDialog
{
public:
	CMainPreview(COLORREF tkCOLOR, char* envinfo, CWnd* pParent = NULL);

public:
	WORD	GetUnion();

	void SetScreen(struct	_graph*	pBong, struct _graph* pGraph, bool bEach = false, bool bTitle = true);
	void SetChartName(CString chartname);
	//{{AFX_DATA(CMainPreview)
	enum { IDD = IDD_MAIN_PREVIEW };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CMainPreview)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CMainPreview)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();	
	//}}AFX_MSG	
	afx_msg LRESULT OnManage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	CPreviewWnd*	m_pPreviewWnd;
	struct	_graph*	m_pGraph;
	struct	_graph*	m_pBong;
	CWnd*		m_pwndChild;

	char*		m_pcEnv;
	COLORREF	m_rgbTick;
	
};

//{{AFX_INSERT_LOCATION}}
#endif // !defined(AFX_MAINPREVIEW_H__80301E3A_BE10_4AB8_B8DA_4856AABC79BE__INCLUDED_)
