// MemberBitmap.h: interface for the CMemberBitmap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MEMBERBITMAP_H__DB3B5067_37D5_453B_977C_623250C6D3AE__INCLUDED_)
#define AFX_MEMBERBITMAP_H__DB3B5067_37D5_453B_977C_623250C6D3AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../inc/_COMMON/Member.h"

class CMemberBitmap : public CMember  
{
public:
	CMemberBitmap(LPVOID lpControl);
	virtual ~CMemberBitmap();

	HGLOBAL		m_hDIB, m_hDIBDown, m_hDIBDis;
	UINT		m_nWidth, m_nHeight;
	
protected:
    class CButton_Friendly : public CButton
    {
		friend class CExButtonApp; 
    };

public:
	CButton_Friendly*		m_pButton;
};

#endif // !defined(AFX_MEMBERBITMAP_H__DB3B5067_37D5_453B_977C_623250C6D3AE__INCLUDED_)
