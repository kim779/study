#pragma once
// Sheet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSheet

class CSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CSheet)

// Construction
public:
	CSheet(UINT nIDCaption, CWnd* pParentWnd = nullptr, UINT iSelectPage = 0);
	CSheet(LPCTSTR pszCaption, CWnd* pParentWnd = nullptr, UINT iSelectPage = 0);

// Attributes
public:
	CWnd* m_parent{};

	std::unique_ptr<class CPage1>	m_page1;
	std::unique_ptr<class CPage2>	m_page2;
	std::unique_ptr<class CPage4>	m_page4;

	bool	m_only{};
	BOOL	m_bInterest{};
	int	m_nType{};
// Operations
public:
	void	recoveryFile();
	void	deleteBackupFile();
	void	selectCombobox();

	CString	getManageGroupdata(int row, int col);
	void	setManageGroup(char* datB);
	
	bool	checkChangeSequence();
	int			getManageCount();

	void		RemovePageX();

	void		OKaction();
	void		OKSubtabaction();
	void		OKactionOne(int ipage = 1);
	void		OkKillaction();
	void		CCaction();
	
	void		initCombo();

	int			GetUploadData(int gno, CString& name, char* datB, int ipage = 0);
	CString		GetUploadData2(int gno, CString& name, int& count);		//ÀüÃ¼ Send¿ë

	int			SetDnloadData(int gno, CString name, int count, char* datB);
	void		reInitInterest();
	void		enableButton(BOOL enable);
	
	void		sendELWIssueDN(char* datB, int datL);
	void		sendELWAssetDN(char* datB, int datL);
	void		sendRecommandDN(char* datB, int datL);

	CString		GetGroupORder();
	CString		GetGroupORder2(CString& data);

	void		updateCode(CString code);
	void		updateGroup(CString codes);
	void		updateParentGroup(CString codes);

	void		selectGroup(int GroupNumber);
	void		updateGroupNCode(int GroupNumber, CString code);
	
	void		EnablePage1Btn(BOOL bEnable);

	bool		GetIsEdit();
	int			saveGroup();

	void		reloadConfig();

	int			getGroupCount();

	void		SetControlFlag(BOOL bFlag,int nType);
	void		SetMapData(BOOL bOthers,CString mapName);

	void		SendMsgToPage(int ipage, int igubn, CString Sdata = "");
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSheet)
	public:
	virtual BOOL OnInitDialog();
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL IsMenuStatusClosed();
	virtual ~CSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSheet)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg LONG OnParent (WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

