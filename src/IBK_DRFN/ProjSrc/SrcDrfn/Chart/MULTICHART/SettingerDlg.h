#if !defined(AFX_SETTINGERDLG_H__BD19122C_37CD_433E_B7E4_2847E84DB60A__INCLUDED_)
#define AFX_SETTINGERDLG_H__BD19122C_37CD_433E_B7E4_2847E84DB60A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SettingerDlg.h : header file
//
#include "RscDlg.h"
#include "Resource.h"
#include "./control/XListCtrl.h"
#include "TimerData.h"
#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////
// CSettingerDlg dialog
class CTabCodeSearch;
class CSettingerDlg : public CRscDlg
{
// Construction
public:
	CSettingerDlg(CWnd* pParent = NULL);   // standard constructor
	void SetData(CTabCodeSearch *pTabCodeSearch);
	CPoint m_ptButtomLeftOfBtn;

// Dialog Data
	//{{AFX_DATA(CSettingerDlg)
	enum { IDD = IDD_DLGSETTINGER };
	CEdit	m_editNumberTo;
	CEdit	m_editNumberFrom;
	CEdit	m_editFileName;
	CButton	m_chkUseSound;
	CButton	m_chkUseColor;
	CComboBox	m_cbStartTime;
	CEdit	m_editVolume;
	CButton	m_btInsert;
	CButton	m_btDelete;
	CXListCtrl	m_list;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSettingerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private:
	BOOL	m_bUpdatingList;
	CStringArray m_stringArrayTime;
	CArray<CTimerData,CTimerData>* m_plistTimerData;
	int*		m_pNAlarmFrom;
	int*		m_pNAlarmTo;
	BOOL*		m_pBUseAlarmSound;
	BOOL*		m_pBUseAlarmBkClr;
	CString*	m_pStrSoundFile;
	long FindPosInArray(LPCTSTR lpTime);
	void LoadInfo();
	void SaveInfo();
	long FindPosInListForInsert(LPCTSTR lpTime);

protected:

	// Generated message map functions
	//{{AFX_MSG(CSettingerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtinsert();
	afx_msg void OnBtdelete();
	afx_msg void OnSelchangeCbstarttime();
	afx_msg void OnItemchangedList3(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	afx_msg void OnBtnsearchsnd();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTINGERDLG_H__BD19122C_37CD_433E_B7E4_2847E84DB60A__INCLUDED_)
