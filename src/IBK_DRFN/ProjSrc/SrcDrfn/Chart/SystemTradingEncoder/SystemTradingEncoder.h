// SystemTradingEncoder.h : main header file for the SYSTEMTRADINGENCODER DLL
//

#if !defined(AFX_SYSTEMTRADINGENCODER_H__60836235_D266_4A49_9157_6E7F811DB217__INCLUDED_)
#define AFX_SYSTEMTRADINGENCODER_H__60836235_D266_4A49_9157_6E7F811DB217__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSystemTradingEncoderApp
// See SystemTradingEncoder.cpp for the implementation of this class
//

class CSystemTradingEncoderApp : public CWinApp
{
public:
	CSystemTradingEncoderApp();
	BOOL	EncryptionFile(BOOL bEnc, int nKey, LPCSTR szSource, LPCSTR szDestination, LPSTR lpszLine1, LPSTR lpszLine2, LPSTR lpszLine3);
	void	DecryptCharacterz(LPSTR szEncCharacter, LPSTR szDestination);
	void	EncryptCharacterz(char cCharacter, LPSTR szDestination);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSystemTradingEncoderApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSystemTradingEncoderApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSTEMTRADINGENCODER_H__60836235_D266_4A49_9157_6E7F811DB217__INCLUDED_)
