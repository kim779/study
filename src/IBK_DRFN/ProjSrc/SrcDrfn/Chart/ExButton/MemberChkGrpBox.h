// MemberChkGrpBox.h: interface for the CMemberChkGrpBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MEMBERCHKGRPBOX_H__71C30AA3_20F5_48C7_A7E8_428FC00E3B0D__INCLUDED_)
#define AFX_MEMBERCHKGRPBOX_H__71C30AA3_20F5_48C7_A7E8_428FC00E3B0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../inc/_COMMON/Member.h"

class CMemberChkGrpBox : public CMember  
{
public:
	CMemberChkGrpBox(LPVOID lpControl);
	virtual ~CMemberChkGrpBox();

	CButton		m_btnCheckBox;
	TouchStyle	m_nStyle;

protected:
    class CButton_Friendly : public CButton
    {
		friend class CExButtonApp; 
    };

public:
	CButton_Friendly*		m_pButton;
};

#endif // !defined(AFX_MEMBERCHKGRPBOX_H__71C30AA3_20F5_48C7_A7E8_428FC00E3B0D__INCLUDED_)
