#pragma once
// Page3.h : header file
//

#include "afxtempl.h"
#include "../../h/interst.h"
#include "../../control/fx_misc/fxStatic.h"
#include "../../control/fx_misc/fxEdit.h"
#include "../../control/fx_misc/fxButton.h"

/////////////////////////////////////////////////////////////////////////////
// CPage3 dialog

class CPage3 : public CPropertyPage
{
	DECLARE_DYNCREATE(CPage3)

// Construction
public:
	CPage3(CWnd* parent = nullptr);
	~CPage3();

// Dialog Data
	//{{AFX_DATA(CPage3)
	enum { IDD = IDD_PAGE03 };
	CfxButton	m_saveall;
	CfxEdit		m_editjprc;
	CfxEdit		m_editvol;
	CfxEdit		m_editrate;
	CfxEdit		m_editjnum;
	CfxEdit		m_editdiff;
	CfxEdit		m_editbid;
	CfxEdit		m_editask;
	CfxEdit		m_editcurr;
	CComboBox	m_mmgb;
	CComboBox	m_jmgb;
	CfxStatic	m_ssVol;
	CfxStatic	m_ssRate;
	CfxStatic	m_ssDiff;
	CfxStatic	m_ssCurr;
	CfxStatic	m_ssBid;
	CfxStatic	m_ssAsk;
	CfxStatic	m_jnam;
	CfxStatic	m_code;
	CComboBox	m_gname;
	CListCtrl	m_list2;
	CListCtrl	m_list1;
	//}}AFX_DATA

public:
	CWnd*		m_parent;
	CString		m_root;
	CString		m_name;
	CFont*		m_font;
	CFont*		m_fontBT;
	bool		m_activePage;

	int		m_activegno;

	CArray <_inters*,   _inters*>   m_inters;		// interest file_load
	CArray <_codeinfo*, _codeinfo*> m_cinfos;		// codeinfo file load

public:
	void	OKaction();
	void	CCaction();

	void	initList();
	void	initCombo();
	void	initEdit();
	void	initStatic();
	void	initCondition(bool enableOnley = false);
	void	initialDialog();

	int	loadingInterest(int gno);
	void	ClearInterest();
	void	enableWindow(UINT nid);
	void	updateEditData(UINT chkid);

	int	loadingCondition();
	void	savingCondition();
	void	ClearCondition();
	void	addCondition();
	void	delCondition();
	void	deleteAllCondition();

	void	DeleteItemToList2(int ipos);
	void	selcMark(CListCtrl* listctrl, int nItem, bool scroll = true);
	void	deselcMark(CListCtrl* listctrl);

	void	findWave();
	void	listenWave();

	CfxEdit* idTOwnd(UINT id);

	void	sendTransaction(CString code);
	void	parsingData(char* datB, int datL);

protected:
	BOOL	AddColumn(CListCtrl *ctrl, LPCTSTR string, int item, int width);
	int	AddItem(CListCtrl *ctrl, LPCSTR string, int nItem, int nSubItem = -1);
	bool	ExistFile(int gno, bool temp = false);

	CString	parseX(CString &srcstr, CString substr);
	void	sendTR(CString trCode, char* datB, int datL, int key = 0);
	CFont*	getAxFont(CString fName = "±¼¸²Ã¼", int point = 9, int style = 0);
	CString	Variant(int comm, CString data);
	bool	GuideMsg(CString msg, bool question = false);
	CString	idTOstring(UINT id);
	CString chTOstring(char* datB, int datL);

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage3)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage3)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeGname();
	afx_msg void OnClickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickList2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeComboMmgb();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

