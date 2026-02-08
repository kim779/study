// ExButton.h : main header file for the EXBUTTON DLL
//

#if !defined(AFX_EXBUTTON_H__C83C0B44_624C_4345_8415_CA48790B656C__INCLUDED_)
#define AFX_EXBUTTON_H__C83C0B44_624C_4345_8415_CA48790B656C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CExButtonApp
// See ExButton.cpp for the implementation of this class
//

interface ISkinColorManager;
class CMember;
class CExButtonApp : public CWinApp
{
public:
	CExButtonApp();

	long	ActionControl(LPVOID lpAction);

	COLORREF	GetColor(LPCTSTR lpszKey);
	
	LPVOID	InitDrawButton(LPVOID lpPointer, BOOL bUseDefSkin = FALSE);
	LPVOID	InitBitmapButton(LPVOID lpPointer);
	LPVOID	InitImageListButton(LPVOID lpPointer);
	LPVOID	InitCheckGroupBox(LPVOID lpPointer);
	LPVOID	InitCheckBox(LPVOID lpPointer, BOOL bUseDefSkin = FALSE);

	void	ExitButton(LPVOID lpPointer);
	
	BOOL	SetDrawButton(UINT nButton);
	BOOL	SetBitmapFile(LPCTSTR lpszUp, LPCTSTR lpszDown, LPCTSTR lpszDis);
	BOOL	LoadBMP(LPCTSTR lpszBMPFile, HGLOBAL* phDIB);
	void	DrawItemBitmap(LPDRAWITEMSTRUCT lpDrawItemStruct);
	void	DrawDIB(CDC* pDC, HGLOBAL hDIB);

	void	PreSubclassWindow_DrawButton();
	void	DrawItemButton(LPDRAWITEMSTRUCT lpDrawItemStruct);
	void	DrawButtonLine(CDC* pDC, UINT nState, CRect& rcItem);
	void	DrawButtonText(CDC* pDC, UINT nState, CRect& rcItem);
	BOOL	SetIcon(CSize size, HICON hIcon, HICON hIconPushed, UINT nIconAlign);
	void	DrawButtonIcon(CDC* pDC, UINT nState, CRect& rcItem);
	CPoint	CalculateImagePosition(CDC* pDC, UINT nState, CRect& rcItem, bool bHasPushedImage);
	void	OffsetPoint(CPoint& point, CSize size, CRect rcClient);

	BOOL	SetImageListButton(CImageList* pImageList, int nNormal, int nDown, int nDisable, int nOver, LPCTSTR lpszText, COLORREF clrText, COLORREF clrTextDis);
	void	DrawItemImageList(LPDRAWITEMSTRUCT lpDrawItemStruct);
	void	SetText_ImgList(LPCTSTR lpszText);
	void	DrawButtonText_ImgList(CDC* pDC, UINT nState, CRect& rcItem);
	
	void	SetText(LPCTSTR lpszText);
	LPCTSTR		GetText_DrawButton(LPVOID lpPointer);
	LPCTSTR		GetText_ImgList(LPVOID lpPointer);

	void	PreSubclassWindow_CheckGroupBox();
	int		MoveTitle();
	void	SetCheck_CheckGroupBox(int nCheck);
	int		GetCheck_CheckGroupBox();
	void	OnSetFocus_CheckGroupBox(CWnd* pOldWnd);
	void	OnEnable_CheckGroupBox(BOOL bEnable);
	void	GroupboxControls(BOOL bActivate);
	void	OnClicked_CheckGroupBox();
	void	SetStyle_CheckGroupBox(TouchStyle nStyle);
	LRESULT		OnGetCheck_CheckGroupBox(WPARAM wParam, LPARAM lParam);
	LRESULT		OnSetCheck_CheckGroupBox(WPARAM wParam, LPARAM lParam);

	void	SetColor_CheckBox(COLORREF clrText, COLORREF clrBackground);
	void	SetSkinString_CheckBox(LPCTSTR lpszText, LPCTSTR lpszBackground);
	void	SetSkinIndex_CheckBox(UINT nText, UINT nBackground);
	HBRUSH	CtlColor_CheckBox(CDC* pDC, UINT nCtlColor);

	void	UseToggleMode(LPVOID lpPointer, BOOL bUse);
	BOOL	GetToggleState(LPVOID lpPointer);
	void	SetToggleState(LPVOID lpPointer, BOOL bPush);
	void	ReleasePushed_DrawButton(LPVOID lpPointer);
	void	OnClicked_DrawButton(LPVOID lpPointer);

	void	AddToggleGroup_DrawButton(LPVOID lpPointer, LPVOID lpAddPointer);
	
	FpChangeSkinNotify	GetSkinNotifyFunction();

	CMember*	m_pCurMember;
	ISkinColorManager*	m_pISkinColorManager;

	// Common Member
//	CToolTipCtrl			m_ctrlTooltip;
	BOOL					m_bUseHandCursor;
	HCURSOR					m_hHandCursor;

	BOOL					m_bToggleMode;
	BOOL					m_bPushed;

	enum {
		BOX_ON = 0,			// Column 0 : checked state
		BOX_OFF = 1,		// Column 1 : unchecked state
		BOX_LDOWN_1	 = 2,	// Column 2 : mouse down in checked state
		BOX_LDOWN_2	 = 3,	// Column 3 : mouse down in unchecked state
		BOX_DISABLED_1 = 4,	// Column 4 : CB disabled in checked state
		BOX_DISABLED_2 = 5	// Column 5 : CB disabled in unchecked state
	};	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExButtonApp)
	public:
	virtual int ExitInstance();
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CExButtonApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern CExButtonApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXBUTTON_H__C83C0B44_624C_4345_8415_CA48790B656C__INCLUDED_)
