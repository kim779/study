#pragma once
// Page4.h : header file
//

#include "afxtempl.h"
#include "../../h/interst.h"
#include "../../control/fx_grid/fxGrid.h"

#define MAXGROUP	100
#define MAXROW		100
#define	WM_TOPARENT	(WM_USER + 99)
/////////////////////////////////////////////////////////////////////////////
// CPage4 dialog

class CPage4 : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage4)

// Construction
public:
	CPage4(CWnd* parent = nullptr);
	~CPage4();

// Dialog Data
	//{{AFX_DATA(CPage4)
	enum { IDD = IDD_PAGE04 };
	CListCtrl	m_list1;
	//}}AFX_DATA

public:
	class	CPage1* m_page{};
	void SetOther(class CPage1* page);
	int GetActiveGroup();
	void SetArray(CArray<struct _inters*, struct _inters*>& arInters);
public:

	bool		m_bIsEdit{};	//수정했는지 여부
	CWnd*		m_parent{};
	CString		m_root;
	CString		m_name;
	CFont*		m_font{};
	bool		m_activePage{};

	class CfxGrid*	m_grid{};
	int		m_rowC{};
	int		m_colC{};

	int		m_activegno{};
	CArray <_inters*,   _inters*>   m_inters;		// interest file_load
	CArray <_inters*,   _inters*>   m_ActiveInters;
public:
	bool	GetIsEdit() { return m_bIsEdit; }
	void	OKaction();
	void	CCaction();
	
	void	initList();
	int	loadList(int param = -1);

	void	initGrid();
	void	loadGrid();
	bool	loadConfig();
	bool    saveConfig();

	int	loadingInterest(int gno);
	int	loadingActiveInterest(int gno);
	void	savingInterest(int gno);
	void	clearInterest();
	void	clearActiveInterest();
	void	savingAllGroup();			
	void	savingSameCodeInGroup();
	bool	findingCodeInGroup(CString code, CString qty, CString price);
	void	CheckGroupAllSave();


	void	selcMark(CListCtrl* listctrl, int nItem, bool scroll = true);
	void	deselcMark(CListCtrl* listctrl);

	char			m_pdata[1024 * 4]{};
	void			SendMsgToPage(int igubn, CString sData = "");
	CString		GetGridData();
	int			GetUploadData(int gno, CString& name, char* datB);
	CString		GetGroupName(int gno);
	BOOL	endSendAction();
	void			CheckPage4();
protected:
	BOOL	AddColumn(CListCtrl *ctrl, LPCTSTR string, int item, int width);
	int	AddItem(CListCtrl *ctrl, LPCSTR string, int nItem, int nSubItem = -1);
	bool	ExistFile(int gno, bool temp = false);

	CString	parseX(CString &srcstr, CString substr);
	CFont*	getAxFont(CString fName = "굴림체", int point = 9, int style = 0);
	CString	Variant(int comm, CString data);
	CString	idTOstring(UINT id);
	CString chTOstring(char* datB, int datL);


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage4)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:
	bool IsWin8();
	void SetSize();
	// Generated message map functions
	//{{AFX_MSG(CPage4)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnsave();
	afx_msg void OnBtnjango();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

