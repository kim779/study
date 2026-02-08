#if !defined(AFX_CONFIGDLG_H__773CEE7A_A488_4186_A08E_34676C248317__INCLUDED_)
#define AFX_CONFIGDLG_H__773CEE7A_A488_4186_A08E_34676C248317__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfigDlg.h : header file
//

#include <afxtempl.h>

#define PORT_DEFAULT	15101

#define PROFILE_FILE	"MacExec.ini"

class Config 
{
public:
	CString alias, fileName, ip, regKey, userID, password;
	UINT port;
	Config(const char* a)
	{
		alias = a;
		fileName = ip = regKey = "";
		port = PORT_DEFAULT;
	};

	void Save(const char* profile)
	{
		WritePrivateProfileString(alias, "FILENAME", fileName, profile);
		WritePrivateProfileString(alias, "IP", ip, profile);
		WritePrivateProfileString(alias, "REGKEY", regKey, profile);
		CString s;
		s.Format("%d", port);
		WritePrivateProfileString(alias, "PORT", s, profile);
		WritePrivateProfileString(alias, "USERID", userID, profile);
		WritePrivateProfileString(alias, "PASSW", password, profile);
	}

	void Load(const char* profile)
	{
		char buffer[MAX_PATH + 1];
		GetPrivateProfileString(alias, "FILENAME", "", buffer, MAX_PATH + 1, profile);
		fileName = buffer;
		GetPrivateProfileString(alias, "IP", "", buffer, MAX_PATH, profile);
		ip = buffer;
		GetPrivateProfileString(alias, "REGKEY", "", buffer, MAX_PATH, profile);
		regKey = buffer;
		GetPrivateProfileString(alias, "USERID", "", buffer, MAX_PATH, profile);
		userID = buffer;
		GetPrivateProfileString(alias, "PASSW", "", buffer, MAX_PATH, profile);
		password = buffer;

		port = GetPrivateProfileInt(alias, "PORT", PORT_DEFAULT, profile);
	}
};


typedef CTypedPtrList<CPtrList, Config*> CConfigList;

/////////////////////////////////////////////////////////////////////////////
// CConfigDlg dialog

class CConfigDlg : public CDialog
{
// Construction
public:
	CConfigDlg(Config* cfg = NULL, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CConfigDlg)
	enum { IDD = IDD_CONFIG };
	CString	m_alias;
	CString	m_fileName;
	CString	m_ip;
	UINT	m_port;
	CString	m_regKey;
	CString	m_password;
	CString	m_userID;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfigDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConfigDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBrowse();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL m_addMode;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGDLG_H__773CEE7A_A488_4186_A08E_34676C248317__INCLUDED_)
