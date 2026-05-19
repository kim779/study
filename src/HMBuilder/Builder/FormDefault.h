#pragma once
#include "afxwin.h"


// CFormDefault 대화 상자입니다.

class CFormDefault : public CDialogEx
{
	DECLARE_DYNAMIC(CFormDefault)

public:
	CFormDefault(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CFormDefault();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_FORM };
public:
	CString m_workpath;

	CString	m_formname;
	CString	m_desc;
	CString	m_Title;
	CString	m_width;
	CString	m_height;
	CString	m_fontname;
	CString	m_fontsize;
	CString m_sLayOut;
	CMapStringToString m_Map;
	enum {NONE = 0, NAME, TITLE, DESC, WIDTH, HEIGHT, FSIZE, OK}	m_taborder;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonFont();
//	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void OnOK();

	CString GetLayOut(){ return m_sLayOut;  }
	bool Init();

	CListBox m_Listlayout;
	afx_msg void OnBnClickedListDelete();
};
