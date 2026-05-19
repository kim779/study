#pragma once

#include "resource.h"
#include "multiPropGrid.h"

// CGridColumn 대화 상자입니다.

class AFX_EXT_CLASS CGridColumn : public CDialogEx
{
	DECLARE_DYNAMIC(CGridColumn)

public:
	CGridColumn(CWnd* pParent = NULL, class mapForm* mapForm = NULL, int index = -1);   // 표준 생성자입니다.
	virtual ~CGridColumn();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_GRIDCOLUMN };
	BOOL	m_multiLine;
	CButton	m_okBN;

protected:
	class mapForm	*m_mapH;
	CMultiPropGrid	m_propGrid;
	CString		m_gridInfo;
	CString		m_gridExInfo;
	CString		m_tableInfo;
	int		m_index;
	char		m_kind;


	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnAdd();
	afx_msg void OnDelete();
	afx_msg void OnInsert();
	afx_msg void OnMulti();
};
