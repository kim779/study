// Grp_Data.h: interface for the CGrp_Data class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include <afxtempl.h>

class CGrp_Data
{
public:
	CGrp_Data(class CGrpWnd *pGrpWnd, int dKind, int dUnit, int dIndex);
	virtual ~CGrp_Data();

public:
	int	AttachGraphData(char* data, int count, int mgap);
	bool	UpdateRTM(char* RTM);
	char*	GetGraphData(int index);
	int	GetDataGap() { return m_dGap; }
	int	GetDataCount() { return m_DataQue.GetSize(); }
	static int	CharToInt(const char* cnvB, int cnvL);

public:
	int	m_dKind;
	int	m_dUnit;
	int	m_dIndex;
	int	m_dGap;
	int	m_szFrame;			// Frame Size (1 Que Size)

private:
	bool	CreateDataQue(int count);
	inline	double	CharToDouble(const char* cnvB, int cnvL);

private:
	class CGrpWnd *m_pGrpWnd;
	CArray	<char*, char*> m_DataQue;	// Data Array
};
