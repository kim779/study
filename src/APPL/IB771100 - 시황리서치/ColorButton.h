
#pragma once

#ifndef COLOURPOPUP_INCLUDED
#include "ColourPopup.h"
#endif//COLOURPOPUP_INCLUDED

void AFXAPI DDX_ColorButton(CDataExchange *pDX, int nIDC, COLORREF& crColour);

class CColorButton : public CButton
{
public:
	DECLARE_DYNCREATE(CColorButton);
	
	CColorButton(void);
	
	virtual ~CColorButton(void);
	
	__declspec(property(get=GetColor,put=SetColor))				COLORREF	Color;
	__declspec(property(get=GetDefaultColor,put=SetDefaultColor))		COLORREF	DefaultColor;
	__declspec(property(get=GetTrackSelection,put=SetTrackSelection))	BOOL		TrackSelection;
	__declspec(property(put=SetCustomText))					LPCTSTR		CustomText;
	__declspec(property(put=SetDefaultText))				LPCTSTR		DefaultText;
	
	COLORREF GetColor(void) const;
	
	void SetColor(COLORREF Color);
	
	COLORREF GetDefaultColor(void) const;
	
	void SetDefaultColor(COLORREF Color);
	
	void SetCustomText(LPCTSTR tszText);
	
	void SetDefaultText(LPCTSTR tszText);
	
	void SetTrackSelection(BOOL bTrack);
	
	BOOL GetTrackSelection(void) const;
	
	//{{AFX_VIRTUAL(CColorButton)
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL
	
protected:
	//{{AFX_MSG(CColorButton)
	afx_msg BOOL OnClicked();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg LONG OnSelEndOK(UINT lParam, LONG wParam);
	afx_msg LONG OnSelEndCancel(UINT lParam, LONG wParam);
	afx_msg LONG OnSelChange(UINT lParam, LONG wParam);
	
	static void DrawArrow(CDC* pDC, 
		RECT* pRect, 
		int iDirection = 0,
		COLORREF clrArrow = RGB(0,0,0));
	
	
	DECLARE_MESSAGE_MAP()
		
	COLORREF	m_Color;
	COLORREF	m_DefaultColor;
	CString		m_strDefaultText;
	CString		m_strCustomText;
	BOOL		m_bPopupActive;
	BOOL		m_bTrackSelection;
	
private:
	
	typedef CButton _Inherited;
	
};



