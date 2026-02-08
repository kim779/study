// Member.h: interface for the CMember class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MEMBER_H__3738A8F3_1DEB_4256_AF30_D63A4B0E2098__INCLUDED_)
#define AFX_MEMBER_H__3738A8F3_1DEB_4256_AF30_D63A4B0E2098__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

interface ISkinColorManager;
class CMember  
{
public:
	CMember(LPVOID lpControl, BOOL bUseDefSkin = FALSE);
	virtual ~CMember();

public:
	LPVOID				m_pControl;
	BOOL				m_bUseOnlyDefaultSkinTable;
	
	ISkinColorManager*	m_pISkinColorManager;
	void SetSkinColorManager(ISkinColorManager* pISkinColorManager) {m_pISkinColorManager = pISkinColorManager;}
};

#endif // !defined(AFX_MEMBER_H__3738A8F3_1DEB_4256_AF30_D63A4B0E2098__INCLUDED_)
