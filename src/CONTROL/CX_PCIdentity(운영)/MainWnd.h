#pragma once
// MainWnd.h : header file
//

#include "sk_pc_identity.h"

/////////////////////////////////////////////////////////////////////////////
// CMainWnd window

class CMainWnd : public CWnd
{
// Construction
public:
	CMainWnd(CWnd* parent);

// Attributes
public:
	PC_INFO_CONTEXT m_pContext;
	CString	m_os;
	CString	m_cpu;
	CString	m_h_hdd_key;
	CString	m_h_drive_type;
	CString	m_h_controler_type;
	CString	m_h_model_number;
	CString	m_h_serial_number;
	CString	m_n_subnetmask;
	CString	m_n_gateway;
	CString	m_n_dhcp_enabled;
	CString	m_h_drive_type2;
	CString	m_h_controler_type2;
	CString	m_n_dhcp_enabled2;
	CString	m_n_ip_address;
	CString	m_n_mac_key;
	CString	m_n_adap_name;
	CString	m_n_adap_desc;
	CString	m_n_physical_mac;
	CString	m_n_current_mac;
	CWnd* m_parent;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainWnd)
public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainWnd();
	void LoadData();
	CString GetData(int nType);
//	CString GetMacAddress();
	LPBYTE String2Binary(LPCTSTR strData, int nDataLen);
	void WriteLog( LPCSTR log);

	// Generated message map functions
protected:
	//{{AFX_MSG(CMainWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CMainWnd)
	afx_msg void SetProperties(LPCTSTR sParam);
	afx_msg BSTR GetProperties();
	afx_msg BSTR GetPCData(short nType);
	afx_msg BSTR GetOSDesc();
	afx_msg BSTR GetCPUDesc();
	afx_msg void Load();
	afx_msg BSTR GetMAC();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};