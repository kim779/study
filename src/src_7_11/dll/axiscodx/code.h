// code.h: interface for the CCode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CODE_H__AB384709_2B2F_4897_8CE7_926B55874140__INCLUDED_)
#define AFX_CODE_H__AB384709_2B2F_4897_8CE7_926B55874140__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCode  
{
public:
	CCode();
	virtual ~CCode();

public:
	CString	m_master;

public:
	bool	GetMaster(char* root, int* ncode);
	bool	GetCategory(char* root, int kind, int index, char* rcode, bool prev);
	int	GetSpinData(DWORD type, int nfloat, int xfloat, char* data, double value, bool prev);
	double	GetDelta(DWORD type, double value, bool prev);
};

#endif // !defined(AFX_CODE_H__AB384709_2B2F_4897_8CE7_926B55874140__INCLUDED_)
