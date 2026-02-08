// MemberDraw.h: interface for the CMemberDraw class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MEMBERDRAW_H__7FAF8A4B_5419_47C3_B926_684CF3C80050__INCLUDED_)
#define AFX_MEMBERDRAW_H__7FAF8A4B_5419_47C3_B926_684CF3C80050__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../inc/_COMMON/Member.h"

class CMemberDraw : public CMember  
{
public:
	CMemberDraw(LPVOID lpControl, ISkinColorManager* pISkinColorManager, BOOL bUseDefSkin);
	virtual ~CMemberDraw();

	void	SetDefaultColor();
	void	SetMemberDraw(COLORREF clrOutLine, COLORREF clrBackground, COLORREF clrText);
	void	SetMemberIndex(UINT nOutLine, UINT nBackground, UINT nText);
	void	SetSkinColor();
	void	SetText(LPCTSTR lpszText);

	FpChangeSkinNotify	GetNotifySkinTableFunction();

public:	
	LPSTR		m_lpszText;

	COLORREF	m_clrOutLine;
	COLORREF	m_clrBackground;
	COLORREF	m_clrText;

	UINT		m_nOutLine;
	UINT		m_nBackground;
	UINT		m_nText;
  
	HICON    m_hIcon;
    CSize    m_sizeImage;

	UINT	m_nIconAlign;
	BOOL	m_bUseSkinTable;
		
protected:
    class CStatic_Friendly : public CStatic
    {
		friend class CExStaticApp; 
    };
	
public:
	CStatic_Friendly*		m_pStatic;
};

#endif // !defined(AFX_MEMBERDRAW_H__7FAF8A4B_5419_47C3_B926_684CF3C80050__INCLUDED_)
