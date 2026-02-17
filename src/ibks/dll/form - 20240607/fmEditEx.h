#if !defined(AFX_FMEDITEX_H__41AE5B13_9183_465E_A552_2799CDF201DE__INCLUDED_)
#define AFX_FMEDITEX_H__41AE5B13_9183_465E_A552_2799CDF201DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// fmEditEx.h : header file
//

#include "fmEdit.h"

const	int	S_Deny			= -1;
const	int	S_UpdKey		= 0;
const	int	S_UpdKeySi		= 1;
const	int	S_UpdKeySp		= 2;
const	int	S_UpdKeySpSo		= 3;
const	int	S_UpdKeySftL1		= 4;
const	int	S_UpdSiKey		= 5;
const	int	S_UpdSiKeySp		= 6;
const	int	S_UpdSiKeySpSo		= 7;
const	int	S_UpdSiSkipEraseToEof	= 8;
const	int	S_UpdSoKey		= 9;
const	int	S_UpdSoKeySi		= 10;
const	int	S_UpdSoKeySftL1		= 11;
const	int	S_SftR1UpdKey		= 12;
const	int	S_SftR2UpdKey		= 13;
const	int	S_SftR3UpdSiKeySo	= 14;
const	int	S_SftR4UpdSoKeySi	= 15;
const	int	S_SkipSftR1UpdKey	= 16;
const	int	S_SkipSftR2UpdKey	= 17;
const	int	S_SftL1			= 18;
const	int	S_SftL2			= 19;
const	int	S_SftL1UpdSp		= 20;
const	int	S_SkipSftL1		= 21;
const	int	S_SkipSftL2		= 22;

/////////////////////////////////////////////////////////////////////////////
// CfmEditEx command target

class AFX_EXT_CLASS CfmEditEx : public CfmEdit
{
	DECLARE_DYNCREATE(CfmEditEx)

public:
	CfmEditEx();	           // protected constructor used by dynamic creation
	CfmEditEx(CAxisForm* axform, struct _formR* formR, int iform);
	virtual ~CfmEditEx();

// Attributes
protected :
	int	m_method;
	wch_t	m_chartype[4];

// Operations
public:
	void	UpdateData(int key, bool moving, int& result);
	void	InsertData(int key, bool moving, int& result);

	void	keyDelete(int& result);
	void	keyBackspace(int& result);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CfmEditEx)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	void	getWhichHangul();
	int	editInsMixSingle();
	int	editInsMixDouble();
	int	editUpdMixSingle();
	int	editUpdMixDouble();
	int	editDelMixChar();
	void	processEdit(int key, bool moving, int& result);
	void	processShiftLeft(int count);
	void	processShiftRight(int count);

	// Generated message map functions
	//{{AFX_MSG(CfmEditEx)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CfmEditEx)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FMEDITEX_H__41AE5B13_9183_465E_A552_2799CDF201DE__INCLUDED_)
