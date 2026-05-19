#pragma once

#include "TrListctrl.h"
#include "afxwin.h"

#define IDC_LIST_SERVERIN		1000
#define IDC_LIST_FORM			1001

#define TR_TYPE_STRUCTURE	0
#define TR_TYPE_BINARY		1
#define TR_TYPE_TAB		2
#define TR_TYPE_FID		3

#define	GB_TR		0x08
#define GB_SYMBOL	0x0c
#define GB_INOUT	0x0e

#define	GB_XML_TR	','
#define GB_XML_SYMBOL	'/'
#define GB_XML_INOUT	'*'
#define GB_XML_TYPE	'^'

// CTrLayOut 대화 상자입니다.

class CTrLayOut : public CDialogEx
{
	DECLARE_DYNAMIC(CTrLayOut)

public:
	CTrLayOut(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTrLayOut();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TRLAYOUT };

protected:
	CString	m_formObjects;
	enum	{t_NONE, t_TABLE, t_GRID, t_GRIDDESC, t_DONE} m_tGrid;
	CString	m_sGrid;
public:
	CString	m_trname;
	CString m_trtype;
	CString m_trformat;
	CArray	<CString, CString> m_trin;
	CArray	<CString, CString> m_trout;
	int	m_ntrformat;
	CFont	m_font;

	int	m_row;
	int	m_frow;
	int	m_Gap;
	CString m_serverTR;
	CString m_disciptionTR;
	bool	m_bsave;
	enum	{NONE = 0, INPUT, OUTPUT} m_selKind;

	CTrListctrl m_serverlist;
	CTrListctrl m_formlist;
	CComboBox m_inoutcombo;
	CComboBox m_formTr;
	CMapStringToString m_CMaptr;
	CWnd*	m_pParent;

public:
	bool	readTrList(CString path = _T(""));
	void	makeData(CString data, CString fileName);
	void	makeTrname(CString trname);
	void	makeTrtype(CString trkind);
	void	makeTrformat(CString trformat);
	void	makeTrin(CString trin);
	void	makeTrout(CString trout);
	void	formObjectLayout(struct _mapH* mapH = NULL, CString tmp = _T(""), bool binit = false);
	void	makeformdata(CString trsymbol);
	void	makeFormsymbol();
	void	makeforminout(CString key);
	void	makeforminoutset(int kind, CString tr);
	void	addTrlayout(bool binit, CString data = _T(""));
	bool	checkComboname(CString key);

	void	initcombo(CString sel, bool bFind = false);
	void	SetMapH();

	CString	Parser(CString &srcstr, CString substr);
	void	Cleardata(bool bcombo = false);
	CString	findgridparent(int row);

	void	setsymbol(CTrListctrl* list, CString inout, CString trsymbol, bool grid = false, bool gridDesc = false, CString grids = _T(""));
	void	setFont();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	afx_msg void OnBnSave();
	afx_msg void OnBnClickedClear();
	afx_msg void OnBnClickedReset();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedNew();
	afx_msg void OnBnClickedBtnDelete();
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
};
