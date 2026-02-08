// PnToolInfo.h: interface for the CPnToolInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PNTOOLINFO_H__5E972F96_B6A4_40AD_9152_D3D20D3C2FDC__INCLUDED_)
#define AFX_PNTOOLINFO_H__5E972F96_B6A4_40AD_9152_D3D20D3C2FDC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct _pnToolInfo {
	int	iBtnState;
	int	iKind;
	int	iIndex;
	RECT	rc;
	char	acText[64];
	char	acReserved[36];
};
#define SZ_PNTOOLINFO	sizeof(struct _pnToolInfo)	// = 128


class CPnToolInfo  
{
public:
	CPnToolInfo();
	virtual ~CPnToolInfo();


public:
	int	m_iBtnState;
	int	m_iKind;
	int	m_iIndex;	// -1 : hide;
	CRect	m_rect;
	CString	m_strText;

public:
	void	GetStruck(char* pcBuffer);
	void	SetStruck(char* pcBuffer);

	void	operator =( const CPnToolInfo& src);
};

#endif // !defined(AFX_PNTOOLINFO_H__5E972F96_B6A4_40AD_9152_D3D20D3C2FDC__INCLUDED_)
