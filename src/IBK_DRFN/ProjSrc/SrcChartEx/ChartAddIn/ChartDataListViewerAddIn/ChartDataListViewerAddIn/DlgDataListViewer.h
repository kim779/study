#if !defined(AFX_DLGDATALISTVIEWER_H__5D2A506E_7779_11D5_81A5_0050FC28B229__INCLUDED_)
#define AFX_DLGDATALISTVIEWER_H__5D2A506E_7779_11D5_81A5_0050FC28B229__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDataListViewer.h : header file
//

#include <afxtempl.h>

#include "Include_AddIn.h"							// for Ixxxxxx.h
#include "HeaderCtrlExt.h"
#include "Data.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgDataListViewer dialog

class CChartDataListViewerAddInImp;// 20081023 이문수 >>

class CDlgDataListViewer : public CDialog
{
// Construction
//20081023 이문수 >>
public:

	CWnd *							m_pDataListViewMngWnd;

	CChartDataListViewerAddInImp *	m_pAddInImp;

//20081023 이문수 <<

public:

	void LoadExcel2();

	int m_nHighlightListItem;
	BOOL m_bPacketHeader;

	void RealUpdate();	

	void SetPacketNames(const CString& strTitle,
					    const CString& strPacketNamesPerBlock,
						const CString& strPacketNames);
	void MouseMove(int nIndex);
	void SetPacketListAddr( IPacketManager * pPacketList);
//	CDlgDataListViewer(CWnd* pParent = NULL);   // standard constructor
	CDlgDataListViewer(CWnd* pParent, const CString& strTitle,
									  const CString& strHorzScale,
									  const CString& strPacketNamesPerBlock,
									  const CString& strPacketNames,
									  const char *p_szSaveFilePath,
									  const CString& strRQs,
									  const bool bViewPacketSequence,
									  CChartDataListViewerAddInImp * pAddInImp, //20081023 이문수 >>
									  HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode);
	~CDlgDataListViewer();

// Dialog Data
	//{{AFX_DATA(CDlgDataListViewer)
	enum { IDD = IDD_DATALIST_VIEWER };
	CButton	m_btnReverse;
	CButton	m_btnClipboard;
	CButton	m_btnUpdate;
	CButton	m_btnRuntoExcel;
	CListCtrl	m_ctlList;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDataListViewer)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CHeaderCtrlEx m_HeaderCtrl;

	// Generated message map functions
	//{{AFX_MSG(CDlgDataListViewer)
	virtual BOOL OnInitDialog();
	afx_msg void OnExcelFileExport();
	afx_msg void OnClose();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnUpdate();
	afx_msg void OnClipboard();
	afx_msg void OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReverse();
	//}}AFX_MSG
	afx_msg void OnCustomdrawList(NMHDR*, LRESULT*);
	DECLARE_MESSAGE_MAP()

//20081023 이문수 지표변경시 다운되는 현상 수정 >>
//private:
public:
//20081023 이문수 지표변경시 다운되는 현상 수정 <<

	bool m_bViewPacketSequence;
	CString m_strHorzScale;
	bool IsAllZeroData(CList<double, double>* datalist);
	CPoint GetRowandColumnCount();
	void LoadExcel();
	CString GetStrValue(HKEY Hkey,CString strkey, CString strsubkey,CString strRValue);
	void LoadListFocusMove(int nIndex);
	void InsertDatatoList(const CString& strTitles, const CString& strPacketNames, const CString& strPacketNamesPerBlock);
	IPacketManager *m_pPacketList;
	CString m_strTitles;
	CString m_strPacketNames;
	CString m_strPacketNamesPerBlock;
	CString m_strFilePath;
	CWnd * m_pOcxWnd;
	CBrush m_brBkColor;
	int m_nFutureDateCount;
	CArray <CData,CData> m_arData;
	CObList m_DataList;
	int m_nPacketCount;
	int m_nRowCount;
	CObList m_PacketList;
	//CData m_Data;
	unsigned long * m_piItem;
	unsigned long * m_piSubItem;

	void ReCalc_WindowSize(const CString &szPacketNames);
	void Make_ListHeader(const CString& strTitles, const CString& strPacketNamesPerBlock);
 	void UpdateRecord(int nIndex);
 	void InsertRecord(int nIndex);

	void	MakeHeader_PacketSequence();
	void	InsertData_PacketSequence();
	
	int GetMaxCount() const;
	void EnsureVisible();
	CString CommaStr(CString in);	// 숫자 스트링에 콤마 넣기
	BOOL	m_bReverse;
	//2007.05.16 by LYH 복수종목 적용
	CString m_strRQs;

// (2008/5/14 - Seung-Won, Bae) for Multi-Language of Special Date or Price Unit Text
protected:
	HWND	m_hOcxWnd;

// (2009/9/15 - Seung-Won, Bae) Do not support special time for WorldOn.
protected:
	CChartInfo::CHART_MODE	m_eChartMode;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDATALISTVIEWER_H__5D2A506E_7779_11D5_81A5_0050FC28B229__INCLUDED_)
