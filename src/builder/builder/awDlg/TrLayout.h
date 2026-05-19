#pragma once

#include "resource.h"
#include "gridListCtrl.h"
#include "..\h\seq.H"

// CTrLayout 대화 상자입니다.

class AFX_EXT_CLASS CTrLayout : public CDialogEx
{
	DECLARE_DYNAMIC(CTrLayout)

public:
	CTrLayout(CWnd* pParent = NULL, CString root = _T(""));   // 표준 생성자입니다.
	virtual ~CTrLayout();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TRLAYOUT };
	CComboBox m_inoutCB;
	CComboBox m_trCB;

protected:
	CString	m_root;
	CArray	<CString, CString> m_in;
	CArray	<CString, CString> m_out;
	int	m_row;
	int	m_frow;
	int	m_gap;

	CString m_symbol;
	CString m_desc;

	enum	{ tNONE = 0, tINPUT, tOUTPUT } m_selKind;
	enum	{ tNo = 0, tTable, tGrid, tGridDesc, tDone } m_type;

	CGridListCtrl m_info;
	CGridListCtrl m_list;
	CMapStringToString m_trMap;
	CWnd*	m_parent;
	CString	m_formObjects;
	CString	m_sGrid;

private:
	CFont	m_font;

public:
	bool	ReadTrList(CString path = _T(""));
	void	FormObjectLayout(CString info = _T(""), CString text = _T(""), bool init = false);

protected:
	void	makeData(CString data, CString fileName);
	void	makeName(CString trname);
	void	makeFormat(CString trformat);
	void	makeIn(CString trin);
	void	makeOut(CString trout);
	void	makeFormData(CString trsymbol);
	void	makeInOut(CString key);
	void	makeInOutSet(int kind, CString tr);
	void	addTrLayout(bool binit, CString data = _T(""));
	bool	checkCombo(CString key);
	void	initCombo(CString sel, bool bFind = false);
	void	setMapH();
	void	clear(bool bcombo = false);
	CString	findGridParent(int row);
	void	setSymbol(CGridListCtrl* list, CString inout, CString trsymbol, bool grid = false, bool desc = false, CString grids = _T(""));

private:
	CString	parser(CString &srcstr, CString substr);
	void	setFont();
	CString	UTF8ToAnsi(char* pszUTF8);

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
