#pragma once

#include "resource.h"
#include "gridListCtrl.h"

// CRtsInfo 대화 상자입니다.

class AFX_EXT_CLASS CRtsInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CRtsInfo)

public:
	CRtsInfo(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CRtsInfo();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_RTS };
	CComboBox m_rtscombo;

protected:
	CGridListCtrl m_info;
	CGridListCtrl m_list;
	
	int	m_row;
	CFont	m_font;
	CWnd*	m_parent;

	CString m_data;

	CMapStringToString	m_selectMap;
	CMapStringToString	m_rtsMap;

public:
	void	SetRtsInfo(CString info);
	void	SetFormInfo(CString text);

protected:
	void	makeData(CString trsymbol);
	void	makeRtsSymbol(CString key);

	void	selectSymbolList(CString symbol);
	bool	compareRTS(CString key);

	int	findRow(CString symbol);
	void	clear();
	void	selectAll(bool check);
	void	setMapH();

private:
	CString	parser(CString &srcstr, CString substr);
	void	setFont();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedRtsSave();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedRtsDelete();
};
