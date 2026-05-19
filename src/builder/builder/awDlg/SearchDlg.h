#pragma once
#include "resource.h"
#include "SearchListCtrl.h"
#include "afxwin.h"

// CSearchDlg 대화 상자입니다.

class AFX_EXT_CLASS CSearchDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSearchDlg)

public:
	CSearchDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSearchDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SEARCH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CSearchListCtrl m_listResult;
	virtual BOOL OnInitDialog();
	bool FindString(const CStringArray& arr, LPCTSTR pszString);
	void SetRoot(CString sRoot) { m_sRoot = sRoot; }
	//void SetName(CString sMap)  { m_sMap = sMap; }
	//void SetID(CString sID)	    { m_sID = sID; }
	//void setPath(CString Path)  { m_sPath = Path; }
public:
	afx_msg void OnCheckCommand(UINT cmdID);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	afx_msg void OnBnClickedButtonSearch();
	afx_msg LRESULT OnReceiveClose(WPARAM wParam, LPARAM lParam);
	void SearchRun();
	void SetGridData(char* pchData);
private:
	CStringArray		m_aryTerms;
	CStringArray		m_aryObject;
	CString			m_sRoot;
public:
	afx_msg void OnBnClickedCheckResearsh();
	afx_msg void OnBnClickedButtonReset();
	afx_msg void OnBnClickedButtonSave();
	CComboBox m_comboObject;
	afx_msg void OnDtnDatetimechangeEdate(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDtnDatetimechangeSdate(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonExcel();
};
