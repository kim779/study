// MemberImgList.h: interface for the CMemberImgList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MEMBERIMGLIST_H__B390621D_1826_4AE9_8D87_B10DEAACC8AA__INCLUDED_)
#define AFX_MEMBERIMGLIST_H__B390621D_1826_4AE9_8D87_B10DEAACC8AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../inc/_COMMON/Member.h"

class CMemberImgList : public CMember  
{
public:
	CMemberImgList(LPVOID lpControl, ISkinColorManager* pISkinColorManager, BOOL bUseDefSkin);
	virtual ~CMemberImgList();

	void	SetDefaultColor();
	void	SetSkinColor();

protected:
    class CComboBox_Friendly : public CComboBox
    {
		friend class CExComboBoxApp; 
    };
	
public:
	CComboBox_Friendly*		m_pComboBox;

	COLORREF		m_clrOutLine;
	COLORREF		m_clrBackground;
	COLORREF		m_clrText;

	UINT			m_nOutLine;
	UINT			m_nBackground;
	UINT			m_nText;

	BOOL			m_bLBtnDown;
};

#endif // !defined(AFX_MEMBERIMGLIST_H__B390621D_1826_4AE9_8D87_B10DEAACC8AA__INCLUDED_)
