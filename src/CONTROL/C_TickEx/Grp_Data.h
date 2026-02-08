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
	int	AttachGraphData(char* data, int count, int mgap, int lastTick);
	bool	UpdateRTM();
	struct _cgTickEx*	GetGraphData(size_t index);
	int	GetDataGap() { return m_dGap; }
	size_t	GetDataCount() { return _vDataQue.size(); }
	struct _cgTickEx*	GetLastData();
	static int	CharToInt(CString sData);

public:
	int	m_dKind{};
	int	m_dKey{};
	int	m_dIndex{};
	int	m_dGap{};
	int	m_iLastTick{};
	int	m_szFrame{};			// Frame Size (1 Que Size)

private:
	inline	double	CharToDouble(CString sData);

private:
	class CGrpWnd* m_pGrpWnd{};
	std::vector<std::unique_ptr<struct _cgTickEx>>	_vDataQue;

};
