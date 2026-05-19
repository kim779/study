#pragma once

#include "TrListctrl.h"
#include "afxwin.h"

#define IDC_LIST_RTSINFO		1000
#define IDC_LIST_RTSLIST		1001

#define	GB_RTS		0x08
#define GB_RTSSYMBOL	0x0c
#define GB_RTSINOUT	0x0e

#define	GB_XML_RTS		','
#define GB_XML_RTSSYMBOL	'/'
#define GB_XML_RTSTYPE		'^'

// CRtsinfo 대화 상자입니다.

class CRtsinfo : public CDialogEx
{
	DECLARE_DYNAMIC(CRtsinfo)

public:
	CRtsinfo(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CRtsinfo();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_RTSINFO };

public:
	CTrListctrl m_rtsinfo;
	//CTrListctrl m_rtsFirst;
	CTrListctrl m_rtsList;
	
	int	m_frow;
	CFont	m_font;
	CString m_rtskey, m_rtskeyType;
	bool	m_bgrid;
	CWnd*	m_pParent;

	CString m_rtsData;

	CMapStringToString	m_selectsymbol;
	CMapStringToString	m_rtssymbol;
	CMapStringToString	m_tmprtssymbol;

public:
	void	makeFormsymbol(struct _mapH* mapH = NULL);
	void	makeformdata(CString trsymbol);
	void	makeformTR(CString tmp);
	void	makertssymbol(CString key);

	void	SelectSymbolList(CString type);
	bool	CompareRTS(CString key);

	CString	Parser(CString &srcstr, CString substr);
	void	setFont();
	void	reindexmap(CString key);
	int	findrow(CString symbol);
	void	Clear();
	void	SelectAll(bool check);
	bool	findcombo(CString comboname);
	void	SetMapH();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRtsBtn();
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	afx_msg void OnBnClickedRtsSave();
	afx_msg void OnBnClickedOk();
	CComboBox m_rtscombo;
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedRtsDelete();
};
