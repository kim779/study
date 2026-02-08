#if !defined(AFX_INDIDISP_H__EEC97C8E_7CD3_41BA_A059_4195CB410164__INCLUDED_)
#define AFX_INDIDISP_H__EEC97C8E_7CD3_41BA_A059_4195CB410164__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CIndiDisp : public CDialog
{
public:
	CIndiDisp(CWnd* pParent = NULL);
	void SetScreen(char kind, struct _line* line);
	void SetScreen(struct _line* line, CString title, DWORD* opt, DWORD value);

	//{{AFX_DATA(CIndiDisp)
	enum { IDD = IDD_INDI_DISP };
	CButton	m_check;
	CStatic	m_stColor;
	CStatic	m_stWidth;
	CStatic	m_stKind;
	CWidthCtrl	m_width;
	CKindCtrl	m_kind;
	CColorCtrl	m_color;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CIndiDisp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CIndiDisp)
	afx_msg LRESULT OnControls(WPARAM wParam, LPARAM lParam);
	afx_msg void OnCheck1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	struct	_line*	m_pLine;
	DWORD*	m_doption;
	DWORD	m_value;
};

//{{AFX_INSERT_LOCATION}}
#endif // !defined(AFX_INDIDISP_H__EEC97C8E_7CD3_41BA_A059_4195CB410164__INCLUDED_)
