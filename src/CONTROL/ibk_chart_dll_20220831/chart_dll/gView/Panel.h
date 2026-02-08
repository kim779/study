// Panel.h: interface for the CPanel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PANEL_H__9FDC5AC2_8EFD_470A_9330_061B01EDB224__INCLUDED_)
#define AFX_PANEL_H__9FDC5AC2_8EFD_470A_9330_061B01EDB224__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPanel  
{
public:
	CPanel();
	virtual ~CPanel();

public:
	BYTE		m_btPnID;		// panel ID
	WORD		m_wPnOption;		// panel option


};

#endif // !defined(AFX_PANEL_H__9FDC5AC2_8EFD_470A_9330_061B01EDB224__INCLUDED_)
