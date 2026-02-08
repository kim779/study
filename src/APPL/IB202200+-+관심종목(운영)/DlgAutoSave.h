#pragma once
// DlgAutoSave.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DlgAutoSave dialog

class DlgAutoSave : public CDialog
{
// Construction
public:
	DlgAutoSave(CWnd* pParent = nullptr);   // standard constructor
	class CGroupWnd* m_parent;
	
	CString m_strUploading;
	void Upload();
	void UploadFinsh();
	int m_istep;   //0 다이알로그 팝업 저장 취소 선택 단계 ,   1 저장완료 단계
// Dialog Data
	//{{AFX_DATA(DlgAutoSave)
	enum { IDD = IDD_DLGAUTOSAVE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgAutoSave)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgAutoSave)
	virtual void OnOK();
	afx_msg void OnBtnSave();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	virtual void OnCancel();
	//}}AFX_MSG
	afx_msg LONG OnManage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

