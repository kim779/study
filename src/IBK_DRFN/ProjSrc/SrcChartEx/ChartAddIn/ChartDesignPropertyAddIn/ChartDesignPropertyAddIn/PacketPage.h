#if !defined(AFX_PACKETPAGE_H__AFDD1A26_0F86_4B20_A273_C43DBD74F15F__INCLUDED_)
#define AFX_PACKETPAGE_H__AFDD1A26_0F86_4B20_A273_C43DBD74F15F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PacketPage.h : header file
//

#include "resource.h"
#include "PacketSuperGrid.h"
#include "DlgToolTip.h"

#include "DataOfPacketPage.h"
/////////////////////////////////////////////////////////////////////////////
// CPacketPage dialog

interface IPropertyMap;
class CPacketPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CPacketPage)

// Construction
public:
	CString GetUseRealPacketItems();
	//Get m_strReceiveDataFromOcx
	CString GetReceivedAllDataFromOcx();
	//Set Received Value From ocx
	void SetProperty( IPropertyMap *p_pIPropertyMap);

	CPacketPage();
	~CPacketPage();

// Dialog Data
	//{{AFX_DATA(CPacketPage)
	enum { IDD = IDD_PROPPAGE_PACKET };
	CStatic	m_Frame;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPacketPage)
	public:
	virtual BOOL OnApply();
	virtual BOOL OnKillActive();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPacketPage)
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CString GetAllDataFromListCtrl();
	//listctrl
	CPacketSuperGrid m_PacketList;
	//have received packetpage all(checkd and unchecked) data from ocx.
	CString m_strReceiveDataFromOcx;

	//tooltip
	CDlgToolTip		m_toolTip;
	//Create tooltip object and Add text to StringArray 
	void CreateToolTip();

	CDataOfPacketPage m_PacketPagedata;

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

#endif // !defined(AFX_PACKETPAGE_H__AFDD1A26_0F86_4B20_A273_C43DBD74F15F__INCLUDED_)
