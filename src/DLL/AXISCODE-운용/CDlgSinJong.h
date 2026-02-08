#pragma once


// CDlgSinJong 대화 상자
#include "skinlistctrl.h"
static	BOOL	m_gsortSinjong;
class CDlgSinJong : public CDialog
{
	DECLARE_DYNAMIC(CDlgSinJong)

public:
	//CDlgSinJong(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	CDlgSinJong(int kind, int type, CPoint pt = CPoint(-1, -1), CWnd* pParent = nullptr);
	virtual ~CDlgSinJong();

	enum { IDD = IDD_DIALOG_SINJONG };
	CAxisCodeApp* m_pApp;
	CPoint	m_point;
	CString	m_szCode;
	CString	m_szName;
	CFont	m_Font;
	int	m_kind;
	int	m_Type;
	std::vector<std::tuple<CString, CString, char, char>> m_SinJongArray;
	std::vector<std::tuple<CString, CString, CString>> _vData;
	std::vector<std::pair<CString, CString>> _vCODE;
	CString _szData;
	std::map<CString, struct hjcodex*> _mapCODE;
	void SetDlgPos(int x, int y);
	BOOL	ProcPart(int ntype);
	void listsort(const int column);
	void Set_KS_HanMode();

	CString GetCode() { return m_szCode; }
	CString GetName() { return m_szName; }
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SINJONG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CComboBox m_cbGubn;
	afx_msg void OnChangeEditSearch();
	CSkinListCtrl m_ListCtrl;
	CString m_szEditSearch;
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
//	afx_msg void OnGetdispinfoListSinjong(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetdispinfoListSinjong(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeComGubn();
	afx_msg void OnClickListSinjong(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListSinjong(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedBtnConfirm();
};
