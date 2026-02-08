#if !defined(AFX_REALPACKETPAGE_H__EBDB7BB1_765A_41F9_9F44_591D2AA0F067__INCLUDED_)
#define AFX_REALPACKETPAGE_H__EBDB7BB1_765A_41F9_9F44_591D2AA0F067__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RealPacketPage.h : header file
//
#include "resource.h"
#include "PacketPage.h"
#include "TreeCombo.h"
#include "DlgToolTip.h"
#include "NoSystemMenuEdit.h"
#include "NoSystemMenuCombo.h"

/////////////////////////////////////////////////////////////////////////////
// CRealPacketPage dialog
class CRealPacketPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CRealPacketPage)

// Construction
public:
	CRealPacketPage();
	~CRealPacketPage();

	//Realpacket정보를 관리하는 클래스를 설정한다.
	void SetRealPacketInfo(CString szRealPacket);
	//packetPage에서 실시간유무를 수정하기 전에 이전에 설정되어 있던 아이템과 정보를
	//리스트로 만들기 위해서 정보를 얻는다.
	CString GetSavedRealPacketInfo();
	//ocx에서 저장되어 있던 data를 받아서 멤버변수에 Set.
	void SetProperty( IPropertyMap *p_pIPropertyMap);
	//SharedDataPage에서 설정한 data를 받아서 멤버변수에 Set.
	void SetKeyName(CString keyname);

// Dialog Data
	//{{AFX_DATA(CRealPacketPage)
	enum { IDD = IDD_PROPPAGE_REALPACKET };
	CComboBox	m_comboPacketList;
	CComboBox	m_comboBasePacket;
	CNoSystemMenuEdit	m_RealDataEdit;
	CComboBox	m_cmbSharedName;
	CListCtrl	m_lsAdviseList;
	CString	m_sLocalSharedList;
	CString	m_szRealList;
	CString	m_strPacket;
	CString	m_szKeyName;
	BOOL	m_bUseRealData;
	int		m_nPacketSize;
	BOOL	m_bRedrawInCaseOfRealApplyToAllPacket;
	BOOL	m_bFireRealEvent;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CRealPacketPage)
	public:
	virtual BOOL OnSetActive();
	virtual BOOL OnApply();
	virtual BOOL OnKillActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CRealPacketPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnAdd();
	afx_msg void OnBtnDel();
	afx_msg void OnSelchangeCmbSharedname();
	afx_msg void OnSelchangePacketTree();
	afx_msg void OnSelendokPacketTree();
	afx_msg void OnSelchangeComboBasepacket();
	afx_msg void OnBtnEdit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CString m_szRealPackets;
	//m_lsAdviseList에서 선택한 Item의 Index.
	int     m_nSelectedPacketItem;        
	//edit의 text.
	CString m_szOneAdvise;

	CTreeCombo m_TreeCombo;

	void MakeRealList();
	//RealpacketPage에서 두번째 콤보박스 채우기.
	//SharedDataPage에서 선택했던 Item들을 콤보박스에 넣기.
	int  InitSharedListCombo(CComboBox* pCombo,CString szSharedList);

	int  GetSelectItem();

	void InitList();
	void AddData(CListCtrl* pList);
	int  AddData(CString ChartPacket, CString packet, CString field, CString key);

	CString		m_szBasePacketName;
	void		InitBasePacketCombo();
	void		InitPacketListCombo();
	void		Init_AdviseList();

// (2006/7/5 - Seung-Won, Bae) Manage Property Map for Update
protected:
	IPropertyMap *	m_pIPropertyMap;

// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
protected:
	HWND	m_hOcxWnd;
public:
	void	SetOcxHwnd( HWND p_hOcxWnd);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REALPACKETPAGE_H__EBDB7BB1_765A_41F9_9F44_591D2AA0F067__INCLUDED_)
