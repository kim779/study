// PnAidInfo.h: interface for the CPnAidInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PNAIDINFO_H__551C5C36_C809_497D_9AB1_31D27ABC155F__INCLUDED_)
#define AFX_PNAIDINFO_H__551C5C36_C809_497D_9AB1_31D27ABC155F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


struct _pnAidInfo {
	int	iKind;
	RECT	rc;
	char	acName[32];
	char	acText[16];
	char	acReserved[60];
};
#define SZ_PNAIDINFO	sizeof(struct _pnAidInfo)	// = 128

class CPnAidInfo  
{
public:
	CPnAidInfo();
	virtual ~CPnAidInfo();

public:
	int	m_iKind;
	CRect	m_rect;
	CString	m_strText;
	CString	m_strName;


private:

public:
	void	GetStruck(char* pcBuffer);
	void	SetStruck(char* pcBuffer);
	void	operator =( const CPnAidInfo& src);

private:

};

#endif // !defined(AFX_PNAIDINFO_H__551C5C36_C809_497D_9AB1_31D27ABC155F__INCLUDED_)
