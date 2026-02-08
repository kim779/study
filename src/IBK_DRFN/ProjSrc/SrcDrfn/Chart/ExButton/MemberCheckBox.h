// MemberCheckBox.h: interface for the CMemberCheckBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MEMBERCHECKBOX_H__0A81237B_932A_461F_BA4D_4345257AFBE0__INCLUDED_)
#define AFX_MEMBERCHECKBOX_H__0A81237B_932A_461F_BA4D_4345257AFBE0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../inc/_COMMON/Member.h"

class CMemberCheckBox : public CMember  
{
public:
	CMemberCheckBox(LPVOID lpControl, ISkinColorManager* pISkinColorManager, BOOL bUseDefSkin);
	virtual ~CMemberCheckBox();

	void	SetDefaultColor();
	void	SetSkinTable(UINT nBackground, UINT nText);
	void	SetColor(COLORREF clrBackground, COLORREF clrText);
	void	SetSkinColor();

public:
	COLORREF	m_clrBackground;			// background color == COLOR_3DFACE
	COLORREF	m_clrText;

	UINT		m_nBackground;
	UINT		m_nText;

	HBRUSH m_brush;			// background brush

	BOOL	m_bUseSkinTable;

protected:
    class CCheckBox_Friendly : public CButton
    {
		friend class CExButtonApp; 
    };

public:
	CCheckBox_Friendly*		m_pButton;

};

#endif // !defined(AFX_MEMBERCHECKBOX_H__0A81237B_932A_461F_BA4D_4345257AFBE0__INCLUDED_)
