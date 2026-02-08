// CodeDataFromInputBar.h: interface for the CCodeDataFromInputBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CODEDATAFROMINPUTBAR_H__1943BC6A_7285_42F7_928A_8B99C5A99F71__INCLUDED_)
#define AFX_CODEDATAFROMINPUTBAR_H__1943BC6A_7285_42F7_928A_8B99C5A99F71__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCodeDataFromInputBar  
{
public:
	CCodeDataFromInputBar();
	virtual ~CCodeDataFromInputBar();
	CString	m_strCode;
	long	m_lEndTime;
	char	m_chType;
	long	m_lTypeNum;
	// gm0604 added
	long	m_lDataCnt;
	long	m_lPtrST;
	CString m_strSTName;
	// gm0604 added (end)
};

#endif // !defined(AFX_CODEDATAFROMINPUTBAR_H__1943BC6A_7285_42F7_928A_8B99C5A99F71__INCLUDED_)
