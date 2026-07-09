#pragma once


// agencyDlg 대화 상자입니다.

typedef struct _aglistItem { 
	char*	Code[12];
	char*	Name[50];
	char*	balcode[14];
}AGLISTITEM;

static bool		m_agsort;
class agencyDlg : public CDialogEx
{
	DECLARE_DYNAMIC(agencyDlg)

public:
	agencyDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	agencyDlg(CPoint pt, CWnd* pParent = NULL);
	virtual ~agencyDlg();

	CString		m_szCode, m_szName;
	CString		m_sRoot, m_sUser;
	CWnd*		m_pMain;
	CPoint		m_Point;
	CBrush*		m_pCtlBrush;

	int			itype;
	int			m_iSelectRow;

	CMapStringToString	m_mapAg; 
	bool		m_btype;

	CMapStringToString m_mapCodeAg;
	bool		m_bshowtype;
// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_AGENCY };

	int			DeleteSpace(char* pBuff, int nLen);
	BOOL		IsNumeric(CString strVal);
	CString		GetCode(){ return m_szCode; }
	CString		GetName(){ return m_szName; }
	void		SetDlgPos(int x, int y);
	void		SetName(CString szName){ m_szName = szName; }
	void		InitList();
	void		LoadAuthAg();
	void		ResetAuthAg();
	CString		Parser(CString &strSrc, CString strSub = _T("\t"));

	bool		checkUpcode(AGCODE* agcode);
	bool		checkAgcode(AGCODE* agcode);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	BOOL FindCode(CString szCode);

	CListCtrl m_list;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDblclkListAgency(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnChangeEditCode();
	CEdit m_Search;
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	afx_msg void OnCustomdrawListAgency(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClickListAgency(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
