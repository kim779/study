// code.h: interface for the CCode class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

class CCode  
{
public:
	CString	m_master;

public:
	bool	GetMaster(char* root, int* ncode);
	bool	GetCategory(char* root, int kind, int index, char* rcode, bool prev);
	int	GetSpinData(DWORD type, int nfloat, int xfloat, char* data, double value, bool prev);
	double	GetDelta(DWORD type, double value, bool prev);
};

