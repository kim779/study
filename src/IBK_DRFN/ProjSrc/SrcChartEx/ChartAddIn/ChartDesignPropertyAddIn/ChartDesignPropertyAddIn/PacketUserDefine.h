#if !defined(AFX_PACKETUSERDEFINE_H__211A0DEF_2D78_4EA4_AB90_06E4D9DD43A0__INCLUDED_)
#define AFX_PACKETUSERDEFINE_H__211A0DEF_2D78_4EA4_AB90_06E4D9DD43A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PacketUserDefine.h : header file
//

#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CPacketUserDefine dialog

//Packet 속성 Page에서 사용자가 Packet항목을 입력할때 사용하는 Dlg
class CPacketUserDefine : public CDialog
{
// Construction
public:
	CPacketUserDefine( HWND p_hOcxWnd, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPacketUserDefine)
	enum { IDD = IDD_USERDEFINE_PACKET };
	CComboBox	m_ComboDetailPacketType;
	CString	m_strPacketName;
	CString	m_strFieldName;
	BOOL	m_bCheckReal;
	CString	m_strLength;
	int		m_nPacketType;
	CString	m_strDetailPacketType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPacketUserDefine)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPacketUserDefine)
	afx_msg void OnSelchangePackettype();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void WhenPacketTypeisEtc();
	void WhenPacketTypeisNumeral();
	void WhenPacketTypeisDataDate();

// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
protected:
	HWND	m_hOcxWnd;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PACKETUSERDEFINE_H__211A0DEF_2D78_4EA4_AB90_06E4D9DD43A0__INCLUDED_)
