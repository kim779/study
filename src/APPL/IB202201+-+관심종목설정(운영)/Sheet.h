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
	virtual ~CSheet();

// Attributes
public:
	CWnd*		m_parent;

	bool	_ball{};
	std::unique_ptr<class CPage1>	m_page1;
	std::unique_ptr<class CPage2>	m_page2;
	std::unique_ptr<class CPage4>	m_page4;

	bool m_only;
// Operations
public:
//	void	recoveryFile();
//	void	deleteBackupFile();
	void	selectCombobox();

	CString	getManageGroupdata(int row, int col);
	void	setManageGroup(char* datB);
	
	bool	checkChangeSequence();
	int	getManageCount();

	COLORREF	getIndexColor(int color);
	void		RemovePageX();

	void		OKaction();
	void		OKSubtabaction(int igubn = 0); //0 확인버튼 1 저장버튼
	void		OKactionOne();
	void		OkKillaction();
	void		CCaction();
	
	void		initCombo();

	int		GetUploadData(int gno, CString& name, char* datB);
//	CString		GetUploadData2(int gno, CString& name, int& count);		//전체 Send용

	int		SetDnloadData(int gno, CString name, int count, char* datB);
	//void		reInitInterest();
	void		enableButton(BOOL enable);
	
	void		sendELWIssueDN(char* datB, int datL);
	void		sendELWAssetDN(char* datB, int datL);
	void		sendRecommandDN(char* datB, int datL);

//	CString		GetGroupORder();
	CString		GetGroupORder2(CString& data);

	void		updateCode(CString code);
	void		updateGroup(CString codes);
	void		updateParentGroup(CString codes);

	void		BuildPropPageArray();

	void		selectGroup(int GroupNumber);
	void		updateGroupNCode(int GroupNumber, CString code);
	void		appendITEMS(int GroupNumber, CString codes);	//2012.02.10 KSJ
	
	void		EnablePage1Btn(BOOL bEnable);

	bool		GetIsEdit();
	//int		saveGroup();

	void		reloadConfig();

	int		getGroupCount();
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
	void CSheet::receiveOub(int key, CString recevieData);


	std::vector<std::pair<CString, CString>> _vGroup;


	// Generated message map functions
protected:
	//{{AFX_MSG(CSheet)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnClose();
	//}}AFX_MSG
	afx_msg LONG OnParent (WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

