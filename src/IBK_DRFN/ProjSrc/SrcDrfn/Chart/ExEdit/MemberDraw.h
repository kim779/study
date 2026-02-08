// MemberDraw.h: interface for the CMemberDraw class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MEMBERDRAW_H__46E520F8_3FAE_4876_AE8B_FD04F7B19B0F__INCLUDED_)
#define AFX_MEMBERDRAW_H__46E520F8_3FAE_4876_AE8B_FD04F7B19B0F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../inc/_COMMON/Member.h"

class CMemberDraw : public CMember  
{
public:
	CMemberDraw(LPVOID lpControl, ISkinColorManager* pISkinColorManager, BOOL bUseDefSkin);
	virtual ~CMemberDraw();

	void	SetDefualtSkin();
	void	SetSkinColor();

protected:
    class CEdit_Friendly : public CEdit
    {
		friend class CExEditApp; 
    };
	
public:
	CEdit_Friendly*		m_pEdit;

	COLORREF	m_clrText;
	COLORREF	m_clrBackground;
	COLORREF	m_clrLine;

	UINT		m_nText;
	UINT		m_nBackground;
	UINT		m_nLine;

	BOOL		m_bNumericType;
};

#endif // !defined(AFX_MEMBERDRAW_H__46E520F8_3FAE_4876_AE8B_FD04F7B19B0F__INCLUDED_)
