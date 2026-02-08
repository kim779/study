// Grp_Data.h: interface for the CGrp_Data class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include <afxtempl.h>

class CGrp_Data
{
public:
	CGrp_Data(class CGrpWnd *pGrpWnd, int dKind, int dKey, int dIndex);
	virtual ~CGrp_Data();

public:
	int	AttachGraphData(char* data, int count, int mgap);
	bool	UpdateRTM(bool &bChangeDisplay);
	char*	GetGraphData(int index);
	int	GetDataGap() { return m_dGap; }
	int	GetDataCount() { return m_DataQue.GetSize(); }
	static int	CharToInt(const char* cnvB, int cnvL);

public:
	int	m_dKind{};
	int	m_dKey{};
	int	m_dIndex{};
	int	m_dGap{};
	int	m_szFrame{};			// Frame Size (1 Que Size)

private:
	bool	CreateDataQue(int count);
	bool	AddGraphData(char* data, int dLen);
	bool	AddGraphData(int index, char* data, int dLen);
	inline	double	CharToDouble(const char* cnvB, int cnvL);

private:
	class CGrpWnd* m_pGrpWnd{};
	CArray	<char*, char*> m_DataQue;	// Data Array
};