// ExComboBox.h : main header file for the EXCOMBOBOX DLL
//

#if !defined(AFX_EXCOMBOBOX_H__CFF37BEC_99A6_41F5_8504_5AFA5D57477D__INCLUDED_)
#define AFX_EXCOMBOBOX_H__CFF37BEC_99A6_41F5_8504_5AFA5D57477D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CExComboBoxApp
// See ExComboBox.cpp for the implementation of this class
//

interface ISkinColorManager;
class CMember;
class CExComboBoxApp : public CWinApp
{
public:
	CExComboBoxApp();
	long ActionControl(LPVOID lpAction);
	void ExitComboBox(LPVOID lpPointer);
	LPVOID InitComboBox(LPVOID lpPointer, BOOL bUseDefSkin = FALSE);

	void	PreSubclassWindow_ComboBox(LPVOID lpPointer);
	void	DrawItem_ComboBox(LPVOID lpPointer, LPDRAWITEMSTRUCT lpDrawItemStruct); 
	void	OnPaint_ComboBox(LPVOID lpPointer);
	void	DrawComboNormal(LPVOID lpPointer);
	void	DrawCombo(LPVOID lpPointer, CDC* pDC, CRect& rcItem);
	void	DrawComboPressed(LPVOID lpPointer);
	void	OnMouseMove_ComboBox(LPVOID lpPointer, UINT nFlags, CPoint point);
	void	OnLButtonDown_ComboBox(LPVOID lpPointer, UINT nFlags, CPoint point);
	void	OnLButtonUp_ComboBox(LPVOID lpPointer, UINT nFlags, CPoint point);
	void	OnTimer_ComboBox(LPVOID lpPointer, UINT nIDEvent);

	CImageList	m_imgList;
	CMember*	m_pCurMember;
	ISkinColorManager*	m_pISkinColorManager;
	int			m_nOffset;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExComboBoxApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CExComboBoxApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXCOMBOBOX_H__CFF37BEC_99A6_41F5_8504_5AFA5D57477D__INCLUDED_)
